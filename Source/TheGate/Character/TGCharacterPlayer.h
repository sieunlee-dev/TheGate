// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "TGCharacterPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
	Default UMETA(DisplayName = "Default"),
	Magic UMETA(DisplayName = "Magic"),
	Melee
};

template<typename EPlayerStance>
static __forceinline FString GetEnumerationToString(const EPlayerStance& InValue)
{
	// UE_LOG(LogClass, Warning, TEXT("View Mode : %S"), *GetEnumerationToString(Viewmode));
	return StaticEnum<EPlayerStance>()->GetNameStringByValue(static_cast<int64>(InValue));
}

/**
 * 
 */
UCLASS()
class THEGATE_API ATGCharacterPlayer : public ATGCharacterBase
{
	GENERATED_BODY()
		
public:
	ATGCharacterPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Character Control Section
protected:
	virtual void SetCharacterControlData(const class UTGCharacterControlData* CharacterControlData) override;
	void ChangeCharacterControl();
	void SetCharacterControl(ECharacterControlType NewCharacterControlType);

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UTGCharacterControlData*> CharacterControlManager;
	
// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ChangeControlAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ShoulderLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QuaterMoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	bool bIsInputLocked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	bool bIsMovmentLocked;

	ECharacterControlType CurrentCharacterControlType;

protected:
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);
	void QuaterMove(const FInputActionValue& Value);
	void Jumping(const FInputActionValue& Value);
	void Sprinting(const FInputActionValue& Value);	

#pragma region // Montage Test

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	void Attack();

#pragma endregion

protected:
	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void HitActionBegin(FString KeyName);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void HitActionEnd(class UAnimMontage* TargetMontage, bool bInterrupted = true);
	
	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void RollActionBegin();

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void RollActionEnd(class UAnimMontage* TargetMontage, bool bInterrupted = true);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGMoverComponent> MoverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGGrabberComponent> GrabberComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPhysicsHandleComponent> PhysicsHandleComponent;

	// BP에서 생성하고 넣어주자
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTimelineComponent> TimelineComponent;

// Roll Timeline
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Timeline, Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* RollCurve;

	// 타임라인 콜백
	UFUNCTION()
	void Rolling(const float CurveValue);

	FOnTimelineFloat RollingCallback;

// Stance Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	EPlayerStance PlayerStance;

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void SetMagicStance();

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void SetDefaultStance();

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void SetMeleeStance(AActor* const InWeaponActor);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void SetCameraOffset(const FVector& OutOffset);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	float MagicWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	float DefaultWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	float MeleeWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	FVector AimBoomOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	FVector DefaultBoomOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Controller, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APlayerController> PlayerController;

// Spawned Actor
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawned)
//	TObjectPtr<AActor> EquippedWeapon;
};
