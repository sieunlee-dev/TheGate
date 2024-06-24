// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TGAOEBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAOEOverlapActor, AActor*, Actor);

UCLASS()
class THEGATE_API ATGAOEBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATGAOEBase();

public:
	// 클래스 내부에 작성, 인자로 주인 클래스
	//DECLARE_EVENT_OneParam(ATGAOEBase, FOnAOEOverlapEvent, AActor*)
	DECLARE_EVENT(ATGAOEBase, FOnAOEOverlapEvent)

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	virtual void CollisionTrigger();

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	virtual void SpawnAOESphere();

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	virtual void MakeArrayToChannel(const ECollisionChannel& CollisionChannel);

	UFUNCTION(BlueprintCallable, Category = "Custom Function")
	virtual void MakeArrayToPawn(TArray<AActor*>& InArray, APawn* InPawn);

	virtual void OnAOEOverlap(AActor* InActor);

	bool SphereOverlapActors();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Initalize)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Initalize)
	TArray<AActor*> ActorsToIgnores;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initalize)
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initalize)
	bool bIsIngoreInstigator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initalize)
	bool bIsDrawDebugSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initalize)
	bool bIsTriggerOnBeginPlay;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	FOnAOEOverlapActor OnAOEOverlapActor;

	FOnAOEOverlapEvent OnAOEOverlapEvent;

	FTimerHandle TimerHandle;
};
