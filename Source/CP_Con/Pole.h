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

//Declaring the delegates signature
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataReceptionSignature, TArray<uint8>, DataR);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataReception, float, DataR);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataReceptionPtr, TArray<uint8>*, DataRecv);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataReception, const TArray<uint8>&, Arr);

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

	UPROPERTY(BlueprintReadOnly)
		bool IsConnectionOpen = false;
	
	UPROPERTY(BlueprintReadOnly)
		bool connected = false;

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

	//UPROPERTY(EditAnywhere)
		//UStaticMeshComponent* Base;

	//UPROPERTY(EditAnywhere)
		//USceneComponent* Cam;

	//UPROPERTY(EditAnywhere)
		//USceneComponent* _BaseComponent;

	UPROPERTY(EditAnywhere)
		AWorldSettings* WorldSettings;


		

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float data = 0;




	uint8* DataRecv = new uint8[32];
	TArray<float> DataSnd;
	
	TArray<uint8> DataToSend;
	float* DataSend;

	uint8* DataSendPtr;
	uint32 buffsize = 0;
	int32 bytesread = 0;

	float* data_ptr;
	void Open_Connection();
	void Close_Connection();

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void Conduct_Connection();

	UFUNCTION(BlueprintCallable, Category = "Socket")
		TArray<float>  ParseData(TArray<uint8> msg);

	void SendData(TArray<uint8> msg);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SendFloat"), Category = "Socket")
		void SendFloat(TArray<float> msg);

	UFUNCTION(BlueprintCallable, Category = "Training")
		bool CheckDone(float done);
	UFUNCTION(BlueprintCallable, Category = "Socket")
		void StartServer(FString ipAddress, int32 port);


	//UFUNCTION(BlueprintCallable)
		//void OnRData(float DataR);

	//UFUNCTION(BlueprintCallable)
	//void OnReceivedDataPtr(TArray<uint8>* DataR);

	//Creating the variables of delegate type (other functions can subscrite to this function)
	//UPROPERTY(BlueprintAssignable)
	//FOnDataReceptionSignature OnDataReceptionDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnDataReception OnDataReceptionDelegate;

	//UPROPERTY(BlueprintAssignable)
	//FOnLenArray OnLenArrayDelegate;

	UFUNCTION(BlueprintCallable)
	void GetReceivedData();

	//UFUNCTION(BlueprintCallable)
	//void RecibirEntero(int32 entero);
	//UPROPERTY(BlueprintAssignable)
	//FOnDataReceptionPtr OnDataReceiveDelegatePtr;

	//void ReturnReceivedData(TArray<uint8> msg);
	//UFUNCTION(BlueprintCallable)
		//TArray<uint8> get_Data();

	void Reset_Env();

	float Timer = 0;

	FSocket* ListenSocket;
	FSocket* ConnectionSocket;
};
