// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItemDefault.h"

// Sets default values
AWeaponItemDefault::AWeaponItemDefault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponItemDefault::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponItemDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

