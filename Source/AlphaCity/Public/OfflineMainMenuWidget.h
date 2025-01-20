#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OfflineMainMenuWidget.generated.h"

UCLASS()
class ALPHACITY_API UOfflineMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UOfflineMainMenuWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintCallable) void OnLoginClicked();
	UFUNCTION(BlueprintCallable) void OnSignUpClicked();

	UPROPERTY(EditAnywhere) FString ApiGatewayEndpoint;
	UPROPERTY(EditAnywhere) FString LoginURI;
	UPROPERTY(EditAnywhere) FString SignUpURI;
	UPROPERTY(EditAnywhere) FString StartSessionURI;
	UPROPERTY(BlueprintReadWrite) FString username;
	UPROPERTY(BlueprintReadWrite) FString password;
	UPROPERTY(BlueprintReadWrite) FString newUsername;
	UPROPERTY(BlueprintReadWrite) FString newPassword;
	UPROPERTY(BlueprintReadWrite) FString newEmail;

protected:
	UFUNCTION(BlueprintImplementableEvent) void PrintErrorMessage();

private:
	FHttpModule* Http;
	
	void LoginRequest(FString user, FString pass);
	void OnLoginResponse(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void SignUpRequest(FString user, FString pass, FString email);
	void StartSessionRequest(FString idToken);
	void OnStartSessionResponse(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
};
