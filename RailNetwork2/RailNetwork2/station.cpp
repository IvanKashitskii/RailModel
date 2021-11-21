#include "station.h"

station::station(string new_name) : name(new_name) {}

station::station() : name("") {}

string station::get_name() const {
	return this->name;
}

void station::rename(string new_name) {
	this->name = new_name;
}

int const station::get_id() const{
	return this->id;
}

string station::print() const
{
	return string("станция ") + this->get_name();
}

station const* find_station(const vector <station>& ST, int code) {
	for (unsigned i = 0; i<ST.size(); i++) {
		if ((ST[i]).get_id() == code) return &(ST[i]);
	}
	return nullptr;
}

station const* find_station(const vector<station>& ST, string st_name)
{
	for (unsigned i = 0; i < ST.size(); i++) {
		if ((ST[i]).get_name() == st_name) return &(ST[i]);
	}
	return nullptr;
}

station* find_station (vector <station>& ST, int code) {
	for (unsigned i = 0; i < ST.size(); i++) {
		if ((ST[i]).get_id() == code) return &(ST[i]);
	}
	return nullptr;
}

station* find_station(vector <station>& ST, string st_name) {
	for (unsigned i = 0; i < ST.size(); i++) {
		if ((ST[i]).get_name() == st_name) return &(ST[i]);
	}
	return nullptr;
}

bool operator==(station const A, station const B) {
	return (A.get_id() == B.get_id());
}

bool operator!=(station const A, station const B) {
	return !(A == B);
}