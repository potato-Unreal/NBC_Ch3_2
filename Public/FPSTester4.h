// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSTester4.generated.h"

UCLASS()
class NBC_CH3_2_API AFPSTester4 : public AActor
{
	GENERATED_BODY()

public:
	AFPSTester4();

	USceneComponent* SceneComp;
	UStaticMeshComponent* StaticMeshComp1;
	UStaticMeshComponent* StaticMeshComp2;
	UStaticMeshComponent* StaticMeshComp3;

	float MoveSpeed;
	float RotationSpeed;
	float ScaleSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool bSwitch;
	UPROPERTY(BlueprintReadOnly)
	int TickRate;

	void MoveActor(bool value, float DeltaTime);
	void RotationActor(bool value, float DeltaTime);
	void ScaleActor(bool value, float DeltaTime);

	virtual void Tick(float DeltaTime) override;
};
