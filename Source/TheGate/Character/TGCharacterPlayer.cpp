// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TGCharacterControlData.h"
#include "Component/TGMoverComponent.h"
#include "Component/TGGrabberComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Widget.h"
#include "TheGate.h"

ATGCharacterPlayer::ATGCharacterPlayer()
{
	PlayerStance = EPlayerStance::Default;
#pragma region Mesh

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Game/LearningKit_Games/Assets/Characters/Mesh/SK_EpicCharacter.SK_EpicCharacter"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	//static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/TheGate/Blueprint/ABP_Player.ABP_Player_C"));
	//if (AnimInstanceClassRef.Class)
	//{
	//	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	//}
#pragma endregion

#pragma region Camera

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

#pragma endregion

#pragma region Input

	// Add IMC
	static ConstructorHelpers::FObjectFinder<UTGCharacterControlData> ShoulderDataRef(TEXT("/Game/TheGate/DataAsset/Control/TGC_Shoulder.TGC_Shoulder"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UTGCharacterControlData> QuaterDataRef(TEXT("/Game/TheGate/DataAsset/Control/TGC_Quater.TGC_Quater"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	// Add IA
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/TheGate/Input/Actions/IA_Jump.IA_Jump"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Game/TheGate/Input/Actions/IA_ChangeControl.IA_ChangeControl"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Game/TheGate/Input/Actions/IA_ShoulderMove.IA_ShoulderMove"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Game/TheGate/Input/Actions/IA_ShoulderLook.IA_ShoulderLook"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Game/TheGate/Input/Actions/IA_QuaterMove.IA_QuaterMove"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintRef(TEXT("/Game/TheGate/Input/Actions/IA_Sprint.IA_Sprint"));
	if (nullptr != InputActionSprintRef.Object)
	{
		SprintAction = InputActionSprintRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Game/TheGate/Input/Actions/IA_Attack.IA_Attack"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;

#pragma endregion

#pragma region Components

	MoverComponent = CreateDefaultSubobject<UTGMoverComponent>(TEXT("Mover"));
	ensure(MoverComponent != nullptr);
	MoverComponent->SetupAttachment(GetCapsuleComponent());

	GrabberComponent = CreateDefaultSubobject<UTGGrabberComponent>(TEXT("Grabber"));
	ensure(GrabberComponent != nullptr);
	GrabberComponent->SetupAttachment(GetCapsuleComponent());

	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	ensure(PhysicsHandleComponent != nullptr);

	// https://zeniff.tistory.com/19
	TimelineComponent = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	ensure(TimelineComponent != nullptr);

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveRef(TEXT("/Game/TheGate/Player/RollCurve.RollCurve"));
	if (CurveRef.Succeeded())
		RollCurve = CurveRef.Object;

#pragma endregion

}

void ATGCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = CastChecked<APlayerController>(GetController());
	ensure(nullptr != PlayerController);

	SetCharacterControl(CurrentCharacterControlType);

	// 애님 이벤트 바인딩
	// https://forums.unrealengine.com/t/play-montage-in-c-with-onblendout-oninterrupted-etc/447184/2
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->OnMontageEnded.AddDynamic(this, &ATGCharacterPlayer::HitActionEnd);

	if (RollCurve && TimelineComponent)
	{
		// ONFUNCTION 매크로 필요
		//RollingCallback.BindUFunction(this, FName("Rolling"));
		RollingCallback.BindDynamic(this, &ATGCharacterPlayer::Rolling);

		TimelineComponent->AddInterpFloat(RollCurve, RollingCallback, FName("RollCurve"), FName("DistanceMultiplier"));
		TimelineComponent->SetLooping(false);
		//TimelineComponent->SetTimelineFinishedFunc()
	}
	
}

void ATGCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UTGCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);

	SetCharacterControlData(NewCharacterControl);

	//APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void ATGCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::Jumping);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATGCharacterPlayer::Jumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::ChangeCharacterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::QuaterMove);
	//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ATGCharacterPlayer::Attack);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATGCharacterPlayer::Sprinting);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATGCharacterPlayer::Sprinting);
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &ATGCharacterPlayer::RollActionBegin);
}

void ATGCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void ATGCharacterPlayer::SetCharacterControlData(const UTGCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ATGCharacterPlayer::HitActionBegin(FString KeyName)
{
	if (!HitMontages.Contains(KeyName))
		return;

	// 입력 제한
	DisableInput(PlayerController);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(HitMontages[KeyName]);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ATGCharacterPlayer::HitActionEnd);
	//AnimInstance->OnMontageEnded.AddDynamic(this, &ATGCharacterPlayer::HitActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, HitMontages[KeyName]);
}

void ATGCharacterPlayer::HitActionEnd(UAnimMontage* TargetMontage, bool bInterrupted)
{
	// 성공, 방해 둘다 바인딩?
	EnableInput(PlayerController);
}

void ATGCharacterPlayer::RollActionBegin(/*const FInputActionValue& Value*/)
{
	if (bIsMovmentLocked)
		return;

	bIsMovmentLocked = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RollMontage);

	// OnCompleted 델리게이트
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ATGCharacterPlayer::RollActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RollMontage);

	// 타임라인 재생
	if (RollCurve)
		TimelineComponent->PlayFromStart();
	//float TimelineValue = TimelineComponent->CalculateCurrentTimelineValue;
	//RollingCallback.ExecuteIfBound(TimelineValue);
}

void ATGCharacterPlayer::RollActionEnd(UAnimMontage* TargetMontage, bool bInterrupted)
{
	bIsMovmentLocked = false;
	UE_LOG(LogTemp, Log, TEXT("RollActionEnd"));

}

void ATGCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	if (!MoverComponent->GetIsCanMove())
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	MoverComponent->ShoulderMove(this, MovementVector);
}

void ATGCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	//if (!MoverComponent->GetIsCanMove())
	//	return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();
	MoverComponent->ShoulderLook(this, LookAxisVector);

	/// Grabber Test
	PlayerController = CastChecked<APlayerController>(GetController());
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

	FVector LookAtDirection = HitResult.ImpactPoint - GetActorLocation();
	FRotator LooAtRotation = FRotator(0.f, LookAtDirection.Rotation().Yaw, 0.f);
	
	GrabberComponent->MouseRotator = LooAtRotation;
}

void ATGCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	if (!MoverComponent->GetIsCanMove())
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();
	MoverComponent->QuaterMove(this, MovementVector);
}

void ATGCharacterPlayer::Jumping(const FInputActionValue& Value)
{
	if (!MoverComponent->GetIsCanMove())
		return;

	Value.IsNonZero() ? Jump() : StopJumping();
}

void ATGCharacterPlayer::Sprinting(const FInputActionValue& Value)
{
	//GetCharacterMovement()->MaxWalkSpeed = (bIsSprinting ? Stat->GetTotalStat().SprintSpeed : Stat->GetTotalStat().JogSpeed);
	//UE_LOG(LogTemp, Log, TEXT("Sprint : %f / Default : %f"), SprintSpeed, DefaultSpeed);

	GetCharacterMovement()->MaxWalkSpeed = MoverComponent->GetMovementSpeed(Value.IsNonZero());
}

void ATGCharacterPlayer::Rolling(const float CurveValue)
{
	if (!MoverComponent->GetIsCanMove())
		return;

	MoverComponent->Rolling(this, CurveValue);
}

void ATGCharacterPlayer::Attack()
{
	// Pressed Key
	Super::ProcessComboCommand();
}

void ATGCharacterPlayer::SetMagicStance()
{
	PlayerStance = EPlayerStance::Magic;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = MagicWalkSpeed;
}


void ATGCharacterPlayer::SetDefaultStance()
{
	PlayerStance = EPlayerStance::Default;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void ATGCharacterPlayer::SetMeleeStance(AActor* const InWeaponActor)
{
	PlayerStance = EPlayerStance::Melee;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->MaxWalkSpeed = MeleeWalkSpeed;

	/*
	// Spawn Actor
	if (nullptr != SpawnActorRef) {
		FVector Location = GetActorLocation() + FVector(200, 0, 0);
		FRotator Rotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 액터 스폰
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(SpawnActorRef, Location, Rotation, SpawnParams);
		if (SpawnedActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Spawned actor: %s"), *SpawnedActor->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn actor."));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn actor."));
	}*/
	
	if (nullptr != InWeaponActor) {
		SpawnActorObj = InWeaponActor;
		SpawnActorObj->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("Hand_RSocket"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn actor."));
	}

}

void ATGCharacterPlayer::SetCameraOffset(const FVector& OutOffset)
{
	CameraBoom->SocketOffset = OutOffset;
}

