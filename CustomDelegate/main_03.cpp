#include <iostream>

// 带参数无返回值
template<typename... ParamTypes>
class IDelegateInstance
{
public:
	virtual void Execute(ParamTypes...) = 0;
	virtual bool IsBound() const = 0;
	virtual ~IDelegateInstance() = default;
};


template<typename FunctorType,typename... ParamTypes>
class TBaseFunctorDelegateInstance : public IDelegateInstance<ParamTypes...>
{
public:
	explicit TBaseFunctorDelegateInstance(FunctorType&& InFunc) :Functor(std::move(InFunc)) {}

	virtual void Execute(ParamTypes... Params) override
	{
		Functor(Params...);
	}

	virtual  bool IsBound() const override
	{
		return true;
	};


private:
	FunctorType Functor;
};


template<typename... ParamTypes>
class TDelegate
{
public:
	TDelegate() {}
	~TDelegate() { Reset(); }
	void Reset()
	{
		if (DelegateInstance)
		{
			delete DelegateInstance;
			DelegateInstance = nullptr;
		}
	}

	void Execute(ParamTypes... Params)
	{
		if (DelegateInstance && DelegateInstance->IsBound())
		{
			DelegateInstance->Execute(std::forward<ParamTypes>(Params)...);
		}
	}

	template<typename FunctorType>
	void BindLambda(FunctorType && InFunctor)
	{
		Reset();
		using InstanceType = TBaseFunctorDelegateInstance<std::decay_t<FunctorType>,ParamTypes...>;
		DelegateInstance = new InstanceType(std::forward<FunctorType>(InFunctor));
	}

private:
	IDelegateInstance<ParamTypes...>* DelegateInstance = nullptr;
};

//int main()
//{
//	TDelegate<int,float> t1;
//	t1.BindLambda([](int a ,float b)
//	{
//			std::cout << a << " | " << b << std::endl;
//	});
//
//	t1.Execute(12, 6.6f);
//	t1.Execute(14, 6.8f);
//
//	return 0;
//}