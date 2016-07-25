// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "BallSpawner.h"


// Sets default values
ABallSpawner::ABallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create some basic components.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	//Set up components (on the root component)
	VisibleComponent->SetupAttachment(RootComponent);
	StaticMesh->SetupAttachment(RootComponent);

	//Mesh settings
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ballMesh(TEXT("/Game/Meshes/TwinStickProjectile"));
	StaticMesh->SetStaticMesh(ballMesh.Object);

	const ConstructorHelpers::FObjectFinder<UMaterial> ballMaterial(TEXT("/Game/StarterContent/Materials/M_ColorGrid_LowSpec"));
	StaticMesh->SetMaterial(0, ballMaterial.Object);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	SetActorHiddenInGame(true);
}

// Called when the game starts or when spawned
void ABallSpawner::BeginPlay()
{
	Super::BeginPlay();

	//Get reference to gamestate
	gameState = GetWorld() != NULL ? GetWorld()->GetGameState<APacGameState>() : NULL;

	FVector zeroLocation = GetActorLocation();
	
	for (int i = 0; i < width; i++)
	{
		balls.Add(new TArray<AActor*>);
		for (int j = 0; j < height; j++)
		{
			FVector location = zeroLocation + FVector(i*step, j*step, 0.0f);

			float specialChance = 0.03f;
			float rand = FMath::RandRange(0.0f, 1.0f);
			if (rand <= specialChance)
			{
				balls[i]->Add(GetWorld()->SpawnActor(ASpecialBall::StaticClass(), &location));
			}
			else
			{
				balls[i]->Add(GetWorld()->SpawnActor(AEnergyBall::StaticClass(), &location));
			}
		}
	}
}

// Called every frame
void ABallSpawner::Tick( float DeltaTime )
{
	
}

