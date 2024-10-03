#include "PortalsSubsystem.h"
#include "Actors/Portal.h"
#include "Kismet/GameplayStatics.h"
#include "AlphaCharacter.h"
#include "AlphaPlayerController.h"
#include "Actors/Portal.h"

void UPortalsSubsystem::TryTeleportToLevel(APortal* originPortal, APlayerController* playerController)
{
	OnTeleportingCharacterEvent.Broadcast(true);

	FillPortalData(originPortal, originPortalData);

	if (AAlphaCharacter* alphaCharacter = Cast<AAlphaCharacter>(playerController->GetPawn()))
	{
		alphaCharacter->TestFunction(originPortal->GetTargetLevelName());
	}
}

void UPortalsSubsystem::TeleportCharacter()
{
	if (originPortalData.targetLevelName.IsEmpty())
	{
		return;
	}

	APortal* targetPortal = nullptr;
	TArray<AActor*> portalActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortal::StaticClass(), portalActors);
	for (AActor* actor : portalActors)
	{
		APortal* portal = Cast<APortal>(actor);
		if (portal->GetTargetLevelName() == originPortalData.originLevelName)
		{
			targetPortal = portal;
			break;
		}
	}

	if (targetPortal == nullptr)
	{
		return;
	}

	FillPortalData(targetPortal, targetPortalData);

	targetPortal->SetAsExitPortal();

	const FVector targetLocation = targetPortalData.location;
	const FVector targetDirection = targetPortalData.rotation.Quaternion().GetRightVector();
	const FRotator targetRotation = FRotator(
		targetPortalData.rotation.Pitch,
		targetPortalData.rotation.Yaw + 60,
		targetPortalData.rotation.Roll);
	TeleportCharacter(targetLocation, targetDirection, targetRotation);

	OnTeleportingCharacterEvent.Broadcast(false);
}

void UPortalsSubsystem::TeleportCharacter(const FVector targetLocation, const FVector targetDirection, const FRotator targetRotation)
{
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (character == nullptr)
	{
		return;
	}

	AAlphaCharacter* alphaCharacter = Cast<AAlphaCharacter>(character);
	if (alphaCharacter == nullptr)
	{
		return;
	}

	alphaCharacter->Teleport(targetLocation, targetDirection, targetRotation);
}

void UPortalsSubsystem::FillPortalData(APortal* inPortal, FPortalData& data)
{
	data.location = inPortal->GetActorLocation();
	data.direction = inPortal->GetActorRightVector();
	data.rotation = inPortal->GetActorRotation();
	data.originLevelName = inPortal->GetOriginLevelName();
	data.targetLevelName = inPortal->GetTargetLevelName();
}