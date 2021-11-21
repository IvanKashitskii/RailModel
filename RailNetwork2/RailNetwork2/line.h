#pragma once
#include <string>
#include "station.h"
struct line: RailObject {
	line(station* const st1, station* const st2, float len, float time = 0, float speed = 60);
	virtual int const get_id() const;
	virtual string print() const;
	float length() const;
	float runtime() const;
	station* station1() const;
	station* station2() const;
private:
		station* st1;
		station* st2;
		float length_;
		float runtime_;
};

line const* find(const vector <line>& A, station const* a, station const* b);
line* find(vector <line>& A, station const* a, station const* b);


