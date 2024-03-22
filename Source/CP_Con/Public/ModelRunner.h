// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <random>

#include "NNE.h"
#include "NNERuntimeCPU.h"
#include "NNEModelData.h"

#include "ModelRunner.generated.h"


UENUM(BlueprintType)
enum ActionSpace {
	Discrete      UMETA(DisplayName = "Discrete"),
	MultiDiscrete       UMETA(DisplayName = "MultiDiscrete"),
	Box        UMETA(DisplayName = "Box"),
	MultiBinary      UMETA(DisplayName = "MultiBinary"),
};


UCLASS()
class CP_CON_API AModelRunner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AModelRunner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="Inference")
	TObjectPtr<UNNEModelData> ModelData;

	UFUNCTION(BlueprintCallable, Category = "Inference")
	TArray<float>  makeInference(TArray<double> observations);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inference")
	TEnumAsByte < ActionSpace > actionSpaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inference")
	FString Shape;

	void configureActionSpace();

	TArray<float> getActions(TArray<float> OutputD);

	TArray<float> Actions;

	TConstArrayView<UE::NNE::FTensorDesc> OutputTensorDescs;
	TArray<UE::NNE::FTensorShape> OutputTensorShapes;
	TUniquePtr<UE::NNE::IModelInstanceCPU> ModelInstance;
	TArray<double> InputData;
	TArray<float> OutputData;
	TArray<UE::NNE::FTensorBindingCPU> InputBindings;
	TArray<UE::NNE::FTensorBindingCPU> OutputBindings;
	TArray<FString> Out;
	TArray<float> nums;
	float discrete_shape;

};
