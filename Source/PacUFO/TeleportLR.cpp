// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "TeleportLR.h"


// Sets default values
ATeleportLR::ATeleportLR()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create some basic components.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));

	//Set up components (on the root component)
	VisibleComponent->SetupAttachment(RootComponent);
	StaticMesh->SetupAttachment(RootComponent);
	AudioComponent->SetupAttachment(RootComponent);

	//Mesh settings
	const ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	StaticMesh->SetStaticMesh(cubeMesh.Object);

	const ConstructorHelpers::FObjectFinder<UMaterial> teleMaterial(TEXT("/Game/Meshes/EnemyMaterial2"));
	StaticMesh->SetMaterial(0, teleMaterial.Object);

	SetActorScale3D(FVector(1.5f, 1.75f, 2.0f));

	//Overlap Events
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->bGenerateOverlapEvents = true;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	//Set audio clip
	const ConstructorHelpers::FObjectFinder<USoundCue> spellSound(TEXT("/Game/Sounds/spell2_Cue"));
	AudioComponent->SetSound(spellSound.Object);
	AudioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ATeleportLR::BeginPlay()
{
	Super::BeginPlay();
	
	//Find player
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(FName(TEXT("player"))))
		{
			player = (AUfo*)Actor;
		}
	}
}

// Called every frame
void ATeleportLR::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATeleportLR::NotifyActorBeginOverlap(AActor* Other)
{
	if (Other->ActorHasTag(TEXT("player")))
	{
		player = (AUfo*)Other;

		if (player)
		{
			if (!player->teleporting)
			{
				AudioComponent->Play();
				player->teleporting = true;

				GetWorldTimerManager().ClearTimer(teleHandle);
				GetWorldTimerManager().SetTimer(teleHandle, this, &ATeleportLR::TeleportPlayer, 0.5f, false);
			}
		}
	}
}

void ATeleportLR::NotifyActorEndOverlap(AActor* Other)
{

}

void ATeleportLR::TeleportPlayer()
{
	if (player)
	{
		FVector newloc;
		if (player->StaticMesh->GetComponentLocation().X > 0)
		{
			newloc = FVector(FVector(-900.0f, 0.0f, 100.0f));
		}
		else
		{
			newloc = FVector(FVector(1300.0f, 0.0f, 100.0f));
		}
		player->StaticMesh->SetAllPhysicsPosition(newloc);
		player->teleporting = false;
	}
}