// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Misc/App.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyPawn::AMyPawn()
//리플렉션 프로퍼티
	: MaxMoveSpeed(600.0f), DashLength(1500.0f), RunSpeed(900.0f), CrouchSpeed(300.0f), MouseSpeed(3.0f),
	SpeedX(0), SpeedY(0), bIsCrouch(false), bIsDash(false), SpringArmLength_Human(300.0f),
	Gravity(980.0f), WingRotationSpeed(50.0f), FrontSpeed(800.0f), BackSpeed(400.0f), RotationSpeed(50.0f),
	//리플렉션 미적용 프로퍼티
	SpringArmLength_Drone(1500.0f), SlopeSpeed(10.0f),
	NowIMC(0), DashCount(0), CameraRate(0.0),
	SlopeX(0.0), SlopeY(0.0), OnOff(0)
{
	PrimaryActorTick.bCanEverTick = true;

	//컴포넌트 선언

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComp->InitCapsuleSize(34.0f, 88.0f);
	RootComponent = CapsuleComp;

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal mesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerSkeletalMesh(TEXT("/Game/Character/Friend.Friend"));
	SkeletalMeshComp->SetSkeletalMesh(PlayerSkeletalMesh.Object);

	StaticMeshCompBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drone Body mesh"));
	StaticMeshCompBody->SetupAttachment(CapsuleComp);
	ConstructorHelpers::FObjectFinder<UStaticMesh> PlayerDroneBodyMesh(TEXT("/Game/Fab/Sci-_Fi_Surveillance_Drone/sci_fi_surveillance_drone.sci_fi_surveillance_drone"));
	StaticMeshCompBody->SetStaticMesh(PlayerDroneBodyMesh.Object);

	StaticMeshCompWing1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drone Wing mesh1"));
	StaticMeshCompWing2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drone Wing mesh2"));
	StaticMeshCompWing3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drone Wing mesh3"));
	StaticMeshCompWing1->SetupAttachment(StaticMeshCompBody);
	StaticMeshCompWing2->SetupAttachment(StaticMeshCompBody);
	StaticMeshCompWing3->SetupAttachment(StaticMeshCompBody);
	ConstructorHelpers::FObjectFinder<UStaticMesh> PlayerDroneWingMesh(TEXT("/Game/Fab/Sci-_Fi_Surveillance_Drone/sci_fi_surveillance_drone_Submesh_2123F8BA1.sci_fi_surveillance_drone_Submesh_2123F8BA1"));
	StaticMeshCompWing1->SetStaticMesh(PlayerDroneWingMesh.Object);
	StaticMeshCompWing2->SetStaticMesh(PlayerDroneWingMesh.Object);
	StaticMeshCompWing3->SetStaticMesh(PlayerDroneWingMesh.Object);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = SpringArmLength_Human;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	//드론 숨김처리

	StaticMeshCompBody->SetVisibility(false);
	StaticMeshCompWing1->SetVisibility(false);
	StaticMeshCompWing2->SetVisibility(false);
	StaticMeshCompWing3->SetVisibility(false);
}
//회전 날개 /Game/Fab/Sci-_Fi_Surveillance_Drone/sci_fi_surveillance_drone_Submesh_2123F8BA1.sci_fi_surveillance_drone_Submesh_2123F8BA1
//헬리 콥터 /Game/Fab/Sci-_Fi_Surveillance_Drone/sci_fi_surveillance_drone.sci_fi_surveillance_drone
//캐릭터 /Game/Character/Friend.Friend

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	//프로퍼티 초기화
	
	bIsLand = true;
	MoveSpeed = MaxMoveSpeed;
	WingRotation = FRotator(0.0, WingRotationSpeed, 0.0);
	TickGravity = -Gravity;

}

void AMyPawn::DashLerp()
{
	DashCount++;
	AddActorWorldOffset(GetActorForwardVector() * (DashLength / 40));
	
	if (DashCount >= 40)
	{
		GetWorld()->GetTimerManager().ClearTimer(DashLerpHandle);
		bIsDash = false;
	}
}

void AMyPawn::DashDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(DashCoolDownHandle);
}

bool AMyPawn::bIsMove()
{
	if (FMath::IsNearlyZero(SpeedX) && FMath::IsNearlyZero(SpeedY))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void AMyPawn::CameraLengthToDrone()
{
	CameraRate++;
	float Alpha = CameraRate / 100;

	if (CameraRate < 100)
	{
		SpringArmComp->TargetArmLength = FMath::Lerp(SpringArmLength_Human, SpringArmLength_Drone, Alpha);
	}
	else if (CameraRate >= 100)
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraChangeHandle);
		CameraRate = 0;
	}
}

void AMyPawn::CameraLengthToHuman()
{
	CameraRate++;
	float Alpha = CameraRate / 100;

	if (CameraRate < 100)
	{
		SpringArmComp->TargetArmLength = FMath::Lerp(SpringArmLength_Drone, SpringArmLength_Human, Alpha);
	}
	else if (CameraRate >= 100)
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraChangeHandle);
		CameraRate = 0;
	}
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SlopeX = 0.0 - StaticMeshCompBody->GetRelativeRotation().Roll;
	SlopeY = 0.0 - StaticMeshCompBody->GetRelativeRotation().Pitch;

	//땅에 있는지 없는 지 체크합니다.
	bIsLand = UpdateGroundState(DeltaTime);

	StaticMeshCompWing1->AddRelativeRotation(WingRotation * DeltaTime);
	StaticMeshCompWing2->AddRelativeRotation(WingRotation * DeltaTime);
	StaticMeshCompWing3->AddRelativeRotation(WingRotation * DeltaTime);

	//현재 땅에 있다면 중력적용 x
	if (bIsLand)
	{
		WingRotation = FRotator(0.0, WingRotationSpeed / 30, 0.0);
	}
	else //현재 땅에 없다면 중력적용o
	{
		WingRotation = FRotator(0.0, WingRotationSpeed, 0.0);
		const FVector GravityScale = FVector(0, 0, TickGravity * DeltaTime);
		AddActorWorldOffset(GravityScale, true);

		StaticMeshCompBody->SetRelativeRotation(FRotator(StaticMeshCompBody->GetRelativeRotation().Pitch + (SlopeY * SlopeSpeed) * DeltaTime,
			StaticMeshCompBody->GetRelativeRotation().Yaw, 
			StaticMeshCompBody->GetRelativeRotation().Roll + (SlopeX * SlopeSpeed) * DeltaTime));
	}
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* myController = Cast<AMyPlayerController>(GetController()))
		{
			if (myController->IA_IMC_Changer)
			{
				EnhancedInput->BindAction(myController->IA_IMC_Changer, ETriggerEvent::Started, this, &AMyPawn::IMC_Changer);
			}

			//IMC_Human 바인딩

			if (myController->IA_Move)
			{
				EnhancedInput->BindAction(myController->IA_Move, ETriggerEvent::Triggered, this, &AMyPawn::Move);
			}
			if (myController->IA_Move)
			{
				EnhancedInput->BindAction(myController->IA_Move, ETriggerEvent::Completed, this, &AMyPawn::MoveStop);
			}
			if (myController->IA_Dash)
			{
				EnhancedInput->BindAction(myController->IA_Dash, ETriggerEvent::Started, this, &AMyPawn::Dash);
			}
			if (myController->IA_Run)
			{
				EnhancedInput->BindAction(myController->IA_Run, ETriggerEvent::Triggered, this, &AMyPawn::StartRun);
			}
			if (myController->IA_Run)
			{
				EnhancedInput->BindAction(myController->IA_Run, ETriggerEvent::Completed, this, &AMyPawn::StopRun);
			}
			if (myController->IA_Crouch)
			{
				EnhancedInput->BindAction(myController->IA_Crouch, ETriggerEvent::Started, this, &AMyPawn::Crouch);
			}
			if (myController->IA_Look)
			{
				EnhancedInput->BindAction(myController->IA_Look, ETriggerEvent::Triggered, this, &AMyPawn::Look);
			}

			//IMC_Drone 바인딩

			if (myController->IA_Drone_Move)
			{
				EnhancedInput->BindAction(myController->IA_Drone_Move, ETriggerEvent::Triggered, this, &AMyPawn::Drone_Move);
			}
			if (myController->IA_Drone_Up)
			{
				EnhancedInput->BindAction(myController->IA_Drone_Up, ETriggerEvent::Triggered, this, &AMyPawn::Drone_Up);
			}
			if (myController->IA_Drone_Down)
			{
				EnhancedInput->BindAction(myController->IA_Drone_Down, ETriggerEvent::Triggered, this, &AMyPawn::Drone_Down);
			}
			if (myController->IA_Drone_RightRotation)
			{
				EnhancedInput->BindAction(myController->IA_Drone_RightRotation, ETriggerEvent::Triggered, this, &AMyPawn::Drone_RightRotation);
			}
			if (myController->IA_Drone_LeftRotation)
			{
				EnhancedInput->BindAction(myController->IA_Drone_LeftRotation, ETriggerEvent::Triggered, this, &AMyPawn::Drone_LeftRotation);
			}
			if (myController->IA_Drone_Look)
			{
				EnhancedInput->BindAction(myController->IA_Drone_Look, ETriggerEvent::Triggered, this, &AMyPawn::Drone_Look);
			}
			if (myController->IA_Drone_Air)
			{
				EnhancedInput->BindAction(myController->IA_Drone_Air, ETriggerEvent::Started, this, &AMyPawn::Drone_Air);
			}
		}
	}
}

//IMC Human - Drone 전환

void AMyPawn::IMC_Changer(const FInputActionValue& value)
{
	//이미 실행중일 경우 실행하지 않음
	if (GetWorld()->GetTimerManager().IsTimerActive(CameraChangeHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("Timing!"));
		return;
	}

	AMyPlayerController* AMP = Cast<AMyPlayerController>(GetController());
	if (NowIMC == 0)
	{
		//움직이는 상태일 경우 실행하지 않음
		if (bIsCrouch || bIsDash || !bIsMove())
		{
			UE_LOG(LogTemp, Warning, TEXT("Moving!"));
			return;
		}
		else
		{
			NowIMC = 1;
			AMP->ChangeToDrone();

			SkeletalMeshComp->SetVisibility(false);

			StaticMeshCompBody->SetVisibility(true);
			StaticMeshCompWing1->SetVisibility(true);
			StaticMeshCompWing2->SetVisibility(true);
			StaticMeshCompWing3->SetVisibility(true);

			SpringArmComp->bUsePawnControlRotation = false;
			bUseControllerRotationYaw = false;

			GetWorld()->GetTimerManager().SetTimer(CameraChangeHandle, this, &AMyPawn::CameraLengthToDrone, 0.01f, true);
		}
	}
	else if (NowIMC == 1)
	{
		NowIMC = 0;
		AMP->ChangeToHuman();

		SkeletalMeshComp->SetVisibility(true);

		StaticMeshCompBody->SetVisibility(false);
		StaticMeshCompWing1->SetVisibility(false);
		StaticMeshCompWing2->SetVisibility(false);
		StaticMeshCompWing3->SetVisibility(false);

		StaticMeshCompBody->SetRelativeRotation(SkeletalMeshComp->GetRelativeRotation());

		SpringArmComp->bUsePawnControlRotation = true;
		bUseControllerRotationYaw = true;

		GetWorld()->GetTimerManager().SetTimer(CameraChangeHandle, this, &AMyPawn::CameraLengthToHuman, 0.01f, true);
	}
}

bool AMyPawn::UpdateGroundState(float DeltaTime)
{
	float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	const FVector CapsuleBottom = GetActorLocation() - FVector(0, 0, CapsuleHalfHeight);

	FVector StartTrace = CapsuleBottom;
	FVector EndTrace = CapsuleBottom - FVector(0,0,50);

	FHitResult GroundHit;

	UKismetSystemLibrary::LineTraceSingle(
		this,
		StartTrace,
		EndTrace,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), // 충돌 채널 (가시성)
		false,  // 물리적 충돌 검사 여부
		TArray<AActor*>(), // 무시할 액터 리스트
		EDrawDebugTrace::None,
		GroundHit,
		true); // return 값 설정 (HitResult.bBlockingHit 값 포함)

	return  GroundHit.bBlockingHit;
}

//여기부터 IMC_Human의 기능 바인딩

void AMyPawn::Move(const FInputActionValue& value)
{
	if (!Controller || bIsDash) { return; }
	
	const FVector2D InputValue = value.Get<FVector2D>();
	if (!FMath::IsNearlyZero(InputValue.X))
	{
		AddActorLocalOffset(FVector(InputValue.X * MoveSpeed * FApp::GetDeltaTime() , 0.0, 0.0));
		SpeedX = InputValue.X * MoveSpeed;
	}
	else
	{
		SpeedX = 0;
	}
	if (!FMath::IsNearlyZero(InputValue.Y))
	{
		AddActorLocalOffset(FVector(0.0f, InputValue.Y * MoveSpeed * FApp::GetDeltaTime(), 0.0));
		SpeedY = InputValue.Y * MoveSpeed;
	}
	else
	{
		SpeedY = 0;
	}
}

void AMyPawn::MoveStop(const FInputActionValue& value)
{
	SpeedX = 0;
	SpeedY = 0;
}

void AMyPawn::Dash(const FInputActionValue& value)
{
	if (!Controller || bIsCrouch || bIsDash) { return; }

	const bool InputValue = value.Get<bool>();
		//대쉬 실행, 쿨타임 적용
	if (!bIsDash)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(DashCoolDownHandle))
		{
			bIsDash = true;
			DashCount = 0;
			GetWorld()->GetTimerManager().SetTimer(DashLerpHandle, this, &AMyPawn::DashLerp, 0.01f, true);
			GetWorld()->GetTimerManager().SetTimer(DashCoolDownHandle, this, &AMyPawn::DashDelay, 2.0f, false);
			UE_LOG(LogTemp, Warning, TEXT("Dash!"));
		}
	}
}

void AMyPawn::StartRun(const FInputActionValue& value)
{
	if (!Controller || bIsCrouch) { return; }

	const bool InputValue = value.Get<bool>();
	if (InputValue)
	{
		MoveSpeed = RunSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Start Run"));
	}
}

void AMyPawn::StopRun(const FInputActionValue& value)
{
	if (!Controller || bIsCrouch) { return; }
	const bool InputValue = value.Get<bool>();
	if (!InputValue)
	{
		MoveSpeed = MaxMoveSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Stop Run"));
	}
}

void AMyPawn::Crouch(const FInputActionValue& value)
{
	if (!Controller) { return; }

	const bool InputValue = value.Get<bool>();
	if (!bIsCrouch)
	{
		bIsCrouch = true;
		MoveSpeed = CrouchSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Start Crouch"));
	}
	else
	{
		bIsCrouch = false;
		MoveSpeed = MaxMoveSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Stop Crouch"));
	}
}

void AMyPawn::Look(const FInputActionValue& value)
{
	if (!Controller) { return; }

	const FVector2D InputValue = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(InputValue.X))
	{
		AddControllerYawInput(InputValue.X * MouseSpeed);
	}
	if (!FMath::IsNearlyZero(InputValue.Y))
	{
		FRotator PitchRotator = SpringArmComp->GetRelativeRotation();
		PitchRotator.Pitch = FMath::Clamp(PitchRotator.Pitch + InputValue.Y, -89.0f, 89.0f);
		SpringArmComp->SetWorldRotation(PitchRotator * MouseSpeed);
	}
}

//여기부터 IMC_Drone의 기능 바인딩

void AMyPawn::Drone_Move(const FInputActionValue& value)
{
	if (!Controller) { return; }
	if (bIsLand) { return; }

	const FVector2D InputValue = value.Get<FVector2D>();
	FRotator CurrentRotation = StaticMeshCompBody->GetRelativeRotation();

	if (!FMath::IsNearlyZero(InputValue.X))
	{
		FRotator TempX = CurrentRotation;
		TempX.Roll = FMath::Clamp(TempX.Roll + InputValue.X * SlopeSpeed * 3 * FApp::GetDeltaTime(), -15.0f, 15.0f);
		TempX.Yaw = CurrentRotation.Yaw;
		StaticMeshCompBody->SetRelativeRotation(TempX);

		if (InputValue.X > 0)
		{
			AddActorLocalOffset(FVector(InputValue.X * FrontSpeed * FApp::GetDeltaTime(), 0.0, 0.0));
		}
		else if (InputValue.X < 0)
		{
			AddActorLocalOffset(FVector(InputValue.X * (FrontSpeed / 3) * FApp::GetDeltaTime(), 0.0, 0.0));
		}
	}
	if (!FMath::IsNearlyZero(InputValue.Y))
	{
		AddActorLocalOffset(FVector(0.0, InputValue.Y * BackSpeed * FApp::GetDeltaTime(), 0.0));

		FRotator TempY = CurrentRotation;
		TempY.Pitch = FMath::Clamp(TempY.Pitch + InputValue.Y * SlopeSpeed * 3 * FApp::GetDeltaTime(), -15.0f, 15.0f);
		TempY.Yaw = CurrentRotation.Yaw;
		StaticMeshCompBody->SetRelativeRotation(TempY);
	}
}

void AMyPawn::Drone_Up(const FInputActionValue& value)
{
	if (!Controller) { return; }

	bool InputValue = value.Get<bool>();
	if (InputValue)
	{
		AddActorLocalOffset(FVector(0.0, 0.0, InputValue * BackSpeed * FApp::GetDeltaTime()));
	}
}

void AMyPawn::Drone_Down(const FInputActionValue& value)
{
	if (!Controller) { return; }
	if (bIsLand) { return; }

	bool InputValue = value.Get<bool>();
	if (InputValue)
	{
		AddActorLocalOffset(FVector(0.0, 0.0, InputValue * -BackSpeed * FApp::GetDeltaTime()));
	}
}

void AMyPawn::Drone_RightRotation(const FInputActionValue& value)
{

	if (!Controller) { return; }
	if (bIsLand) { return; }

	if (value.Get<bool>())
	{
		AddActorLocalOffset(FVector((FrontSpeed / 2) * FApp::GetDeltaTime(), 0.0, 0.0));
		AddActorLocalRotation(FRotator(0.0,RotationSpeed * FApp::GetDeltaTime(), 0.0));
	}
}

void AMyPawn::Drone_LeftRotation(const FInputActionValue& value)
{
	if (!Controller) { return; }
	if (bIsLand) { return; }

	if (value.Get<bool>())
	{
		AddActorLocalOffset(FVector((FrontSpeed / 2) * FApp::GetDeltaTime(), 0.0, 0.0));
		AddActorLocalRotation(FRotator(0.0, -RotationSpeed * FApp::GetDeltaTime(), 0.0));
	}
}

void AMyPawn::Drone_Look(const FInputActionValue& value)
{
	if (!Controller) { return; }


	const FVector2D InputValue = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(InputValue.X))
	{
		SpringArmComp->AddLocalRotation(FRotator(0.0, InputValue.X, 0.0));
	}
	if (!FMath::IsNearlyZero(InputValue.Y))
	{
		SpringArmComp->AddLocalRotation(FRotator(InputValue.Y, 0.0, 0.0));
	}
}

void AMyPawn::Drone_Air(const FInputActionValue& value)
{
	if (!Controller) { return; }

	if (OnOff == 0)
	{
		OnOff = 1;
		TickGravity = 0.0;
	}
	else if (OnOff == 1)
	{
		OnOff = 0;
		TickGravity = -Gravity;
	}
}

