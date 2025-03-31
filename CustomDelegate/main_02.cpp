#pragma once
#include <algorithm>
#include <iostream>

// 无参数无返回值
class IDelegateInstance
{
public:
	virtual void Execute() = 0;
	virtual bool IsBound() const = 0;
	virtual ~IDelegateInstance() = default;
};

template<typename FunctorType>
class TBaseFunctorDelegateInstance : public IDelegateInstance
{
public:
	explicit TBaseFunctorDelegateInstance(FunctorType && InFunc):Functor(std::move(InFunc)){}
	virtual  void Execute() override
	{
		Functor();
	};
	virtual  bool IsBound() const override
	{
		return true;
	};


private:
	FunctorType Functor;
};


//

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
	void BindLambda(FunctorType && InFunctor)
	{
		Reset();
		using InstanceType = TBaseFunctorDelegateInstance<std::decay_t<FunctorType>>;
		DelegateInstance = new InstanceType(std::forward<FunctorType>(InFunctor));
	}

private:
	IDelegateInstance* DelegateInstance = nullptr;
};


//int main()
//{
//	TDelegate t1;
//	t1.BindLambda([]()
//		{
//
//			std::cout << " hello world ! " << std::endl;
//		});
//	t1.Execute();
//
//
//	return 0;
//}