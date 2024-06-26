// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TGMoverComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEGATE_API UTGMoverComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTGMoverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	class UCharacterMovementComponent* GetMovementComponent() const;
	const float& GetMovementSpeed(const bool& OutIsSprint) const { return OutIsSprint ? SprintSpeed : DefaultSpeed ; }
	const bool& GetIsCanMove() const { return bIsCanMove; }
	void SetIsCanMove(const bool& InIsCanMove) { bIsCanMove = InIsCanMove; }

public:
	void ShoulderMove(APawn* const InPawn, const FVector2D& MovementVector);
	void ShoulderLook(APawn* const InPawn, const FVector2D& LookAxisVector);
	void QuaterMove(APawn* const InPawn, FVector2D& MovementVector);
	void Rolling(APawn* const InPawn, const float CurveValue);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	float DefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	bool bIsCanMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (AllowPrivateAccess = "true"))
	float RollDistance;

private:
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	UPROPERTY(EditAnywhere)
	FVector OriginalLocation;
		
	UPROPERTY(EditAnywhere)
	float MoveTime = 4.f;

	UPROPERTY(EditAnywhere)
	bool ShouldMove = false;
};
