// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TGAnimationAttackInterface.h"
#include "Interface/TGCharacterWidgetInterface.h"
#include "Interface/TGCharacterItemInterface.h"
#include "TGCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTGCharacter, Log, All);

UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};

DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UTGItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() {}
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class THEGATE_API ATGCharacterBase : public ACharacter, public ITGAnimationAttackInterface, public ITGCharacterWidgetInterface, public ITGCharacterItemInterface
{
	GENERATED_BODY()

public:
	ATGCharacterBase();

	//virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UTGCharacterControlData* CharacterControlData);

	
// Combo Action Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGComboActionData> ComboActionData;

	void ProcessComboCommand();
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	virtual void NotifyComboActionEnd();

	void SetComboCheckTimer();
	void ComboCheck();

	// UE Timer Variables
	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	int32 DeadDirection = 0;
	float DeadEventDelayTime = 5.0f;

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGCharacterStatComponent> Stat;

// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UTGUserWidget* InUserWidget) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> StopMontage;

	void StopAnimBegin();
	void StopAnimEnd();
	FTimerHandle StopTimerHandle;


// Sprint Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> JogMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float DefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool isCanMove;

	void SetSprint(const bool& bIsSprinting);

// DoubleJump Section


// Item Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions;

	virtual void TakeItem(class UTGItemData* InItemData) override;
	virtual void DrinkPotion(class UTGItemData* InItemData);
	virtual void EquipWeapon(class UTGItemData* InItemData);
	virtual void ReadScroll(class UTGItemData* InItemData);

// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
};
