// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


ATDSCharacter::ATDSCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	/*CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Blueprints/Character/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());*/

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATDSCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	/*if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}*/

	if (CurrentCursor)
	{
		APlayerController* myPC = Cast<APlayerController>(GetController());
		if (myPC)
		{
			FHitResult TraceHitResult;
			myPC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();

			CurrentCursor->SetWorldLocation(TraceHitResult.Location);
			CurrentCursor->SetWorldRotation(CursorR);
		}
	}

	MovementTick(DeltaSeconds);
}

void ATDSCharacter::SetupPlayerInputComponent(UInputComponent* InputComp)
{
	Super::SetupPlayerInputComponent(InputComp);

	InputComp->BindAxis(TEXT("MoveForward"), this, &ATDSCharacter::InputAxisX);
	InputComp->BindAxis(TEXT("MoveRight"), this, &ATDSCharacter::InputAxisY);

	InputComp->BindAction(TEXT("FireEvent"), EInputEvent::IE_Pressed, this, &ATDSCharacter::InputAttackPressed);
	InputComp->BindAction(TEXT("FireEvent"), EInputEvent::IE_Released, this, &ATDSCharacter::InputAttackReleased);
}

void ATDSCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitWeapon();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

void ATDSCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void ATDSCharacter::InputAxisY(float Value)
{
	AxisY = Value;
}

void ATDSCharacter::InputAttackPressed()
{
	AttackCharEvent(true);
}

void ATDSCharacter::InputAttackReleased()
{
	AttackCharEvent(false);
}

void ATDSCharacter::MovementTick(float DeltaTime)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisX);
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), AxisY);

	

	APlayerController* myController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(myController))
	{
		FHitResult HitResult;
		myController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), true, HitResult);
		FVector HitLocation = HitResult.Location;
		UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitLocation);
		float FindRotationResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitLocation).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotationResultYaw, 0.0f)));
	}

}

void ATDSCharacter::AttackCharEvent(bool bIsFiring)
{
	AWeaponDefault* myWeapon = nullptr;
	myWeapon = GetCurrentWeapon();
	if (myWeapon)
	{
		myWeapon->SetWeaponStateFire(bIsFiring);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATPSCharacter::AttackCrarEvent - CurrentWeapon -NULL"));
	}
}

void ATDSCharacter::CharacterUpdate()
{
	switch (MovementState)
	{
	case EMovementState::Aim_State:
		ResSpeed = MovementSpeedInfo.AimSpeed;
		break;
	case EMovementState::Walk_State:
		ResSpeed = MovementSpeedInfo.WalkSpeed;
		break;
	case EMovementState::AimWalk_State:
		ResSpeed = MovementSpeedInfo.AimWalkSpeed;
		break;
	case EMovementState::Run_State:
		ResSpeed = MovementSpeedInfo.RunSpeed;
		break;
	case EMovementState::SprintRun_State:
		ResSpeed = MovementSpeedInfo.SprintRunSpeed;
		break;
	default:
		break;
	}

	GetCharacterMovement()->MaxWalkSpeed = ResSpeed;
}

void ATDSCharacter::ChangeMovementState()
{
	if (!WalkEnable && !AimEnable && !SprintRunEnable)
	{
		MovementState = EMovementState::Run_State;
	}
	else 
	{
		if (SprintRunEnable && IsForvardMove())
		{
			WalkEnable = false;
			AimEnable = false;
			MovementState = EMovementState::SprintRun_State;
		}
		if (WalkEnable && !SprintRunEnable && AimEnable)
		{
			MovementState = EMovementState::AimWalk_State;
		}
		else
		{
			if (!WalkEnable && !SprintRunEnable && AimEnable)
			{
				MovementState = EMovementState::Aim_State;
			}
			else
			{
				if (WalkEnable && !SprintRunEnable && !AimEnable)
				{
					MovementState = EMovementState::Walk_State;
				}
			}
		}
	}
	CharacterUpdate();
}

bool ATDSCharacter::IsForvardMove()
{
	FVector fVector = GetActorForwardVector();
	FVector lmVector = GetLastMovementInputVector();
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f = FloatVariable"), FVector::DotProduct(fVector, lmVector)));
	return FVector::DotProduct(fVector, lmVector) > 0.9f;
}

AWeaponDefault* ATDSCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

void ATDSCharacter::InitWeapon()
{
	if (InitWeaponClass)
	{
		FVector SpawnLocation = FVector(0);
		FRotator SpawnRotation = FRotator(0);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();

		AWeaponDefault* myWeapon = Cast<AWeaponDefault>(GetWorld()->SpawnActor(InitWeaponClass,
			&SpawnLocation, &SpawnRotation, SpawnParams));
		if (myWeapon)
		{
			FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
			myWeapon->AttachToComponent(GetMesh(), Rule, FName("WeaponSocketRightHand"));
			CurrentWeapon = myWeapon;

			myWeapon->UpdateStateWeapon(MovementState);
		}
	}
}

UDecalComponent* ATDSCharacter::GetCursorToWorld()
{
	return CurrentCursor;
}
