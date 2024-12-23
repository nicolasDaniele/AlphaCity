#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "OfflineMainMenuWidget.generated.h"

UCLASS()
class ALPHACITY_API UOfflineMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOfflineMainMenuWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable) void OnLoginClicked();

	UPROPERTY(EditAnywhere) FString ApiGatewayEndpoint;
	UPROPERTY(EditAnywhere) FString LoginURI;
	UPROPERTY(EditAnywhere) FString StartSessionURI;
	UPROPERTY(BlueprintReadWrite) FString username;
	UPROPERTY(BlueprintReadWrite) FString password;

protected:
	UFUNCTION(BlueprintImplementableEvent) void PrintErrorMessage();

private:
	FHttpModule* Http;
	
	void LoginRequest(FString user, FString pass);
	void OnLoginResponse(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void StartSessionRequest(FString idToken);
	void OnStartSessionResponse(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
};
