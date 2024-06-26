// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TGMoverComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UTGMoverComponent::UTGMoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTGMoverComponent::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetOwner()->GetActorLocation();

}


// Called every frame
void UTGMoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// TestFunction();

	//AActor* Owner = GetOwner();
	//FString Name = (*Owner).GetActorNameOrLabel();
	//UE_LOG(LogTemp, Display, TEXT("Mover Owner : %s"), *Name);

	//FVector CurrentLocation = GetOwner()->GetActorLocation();
	//FVector TargetLocation = OriginalLocation + MoveOffset;
	//// 속도 = 거리 / 시간
	//float Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;

	//FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	//GetOwner()->SetActorLocation(NewLocation);
}

UCharacterMovementComponent* UTGMoverComponent::GetMovementComponent() const
{
	UCharacterMovementComponent* MovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	if (MovementComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Mover has not MovementComponent"));
	}
	return MovementComponent;
}

void UTGMoverComponent::ShoulderMove(APawn* const InPawn, const FVector2D& MovementVector)
{
	const FRotator Rotation = InPawn->GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	InPawn->AddMovementInput(ForwardDirection, MovementVector.X);
	InPawn->AddMovementInput(RightDirection, MovementVector.Y);
}

void UTGMoverComponent::ShoulderLook(APawn* const InPawn, const FVector2D& LookAxisVector)
{
	InPawn->AddControllerYawInput(LookAxisVector.X);
	InPawn->AddControllerPitchInput(LookAxisVector.Y);
}

void UTGMoverComponent::QuaterMove(APawn* const InPawn, FVector2D& MovementVector)
{
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f;
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	InPawn->GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	InPawn->AddMovementInput(MoveDirection, MovementVectorSize);
}

void UTGMoverComponent::Rolling(APawn* const InPawn, const float CurveValue)
{
	// Size2D()
	FVector RollVector = InPawn->GetActorForwardVector() * CurveValue * RollDistance;

	UCharacterMovementComponent* Movement = GetMovementComponent();
	RollVector.Z = Movement->Velocity.Z;

	Movement->Velocity = RollVector;
}


