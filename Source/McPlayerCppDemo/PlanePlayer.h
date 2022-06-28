// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MillicastSubscriberComponent.h"

// Millicast class forward declaration
class UMillicastMediaSource;
class UMillicastDirectorComponent;
class UMillicastSubscriberComponent;
class UMillicastTexture2DPlayer;
class UMillicastVideoTrack;
class UMillicastAudioTrack;
struct FMillicastSignalingData;

// UE class forward declaration
class UStaticMeshComponent;
class UBoxComponent;


DECLARE_LOG_CATEGORY_EXTERN(LogMillicastPlanePlayer, Log, All);

#include "PlanePlayer.generated.h"

UCLASS()
class MCPLAYERCPPDEMO_API APlanePlayer : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	UMillicastMediaSource* MediaSource;

	UPROPERTY()
	UMillicastDirectorComponent* Director;

	UPROPERTY()
	UMillicastSubscriberComponent* Subscriber;

	UPROPERTY()
	UMillicastTexture2DPlayer* TexturePlayer;

	UPROPERTY(EditAnywhere, Category = "Properties", META = (AllowPrivateAccess = true))
	UStaticMeshComponent* StaticMeshComponent;

	bool Initialized;

public:	
	// Sets default values for this actor's properties
	APlanePlayer();
	
	void Initialize(UMillicastMediaSource* InMediaSource = nullptr);

	// Director events
	UFUNCTION()
	void OnAuthenticated(const FMillicastSignalingData& SignalingData);
	UFUNCTION()
	void OnAuthenticationFailure(int Code, const FString& Reason);

	// Subscriber events
	UFUNCTION()
	void OnSubscribed();
	UFUNCTION()
	void OnSubscribeFailure(const FString& Message);

	// -- Media Tracks
	UFUNCTION()
	void OnVideoTrack(UMillicastVideoTrack* InVideoTrack);
	UFUNCTION()
	void OnAudioTrack(UMillicastAudioTrack* InAudioTrack);

	// -- Broadcast events
	UFUNCTION()
	void OnActive(const FString& StreamId, const TArray<FMillicastTrackInfo>& Tracks, const FString& SourceId);
	UFUNCTION()
	void OnInactive(const FString& StreamId, const FString& SourceId);
	UFUNCTION()
	void OnStopped();
	UFUNCTION()
	void OnVad(const FString& Mid, const FString& SourceId);
	UFUNCTION()
	void OnLayers(const FString& Mid, const TArray<FMillicastLayerData>& ActiveLayers, const TArray<FMillicastLayerData>& InactiveLayers);
	UFUNCTION()
	void OnViewerCount(int Count);

	void View();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
