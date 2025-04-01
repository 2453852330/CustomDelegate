#pragma once
#include <iostream>

class IDelegateInstance
{
public:
	virtual void Execute() = 0;
	virtual bool IsBound() const = 0;
	virtual ~IDelegateInstance() = default;
};

template<typename RetType, typename ObjectClass, typename... ParamTypes>
struct TMemberFuncPtr
{
	typedef RetType(ObjectClass::* Type)(ParamTypes...);
};

template<typename ObjectClass>
class TBaseFunctorDelegateInstance : public IDelegateInstance
{
public:
	using FMethodPtr = typename TMemberFuncPtr<void, ObjectClass>::Type;

	TBaseFunctorDelegateInstance(ObjectClass* in_obj, FMethodPtr in_func)
	{
		obj = in_obj;
		func = in_func;
	}
	virtual void Execute() override
	{
		(obj->*func)();
	};
	virtual bool IsBound() const override
	{
		return true;
	};

private:
	ObjectClass* obj = nullptr;
	FMethodPtr func;
};



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
	void Execute()
	{
		if (DelegateInstance && DelegateInstance->IsBound())
		{
			DelegateInstance->Execute();
		}
	}

	// ΪʲôҪ�� typename��
	// ��Ϊ��ģ���������У�TMemberFuncPtr<...>::Type ��������ģ������ġ�Ƕ���������͡���
	// ��������֪������һ�����ͻ���һ����̬��Ա�������������������ʽ������ �����Ǹ����͡���

	// ���� C++ ���﷨�涨��ֻҪ��ģ���е����ͳ�Ա����::Type��������������ģ��������ͱ���� typename��

	template<typename ObjectClass>
	void BindObject(ObjectClass* object,typename TMemberFuncPtr<void,ObjectClass>::Type inFunc)
	{
		Reset();
		DelegateInstance = new TBaseFunctorDelegateInstance<ObjectClass>(
			object, inFunc
		);
	}

private:
	IDelegateInstance* DelegateInstance = nullptr;
};



class ATool
{
public:
	void BindFunction()
	{
		std::cout << "bind function" << std::endl;
	}
};

//int main()
//{
//	ATool tool;
//
//	TDelegate t1;
//	t1.BindObject(&tool, &ATool::BindFunction);
//	t1.Execute();
//
//	return 0;
//}