// copyright Millicast 2022. All right reserved.


#include "PlanePlayer.h"

// Millicast includes
#include "MillicastDirectorComponent.h"
#include "MillicastSubscriberComponent.h"
#include "MillicastMediaSource.h"
#include "IMillicastMediaTrack.h"
#include "MillicastTexture2DPlayer.h"

// UE inlcudes
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY(LogMillicastPlanePlayer);

// Sets default values
APlanePlayer::APlanePlayer() : Initialized{ false }
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the static mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MillicastMesh"));
	// Set the mesh as the root component
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	// Find the plane mesh
	auto Finder = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane"));
	// Set it as a mesh
	StaticMeshComponent->SetStaticMesh(Finder.Object);

	// Find the Millicast Material
	auto FinderMat = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/FirstPersonBP/MillicastTextureMat.MillicastTextureMat"));
	// Replace the mesh material by our Material
	StaticMeshComponent->SetMaterial(0, FinderMat.Object);

	// Create Millicast Director component
	Director = CreateDefaultSubobject<UMillicastDirectorComponent>(TEXT("MillicastDirectorComponent"));
	// Create Millicast Subscriber Component
	Subscriber = CreateDefaultSubobject<UMillicastSubscriberComponent>(TEXT("MillicastSubscriberComponent"));

	// Add director and subscriber as owned
	AddOwnedComponent(Director);
	AddOwnedComponent(Subscriber);

	// Create the Millicast Texture Player
	TexturePlayer = CreateDefaultSubobject<UMillicastTexture2DPlayer>(TEXT("MillicastTexturePlayer"));
}

void APlanePlayer::Initialize(UMillicastMediaSource* InMediaSource)
{
	// Initialize Components
	Director->Initialize(InMediaSource);
	Subscriber->Initialize(InMediaSource);

	// Create video texture and assign it to the Texture Player
	TexturePlayer->ChangeVideoTexture(NewObject<UMillicastMediaTexture2D>(this, TEXT("MillicastTexture"),
		RF_Public | RF_Standalone | RF_Transactional));
	// Update the video texture resource
	TexturePlayer->VideoTexture->UpdateResource();

	// Create a dynamic material to draw the frame on the texture
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(StaticMeshComponent->GetMaterial(0), this);
	DynMaterial->SetTextureParameterValue("Texture", TexturePlayer->VideoTexture);

	StaticMeshComponent->SetMaterial(0, DynMaterial);

	// Bind director events
	Director->OnAuthenticated.AddDynamic(this, &APlanePlayer::OnAuthenticated);
	Director->OnAuthenticationFailure.AddDynamic(this, &APlanePlayer::OnAuthenticationFailure);

	// bind subscriber events
	Subscriber->OnSubscribed.AddDynamic(this, &APlanePlayer::OnSubscribed);
	Subscriber->OnSubscribedFailure.AddDynamic(this, &APlanePlayer::OnSubscribeFailure);
	Subscriber->OnVideoTrack.AddDynamic(this, &APlanePlayer::OnVideoTrack);
	Subscriber->OnAudioTrack.AddDynamic(this, &APlanePlayer::OnAudioTrack);
	Subscriber->OnActive.AddDynamic(this, &APlanePlayer::OnActive);
	Subscriber->OnInactive.AddDynamic(this, &APlanePlayer::OnInactive);
	Subscriber->OnStopped.AddDynamic(this, &APlanePlayer::OnStopped);
	Subscriber->OnVad.AddDynamic(this, &APlanePlayer::OnVad);
	Subscriber->OnLayers.AddDynamic(this, &APlanePlayer::OnLayers);
	Subscriber->OnViewerCount.AddDynamic(this, &APlanePlayer::OnViewerCount);

	Initialized = true;
}

void APlanePlayer::OnAuthenticated(const FMillicastSignalingData& SignalingData)
{
	// Authentication was succesful, we can now subscribe with the ws url and json webtoken

	// Ensure we have a subscriber
	if (Subscriber) 
	{
		// Subscribe to Millicast feed
		Subscriber->Subscribe(SignalingData, nullptr);
	}
}

void APlanePlayer::OnAuthenticationFailure(int Code, const FString& Reason)
{
	// Authentication failed
	UE_LOG(LogMillicastPlanePlayer, Error, TEXT("Could not authenticate to Millicast %d %s"), Code, *Reason);
}

void APlanePlayer::OnSubscribed()
{
	UE_LOG(LogMillicastPlanePlayer, Display, TEXT("Subscribed"));
}

void APlanePlayer::OnSubscribeFailure(const FString& Message)
{
	UE_LOG(LogMillicastPlanePlayer, Error, TEXT("Could not subscribe to Millicast %s"), *Message);
}

void APlanePlayer::OnVideoTrack(UMillicastVideoTrack* InVideoTrack)
{
	// Ensure the texture player is not null
	if (TexturePlayer)
	{
		// Add the texture player to the video tracks to consume the video frames
		InVideoTrack->AddConsumer(TexturePlayer);
	}
}

void APlanePlayer::OnAudioTrack(UMillicastAudioTrack* InAudioTrack)
{

}

void APlanePlayer::OnActive(const FString& StreamId, const TArray<FMillicastTrackInfo>& Tracks, const FString& SourceId)
{
}

void APlanePlayer::OnInactive(const FString& StreamId, const FString& SourceId)
{
}

void APlanePlayer::OnStopped()
{
}

void APlanePlayer::OnVad(const FString& Mid, const FString& SourceId)
{
}

void APlanePlayer::OnLayers(const FString& Mid, const TArray<FMillicastLayerData>& ActiveLayers, const TArray<FMillicastLayerData>& InactiveLayers)
{
}

void APlanePlayer::OnViewerCount(int Count)
{
}

void APlanePlayer::View()
{
	if (Initialized)
	{
		Director->Authenticate();
	}
}

// Called when the game starts or when spawned
void APlanePlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlanePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

