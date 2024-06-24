// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaOfEffect/TGAOEBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ATGAOEBase::ATGAOEBase()
{
}

// Called when the game starts or when spawned
void ATGAOEBase::BeginPlay()
{
	Super::BeginPlay();

	OnAOEOverlapActor.AddDynamic(this, &ATGAOEBase::OnAOEOverlap);
	
	if (bIsTriggerOnBeginPlay) 
	{
		// Delay
		const float Duration = 3.f;
		//TimerHandle.Invalidate();

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{ 
				CollisionTrigger();
			}), Duration, true);
		//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}


void ATGAOEBase::CollisionTrigger()
{
	this->SpawnAOESphere();
}

void ATGAOEBase::SpawnAOESphere()
{
	if (bIsDrawDebugSphere) 
	{
		// 충돌한 경우 디버깅
		UKismetSystemLibrary::DrawDebugSphere(
			GetWorld(),
			GetActorLocation(),
			this->Radius,
			12,
			FLinearColor::Blue,
			3.f,
			1.f
		);

		SphereOverlapActors();
	}
	else {
		SphereOverlapActors();
	}
}


void ATGAOEBase::MakeArrayToChannel(const ECollisionChannel& CollisionChannel)
{
	// ECollisionChannel::ECC_Pawn
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(CollisionChannel);
	ObjectTypes.AddUnique(Pawn);
}

void ATGAOEBase::MakeArrayToPawn(TArray<AActor*>& InArray, APawn* InPawn)
{
	if (bIsIngoreInstigator) 
	{
		// 중복 없는 추가 (oper==로 검사)
		InArray.AddUnique(InPawn);
	}
	
}

// DECLARE_EVENT_OneParam
void ATGAOEBase::OnAOEOverlap(AActor* InActor)
{
	// 무언가의 재정의~~
	UE_LOG(LogTemp, Warning, TEXT("Yes Collision : %s"), *UKismetSystemLibrary::GetDisplayName(InActor));

	//if (OnAOEOverlapActor.IsBound()) {
	//	OnAOEOverlapActor.Broadcast(this);
	//}

	//FOnAOEOverlapActor OnAOEOverlap;
	//OnAOEOverlap.Broadcast(this);	
}

bool ATGAOEBase::SphereOverlapActors()
{
	// 충돌할 채널 타입 추가: 폰만 충돌
	MakeArrayToChannel(ECollisionChannel::ECC_Pawn);

	// 제외할 액터 배열 추가: 가해자(나) 제외
	MakeArrayToPawn(ActorsToIgnores, GetInstigator());

	// 겹친 액터들 포인터로 반환
	TArray<AActor*> OutActors;

	bool Result = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), 
		GetActorLocation(),
		this->Radius,
		ObjectTypes,
		nullptr,
		ActorsToIgnores,
		OutActors
	);

	// For Each Loop
	for (const auto& Actor : OutActors)
	{
		// 이벤트 바인딩
		//OnAOEOverlapActor.AddDynamic(this, &ATGAOEBase::OnAOEOverlap);
		if (OnAOEOverlapActor.IsBound()) 
		{
			OnAOEOverlapActor.Broadcast(Actor);
		}
		
		OnAOEOverlapEvent.AddLambda([&]()
			{
				OnAOEOverlap(Actor);
			});

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *UKismetSystemLibrary::GetDisplayName(Actor));
	}

	return Result;
}
