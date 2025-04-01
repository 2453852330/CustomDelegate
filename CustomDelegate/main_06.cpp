#pragma once
#include <iostream>

template<typename RetType, typename... ParamTypes>
class IDelegateInstance
{
public:
	virtual RetType Execute(ParamTypes... Params) = 0;
	virtual bool IsBound() const = 0;
	virtual ~IDelegateInstance() = default;
};

template<typename RetType, typename ObjectClass, typename... ParamTypes>
struct TMemberFuncPtr
{
	typedef RetType(ObjectClass::* Type)(ParamTypes...);
};

template<typename ObjectClass,typename RetType, typename... ParamTypes>
class TBaseFunctorDelegateInstance : public IDelegateInstance<RetType,ParamTypes...>
{
public:
	using FMethodPtr = typename TMemberFuncPtr<RetType, ObjectClass, ParamTypes...>::Type;

	TBaseFunctorDelegateInstance(ObjectClass* in_obj, FMethodPtr in_func)
	{
		obj = in_obj;
		func = in_func;
	}
	virtual RetType Execute(ParamTypes... Params) override
	{
		return (obj->*func)(std::forward<ParamTypes>(Params)...);
	};
	virtual bool IsBound() const override
	{
		return true;
	};

private:
	ObjectClass* obj = nullptr;
	FMethodPtr func;
};


template<typename RetType,typename ...ParamTypes>
class TDelegate
{
public:
	void Reset()
	{
		if (DelegateInstance)
		{
			delete DelegateInstance;
			DelegateInstance = nullptr;
		}
	}

	RetType Execute(ParamTypes... Params)
	{
		if (DelegateInstance && DelegateInstance->IsBound())
		{
			return  DelegateInstance->Execute(std::forward<ParamTypes>(Params)...);
		}
	}

	// ΪʲôҪ�� typename��
	// ��Ϊ��ģ���������У�TMemberFuncPtr<...>::Type ��������ģ������ġ�Ƕ���������͡���
	// ��������֪������һ�����ͻ���һ����̬��Ա�������������������ʽ������ �����Ǹ����͡���

	// ���� C++ ���﷨�涨��ֻҪ��ģ���е����ͳ�Ա����::Type��������������ģ��������ͱ���� typename��

	template<typename ObjectClass>
	void BindObject(ObjectClass* object, typename TMemberFuncPtr<RetType,ObjectClass,ParamTypes...>::Type inFunc)
	{
		Reset();
		DelegateInstance = new TBaseFunctorDelegateInstance<ObjectClass,RetType,ParamTypes...>(
			object, inFunc
		);
	}

private:
	IDelegateInstance<RetType,ParamTypes...>* DelegateInstance = nullptr;
};



class ATool
{
public:
	void BindFunction()
	{
		std::cout << "bind function" << std::endl;
	}

	int CalcSum(int a,int b)
	{
		return  a + b;
	}
};

int main()
{
	ATool tool;

	TDelegate<void> t1;
	t1.BindObject(&tool, &ATool::BindFunction);
	t1.Execute();

	TDelegate<int,int,int> t2;
	t2.BindObject(&tool, &ATool::CalcSum);
	std::cout << " calc sum 3 + 4 = " << t2.Execute(3, 4) << std::endl;


	return 0;
}