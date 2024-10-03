#include "OnlineSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

#include "Kismet/GameplayStatics.h"

void UOnlineSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (IOnlineSubsystem* onlineSubsystem = IOnlineSubsystem::Get())
	{
		sessionInterface = onlineSubsystem->GetSessionInterface();
	}

	CreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UOnlineSessionsSubsystem::OnCreateGameSessionComplete);
	CreateNewSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UOnlineSessionsSubsystem::OnCreateNewGameSessionComplete);
	FindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UOnlineSessionsSubsystem::OnFindGameSessionsComplete);
	FindNewSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UOnlineSessionsSubsystem::OnFindNewGameSessionsComplete);
	JoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UOnlineSessionsSubsystem::OnJoinGameSessionComplete);
	DestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UOnlineSessionsSubsystem::OnDestroyGameSessionComplete);
}

void UOnlineSessionsSubsystem::CreateGameSession(FString SessionName, int32 MaxPlayers, bool bIsFirstSession, APlayerController* playerController)
{
	if (!sessionInterface.IsValid())
	{
		return;
	}

	CreateSessionCompleteDelegateHandle = bIsFirstSession ?
		sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate) :
		sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateNewSessionCompleteDelegate);

	sessionSettings = MakeShareable(new FOnlineSessionSettings());
	sessionSettings->bAllowJoinInProgress = true;
	sessionSettings->bAllowJoinViaPresence = true;
	sessionSettings->bIsDedicated = false;
	sessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	sessionSettings->bShouldAdvertise = true;
	sessionSettings->bUsesPresence = true;
	sessionSettings->NumPublicConnections = MaxPlayers;
	sessionSettings->bUseLobbiesIfAvailable = true;
	
	if (!bIsFirstSession)
	{
		travelingPlayer = playerController;
		sessionSettings->Settings.Add(FName(TEXT("NewSessionName")),
			FOnlineSessionSetting(SessionName, EOnlineDataAdvertisementType::ViaOnlineService));
			//->Set(FName("NewSessionName"), SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		newSessionName = FName(SessionName);

		sessionInterface->DestroySession(firstSessionName, DestroySessionCompleteDelegate);
		return;
	}
	else
	{
		sessionSettings->Settings.Add(FName(TEXT("SessionName")),
			FOnlineSessionSetting(SessionName, EOnlineDataAdvertisementType::ViaOnlineService));
			//->Set(FName("SessionName"), SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		firstSessionName = FName(SessionName);
	}

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), FName(SessionName), *sessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
}

void UOnlineSessionsSubsystem::OnCreateGameSessionComplete(FName SessionName, bool bSucceeded)
{
	if (!bSucceeded)
	{
		return;
	}

	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	//GetWorld()->ServerTravel("/Game/MAPS/AlphaLoungeBeach-02?listen");
	//UGameplayStatics::OpenLevel(GetWorld(), "127.0.0.1:7777");
	//GetWorld()->ServerTravel("/Game/MAPS/TestMap_1?listen"); // For easier testing
}

void UOnlineSessionsSubsystem::OnCreateNewGameSessionComplete(FName SessionName, bool bSucceeded)
{
	if (!bSucceeded)
	{
		return;
	}

	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	
	FindNewGameSessions();
}

void UOnlineSessionsSubsystem::FindGameSessions()
{
	if (!sessionInterface.IsValid())
	{
		return;
	}

	FindSessionsCompleteDelegateHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	OnSearchingSessionEvent.Broadcast(true);

	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	sessionSearch->QuerySettings.SearchParams.Empty();

	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	sessionSearch->MaxSearchResults = 10000;
	sessionSearch->QuerySettings.Set(FName(TEXT("SessionName")), firstSessionName.ToString(), EOnlineComparisonOp::Equals);
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), sessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}
}

void UOnlineSessionsSubsystem::OnFindGameSessionsComplete(bool bSucceeded)
{
	if (!bSucceeded)
	{
		return;
	}

	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	OnSearchingSessionEvent.Broadcast(false);

	TArray<FOnlineSessionSearchResult> searchResults = sessionSearch->SearchResults;
	int32 sessionsArrayIndex = 0;

	for (FOnlineSessionSearchResult result : searchResults)
	{
		if (!result.IsValid())
		{
			continue;
		}

		FOnlineSessionData sessionData;
		FString sessionName = "Empty Session Name";

		result.Session.SessionSettings.Get(FName("SessionName"), sessionName);

		sessionData.SessionName = sessionName;
		sessionData.MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
		sessionData.CurrentPlayers = connectedPlayers.Num();
		sessionData.SetPlayersCount();
		sessionData.SessionsArrayIndex = sessionsArrayIndex++;

		OnSessionFoundEvent.Broadcast(sessionData);
	}
}

void UOnlineSessionsSubsystem::FindNewGameSessions()
{
	if (!sessionInterface.IsValid())
	{
		return;
	}

	FindNewSessionsCompleteDelegateHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindNewSessionsCompleteDelegate);

	newSessionSearch = MakeShareable(new FOnlineSessionSearch());
	newSessionSearch->QuerySettings.SearchParams.Empty();

	newSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	newSessionSearch->MaxSearchResults = 10000;
	newSessionSearch->QuerySettings.Set(FName(TEXT("SessionName")), newSessionName.ToString(), EOnlineComparisonOp::Equals);
	newSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Orange, TEXT("FindNewGameSessions"));

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), newSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindNewSessionsCompleteDelegateHandle);
	}
}

void UOnlineSessionsSubsystem::OnFindNewGameSessionsComplete(bool bSucceeded)
{
	if (!bSucceeded)
	{
		return;
	}

	TArray<FOnlineSessionSearchResult> searchResults = newSessionSearch->SearchResults;
	int32 sessionsArrayIndex = 0;

	for (FOnlineSessionSearchResult result : searchResults)
	{
		if (!result.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Red, TEXT("Invalid Result"));
			continue;
		}

		FOnlineSessionData sessionData;
		FString sessionName = "Empty Session Name";

		if (result.Session.SessionSettings.Get(FName("NewSessionName"), sessionName))
		{
			sessionData.SessionName = sessionName;
			sessionData.MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
			sessionData.CurrentPlayers = connectedPlayers.Num();
			sessionData.SessionsArrayIndex = sessionsArrayIndex++;

			JoinGameSession(sessionData.SessionsArrayIndex);

			// Clear Handle

			break;
		}

	}
}

void UOnlineSessionsSubsystem::JoinGameSession(int32 ArrayIndex)
{
	if (!sessionInterface.IsValid())
	{
		return;
	}

	FOnlineSessionSearchResult result = sessionSearch->SearchResults[ArrayIndex];
	if (!result.IsValid())
	{
		return;
	}

	JoinSessionCompleteDelegateHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	FString sessionName = "";
	result.Session.SessionSettings.Get(FName("SessionName"), sessionName);

	if (!sessionName.IsEmpty())
	{
		if (!sessionInterface->JoinSession(0, FName(sessionName), result))
		{
			sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		}
	}
}

void UOnlineSessionsSubsystem::OnJoinGameSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!sessionInterface.IsValid())
	{
		return;
	}

	sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

	if (newSessionName.IsNone())
	{
		newSessionName = SessionName;
	}

	FString joinAddress = "";
	if (!sessionInterface->GetResolvedConnectString(SessionName, joinAddress))
	{
		return;
	}

	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	APlayerController* playerController = gameInstance->GetFirstLocalPlayerController();
	if (playerController != nullptr)
	{
		playerController->ClientTravel(joinAddress, ETravelType::TRAVEL_Absolute);
	}
}

void UOnlineSessionsSubsystem::OnDestroyGameSessionComplete(FName SessionName, bool bSucceedeed)
{
	
}

void UOnlineSessionsSubsystem::OnPlayerConnected(AController* ConnectedPlayer)
{
	connectedPlayers.Add(ConnectedPlayer);
}

void UOnlineSessionsSubsystem::OnPlayerDisconnected(AController* DisconnectedPlayer)
{
	if (connectedPlayers.Contains(DisconnectedPlayer))
	{
		connectedPlayers.Remove(DisconnectedPlayer);
	}
}