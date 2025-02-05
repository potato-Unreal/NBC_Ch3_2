// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

//회전 날개 /Game/Fab/Sci-_Fi_Surveillance_Drone/sci_fi_surveillance_drone_Submesh_2123F8BA1.sci_fi_surveillance_drone_Submesh_2123F8BA1
//헬리 콥터 /Game/Fab/Sci-_Fi_Surveillance_Drone/sci_fi_surveillance_drone.sci_fi_surveillance_drone

//캐릭터 /Game/Character/Friend.Friend

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			EnhancedInput->BindAction(PlayerController->IA_Move, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		}
	}
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (!Controller) return; 

	const FVector2D MoveValue = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveValue.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveValue.X);
	}
	if (!FMath::IsNearlyZero(MoveValue.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveValue.Y);
	}
}

