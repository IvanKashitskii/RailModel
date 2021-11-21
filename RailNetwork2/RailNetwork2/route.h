#pragma once
#include <vector>
#include "station.h"
#include "line.h"

struct route
{
	route();
	route(route const& r);
	vector <RailObject const*> get() const;
	void push_back(RailObject const* obj);
	void pop_back();
	void pop_front();
	void empty();
	size_t size() const;
	RailObject const* begin() const;
	RailObject const* end() const;
	vector <RailObject const*>::const_iterator find(const RailObject* obj1) const;
	float lenght() const;
	float runtime() const;
	unsigned emptyrun_cost() const;
	route operator+(route const& r);
	RailObject const* operator[](unsigned i) const;
	void print() const;
	station const* from() const;
	station const* to() const;

	friend struct Network;

private:
	void set_start(station const* st);
	void set_destination(station const* st);
	void set_lenght(float const lenght);
	void set_runtime(float const runtime);
	void set_emptyruncost(unsigned const runcost);
	route reverse(route const& r) const;
	station const* from_=nullptr;
	station const* to_=nullptr;
	vector <RailObject const*> route_ = {};
	float lenght_=0;
	float runtime_ = 0;
	unsigned emptyrun_cost_ = 0;
};

route const  check(route const& R);

