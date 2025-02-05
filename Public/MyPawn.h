// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class NBC_CH3_2_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	//컴포넌트 선언

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshCompBody;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshCompWing1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshCompWing2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshCompWing3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	//프로퍼티 선언 - IMC_Human / 리플렉션 적용

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Human Movemont")
	float MaxMoveSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Human Movemont")
	float DashLength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Human Movemont")
	float RunSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Human Movemont")
	float CrouchSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Human Movemont")
	float MouseSpeed;
	UPROPERTY(BlueprintReadOnly)
	float SpeedX;
	UPROPERTY(BlueprintReadOnly)
	float SpeedY;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCrouch;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDash;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Human Camera")
	float SpringArmLength_Human;

	//프로퍼티 선언 - IMC_Drone / 리플렉션 적용

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Drone Movemont")
	float Gravity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Drone Movemont")
	float WingRotationSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Drone Movemont")
	float FrontSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Drone Movemont")
	float BackSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Drone Movemont")
	float RotationSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|HitActor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> HitActor;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Drone Camera")
	float SpringArmLength_Drone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Property|Drone Movemont")
	float SlopeSpeed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//프로퍼티 선언 - IMC_Human / 리플렉션 미적용

	int NowIMC;
	float MoveSpeed;
	int DashCount;
	FTimerHandle DashLerpHandle;
	FTimerHandle DashCoolDownHandle;
	float CameraRate;

	//프로퍼티 선언 - IMC_Drone / 리플렉션 미적용

	float SlopeX;
	float SlopeY;
	bool bIsLand;
	FRotator WingRotation;
	FTimerHandle CameraChangeHandle;
	int OnOff;
	float TickGravity;

	//IMC_Human 보조 함수
	void DashLerp();
	void DashDelay();
	bool bIsMove();

	//IMC_Drone 보조 함수
	void CameraLengthToDrone();
	void CameraLengthToHuman();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void IMC_Changer(const FInputActionValue& value);


	bool UpdateGroundState(float DeltaTime);

	//IMC_Human

	void Move(const FInputActionValue& value);
	void MoveStop(const FInputActionValue& value);
	void Dash(const FInputActionValue& value);
	void StartRun(const FInputActionValue& value);
	void StopRun(const FInputActionValue& value);
	void Crouch(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	//IMC_Drone

	void Drone_Move(const FInputActionValue& value);
	void Drone_Up(const FInputActionValue& value);
	void Drone_Down(const FInputActionValue& value);
	void Drone_RightRotation(const FInputActionValue& value);
	void Drone_LeftRotation(const FInputActionValue& value);
	void Drone_Look(const FInputActionValue& value);
	void Drone_Air(const FInputActionValue& value);
};
