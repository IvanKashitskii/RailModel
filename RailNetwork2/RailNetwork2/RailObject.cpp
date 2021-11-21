#include "RailObject.h"

int RailObject::count = 100000;

RailObject::RailObject() {
	count++;
	this->id = count;
}
