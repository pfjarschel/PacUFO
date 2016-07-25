// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "PacGameState.h"

APacGameState::APacGameState()
{
	gameStarted = false;
	score = 0;
	remainingBalls = 0;
	lives = 0;
	paused = false;
	gameOver = false;
	completedGame = false;
}

void APacGameState::SetPaused(bool isPaused)
{
	paused = isPaused;
}



