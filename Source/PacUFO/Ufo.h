// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PacGameState.h"
#include "GameFramework/Pawn.h"
#include "Ufo.generated.h"

UCLASS()
class PACUFO_API AUfo : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUfo();

	//Components
	UPROPERTY(EditAnywhere)	USceneComponent* VisibleComponent;
	UPROPERTY(EditAnywhere)	UCameraComponent* MainCamera;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere) UAudioComponent* AudioComponent;

	//Materials
	UMaterial* specialMaterial;
	UMaterial* ufoMaterial;

	//Vars and Objs
	APacGameState* gameState;
	FVector ufoVelocity;
	UPROPERTY(EditAnywhere)	float velMultiplier = 300.0f;
	UPROPERTY(EditAnywhere)	float rotMultiplier = 500.0f;
	UPROPERTY(EditAnywhere)	float cameraVelocity= 1000.0f;
	bool isSpecial;
	FTimerHandle specialHandle;
	bool dying;
	bool teleporting;
	float velMultiplierbckp;

	//Main Default methods
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	//New methods
	//Movement
	void MoveX(float val);
	void MoveY(float val);

	void EnterSpecial();
	void ExitSpecial();

	void TogglePause();
	void BackToMenu();
};
