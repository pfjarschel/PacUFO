// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PacGameState.h"
#include "EnergyBall.h"
#include "SpecialBall.h"
#include "GameFramework/Actor.h"
#include "BallSpawner.generated.h"

UCLASS()
class PACUFO_API ABallSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallSpawner();

	//Components
	UPROPERTY(EditAnywhere)	USceneComponent* VisibleComponent;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* StaticMesh;

	//Vars and Objects
	APacGameState* gameState;
	TArray<TArray<AActor*>*> balls;
	UPROPERTY(EditAnywhere) int width = 19;
	UPROPERTY(EditAnywhere) int height = 23;
	UPROPERTY(EditAnywhere) float step = 100.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
