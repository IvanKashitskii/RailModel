#include "route.h"

route::route() {
	this->from_ = nullptr;
	this->to_ = nullptr;
	this->lenght_ = 0;
	this->runtime_ = 0;
	this->emptyrun_cost_ = 0;
	this->route_ = {};
	
}

route::route(route const& r):from_(r.from_), to_(r.to_), lenght_(r.lenght_), runtime_(r.runtime_) {
	for (auto now : r.route_) {
		this->route_.push_back(now);
	}
}

vector <RailObject const*> route::get() const {
	return route_;
}

void route::push_back(RailObject const* obj) {
	route_.push_back(obj);
}

void route::pop_back() {
	route_.pop_back();
}

void  route::pop_front() {
	route_.erase(route_.begin());
}

void route::empty() {
	route_.empty();
	lenght_ = 0;
	runtime_ = 0;
	emptyrun_cost_ = 0;
}

size_t route::size() const {
	return route_.size();
}


RailObject const* route::begin() const {
	return *route_.begin();
}

RailObject const* route::end() const {
	return *(route_.end()-1);
}

vector<RailObject const*>::const_iterator route::find(const RailObject* obj1) const
{
	auto iter = this->route_.begin();
	for (auto obj : this->route_) {
		if (obj == obj1) {
			return iter;
		}
		++iter;
	}
	return this->route_.end();
}

float route::lenght() const {
	return lenght_;
}

float route::runtime() const
{
	return this->runtime_;
}

unsigned route::emptyrun_cost() const
{
	return this->emptyrun_cost_;
}

void route::set_start(station const* st) {
	this->from_ = st;
}

void route::set_destination(station const* st) {
	this->to_ = st;
}

void route::set_lenght(float const lenght) {
	this->lenght_ = lenght;
}

void route::set_runtime(float const runtime)
{
	this->runtime_ = runtime;
}

void route::set_emptyruncost(unsigned const runcost)
{
	this->emptyrun_cost_ = runcost;
}

station const* route::from() const {
	return this->from_;
}

station const* route::to() const {
	return this->to_;
}
// "переворачивает" маршрут
route route::reverse(route const& r) const{
	route rotated_route;
	rotated_route.set_start(this->to_);
	rotated_route.set_destination(this->from_);
	rotated_route.set_lenght(this->lenght_);
	vector <RailObject const*> rotated_route_;
	for (unsigned i = r.size()-1; 0; --i) {
		rotated_route.route_.push_back(r[i]);
	}
	return rotated_route;
}

RailObject const* route::operator[](unsigned i) const 
{
	return this->route_[i];
}

route route::operator+(route const& r) {
	route add1;
	route add2;
	 if (this->end() != r.begin()) {
		if (this->end() == r.end()) {
			add2 = reverse(r);
		}
		else if (this->begin() == r.begin()) {
			add1 = reverse(*this);
			add2 = r;
		}
		else if (this->begin() == r.end()) {
			add2 = *this;
			add1 = r;
		}
		else {
			throw std::logic_error("Inconsistent routes!");
		}
	}

	else {
		add1 = *this;
		add2 = r;
	}
	route R;
	R.from_ = add1.from_;
	R.to_ = add2.to_;
	R.lenght_ = add1.lenght_ + add2.lenght_;
	R.runtime_ = add1.runtime() + add2.runtime();
		for (unsigned i = 0; i < add1.size(); ++i) {
			R.push_back(add1.route_[i]);
		}
		for (unsigned i = 1; i < add2.size(); ++i) {
			R.push_back(add2.route_[i]);
		}
	
	return R;
}

void route::print()const {
	for (auto now : route_) {
		if (station const* st = dynamic_cast <station const*> (now)) cout << st->get_name();
		else cout << " - ";
	}
}

// ??проверяет "законченность" маршрута: что концевые элементы вектора соответствуют точкам начала и конца маршрута
route const check(route const& R) {
	// if ((R.begin() == R[0]) && (R.end() == R[R.size()])) return R;
	// else return route();
	return R;
}