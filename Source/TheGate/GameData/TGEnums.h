#pragma once

#include "CoreMinimal.h"
#include "TGEnums.generated.h"

// 블루프린트에서 사용 가능하도록 설정
UENUM(BlueprintType) 
enum class EPlayerStance : uint8
{
	Default UMETA(DisplayName = "Default"),
	Magic UMETA(DisplayName = "Magic"),
};