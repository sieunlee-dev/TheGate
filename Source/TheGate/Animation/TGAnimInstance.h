// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TGAnimInstance.generated.h"


UENUM()
enum class ELocomotionType : uint8
{
	Idle,
	Jog,
	Sprint,
	Jump,
	Fall,
	Roll,
	None
};

UENUM()
enum class EJumpmotionType : uint8
{
	Land,
	Fall,
	Single,
	Double,
	None
};

UCLASS()
class THEGATE_API UTGAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTGAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ELocomotionType LocomotionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	ELocomotionType PreLocomotionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	EJumpmotionType JumpmotionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	//uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsRolling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float Direction;


protected:
	virtual void CheckLocomotion();
	virtual void CheckJumpmotion();
};
