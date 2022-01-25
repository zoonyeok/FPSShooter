// zoonyeok All Rights Reserved.

#include "ZAssetActionUtility.h"
#include "EditorUtilityLibrary.h"

void UZAssetActionUtility::RenameSelectAsset(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase)
{
	if (SearchPattern.IsEmpty() || SearchPattern.Equals(ReplacePattern,SearchCase))
	{
		return;
	}

	TArray<UObject*> SelectAssets = UEditorUtilityLibrary::GetSelectedAssets();

	uint32 Counter = 0;

	for (UObject* Asset : SelectAssets)
	{
		if (ensure(Asset))
		{
			FString AssetName = Asset->GetName();
			if (AssetName.Contains(SearchPattern, SearchCase))
			{
				FString NewName = AssetName.Replace(*SearchPattern, *ReplacePattern, SearchCase);
				UEditorUtilityLibrary::RenameAsset(Asset, NewName);
				Counter++;
			}
		}
	}
	GiveFeedBack(TEXT("Renamed"), Counter);
}

void UZAssetActionUtility::PrintToScreen(FString Message, FColor Color)
{
	if (ensure(GEngine))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.5f, Color, Message);
	}
}

void UZAssetActionUtility::GiveFeedBack(FString Method, uint32 Counter)
{
	FString Message = FString("No matching files found");
	FColor Color = Counter > 0 ? FColor::Green : FColor::Red;
	if (Counter > 0)
	{
		Message = Method.AppendChar(' ');
		Message.AppendInt(Counter);
		Message.Append(Counter == 1 ? TEXT("file") : TEXT("files"));
	}
	PrintToScreen(Message, Color);
}