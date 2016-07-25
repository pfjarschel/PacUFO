// Fill out your copyright notice in the Description page of Project Settings.

#include "PacUFO.h"
#include "SMainSHUD.h"
#include "PacUFOGameMode.h"

APacUFOGameMode::APacUFOGameMode() : Super()
{
	GameStateClass = APacGameState::StaticClass();
}


