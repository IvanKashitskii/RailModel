#include "line.h"
line:: line(station* const st1, station* const st2, float len, float time, float speed) : st1(st1), st2(st2), length_(len)
{
	if (time == 0) runtime_ = (float)len / (float)speed/24;
	else this->runtime_ = time;
}

int const line::get_id() const {
	return this->id;
}

string line::print() const
{
	return string("перегон ") + this->st1->get_name() + string(" - ") + this->st2->get_name();
}

float line::length() const{
	return this->length_;
}

float line::runtime() const {
	return this->runtime_;
}

station*  line::station1() const {
	return this->st1;
}
station* line::station2() const {
	return this->st2;
}

line const* find(const vector <line>& A, station const* a, station const* b) {
	for (unsigned i = 0; i < A.size(); i++) {
		if ((A[i].station1() == a) && (A[i].station2() == b) || (A[i].station2() == a) && (A[i].station1() == b)) {
			return &A[i];
		}	
	}
	return nullptr;
}

line* find(vector <line>& A, station const* a, station const* b) {
	for (unsigned i = 0; i < A.size(); i++) {
		if ((A[i].station1() == a) & (A[i].station2() == b) || (A[i].station2() == a) & (A[i].station1() == b)) {
			return &A[i];
		}
	}
	return nullptr;}


bool operator==(line const A, line const B) {
	return (A.get_id() == B.get_id());
}

bool operator!=(line const A, line const B) {
	return !(A == B);
}