#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionData.h"
#include "OnlineSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionFoundDelegate, FOnlineSessionData, sessionData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchingSessionDelegate, bool, bIsSearching);

UCLASS()
class ALPHACITY_API UOnlineSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable) void CreateGameSession(FString SessionName, int32 MaxPlayers, bool bIsFirstSession = true, APlayerController* playerController = nullptr);
	UFUNCTION(BlueprintCallable) void FindGameSessions();
	UFUNCTION(BlueprintCallable) void FindNewGameSessions();
	UFUNCTION(BlueprintCallable) void JoinGameSession(int32 ArrayIndex);
	UFUNCTION(BlueprintCallable) void OnPlayerConnected(AController* ConnectedPlayer);
	UFUNCTION(BlueprintCallable) void OnPlayerDisconnected(AController* DisconnectedPlayer);

	UFUNCTION(BlueprintCallable) int GetNumOfConnectedPlayers() const { return connectedPlayers.Num(); }

	UPROPERTY(BlueprintAssignable) FSessionFoundDelegate OnSessionFoundEvent;
	UPROPERTY(BlueprintAssignable) FSearchingSessionDelegate OnSearchingSessionEvent;

private:
	virtual void OnCreateGameSessionComplete(FName SessionName, bool bSucceeded);
	virtual void OnCreateNewGameSessionComplete(FName SessionName, bool bSucceeded);
	virtual void OnFindGameSessionsComplete(bool bSucceeded);
	virtual void OnFindNewGameSessionsComplete(bool bSucceeded);
	virtual void OnJoinGameSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	virtual void OnDestroyGameSessionComplete(FName SessionName, bool bSucceedeed);

	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	TSharedPtr<FOnlineSessionSearch> newSessionSearch;
	TArray<AController*> connectedPlayers;

	TSharedPtr<FOnlineSessionSettings> sessionSettings;

	TObjectPtr<APlayerController> travelingPlayer;
	FName firstSessionName;
	FName newSessionName;

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnCreateSessionCompleteDelegate CreateNewSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindNewSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FDelegateHandle FindNewSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
};
