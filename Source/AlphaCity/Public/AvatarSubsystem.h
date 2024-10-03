#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AvatarSubsystem.generated.h"

UCLASS()
class ALPHACITY_API UAvatarSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection);

	// @TODO: Delete the SkeletalMesh things if not used
	UFUNCTION(BlueprintCallable) void UpdateAvatarMesh(USkeletalMesh* newAvatarMesh);
	UFUNCTION(BlueprintCallable) USkeletalMesh* GetCurrentAvatarMesh() const;
	UFUNCTION(BlueprintCallable) void UpdateMeshUrl(const FString newMeshUrl);
	UFUNCTION(BlueprintCallable) FString GetCurrentMeshUrl() const;

private:
	USkeletalMesh* currentAvatarMesh;
	FString currentMeshUrl;
};
