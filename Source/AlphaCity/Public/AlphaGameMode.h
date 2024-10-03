#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameLiftServerSDK.h"
#include "AlphaGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(GameLift, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(GameMode, Log, All);

class UOnlineSessionsSubsystem;
class AAlphaPlayerController;

UCLASS(Blueprintable)
class ALPHACITY_API AAlphaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAlphaGameMode() { }

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void Logout(AController* Exiting) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnPostLogin(AController* NewPlayer) override;

	UPROPERTY(EditDefaultsOnly) bool bIsMainMenu;
	TObjectPtr<UOnlineSessionsSubsystem> onlineSubsystem;

private:
	void InitGameLift();

	FGameLiftServerSDKModule* gameLiftSdkModule;
	FProcessParameters* processParameters;
	FServerParameters* serverParameters;
};
