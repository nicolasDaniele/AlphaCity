#pragma once 

#include "CoreMinimal.h"
#include "OnlineSessionData.generated.h"

USTRUCT(BlueprintType)
struct ALPHACITY_API FOnlineSessionData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly) FString SessionName;
	UPROPERTY(BlueprintReadOnly) FString PlayersCount;
	UPROPERTY(BlueprintReadOnly) int32 CurrentPlayers = 0;
	UPROPERTY(BlueprintReadOnly) int32 MaxPlayers = 0;
	UPROPERTY(BlueprintReadOnly) int32 SessionsArrayIndex = 0;

	void SetPlayersCount()
	{
		PlayersCount = FString(FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers));
	}
};