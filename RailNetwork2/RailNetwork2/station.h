#pragma once

#include "RailObject.h"
struct station: RailObject {
	station(string new_name);
	station();
	string get_name() const;
	void rename(string new_name);
	virtual int const get_id() const;
	virtual string print() const;
private:
	string name;
};

station const* find_station(const vector <station>& ST, int code);
station const* find_station(const vector <station>& ST, string st_name);

station* find_station(vector <station>& ST, int code);
station* find_station(vector <station>& ST, string st_name);

bool operator==(station const A, station const B);
bool operator!=(station const A, station const B);
