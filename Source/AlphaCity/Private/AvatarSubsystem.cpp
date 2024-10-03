#include "AvatarSubsystem.h"

void UAvatarSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	currentAvatarMesh = nullptr;
	currentMeshUrl = "";
}

void UAvatarSubsystem::UpdateAvatarMesh(USkeletalMesh* newAvatarMesh)
{
	currentAvatarMesh = newAvatarMesh;
}

USkeletalMesh* UAvatarSubsystem::GetCurrentAvatarMesh() const
{
	return currentAvatarMesh;
}

void UAvatarSubsystem::UpdateMeshUrl(const FString newMeshUrl)
{
	currentMeshUrl = newMeshUrl;
}

FString UAvatarSubsystem::GetCurrentMeshUrl() const
{
	return currentMeshUrl;
}