#include "AlphaPlayerController.h"
#include "GameFramework/HUD.h"
#include "GameFramework/GameModeBase.h"

void AAlphaPlayerController::TryTravelToLevel(const FString& LevelName)
{
	GEngine->AddOnScreenDebugMessage(-1, 18.f, FColor::Cyan,
		FString::Printf(TEXT("%s TryTravelToLevel: %s"), *GetName(), *LevelName));

	//Server_TravelToLevel(LevelName);
	//Client_TravelToLevel(LevelName);
}

void AAlphaPlayerController::Server_TravelToLevel_Implementation(const FString& LevelName)
{
	GEngine->AddOnScreenDebugMessage(-1, 18.f, FColor::Orange,
		FString::Printf(TEXT("%s Server_TravelToLevel: %s"), *GetName(), *LevelName));

	//ClientTravel("/Game/" + LevelName, TRAVEL_Relative, true);
}

bool AAlphaPlayerController::Server_TravelToLevel_Validate(const FString& LevelName)
{
	return true;
}

void AAlphaPlayerController::Client_TravelToLevel_Implementation(const FString& LevelName)
{
	GEngine->AddOnScreenDebugMessage(-1, 18.f, FColor::Emerald,
		FString::Printf(TEXT("%s Client_TravelToLevel: %s"), *GetName(), *LevelName));

	ClientTravel("/Game/" + LevelName, TRAVEL_Relative, true);
}