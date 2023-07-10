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
			connected = true;
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



						ReceivedData.Init(0, 128);
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

TArray<float> APole::GetAction(TArray<uint8> msg) {
	/*if (msg.Num() != n_actions) {
		TArray<float> empty_actions;
		UE_LOG(LogTemp, Error, TEXT("Number of Actions does not match expected size :("));
		return empty_actions;
	}*/

	TArray<float> data_rcv;
	data_ptr = reinterpret_cast<float*>(msg.GetData());

	int buff_size = bytesread / 4;
	//UE_LOG(LogTemp, Warning, TEXT("Buffer Size: %d"), bytesread);

	for (int idx = 0; idx < (int)buff_size; idx++) {
			data = *(data_ptr + idx);
			data_rcv.Add(data);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Array size: %d"), data_rcv.Num());
	return data_rcv;

}

void APole::SendData(TArray<double> msg) {
	
	double* DataS;
	int32 BytesSent = 0;
	/*if (msg.Num() - 2 != n_obs) {
		UE_LOG(LogTemp, Error, TEXT("Number of Observations does not match expected size :("));
	}*/
	
		for (int idx = 0; idx < msg.Num(); idx++) {

			DataS = msg.GetData() + idx;
			ConnectionSocket->Send(reinterpret_cast<uint8*>(DataS), 8, BytesSent);
			//UE_LOG(LogTemp, Warning, TEXT("Sending: %f"), *DataS);
			//UE_LOG(LogTemp, Warning, TEXT("BytesSent: %d"), BytesSent);
		}
	

}
void APole::SendMultiagentsInfo(TArray<double> Multiagents) {
	SendData(Multiagents);
}
void APole::SendState(TArray<float> Observations, float Reward, bool Done) {

	TArray<double> state;
	double obs_data;
	for (int i = 0; i < Observations.Num(); i++) {
		obs_data = *(Observations.GetData() + i);
		//UE_LOG(LogTemp, Warning, TEXT("Data: %f"), obs_data);
		state.Add(StaticCast<double>(obs_data));

	}
	double s_reward = StaticCast<double>(Reward);
	double s_done = StaticCast<double>(Done);
	state.Add(s_reward);
	state.Add(s_done);
	SendData(state);

}


void APole::SendAgentInfo(FString Agent_ID, TArray<float> Agent_Observations, float Agent_Reward, bool Agent_Done) {

	FRLAgent Agent;
	Agent.ID = Agent_ID;
	Agent.Observations = Agent_Observations;
	Agent.Reward = Agent_Reward;
	Agent.Done = Agent_Done;
	Agent.Action = 0;

	FString JSONPayload;
	FJsonObjectConverter::UStructToJsonObjectString(Agent, JSONPayload, 0, 0);
	//UE_LOG(LogTemp, Warning, TEXT("Data: %s"), *JSONPayload);
	//UE_LOG(LogTemp, Warning, TEXT("Size of Data: %d"), JSONPayload.Len());
	//ConnectionSocket->Send((uint8*)&JSONPayload, sizeof(JSONPayload), BytesSent);
	
	FString* DataS;
	FString* Json_Array = &(JSONPayload);
	int32 BytesSent = 0;

	for (int idx = 0; idx < JSONPayload.Len(); idx++) {
		DataS =  Json_Array + idx;
		ConnectionSocket->Send((uint8*)DataS, sizeof(DataS), BytesSent);
		//UE_LOG(LogTemp, Warning, TEXT("BytesSent: %d"), BytesSent);
	}
}

void APole::GetAgentInfo(TArray<uint8> msg, FString &AgentID, float &AgentAction) {
	FRLAgent Agent;
	FString JSONInfo;

	//const std::string cstr(reinterpret_cast<const char*>(msg.GetData()), msg.Num());

	//JSONInfo = FString(cstr.c_str());

	//FJsonObjectConverter::JsonObjectStringToUStruct(JSONInfo, &Agent, 0, 0);
	//AgentID = Agent.ID;
	//AgentAction = Agent.Action;
}

void APole::SetSpaces(int obs, int actions) {
	n_obs = obs;
	n_actions = actions;
}

// Orden: Identificador, Longitud de observaciones, Arreglo de observaciones, Reward, Done
TArray<double> APole::AddAgent(int ID, TArray<float> Observations, float Reward, bool Done) {
	TArray<double> AgentInfo;
	double s_id = StaticCast<double>(ID);
	AgentInfo.Add(s_id);
	double obs_data;
	for (int i = 0; i < Observations.Num(); i++) {
		obs_data = *(Observations.GetData() + i);
		//UE_LOG(LogTemp, Warning, TEXT("Data: %f"), obs_data);
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





