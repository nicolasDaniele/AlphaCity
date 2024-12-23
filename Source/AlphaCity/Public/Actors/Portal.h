#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleportable.h"
#include "Portal.generated.h"

class UBoxComponent;
class UPortalsSubsystem;

UCLASS()
class ALPHACITY_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal();
	TObjectPtr<UBoxComponent> GetBoxCollider() const { return boxCollider; }
	void SetAsExitPortal() { bIsExit = true; }
	FString GetOriginLevelName() const { return originLevelName; }
	FString GetTargetLevelName() const { return targetLevelName; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly) TObjectPtr<UBoxComponent> boxCollider;
	UPROPERTY(EditAnywhere) FString originLevelName;
	UPROPERTY(EditAnywhere) FString targetLevelName;

private:
	UFUNCTION() void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bIsExit;
	TObjectPtr<UPortalsSubsystem> portalsSubsystem;
};