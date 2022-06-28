// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaneSpawner.generated.h"

class UMillicastMediaSource;

UCLASS()
class MCPLAYERCPPDEMO_API APlaneSpawner : public AActor
{
	GENERATED_BODY()

	TArray<UMillicastMediaSource*> MediaSources;

public:	
	// Sets default values for this actor's properties
	APlaneSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MakeMediaSource(FString StreamName, FString AccountId);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
