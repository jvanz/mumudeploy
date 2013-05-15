#include "mumuthreadsend.h"
#include <iostream>

void MumuThreadSend::run()
{
	std::cout << "Hi, I'm a send thread :) #" << thread()->currentThreadId() << std::endl;
}
