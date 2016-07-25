// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "Enemy.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create some basic components.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	AudioDied = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioDied"));
	AudioKilled = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioKilled"));

	//Set up components (on the root component)
	VisibleComponent->SetupAttachment(RootComponent);
	StaticMesh->SetupAttachment(RootComponent);
	AudioDied->SetupAttachment(RootComponent);
	AudioKilled->SetupAttachment(RootComponent);

	//Mesh and materials settings
	const ConstructorHelpers::FObjectFinder<UStaticMesh> ufoMesh(TEXT("/Game/Meshes/TwinStickUFO"));
	StaticMesh->SetStaticMesh(ufoMesh.Object);
	SetActorScale3D(0.75f*FVector(1.0f, 1.0f, 1.0f));

	enemyMaterials.Add(ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/Meshes/EnemyMaterial1")).Object);
	enemyMaterials.Add(ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/Meshes/EnemyMaterial2")).Object);
	enemyMaterials.Add(ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/Meshes/EnemyMaterial3")).Object);
	enemyMaterials.Add(ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/Meshes/EnemyMaterial4")).Object);
	enemyMaterials.Add(ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/Meshes/EnemyMaterial5")).Object);
	int randMat = FMath::RandRange(0, enemyMaterials.Num() - 1);
	StaticMesh->SetMaterial(0, enemyMaterials[randMat]);


	//Overlap Events
	StaticMesh->bGenerateOverlapEvents = true;
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	//Some Physics settings
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(false);
	//StaticMesh->BodyInstance.bLockZTranslation = true;
	StaticMesh->BodyInstance.bLockXRotation = true;
	StaticMesh->BodyInstance.bLockYRotation = true;
	StaticMesh->SetLinearDamping(10.0f);
	StaticMesh->SetAngularDamping(10.0f);

	//Others
	//Set tags
	Tags.AddUnique(TEXT("enemy"));

	//Set audio clips
	const ConstructorHelpers::FObjectFinder<USoundCue> pickSound(TEXT("/Game/Sounds/Special"));
	AudioDied->SetSound(pickSound.Object);
	AudioDied->bAutoActivate = false;

	const ConstructorHelpers::FObjectFinder<USoundCue> rumbleSound(TEXT("/Game/Sounds/rumble_Cue"));
	AudioKilled->SetSound(rumbleSound.Object);
	AudioKilled->bAutoActivate = false;

	changedDirTime = 0.0f;
	minChangeDirTime = 0.2f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//Set Random Material
	int randMat = FMath::RandRange(0, enemyMaterials.Num() - 1);
	StaticMesh->SetMaterial(0, enemyMaterials[randMat]);
	
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

	ufoVelocity = FVector(0.0f, 1.0f, 0.0f)*velMultiplier;
	GetForward();
}

// Called every frame
void AEnemy::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//Handles movement
	if (!ufoVelocity.IsZero())
	{
		float railOffsetX = FMath::Fmod(StaticMesh->GetComponentLocation().X, 100.0f);
		float railOffsetY = FMath::Fmod(StaticMesh->GetComponentLocation().Y, 100.0f);

		//If path is clear, consider changing direction
		changedDirTime += DeltaTime;
		if (railOffsetX <= 5.0f && railOffsetY <= 5.0f && changedDirTime >= minChangeDirTime)
		{
			ChangeDir();
			changedDirTime = 0.0f;
		}

		//Try to keep it on rails
		GetForward();
		ufoVelocity = forward*velMultiplier;
		if (forward.X != 0)
		{
			if (railOffsetY > 50.0f)
			{
				ufoVelocity.Y += (100.0f - railOffsetY)*velMultiplier / 50.0f;
			}
			else 
			{
				ufoVelocity.Y -= railOffsetY*velMultiplier / 50.0f;
			}
		}
		else if (forward.Y != 0)
		{
			if (railOffsetX > 50.0f)
			{
				ufoVelocity.X += (100.0f - railOffsetX)*velMultiplier / 50.0f;
			}
			else
			{
				ufoVelocity.X -= railOffsetX*velMultiplier / 50.0f;
			}
		}

		ufoVelocity.Z = (100.0f - StaticMesh->GetComponentLocation().Z);

		//Apply Velocity
		StaticMesh->SetAllPhysicsLinearVelocity(ufoVelocity);

		//Set facing to the direction of movement
		FRotator currRot = StaticMesh->GetComponentRotation();
		FRotator nextRot = FRotator(0.0f, ufoVelocity.Rotation().Yaw, 0.0f);
		float deltaRot = (nextRot - currRot).Yaw;

		//Needed for smooth transitions on yaw limits (0 or 360)
		if (deltaRot >= 180 || deltaRot <= -180)
		{
			deltaRot = -FMath::Sign(deltaRot)*(360 - FMath::Abs(deltaRot));
		}

		if (FMath::Abs(deltaRot) > 0)
		{
			StaticMesh->SetAllPhysicsAngularVelocity(FVector(0.0f, 0.0f, FMath::Sign(deltaRot)*rotMultiplier));
		}
		else
		{
			StaticMesh->SetAllPhysicsAngularVelocity(FVector(0.0f, 0.0f, FMath::Sign(deltaRot)*rotMultiplier*0.1f));
		}
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AEnemy::ChangeDir()
{
	//Check which sides are clear
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	//Debug draw lines
	//const FName TraceTag("MyTraceTag");
	//GetWorld()->DebugDrawTraceTag = TraceTag;
	//Params.TraceTag = TraceTag;
	
	FHitResult HitF;
	FHitResult HitL;
	FHitResult HitR;

	float range = 150.0f;

	GetForward();

	FVector Start = StaticMesh->GetComponentLocation();
	
	FVector EndF = Start + forward*range/3.0f;
	FVector EndL = Start + forward.RotateAngleAxis(90.0f, FVector::UpVector)*range;
	FVector EndR = Start + forward.RotateAngleAxis(-90.0f, FVector::UpVector)*range;
	
	bool bHitF = GetWorld()->LineTraceSingleByChannel(HitF, Start, EndF, ECC_WorldStatic, Params);
	bool bHitL = GetWorld()->LineTraceSingleByChannel(HitL, Start, EndL, ECC_WorldStatic, Params);
	bool bHitR = GetWorld()->LineTraceSingleByChannel(HitR, Start, EndR, ECC_WorldStatic, Params);

	//Make sure hit results from level objects
	if (bHitF)
	{
		bHitF = HitF.GetActor()->ActorHasTag(TEXT("level"));
	}
	if (bHitL)
	{
		bHitL = HitL.GetActor()->ActorHasTag(TEXT("level"));
	}
	if (bHitR)
	{
		bHitR = HitR.GetActor()->ActorHasTag(TEXT("level"));
	}

	TArray<int> dummy;
	dummy.Add(0);
	if (!bHitL)
	{
		dummy.Add(1);
	}
	if (!bHitR)
	{
		dummy.Add(2);
	}

	int randInt = FMath::Trunc(FMath::RandRange(0.0f, (float)dummy.Num()));
	if (randInt > dummy.Num() - 1)
	{
		randInt = dummy.Num() - 1;
	}
	int randResult = dummy[randInt];
	switch (randResult)
	{
	case 0:
		if (bHitF)
		{
			ufoVelocity = -forward*velMultiplier;
		}
		break;
	case 1:
		ufoVelocity = forward.RotateAngleAxis(90.0f, FVector::UpVector)*velMultiplier;
		break;
	case 2:
		ufoVelocity = forward.RotateAngleAxis(-90.0f, FVector::UpVector)*velMultiplier;
		break;
	}

	GetForward();
}

void AEnemy::NotifyActorBeginOverlap(AActor* Other)
{
	if (!bHidden && Other->ActorHasTag(TEXT("player")))
	{
		player = (AUfo*)Other;
		if (player)
		{
			if (player->isSpecial)
			{
				AudioDied->Play();

				gameState->score += 500;
				SetActorHiddenInGame(true);

				GetWorldTimerManager().ClearTimer(newHandle);
				GetWorldTimerManager().SetTimer(newHandle, this, &AEnemy::SpawnNew, 3.0f, false);
			}
			else if(!player->dying)
			{
				AudioKilled->Play();
				player->SetActorHiddenInGame(true);
				player->dying = true;
				player->velMultiplier = 0.0f;

				gameState->score -= 1000;
				if (gameState->score < 0)
				{
					gameState->score = 0;
				}

				gameState->lives -= 1;

				if (gameState->lives <= 0)
				{
					gameState->gameOver = true;
					
					//Can't decide if I pause the game completely or just keep it going...
					//UGameplayStatics::SetGamePaused(GetWorld(), true);

					player->StaticMesh->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
					player->velMultiplier = 0.0f;
					player->StaticMesh->SetAllPhysicsPosition(FVector(0.0f, 0.0f, 500.0f));
				}
				else
				{
					GetWorldTimerManager().ClearTimer(playerHandle);
					GetWorldTimerManager().SetTimer(playerHandle, this, &AEnemy::RespawnPlayer, 3.0f, false);
				}
			}
		}
	}
}

void AEnemy::NotifyActorEndOverlap(AActor* Other)
{

}

FVector AEnemy::GetForward()
{
	if (FMath::Abs(ufoVelocity.X) > FMath::Abs(ufoVelocity.Y))
	{
		forward = FVector(1.0f, 0.0f, 0.0f)*FMath::Sign(ufoVelocity.X);
	}
	else
	{
		forward = FVector(0.0f, 1.0f, 0.0f)*FMath::Sign(ufoVelocity.Y);
	}

	return forward;
}

void AEnemy::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (!bHidden && Other->ActorHasTag(TEXT("level")))
	{
		ChangeDir();
	}
}

void AEnemy::SpawnNew()
{
	FVector location = FVector(FMath::RandRange(-700.0f, 1100.0f), FMath::RandRange(-1100.0f, 1100.0f), 300.0f);

	AEnemy* newEnemy = (AEnemy*) GetWorld()->SpawnActor(AEnemy::StaticClass(), &location);
		
	SetLifeSpan(0.1f);
}

void AEnemy::RespawnPlayer()
{
	player->Destroy();

	FVector location = FVector(200.0f, -300.0f, 100.0f);

	player = (AUfo*) GetWorld()->SpawnActor(AUfo::StaticClass(), &location);
}