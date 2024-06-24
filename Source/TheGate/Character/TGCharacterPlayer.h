// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterBase.h"
#include "InputActionValue.h"
#include "TGCharacterPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
	Default UMETA(DisplayName = "Default"),
	Magic UMETA(DisplayName = "Magic"),
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

	ECharacterControlType CurrentCharacterControlType;

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void HitActionBegin(FString KeyName);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void OnHitActionEnd(class UAnimMontage* TargetMontage, bool bInterrupted = true);

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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGMoverComponent> MoverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTGGrabberComponent> GrabberComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPhysicsHandleComponent> PhysicsHandleComponent;

// Stance Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	EPlayerStance PlayerStance;

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void SetMagicStance();

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void SetDefaultStance();

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	void SetCameraOffset(const FVector& OutOffset);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	float MagicWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	float DefaultWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	FVector AimBoomOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stance)
	FVector DefaultBoomOffset;
	
};
