// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/WorldSettings.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Networking.h"
#include "Sockets.h"
#include "Async/Async.h"
#include "SocketSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Pole.generated.h"


UCLASS()
class CP_CON_API APole : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsConnectionOpen = false;
	bool WaitingForConnection = false;

	bool HasInput = false;
	bool AppliedInput = false;
	int Input_ = 0;
	TFuture<void> ClientConnectionFinishedFuture;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Base;

	UPROPERTY(EditAnywhere)
		USceneComponent* Cam;

	UPROPERTY(EditAnywhere)
		USceneComponent* _BaseComponent;

	UPROPERTY(EditAnywhere)
		AWorldSettings* WorldSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<uint8> ReceivedData;



	uint8* DataRecv = new uint8[32];

	uint32 buffsize = 0;
	int32 bytesread = 0;
	float data;
	float* data_ptr;
	void Open_Connection();
	void Close_Connection();
	void Conduct_Connection();
	void ParseData(uint8* msg);

	//UFUNCTION(BlueprintCallable)
		//TArray<uint8> get_Data();

	void Reset_Env();

	float Timer = 0;

	FSocket* ListenSocket;
	FSocket* ConnectionSocket;
};