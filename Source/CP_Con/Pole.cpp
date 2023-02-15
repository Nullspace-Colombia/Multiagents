// Fill out your copyright notice in the Description page of Project Settings.

 
#include "Pole.h"
// Sets default values
APole::APole()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//otComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
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
					//int32 BytesSent = 0;

					if (ConnectionSocket->HasPendingData(size)) {
						//TArray<uint8> ReceivedData;
						
						UE_LOG(LogTemp, Warning, TEXT("Size of the data pending:  %d"), size);
					

						if (ConnectionSocket->Recv(DataRecv, size, bytesread)) {
							ParseData(DataRecv, bytesread);
						}
						FVector currLocation = Base->GetComponentLocation();
						DataSnd.Add(currLocation.X);
						DataSnd.Add(currLocation.Y);
						DataSnd.Add(currLocation.Z);
						DataSnd.Add(currLocation.X);

						DataToSend.Add(currLocation.X);
						DataToSend.Add(currLocation.Y);
						DataToSend.Add(currLocation.Z);
						DataToSend.Add(currLocation.X);
						
						SendData(DataToSend);
						/*
						UE_LOG(LogTemp, Warning, TEXT("Size of the data to send:  %d"), DataSnd.Num());
						UE_LOG(LogTemp, Warning, TEXT("----------------------------------"));
						UE_LOG(LogTemp, Warning, TEXT("Posici�n en X:  %f"), currLocation.X);
						UE_LOG(LogTemp, Warning, TEXT("Posici�n en Y:  %f"), currLocation.Y);
						UE_LOG(LogTemp, Warning, TEXT("Posici�n en Z:  %f"), currLocation.Z);
						UE_LOG(LogTemp, Warning, TEXT("----------------------------------"));
						*/
						DataSnd.Reset();
					

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

void APole::ParseData(uint8* msg, uint32 size) {
	data_ptr = reinterpret_cast<float*>(msg);
	int buff_size = (int)*data_ptr;
	

	for (int idx = 1; idx < (int)buff_size + 1; idx++) {
		data = *(data_ptr + idx);
		UE_LOG(LogTemp, Warning, TEXT("Received data: %f"), data);
		
	}
}


void APole::SendData(TArray<uint8> msg) {
	int32 BytesSent = 0;
	uint8* DataS;
	for (int idx = 0; idx < msg.Num(); idx++) {
		DataS = msg.GetData() + idx;
		ConnectionSocket->Send(DataS, msg.Num(), BytesSent);
	}
}

void APole::SendFloat(TArray<float> msg) {
	int32 BytesSent = 0;
	float* DataS;
	for (int idx = 0; idx < msg.Num(); idx++) {
		DataS = msg.GetData() + idx;
		ConnectionSocket->Send(reinterpret_cast<uint8*>(DataS), msg.Num(), BytesSent);
	}

}
