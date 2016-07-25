// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PacGameState.h"
#include "Ufo.h"
#include "GameFramework/Actor.h"
#include "EnergyBall.generated.h"

UCLASS()
class PACUFO_API AEnergyBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnergyBall();

	//Components
	UPROPERTY(EditAnywhere)	USceneComponent* VisibleComponent;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere) UAudioComponent* AudioComponent;

	//Objects
	APacGameState* gameState;
	AUfo* player;

	//Main default methods
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//New methods
	//Overlap events
	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void NotifyActorEndOverlap(AActor* Other) override;

	int32 CountRemBalls();
};
