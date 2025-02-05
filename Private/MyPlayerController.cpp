// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	: IMC_Human(nullptr), IMC_Drone(nullptr), IA_IMC_Changer(nullptr),
	IA_Move(nullptr), IA_Dash(nullptr), IA_Run(nullptr), IA_Crouch(nullptr), IA_Look(nullptr),
	IA_Drone_Move(nullptr), IA_Drone_Up(nullptr), IA_Drone_Down(nullptr), 
	IA_Drone_RightRotation(nullptr), IA_Drone_LeftRotation(nullptr), IA_Drone_Look(nullptr), IA_Drone_Air(nullptr)
{
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem * Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Human)
			{
				Subsystem->AddMappingContext(IMC_Human, 0);
			}
		}
	}
}

void AMyPlayerController::ChangeToHuman()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Human)
			{
				Subsystem->RemoveMappingContext(IMC_Drone);
				Subsystem->AddMappingContext(IMC_Human, 0);
			}
		}
	}
}

void AMyPlayerController::ChangeToDrone()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (IMC_Drone)
			{
				Subsystem->RemoveMappingContext(IMC_Human);
				Subsystem->AddMappingContext(IMC_Drone, 0);
			}
		}
	}
}
