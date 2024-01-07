#include "Singleton.h"

Singleton& Singleton::GetInstance()
{
	static Singleton instance; // look into how legit this is
	
	return instance;
}
