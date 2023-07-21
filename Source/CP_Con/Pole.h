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
	int n_obs = 0;
	int n_actions = 0;
	
	
	TArray<uint8> DataToSend;
	float* DataSend;

	uint8* DataSendPtr;
	uint32 buffsize = 0;
	int32 bytesread = 0;
	
	float* data_ptr;
	void Open_Connection();
	void Close_Connection();

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void GetAgentInfo(TArray<uint8> msg, FString& AgentID, float &AgentAction);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void Conduct_Connection();

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void ConnectionClient();

	UFUNCTION(BlueprintCallable, Category = "Socket")
		TArray<float>  GetAction(TArray<uint8> msg);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void SendState(TArray<float> Observations, float Reward, bool Done);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void SendAgentInfo(FString Agent_ID, TArray<float> Agent_Observations, float Agent_Reward, bool Agent_Done);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		TArray<double> AddAgent(int ID, TArray<float> Observations, float Reward, bool Done);

	UFUNCTION(BlueprintPure, meta = (CommutativeAssociativeBinaryOperator = "true"), Category = "Socket")
		TArray<double> GetAgentsInfo(TArray<double> A, TArray<double> B);


	UFUNCTION(BlueprintCallable, Category = "Socket")
		void SendMultiagentsInfo(TArray<double> Multiagents);
	void SendData(TArray<double> msg);

	//UFUNCTION(BlueprintCallable, Category = "Training")
		//bool CheckDone(float done);
	UFUNCTION(BlueprintCallable, Category = "Socket")
		void StartServer(FString ipAddress, int32 port);

		UFUNCTION(BlueprintCallable, Category = "Socket")
		void SetSpaces(int obs, int actions);

	UFUNCTION(BlueprintCallable, Category = "Socket")
		void StartClient(FString ipAddress, int32 port);

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


	float Timer = 0;

	FSocket* ListenSocket;
	FSocket* ConnectionSocket;
	FSocket* ClientSocket;
};

USTRUCT(BlueprintType)
struct FRLAgent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Reward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Observations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Done;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Action;
};
