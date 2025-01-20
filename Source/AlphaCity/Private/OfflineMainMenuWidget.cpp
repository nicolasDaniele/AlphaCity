#include "OfflineMainMenuWidget.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Kismet/GameplayStatics.h"

UOfflineMainMenuWidget::UOfflineMainMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Http = &FHttpModule::Get();

	ApiGatewayEndpoint = FString::Printf(TEXT("https://yj0rvy21b5.execute-api.us-east-1.amazonaws.com/Test"));
	LoginURI = FString::Printf(TEXT("/login"));
	SignUpURI = FString::Printf(TEXT("/signup"));
	StartSessionURI = FString::Printf(TEXT("/startsession"));
}

void UOfflineMainMenuWidget::OnLoginClicked()
{
	LoginRequest(username, password);
}

void UOfflineMainMenuWidget::OnSignUpClicked()
{
	SignUpRequest(newUsername, newPassword, newEmail);
}

void UOfflineMainMenuWidget::LoginRequest(FString user, FString pass)
{
	username = user;

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("username"), *FString::Printf(TEXT("%s"), *user));
	JsonObject->SetStringField(TEXT("password"), *FString::Printf(TEXT("%s"), *pass));

	FString JsonBody;
	TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<>::Create(&JsonBody);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> LoginHttpRequest = Http->CreateRequest();
	LoginHttpRequest->SetVerb("POST");
	LoginHttpRequest->SetURL(ApiGatewayEndpoint + LoginURI);
	LoginHttpRequest->SetHeader("Content-Type", "application/json");
	LoginHttpRequest->SetContentAsString(JsonBody);
	LoginHttpRequest->OnProcessRequestComplete().BindUObject(this, &UOfflineMainMenuWidget::OnLoginResponse);
	LoginHttpRequest->ProcessRequest();
}

void UOfflineMainMenuWidget::OnLoginResponse(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;
	}

	FString idToken = JsonObject->GetObjectField("tokens")->GetStringField("idToken");
	StartSessionRequest(idToken);
}

void UOfflineMainMenuWidget::SignUpRequest(FString user, FString pass, FString email)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("username"), *FString::Printf(TEXT("%s"), *user));
	JsonObject->SetStringField(TEXT("password"), *FString::Printf(TEXT("%s"), *pass));
	JsonObject->SetStringField(TEXT("email"), *FString::Printf(TEXT("%s"), *email));

	FString JsonBody;
	TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<>::Create(&JsonBody);

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> LoginHttpRequest = Http->CreateRequest();
	LoginHttpRequest->SetVerb("POST");
	LoginHttpRequest->SetURL(ApiGatewayEndpoint + SignUpURI);
	LoginHttpRequest->SetHeader("Content-Type", "application/json");
	LoginHttpRequest->SetContentAsString(JsonBody);
	LoginHttpRequest->ProcessRequest();
}

void UOfflineMainMenuWidget::StartSessionRequest(FString idToken)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> StartSessionHttpRequest = Http->CreateRequest();
	StartSessionHttpRequest->SetVerb("GET");
	StartSessionHttpRequest->SetURL(ApiGatewayEndpoint + StartSessionURI);
	StartSessionHttpRequest->SetHeader("Content-Type", "application/json");
	StartSessionHttpRequest->SetHeader("Authorization", idToken);
	StartSessionHttpRequest->OnProcessRequestComplete().BindUObject(this, &UOfflineMainMenuWidget::OnStartSessionResponse);
	StartSessionHttpRequest->ProcessRequest();
}

void UOfflineMainMenuWidget::OnStartSessionResponse(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(response->GetContentAsString());

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		return;
	}

	FString IpAddress = JsonObject->GetObjectField("PlayerSession")->GetStringField("IpAddress");
	FString Port = JsonObject->GetObjectField("PlayerSession")->GetStringField("Port");
	JsonObject;

	if (IpAddress.IsEmpty() || Port.IsEmpty())
	{
		PrintErrorMessage();
		return;
	}

	FString LevelName = IpAddress + ":" + Port;
	UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelName));
}