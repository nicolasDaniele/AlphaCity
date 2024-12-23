#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlphaPlayerController.generated.h"

UCLASS()
class ALPHACITY_API AAlphaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAlphaPlayerController() { }

	UFUNCTION(BlueprintCallable) void TryTravelToLevel(const FString& LevelName);

private:
	UFUNCTION(Server, Reliable, WithValidation) void Server_TravelToLevel(const FString& LevelName);
	UFUNCTION(Client, Reliable) void Client_TravelToLevel(const FString& LevelName);
};
