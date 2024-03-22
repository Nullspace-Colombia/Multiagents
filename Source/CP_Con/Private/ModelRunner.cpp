// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelRunner.h"

// Sets default values
AModelRunner::AModelRunner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AModelRunner::BeginPlay()
{
	Super::BeginPlay();
	configureActionSpace();

	if (ModelData)
	{
		UE_LOG(LogTemp, Warning, TEXT("PreLoadedModelData loaded %s"), *ModelData->GetName());
		// You can use PreLoadedModelData here to create a model and corresponding model instance(s)
		// PreLoadedModelData will be unloaded when the owning object or actor unloads
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PreLoadedModelData is not set, please assign it in the editor"));
	}
	if (ModelData)
	{
		// Example for model creation
		TWeakInterfacePtr<INNERuntimeCPU> Runtime = UE::NNE::GetRuntime<INNERuntimeCPU>(FString("NNERuntimeORTCpu"));
		if (Runtime.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Valid Runtime"));
			//ModelHelper = MakeShared<FMyModelHelper>();

			TUniquePtr<UE::NNE::IModelCPU> Model = Runtime->CreateModel(ModelData);
			if (Model.IsValid())
			{
				ModelInstance = Model->CreateModelInstance();

				if (ModelInstance.IsValid())
				{

					UE_LOG(LogTemp, Warning, TEXT("Valid Model Instance"));
					//ModelHelper->bIsRunning = false;

					// Example for querying and testing in- and outputs
					TConstArrayView<UE::NNE::FTensorDesc> InputTensorDescs = ModelInstance->GetInputTensorDescs();

					UE::NNE::FSymbolicTensorShape SymbolicInputTensorShape = InputTensorDescs[0].GetShape();
					TArray<UE::NNE::FTensorShape> InputTensorShapes = { UE::NNE::FTensorShape::MakeFromSymbolic(SymbolicInputTensorShape) };

					ModelInstance->SetInputTensorShapes(InputTensorShapes);

					OutputTensorDescs = ModelInstance->GetOutputTensorDescs();

					UE::NNE::FSymbolicTensorShape SymbolicOutputTensorShape = OutputTensorDescs[0].GetShape();
					OutputTensorShapes = { UE::NNE::FTensorShape::MakeFromSymbolic(SymbolicOutputTensorShape) };

					UE_LOG(LogTemp, Error, TEXT("Outuput size: %d"), OutputTensorShapes[0].Rank());

					// Example for creating in- and outputs

					/*
					InputData.SetNumUninitialized(InputTensorShapes[0].Volume());
					InputBindings.SetNumZeroed(1);
					InputBindings[0].Data = InputData.GetData();
					InputBindings[0].SizeInBytes = InputData.Num() * sizeof(float);

					OutputData.SetNumUninitialized(OutputTensorShapes[0].Volume());
					OutputBindings.SetNumZeroed(1);
					OutputBindings[0].Data = OutputData.GetData();
					OutputBindings[0].SizeInBytes = OutputData.Num() * sizeof(float);
					*/

				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to create the model instance"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create the model"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot find runtime NNERuntimeORTCpu, please enable the corresponding plugin"));
		}
	}
}

// Called every frame
void AModelRunner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AModelRunner::configureActionSpace() {
	if (actionSpaceType == ActionSpace::Box || actionSpaceType == ActionSpace::MultiDiscrete || actionSpaceType == ActionSpace::MultiBinary) {
		float num;
		Shape.ParseIntoArray(Out, TEXT(","), true);
		
		for (int i = 0; i < Out.Num(); i++) {
			num = FCString::Atof(*Out[i]);
			nums.Add(num);
		}
	}
	else if (actionSpaceType == ActionSpace::Discrete) {
		float num;
		num = FCString::Atof(*Shape);
		nums.Add(num);
	}

	

}

TArray<float> AModelRunner::makeInference(TArray<double> observations) {

	//if (ModelHelper.IsValid()) {
		//if (!ModelHelper->bIsRunning)
		//{
			// Process ModelHelper->OutputData from the previous run here
			// Fill in new data into ModelHelper->InputData here
	InputData = observations;


	InputBindings.SetNumZeroed(1);
	InputBindings[0].Data = InputData.GetData();
	InputBindings[0].SizeInBytes = InputData.Num() * sizeof(double);
	UE_LOG(LogTemp, Warning, TEXT("Size of input data: %d"), InputData.Num());

	OutputData.SetNumUninitialized(OutputTensorShapes[0].Volume());
	UE_LOG(LogTemp, Warning, TEXT("Size of output data: %d"), OutputData.Num());
	OutputBindings.SetNumZeroed(1);
	OutputBindings[0].Data = OutputData.GetData();
	OutputBindings[0].SizeInBytes = OutputData.Num() * sizeof(float);

	if (ModelInstance->RunSync(InputBindings, OutputBindings) != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to run the model"));
	}
	Actions.Reset();
	TArray<float> output_actions = getActions(OutputData);
	return output_actions;

}

TArray<float> AModelRunner::getActions(TArray<float> OutputD) {
	float max=0;
	float value;
	int count = 0;
	float action_value;
	float std_dev;
	float mean;
	std::default_random_engine generator;

	if (actionSpaceType == ActionSpace::Box) {
		for (int i = 0; i < nums.Num(); i++) {
			for (int j = 0; j < nums[i]; j++) {
				std_dev = exp(OutputD[j * 2 + count]);
				mean = OutputD[j * 2 + count + 1];
				std::normal_distribution<float> distribution(mean, std_dev);
				action_value = distribution(generator);
				Actions.Add(action_value);
			}
			
			count = count + (nums[i] * 2);
		}
	}
	else {
		for (int i = 0; i < nums.Num(); i++) {
			for (int j = 0; j < nums[i]; j++) {
				value = OutputD[j + count];
				if (value > max) {
					max = value;
					action_value = j + count;
				}
			}
			Actions.Add(action_value);
			count = count + nums[i];
		}
	}
	return Actions;
	
}
