// Fill out your copyright notice in the Description page of Project Settings.

 
#include "Pole.h"
// Sets default values
APole::APole()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Cam->SetupAttachment(RootComponent);
	
	//
}

// Called when the game starts or when spawned
void APole::BeginPlay()
{
	Super::BeginPlay();
	Open_Connection();

}


// End game
void APole::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	Close_Connection();
}


// Called every frame
void APole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	Conduct_Connection();

}

// Called to bind functionality to input
void APole::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APole::Conduct_Connection() {

	if (WaitingForConnection) {
		TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		bool hasConnection = false;
		if (ListenSocket->HasPendingConnection(hasConnection) && hasConnection) {
			ConnectionSocket = ListenSocket->Accept(*RemoteAddress, TEXT("Connection"));
			WaitingForConnection = false;
			UE_LOG(LogTemp, Warning, TEXT("incoming connection"));

			// Start Recv Thread
			ClientConnectionFinishedFuture = Async(EAsyncExecution::LargeThreadPool, [&]() {
				UE_LOG(LogTemp, Warning, TEXT("recv thread started"));
				while (IsConnectionOpen) {
					uint32 size;

					if (ConnectionSocket->HasPendingData(size)) {
						//TArray<uint8> ReceivedData;
						ReceivedData.Init(0, 10);
						if (ConnectionSocket->Recv(DataRecv, size, bytesread)) {
							ParseData(DataRecv);

						}
					}
				}
				});
		}
	}
}

void APole::Reset_Env() {
	UE_LOG(LogTemp, Warning, TEXT("Environment Reset"));
	FVector currLocation = Base->GetComponentLocation();
	currLocation.X = 0;
	Base->SetWorldLocation(currLocation);
}

void APole::Open_Connection() {
	if (!IsConnectionOpen) {
		UE_LOG(LogTemp, Warning, TEXT("Openning Connection"));
		IsConnectionOpen = true;
		WaitingForConnection = true;

		FIPv4Address IPAddress;
		FIPv4Address::Parse(FString("127.0.0.1"), IPAddress);
		FIPv4Endpoint Endpoint(IPAddress, (uint16)10000);

		ListenSocket = FTcpSocketBuilder(TEXT("TcpSocket")).AsReusable();

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		ListenSocket->Bind(*SocketSubsystem->CreateInternetAddr(Endpoint.Address.Value, Endpoint.Port));
		ListenSocket->Listen(1);
		UE_LOG(LogTemp, Warning, TEXT("Listening"));
	}
}

void APole::Close_Connection() {
	if (IsConnectionOpen) {
		UE_LOG(LogTemp, Warning, TEXT("Closing Connection"));
		IsConnectionOpen = false;

		ListenSocket->Close();
	}
}

/*
void APole::ParseData(uint8* msg) {
	data_ptr = reinterpret_cast<float*>(msg);
	UE_LOG(LogTemp, Warning, TEXT("Received data: %f"), *(data_ptr));
	UE_LOG(LogTemp, Warning, TEXT("Received data: %f"), *(data_ptr + 1));
	UE_LOG(LogTemp, Warning, TEXT("Received data: %f"), *(data_ptr + 2));
	UE_LOG(LogTemp, Warning, TEXT("Received data: %f"), *(data_ptr + 3));
}*/

// buffsize 
void APole::ParseData(uint8* msg) {
	data_ptr = reinterpret_cast<float*>(msg);
	int buff_size = (int)*data_ptr;

	for (int idx = 1; idx < buff_size + 1; idx++) {
		UE_LOG(LogTemp, Warning, TEXT("Received data: %f"), *(data_ptr + idx));
	}

}