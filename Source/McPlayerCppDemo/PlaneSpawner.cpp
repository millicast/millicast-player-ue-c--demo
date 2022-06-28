// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneSpawner.h"
#include "MillicastMediaSource.h"

#include "PlanePlayer.h"

// Sets default values
APlaneSpawner::APlaneSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APlaneSpawner::MakeMediaSource(FString StreamName, FString AccountId)
{
	// Create the media source object
	auto MediaSource = NewObject<UMillicastMediaSource>();
	MediaSource->StreamName = MoveTemp(StreamName); 
	MediaSource->AccountId = MoveTemp(AccountId);
	MediaSource->StreamUrl = "https://director.millicast.com/api/director/subscribe";

	MediaSources.Push(MediaSource);
}

// Called when the game starts or when spawned
void APlaneSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	MakeMediaSource("", ""); // Set your stream name / account id here
	// MakeMediaSource("", "");
	// MakeMediaSource("", "");
	// MakeMediaSource("", "");

	// Constants
	constexpr auto NUMBER_HORIZONTAL = 5llu; // Number of planes that fit horizontally
	constexpr auto NUMBER_VERTICAL = 4llu; // Number of planes that fit vertically
	constexpr auto SIZE_Y = 180.f; // Distance between the center of two plane
	constexpr auto SIZE_Z = 170.f; // Distance between the center of two plane
	constexpr auto ROTATION = 90.f; // rotate the plane by 90 degrees so it is in front of us
	// -- Start coordinates
	constexpr int START_X = 1430.f;
	constexpr int START_Y = -360.f;
	constexpr int START_Z = 220.f;

	for (size_t i = 0; i < NUMBER_HORIZONTAL; ++i)
	{
		for (size_t j = 0; j < NUMBER_VERTICAL; ++j)
		{
			FActorSpawnParameters ActorSpawnParameters;

			// Localisation of the plane
			FVector Loc = { START_X, START_Y + SIZE_Y * i, START_Z + SIZE_Z * j };
			// Rotation of the plane. Make quaternions from euler angles.
			FRotator Rot = FRotator::MakeFromEuler({ 90.f, 0.f, 90.f });

			// Spawn the plane
			APlanePlayer* Plane = GetWorld()->SpawnActor<APlanePlayer>(APlanePlayer::StaticClass(), Loc, Rot, ActorSpawnParameters);

			// Choose a random media source from the list
			int IndSource = FMath::RandRange(0, MediaSources.Num() - 1);

			// Init millicast and start viewing
			Plane->Initialize(MediaSources[IndSource]);
			Plane->View();
		}
	}
}

// Called every frame
void APlaneSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

