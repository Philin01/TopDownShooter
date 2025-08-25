// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FuncLibrary/Types.h"
#include "InteractableItems/WeaponDefault.h"
#include "TDSCharacter.generated.h"

UCLASS(Blueprintable)
class ATDSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATDSCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComp) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	//FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UDecalComponent* CursorToWorld;

protected:

	virtual void BeginPlay() override;

public:

	//Cursor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.f, 40.f, 40.f);

	UDecalComponent* CurrentCursor = nullptr;

	//Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementState = EMovementState::Run_State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FCharacterSpeed MovementSpeedInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool SprintRunEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool WalkEnable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool AimEnable = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float ResSpeed = MovementSpeedInfo.RunSpeed;

	//Weapon
	AWeaponDefault* CurrentWeapon = nullptr;

	//for demo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Demo")
	TSubclassOf<AWeaponDefault> InitWeaponClass = nullptr;


	//Inputs
	UFUNCTION()
	void InputAxisX(float Value);
	UFUNCTION()
	void InputAxisY(float Value);
	UFUNCTION()
	void InputAttackPressed();
	UFUNCTION()
	void InputAttackReleased();

	float AxisX;
	float AxisY;
	//Tick func
	UFUNCTION()
	void MovementTick(float DeltaTime);


	//Func
	UFUNCTION(BlueprintCallable)
	void AttackCharEvent(bool bIsFiring);
	UFUNCTION(BlueprintCallable)
	void CharacterUpdate();
	UFUNCTION(BlueprintCallable)
	void ChangeMovementState();
	UFUNCTION(BlueprintCallable)
	bool IsForvardMove();

	UFUNCTION(BlueprintCallable)
	AWeaponDefault* GetCurrentWeapon();
	UFUNCTION(BlueprintCallable)
	void InitWeapon();

	UFUNCTION(BlueprintCallable)
	UDecalComponent* GetCursorToWorld();
	
};

