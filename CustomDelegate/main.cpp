#pragma once
#include <iostream>

#include "TDelegate.h"


int main()
{
	TDelegate<void()> delegate;

	delegate.BindLambda([]()
		{
			std::cout << "Hello from Lambda!\n";
		});


	delegate.Execute();

	return 0;
}
