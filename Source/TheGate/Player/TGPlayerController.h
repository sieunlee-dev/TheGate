// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEGATE_API ATGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATGPlayerController();

public:
	const TObjectPtr<class UTGHUDWidget> GetPlayerHUDRef() {
		return HUDWidget;
	}
	void SetPlayerHUDRef(const TObjectPtr<class UTGHUDWidget> InPlayerHUDRef) {
		HUDWidget = InPlayerHUDRef;
	}

protected:
	virtual void BeginPlay() override;

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UTGHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UTGHUDWidget> HUDWidget;
};
