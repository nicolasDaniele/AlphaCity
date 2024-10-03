#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Teleportable.h"
#include "AlphaCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class ALPHACITY_API AAlphaCharacter : public ACharacter, public ITeleportable
{
	GENERATED_BODY()

public:
	AAlphaCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE TObjectPtr<USpringArmComponent> GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE TObjectPtr<UCameraComponent> GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Teleport(const FVector targetLocation, const FVector targetDirection, const FRotator targetRotation);

	void TestFunction(const FString& LevelToTravel);
	UFUNCTION(BlueprintImplementableEvent) void BlueprintTestFunction(const FString& LevelToTravel);
	UFUNCTION(Server, Reliable) void Server_TestFunction(const FString& LevelToTravel);
	UFUNCTION(Client, Reliable) void Client_TestFunction(const FString& LevelToTravel);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = Camera) TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditDefaultsOnly, Category = Camera) TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = Input) UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = Input) UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = Input) UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = Input) UInputAction* LookAction;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
};
