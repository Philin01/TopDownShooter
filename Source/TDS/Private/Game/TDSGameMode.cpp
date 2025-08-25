// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game/TDSGameMode.h"
#include "Game/TDSPlayerController.h"
#include "Character/TDSCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATDSGameMode::ATDSGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATDSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}