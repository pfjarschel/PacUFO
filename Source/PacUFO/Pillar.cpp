// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "Pillar.h"


// Sets default values
APillar::APillar()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create some basic components.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	//Set up components (on the root component)
	VisibleComponent->SetupAttachment(RootComponent);
	VisibleComponent->SetMobility(EComponentMobility::Static);
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetMobility(EComponentMobility::Static);

	//Mesh settings
	const ConstructorHelpers::FObjectFinder<UStaticMesh> pillarMesh(TEXT("/Game/StarterContent/Architecture/Pillar_50x500"));
	StaticMesh->SetStaticMesh(pillarMesh.Object);
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	const ConstructorHelpers::FObjectFinder<UMaterial> pillarMaterial(TEXT("/Game/StarterContent/Materials/M_Metal_Steel"));
	StaticMesh->SetMaterial(0, pillarMaterial.Object);

	SetActorScale3D(FVector(1.0f, 1.0f, 0.4f));

	Tags.AddUnique(TEXT("level"));
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();
	
	//Changing on the constructor doesn't change what is already placed, so I repeat this call here
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

// Called every frame
void APillar::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

