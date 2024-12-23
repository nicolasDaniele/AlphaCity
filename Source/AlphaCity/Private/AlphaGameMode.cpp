#include "AlphaGameMode.h"
#include "OnlineSessionsSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "AlphaCharacter.h"
#include "AlphaPlayerController.h"

DEFINE_LOG_CATEGORY(GameLift);
DEFINE_LOG_CATEGORY(GameMode);

void AAlphaGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	UE_LOG(GameMode, Log, TEXT("AAlphaGameMode::InitGame"));
	Super::InitGame(MapName, Options, ErrorMessage);

	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (gameInstance)
	{
		onlineSubsystem = gameInstance->GetSubsystem<UOnlineSessionsSubsystem>();
	}

	UE_LOG(GameMode, Log, TEXT("Enabling ticking..."));
	PrimaryActorTick.bCanEverTick = true;
}

void AAlphaGameMode::BeginPlay()
{
	Super::BeginPlay();

#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void AAlphaGameMode::InitGameLift()
{
	UE_LOG(GameLift, Log, TEXT("AAlphaGameMode::InitGameLift()"))
	UE_LOG(GameLift, Log, TEXT("InitGameLift()"))

	serverParameters = new FServerParameters();

	FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), serverParameters->m_authToken);
	FParse::Value(FCommandLine::Get(), TEXT("-hostid="), serverParameters->m_hostId);
	FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), serverParameters->m_fleetId);
	FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), serverParameters->m_webSocketUrl);

	serverParameters->m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());

	UE_LOG(GameLift, Log, TEXT("Server Parameters: "));
	UE_LOG(GameLift, Log, TEXT("------------------------------------------------"));
	UE_LOG(GameLift, Log, TEXT("Host Id: %s"), *serverParameters->m_hostId);
	UE_LOG(GameLift, Log, TEXT("Fleet Id: %s"), *serverParameters->m_fleetId);
	UE_LOG(GameLift, Log, TEXT("Process Id: %s"), *serverParameters->m_processId);
	UE_LOG(GameLift, Log, TEXT("Web Socket Url: %s"), *serverParameters->m_webSocketUrl);
	UE_LOG(GameLift, Log, TEXT("Auth Token: %s"), *serverParameters->m_authToken);
	UE_LOG(GameLift, Log, TEXT("------------------------------------------------"));

	UE_LOG(GameLift, Log, TEXT("Initialising the GameLift Server..."));
	gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	const FGameLiftGenericOutcome initSdkOutcome = gameLiftSdkModule->InitSDK(*serverParameters);

	if (initSdkOutcome.IsSuccess())
	{
		UE_LOG(GameLift, Log, TEXT("InitSDK succeeded"));
	}
	else
	{
		UE_LOG(GameLift, Log, TEXT("ERROR: InitSDK failed"));
		const FGameLiftError gameLiftError = initSdkOutcome.GetError();
		UE_LOG(GameLift, Log, TEXT("ERROR: %s"), *gameLiftError.m_errorMessage);
		return;
	}

	// Define functions to handle GameLift events: OnStartGameSession, OnTerminate, OnHealthCheck
	auto onGameSession = [=, this](Aws::GameLift::Server::Model::GameSession gameSession)
		{
			FString gameSessionId = FString(gameSession.GetGameSessionId());
			UE_LOG(GameLift, Log, TEXT("GameSession Initializing: %s..."), *gameSessionId);
			gameLiftSdkModule->ActivateGameSession();
		};

	auto onTerminate = [=, this]()
		{
			UE_LOG(GameLift, Log, TEXT("Game Server Process is terminating..."));
			gameLiftSdkModule->ProcessEnding();
		};

	auto onHealthCheck = []()
		{
			UE_LOG(GameLift, Log, TEXT("Performing Health Check..."));
			return true;
		};

	// Define log file location
	TArray<FString> logfiles;
	logfiles.Add(TEXT("GameLiftTutorial/Saved/Logs/GameLiftTutorial"));

	// Define server port
	const auto worldPort = GetWorld()->URL.Port;
	UE_LOG(GameLift, Log, TEXT("World Port: %d"), worldPort);

	// Assign process parameters
	processParameters = new FProcessParameters();
	processParameters->OnStartGameSession.BindLambda(onGameSession);
	processParameters->OnTerminate.BindLambda(onTerminate);
	processParameters->OnHealthCheck.BindLambda(onHealthCheck);
	processParameters->port = worldPort;
	processParameters->logParameters = logfiles;

	UE_LOG(GameLift, Log, TEXT("Calling Process Ready..."));
	FGameLiftGenericOutcome processReadyOutcome = gameLiftSdkModule->ProcessReady(*processParameters);
	if (processReadyOutcome.IsSuccess())
	{
		UE_LOG(GameLift, Log, TEXT("Process Ready Succeeded"));
	}
	else
	{
		UE_LOG(GameLift, Log, TEXT("ERROR: Process Ready Failed"));
		const auto processReadyError = processReadyOutcome.GetError();
		UE_LOG(GameLift, Log, TEXT("ERROR: %s"), *processReadyError.m_errorMessage);
	}

	UE_LOG(GameLift, Log, TEXT("Finished initialising GameLift"));
}