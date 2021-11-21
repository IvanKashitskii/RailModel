#pragma once
#include "station.h"
#include "line.h"
#include "route.h"
#include "..\..\Graph algorithms\Graph algorithms\Graph.h"
#include "helper_func.hpp"

struct Network
{
	static Network& instance(const vector <station>& ST, const vector <line>& LN, const string emptyrun_adress = "");
	static Network& instance(const string main_adress, const string times_adress="", const string emptyrun_adress="");
	~Network();
	Graph get_graph(string by="length") const;
	const vector <station>& get_stations() const;
	const vector <line>& get_lines() const;
	station const* to_obj(node const* N) const;
	line const* to_obj(edge const* E) const;
	line const* find_line(station const* A, station const* B) const;
	station const* find_st(unsigned code);
	int find_posit(station const* st) const;
	route* get_route(station const* st1, station const* st2, string by="length") const;
	
private:
	Network(const vector <station>& ST, const vector <line>& LN, const string emptyrun_adress = "");
	Network(Network const&) {};
	Network& operator=(Network const&) {};
	void to_graph(string by = "length");
	route* create_routematrix();
	vector <vector <route>*> all_short_routes(string by = "length") const;
	vector <route> short_routes(station const* from, string by="length") const;
	vector <station> ST;
	vector <line> LN;
	Graph Topology_length;
	Graph Topology_runtime;
	vector <vector <route>*> Routes_length;
	vector <vector <route>*> Routes_runtime;
};

