// Fill out your copyright notice in the Description page of Project Settings.

 
#include "Pole.h"
// Sets default values
APole::APole()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//otComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	//Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Cam->SetupAttachment(RootComponent);
	
	//
}

// Called when the game starts or when spawned
void APole::BeginPlay()
{
	Super::BeginPlay();
	//Open_Connection();

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


	//Conduct_Connection();

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
				//Sending a confirmation array:
				//TArray<uint8> Confirmation;
				//Confirmation.Add(0);
				//Confirmation.Add(1);
				//Confirmation.Add(2);
				//Confirmation.Add(3);
				//SendData(Confirmation);

				while (IsConnectionOpen) {
					uint32 size;
					TArray<uint8> ReceivedData;
					//int32 BytesSent = 0;
					
					if (ConnectionSocket->HasPendingData(size)) {
						//TArray<uint8> ReceivedData;



						ReceivedData.Init(0, 64);
						ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), bytesread);
							//ParseData(DataRecv, bytesread);
						OnDataReceptionDelegate.Broadcast(ReceivedData);
							//OnReceivedData(ReceivedData);
							//OnReceivedDataPtr(ReceivedData.GetData());
						
						/*
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
						DataToSend.Reset();
						*/
						ReceivedData.Reset();
					}
					
				}
				});
			//UE_LOG(LogTemp, Warning, TEXT("After thread execution"));
			
		}
	}
}

/*
void APole::OnReceivedData(TArray<uint8> DataR) {
	if (DataR.IsEmpty()) {
		DataR.Add(1);
	}
	OnDataReceptionDelegate.Broadcast(DataR);
}*/

/*
void APole::OnRData(float DataR) {
	OnDataReceiveDelegate.Broadcast(DataR);
}*/

/*
void APole::OnReceivedDataPtr(TArray<uint8>* DataR) {
	OnDataReceptionDelegate.Broadcast(DataR);
}*/

/*
void APole::RecibirEntero(int32 entero) {

}*/

void APole::GetReceivedData() {
	//DataReceptionDelegate.Broadcast(ReceivedData);
}

void APole::Reset_Env() {
	UE_LOG(LogTemp, Warning, TEXT("Environment Reset"));
	//FVector currLocation = Base->GetComponentLocation();
	//currLocation.X = 0;
	//Base->SetWorldLocation(currLocation);
}

void APole::StartServer(FString ipAddress, int32 port){
	if (!IsConnectionOpen) {
		UE_LOG(LogTemp, Warning, TEXT("Openning Connection"));
		IsConnectionOpen = true;
		WaitingForConnection = true;

		FIPv4Address IPAddress;
		FIPv4Address::Parse(ipAddress, IPAddress);
		FIPv4Endpoint Endpoint(IPAddress, (uint16)port);

		ListenSocket = FTcpSocketBuilder(TEXT("TcpSocket")).AsReusable();

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
		ListenSocket->Bind(*SocketSubsystem->CreateInternetAddr(Endpoint.Address.Value, Endpoint.Port));
		ListenSocket->Listen(1);
		UE_LOG(LogTemp, Warning, TEXT("Listening"));
	}
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

TArray<float> APole::ParseData(TArray<uint8> msg) {
	TArray<float> data_rcv;
	data_ptr = reinterpret_cast<float*>(msg.GetData());

	int buff_size = bytesread / 4;
	UE_LOG(LogTemp, Warning, TEXT("Buffer Size: %d"), bytesread);

	for (int idx = 0; idx < (int)buff_size; idx++) {
		data = *(data_ptr + idx);
		data_rcv.Add(data);
	}
	UE_LOG(LogTemp, Warning, TEXT("Array size: %d"), data_rcv.Num());
	return data_rcv;
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
	
	float* DataS;
	for (int idx = 0; idx < msg.Num(); idx++) {
		int32 BytesSent = 0;
		DataS = msg.GetData() + idx;
		ConnectionSocket->Send(reinterpret_cast<uint8*>(DataS), msg.Num(), BytesSent);
		/*
		UE_LOG(LogTemp, Warning, TEXT("Data Sent: %f"), *DataS);
		UE_LOG(LogTemp, Warning, TEXT("Bytes Sent: %d"), BytesSent);
		UE_LOG(LogTemp, Warning, TEXT("Bytes Sent: %d"), msg.Num());
		*/
	}
	

}

bool APole::CheckDone(float done) {
	bool is_done;
	
	if (done == 1.0){
		is_done = true;
	}
	else if (done == 0.0){
		is_done = false;
	}
	return is_done;
}




