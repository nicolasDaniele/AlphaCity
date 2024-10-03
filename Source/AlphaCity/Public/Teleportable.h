#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Teleportable.generated.h"

UINTERFACE(MinimalAPI)
class UTeleportable : public UInterface
{
	GENERATED_BODY()
};

class ALPHACITY_API ITeleportable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Teleport(const FVector targetLocation, const FVector targetDirection, const FRotator targetRotation);
};
