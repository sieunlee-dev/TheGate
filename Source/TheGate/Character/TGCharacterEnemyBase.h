// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TGCharacterBase.h"
#include "TGCharacterEnemyBase.generated.h"


// BP 수정 가능한 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEndDelegate/*, bool, IsSuccess*/);
/**
 * 
 */
UCLASS()
class THEGATE_API ATGCharacterEnemyBase : public ATGCharacterBase
{
	GENERATED_BODY()
	
public:
	ATGCharacterEnemyBase();

protected:
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Custom", Meta = (AllowPrivateAccess = "true"))
	void MoveToTarget(const float InRadius, AActor* InGoal = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Custom", Meta = (AllowPrivateAccess = "true"))
	void DefaultAttack(const float PlayRate = 1.f);

	// 호출할 객체를 에디터 or 테스크에서 설정?
	UPROPERTY(BlueprintAssignable, Category = "Custom")
	FOnAttackEndDelegate OnAttackEnd;
	
	UFUNCTION()
	void BindAttackDelegate(const FOnAttackEndDelegate& InOnAttackEnd);

	// BP에서 바인딩 될 함수
	UFUNCTION(BlueprintCallable)
	void DefaultAttackEnd(class UAnimMontage* InMontage, bool InbInterrupted);

// Components
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkinnedMeshComponent> SkinnedMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AAIController_EnemyBase> AIController;
	//TObjectPtr<class AAIController> AIController;

// Variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;

protected:
	FTimerHandle DelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Custom)
	bool IsVisability;
	
};
