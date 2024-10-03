#pragma once 

#include "CoreMinimal.h"
#include "PortalData.generated.h"

USTRUCT()
struct ALPHACITY_API FPortalData
{
public:
	GENERATED_BODY()

	FVector location;
	FVector direction;
	FRotator rotation;
	FString originLevelName;
	FString targetLevelName;

	FPortalData()
	{
		location = FVector();
		direction = FVector();
		rotation = FRotator();
		originLevelName = "";
		targetLevelName = "";
	}

	bool operator==(FPortalData other)
	{
		return location == other.location &&
			direction == other.direction &&
			rotation == other.rotation &&
			originLevelName == other.originLevelName &&
			targetLevelName == other.targetLevelName;
	}
};