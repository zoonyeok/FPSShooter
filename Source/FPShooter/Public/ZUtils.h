#pragma once

class ZUtils
{
public:
	/**
	 * Player의 컴포넌트 반환 템플릿으로 일반화
	 **/
	template<typename T>
	static T* GetZPlayerComponentByClass(APawn* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
};