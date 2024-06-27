// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TGCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TGCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "TGComboActionData.h"
#include "Physics/TGCollision.h"
#include "Engine/DamageEvents.h"
#include "Stat/TGCharacterStatComponent.h"
#include "UI/TGWidgetComponent.h"
#include "UI/TGHpBarWidget.h"
#include "Item/TGWeaponItemData.h"

DEFINE_LOG_CATEGORY(LogTGCharacter);

// Sets default values
ATGCharacterBase::ATGCharacterBase()
{	
#pragma region Main Components

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_TGCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(CPROFILE_NOCOLLISION);

#pragma endregion


#pragma region Montage Assets

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/TheGate/Animation/AM_ComboAttack.AM_ComboAttack"));
	//if (ComboActionMontageRef.Object)
	//{
	//	ComboActionMontage = ComboActionMontageRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UTGComboActionData> ComboActionDataRef(TEXT("/Game/TheGate/DataAsset/Action/TGA_ComboAttack.TGA_ComboAttack"));
	//if (ComboActionDataRef.Object)
	//{
	//	ComboActionData = ComboActionDataRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/TheGate/Animation/AM_Dead.AM_Dead"));
	//if (DeadMontageRef.Object)
	//{
	//	DeadMontage = DeadMontageRef.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> StopMontageRef(TEXT("/Game/TheGate/Animation/AM_JogToStand.AM_JogToStand"));
	//if (StopMontageRef.Object)
	//{
	//	StopMontage = StopMontageRef.Object;
	//}

#pragma endregion

#pragma region Stat & Widget Components

	// Stat Component
	Stat = CreateDefaultSubobject<UTGCharacterStatComponent>(TEXT("Stat"));

	//// Widget Component
	//HpBar = CreateDefaultSubobject<UTGWidgetComponent>(TEXT("Widget"));
	//HpBar->SetupAttachment(GetMesh());
	//HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f)); 
	//static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/TheGate/UI/WBP_HpBar.WBP_HpBar_C"));
	//if (HpBarWidgetRef.Class)
	//{
	//	HpBar->SetWidgetClass(HpBarWidgetRef.Class);
	//	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	//	HpBar->SetDrawSize(FVector2D(200.f, 20.f));
	//	HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}

#pragma endregion

#pragma region Item Assets

	// Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ATGCharacterBase::EquipWeapon)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ATGCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ATGCharacterBase::ReadScroll)));

	// Weapon Component
	//Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	//Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

#pragma endregion
	
}

void ATGCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Stat->OnHpZero.AddUObject(this, &ATGCharacterBase::SetDead);

	//FString ClassName = GetClass()->GetDefaultObjectName().ToString();
	//UE_LOG(LogTemp, Log, TEXT("%s / HpBar Exist = %d"), *ClassName, HpBar);
}


void ATGCharacterBase::SetCharacterControlData(const UTGCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
	GetCharacterMovement()->MaxWalkSpeed = CharacterControlData->DefaultSpeed;
}

#pragma region Combo Action Montage

void ATGCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	// Check Next Command Input
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void ATGCharacterBase::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// Callback ComboActionEnd
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ATGCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ATGCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void ATGCharacterBase::NotifyComboActionEnd()
{

}

void ATGCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ATGCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void ATGCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	// Next Montage Section
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

#pragma endregion

void ATGCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
	const float AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_TGACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	//FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	//float CapsuleHalfHeight = AttackRange * 0.5f;
	//FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

float ATGCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	// Fwd = 0.9 / bwd = -0.9
	//Actor -> GetActorForwardVector();
	const FRotator YawRotation(0, EventInstigator->GetControlRotation().Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	DeadDirection = .0 < ForwardDirection.X ? 1 : 0;	

	//SetDead();
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void ATGCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
}

void ATGCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();	
	AnimInstance->StopAllMontages(0.0f);

	//FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, DeadDirection);
	//UE_LOG(LogTemp, Log, TEXT("%s"), NextSection);
	//AnimInstance->Montage_JumpToSection(NextSection, DeadMontage);

	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void ATGCharacterBase::SetupCharacterWidget(UTGUserWidget* InUserWidget)
{
	UTGHpBarWidget* HpBarWidget = Cast<UTGHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetTotalStat().MaxHp);
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UTGHpBarWidget::UpdateHpBar);
	}
}

void ATGCharacterBase::StopAnimBegin()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const float StopSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(StopMontage, StopSpeedRate);

	float Rate = AnimInstance->GetCurrentActiveMontage()->GetAnimationData(TEXT("StopSlot"))->GetLength();
	StopTimerHandle.Invalidate();
	GetWorldTimerManager().SetTimer(StopTimerHandle, this, &ATGCharacterBase::StopAnimEnd, Rate, false);
}

void ATGCharacterBase::StopAnimEnd()
{
	//ensure(CurrentCombo != 0);
	//CurrentCombo = 0;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance->Montage_IsPlaying(StopMontage))
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		GetWorldTimerManager().ClearTimer(StopTimerHandle);
	}
}

void ATGCharacterBase::TakeItem(UTGItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void ATGCharacterBase::DrinkPotion(UTGItemData* InItemData)
{
	UE_LOG(LogTGCharacter, Log, TEXT("Drink Potion"));
}

void ATGCharacterBase::EquipWeapon(UTGItemData* InItemData)
{
	UTGWeaponItemData* WeaponItemData = Cast<UTGWeaponItemData>(InItemData);
	if (WeaponItemData)
	{
		if (WeaponItemData->WeaponMesh.IsPending())
		{
			WeaponItemData->WeaponMesh.LoadSynchronous();
		}
		Weapon->SetSkeletalMesh(WeaponItemData->WeaponMesh.Get());
		Stat->SetModifierStat(WeaponItemData->ModifierStat);
	}
}

void ATGCharacterBase::ReadScroll(UTGItemData* InItemData)
{
	UE_LOG(LogTGCharacter, Log, TEXT("Read Scroll"));
}

int32 ATGCharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void ATGCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}
