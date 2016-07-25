// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "PacGameState.generated.h"

/**
 * 
 */
UCLASS()
class PACUFO_API APacGameState : public AGameState
{
	GENERATED_BODY()

public:
	APacGameState();

	//Vars
	UPROPERTY(BlueprintReadOnly) int32 score;
	UPROPERTY(BlueprintReadOnly) int32 remainingBalls;
	UPROPERTY(BlueprintReadOnly) int32 lives;
	UPROPERTY(BlueprintReadOnly) bool gameStarted;
	UPROPERTY(BlueprintReadOnly) bool completedGame;
	UPROPERTY(BlueprintReadOnly) bool paused;
	UPROPERTY(BlueprintReadOnly) bool gameOver;

	//Methods
	UFUNCTION(BlueprintCallable, Category = "Actor,PacGameState") void SetPaused(bool isPaused);
};
