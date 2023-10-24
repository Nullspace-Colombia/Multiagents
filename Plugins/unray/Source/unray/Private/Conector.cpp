// Fill out your copyright notice in the Description page of Project Settings.


#include "Conector.h"

// Sets default values
AConector::AConector()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AConector::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

