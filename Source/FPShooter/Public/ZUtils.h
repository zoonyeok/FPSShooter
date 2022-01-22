#pragma once

class ZUtils
{
public:
	template<typename T>
	static T* GetZPlayerComponentByClass(APawn* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};