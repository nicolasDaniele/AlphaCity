#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PortalData.h"
#include "PortalsSubsystem.generated.h"

class APortal; 
class AAlphaCharacter;
class UPortalData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeleportingCharacterDelegate, bool, bIsTeleporting);

UCLASS()
class ALPHACITY_API UPortalsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void TryTeleportToLevel(APortal* originPortal, APlayerController* playerController);
	UFUNCTION(BlueprintCallable) void TeleportCharacter();

	UPROPERTY(BlueprintAssignable) FOnTeleportingCharacterDelegate OnTeleportingCharacterEvent;

private:
	APortal* currentTargetPortal;
	APortal* previousTargetPortal;
	FPortalData originPortalData;
	FPortalData targetPortalData;

	void TeleportCharacter(const FVector targetLocation, const FVector targetDirection, const FRotator targetRotation);
	void FillPortalData(APortal* inPortal, FPortalData& info);
};