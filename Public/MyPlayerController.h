// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class NBC_CH3_2_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	//IMC_Human
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|IMC")
	UInputMappingContext* IMC_Human;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|IMC")
	UInputMappingContext* IMC_Drone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|IMC")
	UInputAction* IA_IMC_Changer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Human")
	UInputAction* IA_Move;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Human")
	UInputAction* IA_Dash;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Human")
	UInputAction* IA_Run;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Human")
	UInputAction* IA_Crouch;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Human")
	UInputAction* IA_Look;

	//IMC_Drone
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Drone")
	UInputAction* IA_Drone_Move;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Drone")
	UInputAction* IA_Drone_Up;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Drone")
	UInputAction* IA_Drone_Down;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Drone")
	UInputAction* IA_Drone_RightRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Drone")
	UInputAction* IA_Drone_LeftRotation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Drone")
	UInputAction* IA_Drone_Look;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input|Drone")
	UInputAction* IA_Drone_Air;

	virtual void BeginPlay() override;

	void ChangeToHuman();
	void ChangeToDrone();
};
