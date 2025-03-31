#pragma once
#include "Functor.h"
#include "Policy.h"

template<typename FuncType, typename UserPolicy = FDefaultDelegateUserPolicy>
class TDelegate
{
public:
	TDelegate(){}
	~TDelegate() { Reset(); }

	void Reset()
	{
		if (DelegateInstance)
		{
			delete DelegateInstance;
			DelegateInstance = nullptr;
		}
	}

	void Execute()
	{
		if (DelegateInstance && DelegateInstance->IsBound())
		{
			DelegateInstance->Execute();
		}
	}


	template<typename FunctorType>
	void BindLambda(FunctorType&& InFunctor)
	{
		Reset();
		using InstanceType = TBaseFunctorDelegateInstace<FuncType, UserPolicy, std::decay_t<FunctorType>>;
		DelegateInstance = new InstanceType(std::forward<FunctorType>(InFunctor));
	}


private:
	IDelegateInstance* DelegateInstance = nullptr;
};