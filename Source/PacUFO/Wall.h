// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Wall.generated.h"

UCLASS()
class PACUFO_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

	//Components
	UPROPERTY(EditAnywhere)	USceneComponent* VisibleComponent;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* StaticMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
