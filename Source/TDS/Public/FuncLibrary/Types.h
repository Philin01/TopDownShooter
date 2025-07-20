// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Aim_State UMETA(DisplayName = "Aim State"),
	Walk_State UMETA(DisplayName = "Walk State"),
	AimWalk_State UMETA(DisplayName = "AimWalk State"),
	Run_State UMETA(DisplayName = "Run State"),
	SprintRun_State UMETA(DisplayName = "SprintRun State"),
};

USTRUCT(BlueprintType)
struct FCharacterSpeed
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimSpeed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AimWalkSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintRunSpeed = 800.f;
};

UCLASS()
class TDS_API UTypes : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()
};