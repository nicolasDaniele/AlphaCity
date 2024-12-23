#include "AlphaCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PhysicsVolume.h"

#include "Kismet/GameplayStatics.h"
#include "AlphaPlayerController.h"
#include "AlphaGameMode.h"

AAlphaCharacter::AAlphaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	swimState = ESwimState::NONE;
}

void AAlphaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAlphaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAlphaCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAlphaCharacter::Look);
	}
}

void AAlphaCharacter::StartSwimming_Implementation()
{
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = true;
	swimState = ESwimState::SWIMMING;
}

void AAlphaCharacter::StopSwimming_Implementation()
{
	GetCharacterMovement()->GetPhysicsVolume()->bWaterVolume = false;
	swimState = ESwimState::NONE;
}

void AAlphaCharacter::Move(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}
	
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Swimming)
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(RightDirection, MovementVector.X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
}

void AAlphaCharacter::Look(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		return;
	}

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AAlphaCharacter::Teleport_Implementation(const FVector targetLocation, const FVector targetDirection, const FRotator targetRotation)
{
}

void AAlphaCharacter::TryTravel(const FString& LevelToTravel)
{
	AAlphaPlayerController* alphaPC = Cast<AAlphaPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (alphaPC == nullptr) return;
	/*if (alphaPC != nullptr)
	{
		alphaPC->TryTravelToLevel(LevelToTravel);
	}*/

	AAlphaGameMode* alphaGM = Cast<AAlphaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (alphaGM != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 12.f, FColor::White,
			FString::Printf(TEXT("AlphaCharacter Trying to Travel to Level: %s"),
				*LevelToTravel));

		//alphaGM->MakeClientTravel(alphaPC, LevelToTravel);
	}
}