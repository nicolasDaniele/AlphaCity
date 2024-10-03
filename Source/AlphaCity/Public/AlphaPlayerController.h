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
	//virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable) void TryTravelToLevel(const FString& LevelName);

	UFUNCTION(Server, Reliable, WithValidation) void Server_ClientTest();
	UFUNCTION(Client, Reliable) void Client_ClientTest();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void ServerTest();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void ClientTest();
};
