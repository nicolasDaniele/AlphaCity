#include "AlphaPlayerController.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"

//void AAlphaPlayerController::Destroyed()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
//		FString::Printf(TEXT("%s Destroyed"), *GetName()));
//
//	if (GetSpectatorPawn() != NULL)
//	{
//		DestroySpectatorPawn();
//	}
//
//	if (MyHUD != NULL)
//	{
//		MyHUD->Destroy();
//		MyHUD = NULL;
//	}
//	
//	if (PlayerCameraManager != NULL)
//	{
//		PlayerCameraManager->Destroy();
//		PlayerCameraManager = NULL;
//	}
//
//	AGameModeBase* const gameMode = GetWorld()->GetAuthGameMode();
//	if (gameMode)
//	{
//		gameMode->ForceClearUnpauseDelegates(this);
//	}
//
//	PlayerInput = NULL;
//	CheatManager = NULL;
//
//	Super::Super::Destroyed();
//}

void AAlphaPlayerController::TryTravelToLevel(const FString& LevelName)
{
	// Absolute for Client / Relative for Server ?
	ClientTravel("/Game/" + LevelName, TRAVEL_Absolute);
}

void AAlphaPlayerController::Server_ClientTest_Implementation()
{
	Client_ClientTest();

	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Turquoise,
		FString::Printf(TEXT("%s called Server_ClientTest"), *GetName()));
}

bool AAlphaPlayerController::Server_ClientTest_Validate()
{
	return true;
}

void AAlphaPlayerController::Client_ClientTest_Implementation()
{
	ClientTest();

	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Green,
		FString::Printf(TEXT("%s called Client_ClientTest"), *GetName()));
}

void AAlphaPlayerController::ServerTest_Implementation()
{

}

void AAlphaPlayerController::ClientTest_Implementation()
{

}