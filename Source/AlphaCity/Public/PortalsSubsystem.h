#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PortalData.h"
#include "PortalsSubsystem.generated.h"

class APortal; 
class AAlphaCharacter;
class UPortalData;

UCLASS()
class ALPHACITY_API UPortalsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void TryTeleportToLevel(APortal* originPortal, APlayerController* playerController);
	UFUNCTION(BlueprintCallable) void TeleportCharacter(APortal* originPortal, APlayerController* playerController);

private:
	APortal* currentTargetPortal;
	APortal* previousTargetPortal;
	FPortalData originPortalData;
	FPortalData targetPortalData;

	void TeleportCharacter(APlayerController* playerController, const FVector targetLocation, const FVector targetDirection, const FRotator targetRotation);
	void FillPortalData(APortal* inPortal, FPortalData& info);
};