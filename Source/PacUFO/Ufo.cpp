// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "Ufo.h"
#include "Enemy.h"


// Sets default values
AUfo::AUfo()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Will possess player 0
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create some basic components.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	
	//Set up components (on the root component)
	MainCamera->SetupAttachment(RootComponent);
	VisibleComponent->SetupAttachment(RootComponent);
	StaticMesh->SetupAttachment(RootComponent);
	AudioComponent->SetupAttachment(RootComponent);

	//Mesh and materials settings
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ufoMesh(TEXT("/Game/Meshes/TwinStickUFO"));
	StaticMesh->SetStaticMesh(ufoMesh.Object);

	specialMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile_Pulse")).Object;
	ufoMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/Meshes/BlueMaterial")).Object;
	StaticMesh->SetMaterial(0, ufoMaterial);

	//Overlap Events
	StaticMesh->bGenerateOverlapEvents = true;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//Some Physics settings
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(false);
	StaticMesh->BodyInstance.bLockZTranslation = true;
	StaticMesh->BodyInstance.bLockXRotation = true;
	StaticMesh->BodyInstance.bLockYRotation = true;
	StaticMesh->SetLinearDamping(10.0f);
	StaticMesh->SetAngularDamping(10.0f);

	//Camera settings
	MainCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 2000.0f));
	MainCamera->SetRelativeRotation(FRotator(-90.0f, 90.0f, 0.0f));

	//Sound (music, really)
	const ConstructorHelpers::FObjectFinder<USoundCue> pacMusic(TEXT("/Game/Sounds/pacMusic_cue"));
	AudioComponent->SetSound(pacMusic.Object);
	AudioComponent->bAutoActivate = true;

	//Others
	//Set tags
	Tags.AddUnique(TEXT("player"));

	velMultiplierbckp = velMultiplier;
	
	isSpecial = false;
	dying = false;
	teleporting = false;
}

// Called when the game starts or when spawned
void AUfo::BeginPlay()
{
	Super::BeginPlay();
	
	//Get reference to gamestate
	gameState = GetWorld() != NULL ? GetWorld()->GetGameState<APacGameState>() : NULL;

	//Initialize some state stuff
	if (!gameState->gameStarted)
	{
		gameState->gameStarted = true;
		gameState->lives = 3;
		gameState->score = 0;
	}
}

// Called every frame
void AUfo::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Handles movement
	if (!ufoVelocity.IsZero())
	{
		StaticMesh->SetAllPhysicsLinearVelocity(ufoVelocity);

		//Set facing to the direction of movement
		FRotator currRot = StaticMesh->GetComponentRotation();
		FRotator nextRot = FRotator(0.0f, ufoVelocity.Rotation().Yaw, 0.0f);
		float deltaRot = (nextRot - currRot).Yaw;

		//Needed for smooth transitions on yaw limits (0 or 360)
		if (deltaRot >= 180 || deltaRot <= -180)
		{
			deltaRot= -FMath::Sign(deltaRot)*(360 - FMath::Abs(deltaRot));
		}

		if (FMath::Abs(deltaRot) > 5)
		{
			StaticMesh->SetAllPhysicsAngularVelocity(FVector(0.0f, 0.0f, FMath::Sign(deltaRot)*rotMultiplier));
		}
		else
		{
			StaticMesh->SetAllPhysicsAngularVelocity(FVector(0.0f, 0.0f, FMath::Sign(deltaRot)*rotMultiplier*0.1f));
		}
	}

	//Camera follows character
	FVector deltaLoc = StaticMesh->GetComponentLocation() - MainCamera->GetComponentLocation();
	deltaLoc.Normalize();
	deltaLoc.Z = 0.0f; // MainCamera->GetComponentLocation().Z;
	MainCamera->AddRelativeLocation(deltaLoc*DeltaTime*cameraVelocity);
}

// Called to bind functionality to input
void AUfo::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	//Add movement handling
	InputComponent->BindAxis("MoveLeftRight", this, &AUfo::MoveX);
	InputComponent->BindAxis("MoveUpDown", this, &AUfo::MoveY);
	InputComponent->BindAction("Accept", IE_Released, this, &AUfo::BackToMenu).bExecuteWhenPaused = true;
	InputComponent->BindAction("Action", IE_Released, this, &AUfo::TogglePause).bExecuteWhenPaused = true;
}

//Movement methods
void AUfo::MoveX(float val)
{
	//Clamp to make sure values are not outside this range.
	ufoVelocity.X = FMath::Clamp(val, -1.0f, 1.0f)*velMultiplier;
	ufoVelocity.Z = 0.0f;
}

void AUfo::MoveY(float val)
{
	//Clamp to make sure values are not outside this range.
	ufoVelocity.Y = FMath::Clamp(val, -1.0f, 1.0f)*velMultiplier;
	ufoVelocity.Z = 0.0f;
}

void AUfo::EnterSpecial()
{
	GetWorldTimerManager().ClearTimer(specialHandle);
	isSpecial = true;

	velMultiplier = 500.0f;

	StaticMesh->SetMaterial(0, specialMaterial);
		
	GetWorldTimerManager().SetTimer(specialHandle, this, &AUfo::ExitSpecial, 5.0f, false);
}

void AUfo::ExitSpecial()
{
	isSpecial = false;

	velMultiplier = velMultiplierbckp;

	StaticMesh->SetMaterial(0, ufoMaterial);
}

void AUfo::BackToMenu()
{
	if (gameState->gameOver || gameState->completedGame)
	{
		UGameplayStatics::OpenLevel(this, TEXT("/Game/MainMenu"), false);
	}
}

void AUfo::TogglePause()
{
	gameState->paused = !gameState->paused;

	UGameplayStatics::SetGamePaused(GetWorld(), gameState->paused);
}