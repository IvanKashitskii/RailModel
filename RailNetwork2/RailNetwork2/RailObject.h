#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
using namespace std;

struct RailObject
{
	
	RailObject();
	virtual int const get_id()const=0;
	virtual string print() const = 0;
	// абстрактный класс
protected:
	int id;
private:
	static int count;
};
