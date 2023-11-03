// Copyright (c) 2023 Unray. All rights reserved.

#include "Pole.h"
// Sets default values
APole::APole()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APole::BeginPlay()
{
	Super::BeginPlay();

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
			connected = true;
			// Start Recv Thread
			ClientConnectionFinishedFuture = Async(EAsyncExecution::Thread, [&]() {
				UE_LOG(LogTemp, Warning, TEXT("recv thread started in port: %d"), ListenSocket->GetPortNo());
				//Sending a confirmation array:

				while (IsConnectionOpen) {
					uint32 size;
					TArray<uint8> ReceivedData;
					
					if (ConnectionSocket->HasPendingData(size)) {



						ReceivedData.Init(0, 128);
						ConnectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), bytesread);
						
						OnDataReceptionDelegate.Broadcast(ReceivedData);

						ReceivedData.Reset();
					}

					
				}
				});
			
		}
	}
}




void APole::GetReceivedData() {
	
}

void APole::StartServer(FString ipAddress, int32 port){

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

TArray<float> APole::GetAction(TArray<uint8> msg) {

	TArray<float> data_rcv;
	data_ptr = reinterpret_cast<float*>(msg.GetData());

	int buff_size = bytesread / 4;

	for (int idx = 0; idx < (int)buff_size; idx++) {
			data = *(data_ptr + idx);
			data_rcv.Add(data);
	}
	return data_rcv;

}

void APole::SendData(TArray<double> msg) {
	

	int32 BytesSent = 0;
	int32 BufferSize = msg.Num() * 8;

	ConnectionSocket->Send(reinterpret_cast<uint8*>(msg.GetData()), BufferSize, BytesSent);


}
void APole::SendMultiagentsInfo(TArray<double> Multiagents) {
	SendData(Multiagents);
}
void APole::SendState(TArray<float> Observations, float Reward, bool Done) {

	TArray<double> state;
	double obs_data;
	for (int i = 0; i < Observations.Num(); i++) {
		obs_data = *(Observations.GetData() + i);
		state.Add(StaticCast<double>(obs_data));

	}
	double s_reward = StaticCast<double>(Reward);
	double s_done = StaticCast<double>(Done);
	state.Add(s_reward);
	state.Add(s_done);
	SendData(state);

}

// Orden: Identificador, Longitud de observaciones, Arreglo de observaciones, Reward, Done
TArray<double> APole::AddAgent(int ID, TArray<float> Observations, float Reward, bool Done) {
	TArray<double> AgentInfo;
	double s_id = StaticCast<double>(ID);
	AgentInfo.Add(s_id);
	double obs_data;
	for (int i = 0; i < Observations.Num(); i++) {
		obs_data = *(Observations.GetData() + i);
		AgentInfo.Add(StaticCast<double>(obs_data));

	}
	double s_reward = StaticCast<double>(Reward);
	double s_done = StaticCast<double>(Done);
	
	AgentInfo.Add(s_reward);
	AgentInfo.Add(s_done);

	return AgentInfo;
}

TArray<double> APole::GetAgentsInfo(TArray<double> A, TArray<double> B) {
	TArray<double> Agents;
	Agents = A;
	Agents.Append(B);
	return Agents;
}





