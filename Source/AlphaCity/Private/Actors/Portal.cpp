#include "Actors/Portal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PortalsSubsystem.h"

void APortal::BeginPlay()
{
	Super::BeginPlay();

	UGameInstance* gameInstance = GetWorld()->GetGameInstance();
	if (gameInstance)
	{
		portalsSubsystem = gameInstance->GetSubsystem<UPortalsSubsystem>();
	}
}

APortal::APortal()
{
	PrimaryActorTick.bCanEverTick = false;

	boxCollider = CreateDefaultSubobject<UBoxComponent>("Box Component");
	boxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnBeginOverlap);
	boxCollider->OnComponentEndOverlap.AddDynamic(this, &APortal::OnEndOverlap);

	bIsExit = false;
}

void APortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsExit || portalsSubsystem == nullptr)
	{
		return;
	}

	ITeleportable* teleportable = Cast<ITeleportable>(OtherActor);
	if (teleportable == nullptr)
	{
		return;
	}

	APlayerController* playerController = nullptr;
	if (APawn* pawn = Cast<APawn>(OtherActor))
	{
		playerController = Cast<APlayerController>(pawn->GetController());
	}
	if (playerController == nullptr)
	{
		return;
	}
	
	portalsSubsystem->TryTeleportToLevel(this, playerController);
}

void APortal::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsExit)
	{
		return;
	}

	ITeleportable* teleportable = Cast<ITeleportable>(OtherActor);
	if (teleportable == nullptr)
	{
		return;
	}

	bIsExit = false;
}