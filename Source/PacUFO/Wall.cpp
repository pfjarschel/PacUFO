// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "Wall.h"


// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	const ConstructorHelpers::FObjectFinder<UStaticMesh> wallMesh(TEXT("/Game/StarterContent/Architecture/Wall_400x200"));
	StaticMesh->SetStaticMesh(wallMesh.Object);
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
	const ConstructorHelpers::FObjectFinder<UMaterial> wallMaterial(TEXT("/Game/StarterContent/Materials/M_Metal_Steel"));
	StaticMesh->SetMaterial(0, wallMaterial.Object);

	Tags.AddUnique(TEXT("level"));
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	//Changing on the constructor doesn't change what is already placed, so I repeat this call here
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	
}

// Called every frame
void AWall::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

