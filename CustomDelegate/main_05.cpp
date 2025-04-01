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

	// 为什么要加 typename？
	// 因为在模板上下文中，TMemberFuncPtr<...>::Type 是依赖于模板参数的“嵌套依赖类型”，
	// 编译器不知道它是一个类型还是一个静态成员变量名，所以你必须显式告诉它 “这是个类型”。

	// 这是 C++ 的语法规定，只要是模板中的类型成员（如::Type），并且依赖于模板参数，就必须加 typename。

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