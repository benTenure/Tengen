#pragma once

/*

This will be a VERY simple version of the singleton. Not thread-safe at all but enough
to get some managers started

*/
class Singleton
{
protected:
	Singleton() {}

public:
	Singleton& GetInstance();

	Singleton(Singleton& other) = delete;
	void operator=(const Singleton&) = delete;
};

