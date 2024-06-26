// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/TGGrabberComponent.h"
#include "DrawDebugHelpers.h"
#include "Physics/TGCollision.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UTGGrabberComponent::UTGGrabberComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void UTGGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UTGGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}	
}


UPhysicsHandleComponent* UTGGrabberComponent::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber has not PhysicsHandle"));
	}
	return PhysicsHandle;
}

bool UTGGrabberComponent::IsGrabbableInReach(FHitResult& OutHitResult) const
{
	// SceneComp는 위치가 있다.. 3인칭 전방은 캡슐이 이상적인듯
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	//DrawDebugSphere(GetWorld(), End, 10.f, 10.f, FColor::Blue, false , 5.f);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		CCHANNEL_TGGRABBER,
		Sphere);
}


void UTGGrabberComponent::Grab()
{
	// IA Started로 진입해야됨.. Why..?
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = IsGrabbableInReach(HitResult);
	if (HasHit)
	{	
		// 충돌 시 공간상의 위치, 투사해서 쓸고 지나가는 반경 있음
		//DrawDebugSphere(GetWorld(), HitResult.Location, 10.f, 10.f, FColor::Green, false, 5.f);
		// 충돌 시 실제로 건드리는 지점
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 10.f, FColor::Green, false, 5.f);

		// Handle 깨우기
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();		
		// 잡을 대상의 Physics -> Simulate Physics 활성화 해야 됨
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent,
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}

}

void UTGGrabberComponent::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if (PhysicsHandle == nullptr)
	{
		return;
	}

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicsHandle->ReleaseComponent();
	}

}


