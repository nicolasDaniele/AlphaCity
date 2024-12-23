#include "PortalsSubsystem.h"
#include "Actors/Portal.h"
#include "Kismet/GameplayStatics.h"
#include "AlphaCharacter.h"

void UPortalsSubsystem::TryTeleportToLevel(APortal* originPortal, APlayerController* playerController)
{
	FillPortalData(originPortal, originPortalData);

	TeleportCharacter(originPortal, playerController);
}

void UPortalsSubsystem::TeleportCharacter(APortal* originPortal, APlayerController* playerController)
{
	FillPortalData(originPortal, originPortalData);

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
		if (portal->GetOriginLevelName() == originPortalData.targetLevelName)
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
	TeleportCharacter(playerController, targetLocation, targetDirection, targetRotation);
}

void UPortalsSubsystem::TeleportCharacter(APlayerController* playerController, const FVector targetLocation, const FVector targetDirection, const FRotator targetRotation)
{
	AAlphaCharacter* alphaCharacter = Cast<AAlphaCharacter>(playerController->GetPawn());
	if (alphaCharacter == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 9.f, FColor::Red, TEXT("PortalSubsystem - Invalid AlphaCharacter"));
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