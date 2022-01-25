// zoonyeok All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "ZAssetActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class FPSHOOTER_API UZAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:

	UFUNCTION(CallInEditor)
	void RenameSelectAsset(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase);

private:

	void PrintToScreen(FString Message, FColor Color);

	void GiveFeedBack(FString Method, uint32 Counter);
	
};
