// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PacGameState.h"
#include "Ufo.h"
#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

UCLASS()
class PACUFO_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

	//Components
	UPROPERTY(EditAnywhere)	USceneComponent* VisibleComponent;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere) UAudioComponent* AudioDied;
	UPROPERTY(EditAnywhere) UAudioComponent* AudioKilled;


	//Materials
	TArray<UMaterial*> enemyMaterials;

	//Vars and Objs
	APacGameState* gameState;
	FVector ufoVelocity;
	UPROPERTY(EditAnywhere)	float velMultiplier = 400.0f;
	UPROPERTY(EditAnywhere)	float rotMultiplier = 2000.0f;
	AUfo* player;
	float changedDirTime;
	float minChangeDirTime;
	FVector forward;
	FTimerHandle newHandle;
	FTimerHandle playerHandle;

	//Main Default methods
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	//New methods
	//Movement
	void ChangeDir();
	FVector GetForward();
	void SpawnNew();
	void RespawnPlayer();

	//Overlap events
	virtual void NotifyActorBeginOverlap(AActor* Other) override;
	virtual void NotifyActorEndOverlap(AActor* Other) override;
	virtual void NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit) override;
};
