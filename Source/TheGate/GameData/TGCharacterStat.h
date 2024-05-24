#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TGCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FTGCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FTGCharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.f) /*JogSpeed(0.f), SprintSpeed(0.f)*/ {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	//float JogSpeed;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	//float SprintSpeed;

	FTGCharacterStat operator+(const FTGCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FTGCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FTGCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};
