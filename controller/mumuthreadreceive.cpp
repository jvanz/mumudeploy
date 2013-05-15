#include "mumuthreadreceive.h"
#include <iostream>

void MumuThreadReceive::run()
{
	std::cout << "Hi, I'm a receive thread :) #" << thread()->currentThreadId() << std::endl;
}
