#include <iostream>

// �������з���ֵ

// �ӿ����� RetType
template<typename RetType,typename... ParamTypes>
class IDelegateInstance
{
public:
	virtual RetType Execute(ParamTypes...) = 0;
	virtual bool IsBound() const = 0;
	virtual ~IDelegateInstance() = default;
};


// ʵ������Ҳ���� RetType
template<typename RetType,typename FunctorType, typename... ParamTypes>
// �̳е�ʱ��Ҫ���Ǵ��� RetType
class TBaseFunctorDelegateInstance : public IDelegateInstance<RetType,ParamTypes...>
{
public:
	explicit TBaseFunctorDelegateInstance(FunctorType&& InFunc) :Functor(std::move(InFunc)) {}
	// ִ��
	virtual RetType Execute(ParamTypes... Params) override
	{
		return Functor(Params...);
	}

	virtual  bool IsBound() const override
	{
		return true;
	};


private:
	FunctorType Functor;
};


template<typename RetType,typename... ParamTypes>
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
	// delegate ʵ�����������:
	RetType Execute(ParamTypes... Params)
	{
		if (DelegateInstance && DelegateInstance->IsBound())
		{
			return DelegateInstance->Execute(std::forward<ParamTypes>(Params)...);
		}
	}

	template<typename FunctorType>
	void BindLambda(FunctorType&& InFunctor)
	{
		Reset();
		using InstanceType = TBaseFunctorDelegateInstance<RetType,std::decay_t<FunctorType>, ParamTypes...>;
		DelegateInstance = new InstanceType(std::forward<FunctorType>(InFunctor));
	}

private:
	IDelegateInstance<RetType,ParamTypes...>* DelegateInstance = nullptr;
};

//int main()
//{
//	TDelegate<int ,int, float> t1;
//	t1.BindLambda([](int a, float b)->int
//		{
//			std::cout << a << " | " << b << std::endl;
//			return 1;
//		});
//
//	int result = t1.Execute(12, 6.6f);
//
//	std::cout << result << std::endl;
//
//
//	return 0;
//}