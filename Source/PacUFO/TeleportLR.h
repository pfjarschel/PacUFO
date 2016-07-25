// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ufo.h"
#include "GameFramework/Actor.h"
#include "TeleportLR.generated.h"

UCLASS()
class PACUFO_API ATeleportLR : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleportLR();

	//Components
	UPROPERTY(EditAnywhere)	USceneComponent* VisibleComponent;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere) UAudioComponent* AudioComponent;

	//Objects
	AUfo* player;
	FTimerHandle teleHandle;

	//Main default methods
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	//New methods
	//Overlap events
	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void NotifyActorEndOverlap(AActor* Other) override;

	void TeleportPlayer();
};
