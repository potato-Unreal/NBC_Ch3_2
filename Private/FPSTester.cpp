// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSTester.h"

// Sets default values
AFPSTester::AFPSTester() : MoveSpeed(100.0f), RotationSpeed(100.0f), ScaleSpeed(100.0f), bSwitch(true), TickRate(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//씬 컴포넌트 생성, 루트 설정
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	SetRootComponent(SceneComp);
	//스태틱 메시 컴포넌트 3개 생성, 루트 설정
	StaticMeshComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh 1"));
	StaticMeshComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh 2"));
	StaticMeshComp3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh 3"));
	StaticMeshComp1->SetupAttachment(SceneComp);
	StaticMeshComp2->SetupAttachment(SceneComp);
	StaticMeshComp3->SetupAttachment(SceneComp);
	//스태틱 메시 할당
	ConstructorHelpers::FObjectFinder<UStaticMesh> Shield(TEXT("/Game/LP_RPGWeapons_JC/Models/SM_Shield_01.SM_Shield_01"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> Bow(TEXT("/Game/LP_RPGWeapons_JC/Models/SM_Bow_01.SM_Bow_01"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> Axe(TEXT("/Game/LP_RPGWeapons_JC/Models/SM_Ax_01.SM_Ax_01"));
	StaticMeshComp1->SetStaticMesh(Bow.Object);
	StaticMeshComp2->SetStaticMesh(Axe.Object);
	StaticMeshComp3->SetStaticMesh(Shield.Object);
}

void AFPSTester::MoveActor(bool value, float DeltaTime)
{
	FVector DeltaLocation(0.0f, 0.0f, 20.0f * DeltaTime);

	if (value == true)
	{
		StaticMeshComp1->AddWorldOffset(DeltaLocation);
	}
	else if (value == false)
	{
		StaticMeshComp1->AddWorldOffset(-DeltaLocation);
	}
}

void AFPSTester::RotationActor(bool value, float DeltaTime)
{
	FRotator DeltaRotation(0.0f, 30.0f * DeltaTime, 0.0f);

	if (value == true)
	{
		StaticMeshComp2->AddLocalRotation(DeltaRotation);
	}
	else if (value == false)
	{
		StaticMeshComp2->AddLocalRotation(DeltaRotation);
	}
}

void AFPSTester::ScaleActor(bool value, float DeltaTime)
{
	FVector DeltaScale(2.0f * DeltaTime, 2.0f * DeltaTime, 2.0f*DeltaTime);

	if (value == true)
	{
		StaticMeshComp3->SetWorldScale3D(StaticMeshComp3->GetComponentScale() + DeltaScale);
	}
	else if (value == false)
	{
		StaticMeshComp3->SetWorldScale3D(StaticMeshComp3->GetComponentScale() - DeltaScale);
	}
}

// Called every frame
void AFPSTester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	++TickRate;
	UE_LOG(LogTemp, Warning, TEXT("Log Count : %i"), TickRate);
	if (TickRate < 100) { bSwitch = true; }
	else if (TickRate >= 100 && TickRate < 200) { bSwitch = false; }
	else if (TickRate >= 200) { TickRate = 0; }

	MoveActor(bSwitch, DeltaTime);
	RotationActor(bSwitch, DeltaTime);
	ScaleActor(bSwitch, DeltaTime);
}

