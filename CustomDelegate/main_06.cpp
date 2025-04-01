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

	// 为什么要加 typename？
	// 因为在模板上下文中，TMemberFuncPtr<...>::Type 是依赖于模板参数的“嵌套依赖类型”，
	// 编译器不知道它是一个类型还是一个静态成员变量名，所以你必须显式告诉它 “这是个类型”。

	// 这是 C++ 的语法规定，只要是模板中的类型成员（如::Type），并且依赖于模板参数，就必须加 typename。

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