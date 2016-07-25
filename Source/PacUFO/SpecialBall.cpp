// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "SpecialBall.h"


// Sets default values
ASpecialBall::ASpecialBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ballMesh(TEXT("/Game/Meshes/TwinStickProjectile"));
	StaticMesh->SetStaticMesh(ballMesh.Object);
	SetActorScale3D(2.0f*FVector(1.0f, 1.0f, 1.0f));

	const ConstructorHelpers::FObjectFinder<UMaterial> ballMaterial(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse"));
	StaticMesh->SetMaterial(0, ballMaterial.Object);

	//Overlap Events
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->bGenerateOverlapEvents = true;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	//Set audio clip
	const ConstructorHelpers::FObjectFinder<USoundCue> pickSound(TEXT("/Game/Sounds/Special"));
	AudioComponent->SetSound(pickSound.Object);
	AudioComponent->bAutoActivate = false;

	//Set tags
	Tags.AddUnique(TEXT("collectible"));
	Tags.AddUnique(TEXT("special"));
}

// Called when the game starts or when spawned
void ASpecialBall::BeginPlay()
{
	Super::BeginPlay();

	//Get reference to gamestate
	gameState = GetWorld() != NULL ? GetWorld()->GetGameState<APacGameState>() : NULL;

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
void ASpecialBall::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ASpecialBall::NotifyActorBeginOverlap(AActor* Other)
{
	if (!bHidden && Other->ActorHasTag(TEXT("player")))
	{
		AudioComponent->Play();
		SetLifeSpan(0.5f);
		SetActorHiddenInGame(true);

		player = (AUfo*)Other;

		if (player)
		{
			if (gameState)
			{
				gameState->score += 100;
			}
			player->EnterSpecial();
		}
	}
	else if (!bHidden && Other->ActorHasTag(TEXT("level")))
	{
		SetLifeSpan(0.1f);
		SetActorHiddenInGame(true);
	}

	if (gameState)
	{
		gameState->remainingBalls = CountRemBalls();

		if (gameState->remainingBalls == 0)
		{
			gameState->completedGame = true;

			//Can't decide if I pause the game completely or just keep it going...
			//UGameplayStatics::SetGamePaused(GetWorld(), true);

			player->StaticMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
			player->velMultiplier = 0.0f;
			player->StaticMesh->SetAllPhysicsPosition(FVector(0.0f, 0.0f, 500.0f));
		}
	}
}

void ASpecialBall::NotifyActorEndOverlap(AActor* Other)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("exit"));
}

int32 ASpecialBall::CountRemBalls()
{
	int count = 0;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->ActorHasTag(FName(TEXT("collectible"))) && !Actor->bHidden)
		{
			count++;
		}
	}

	return count;
}