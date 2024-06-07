// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TGMoverComponent.h"
#include "Math/UnrealMathUtility.h"

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

	//AActor* Owner = GetOwner();
	//FString Name = (*Owner).GetActorNameOrLabel();
	//UE_LOG(LogTemp, Display, TEXT("Mover Owner : %s"), *Name);

	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FVector TargetLocation = OriginalLocation + MoveOffset;
	// 속도 = 거리 / 시간
	float Speed = FVector::Distance(OriginalLocation, TargetLocation) / MoveTime;

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	GetOwner()->SetActorLocation(NewLocation);
}

