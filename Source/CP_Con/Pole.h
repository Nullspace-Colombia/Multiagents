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
#include "Json.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Pole.generated.h"




//Declaring the delegates signature
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float data = 0;
			

	int32 bytesread = 0;
	
	float* data_ptr;
	void Open_Connection();
	void Close_Connection();

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void Conduct_Connection();

	UFUNCTION(BlueprintCallable, Category = "Socket")
		TArray<float>  GetAction(TArray<uint8> msg);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void SendState(TArray<float> Observations, float Reward, bool Done);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		TArray<double> AddAgent(int ID, TArray<float> Observations, float Reward, bool Done);

	UFUNCTION(BlueprintPure, meta = (CommutativeAssociativeBinaryOperator = "true"), Category = "Socket")
		TArray<double> GetAgentsInfo(TArray<double> A, TArray<double> B);


	UFUNCTION(BlueprintCallable, Category = "Socket")
		void SendMultiagentsInfo(TArray<double> Multiagents);
	void SendData(TArray<double> msg);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void StartServer(FString ipAddress, int32 port);
	
	UPROPERTY(BlueprintAssignable)
	FOnDataReception OnDataReceptionDelegate;

	UFUNCTION(BlueprintCallable)
	void GetReceivedData();

	FSocket* ListenSocket;
	FSocket* ConnectionSocket;
};

