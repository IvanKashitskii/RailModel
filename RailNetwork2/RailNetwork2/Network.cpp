#include "Network.h"

Network& Network::instance(const vector <station>& ST, const vector <line>& LN, const string emptyrun_adress) {
	static Network N(ST, LN, emptyrun_adress);
	return N;
}

Network& Network::instance(const string main_adress, const string times_adress, const string emptyrun_adress) {
	ifstream fin;
	ifstream fin2;
	if (times_adress.size()) {
		fin2.open(times_adress);
		if (!fin2.is_open()) {
			throw std::invalid_argument("Path to file 2 is incorrect!");
		}
	}
	fin.open(main_adress);
	if (fin.is_open()) {
		string st_name = "";
		vector <station> ST;
		vector <line> LN;
		char ch='\0';
		while (ch != '\n') {
			fin >> st_name;
			fin.get(ch);
			station st(st_name);
			ST.push_back(st);
		}
		ch = '\0';
		if (!fin2.eof()) {
			while (ch != '\n') {
				fin2.get(ch);
			}
		}
		if (!fin.eof()) {
			ch = '\0';
			for (int i = 0; i < ST.size();++i) {
				if (!fin2.eof()) {
					fin2 >> st_name;
				}
				fin >> st_name;
				float length = 0;
				float runtime = 0;
				for (int j = 0; j < ST.size();++j) {
					fin >> length;
					fin.get(ch);
					if (!fin2.eof()) {
						fin2 >> runtime;
						fin2.get(ch);
					}
					if (length != 0) {
						if (fin2.eof()) {
							line new_line(&ST[i], &ST[j], length);
							LN.push_back(new_line);
						}
						else {
							line new_line(&ST[i], &ST[j], length, runtime);
							LN.push_back(new_line);
						}
					}
				}
			}
	
		}
		Network& N = Network::instance(ST, LN, emptyrun_adress);
		return N;
		fin.close();
		if (times_adress.size()) {
			fin2.close();
		}
	}
	else throw std::invalid_argument("Path to file is incorrect!");
}

Network::Network(const vector <station>& ST, const vector <line>& LN, const string emptyrun_adress) : ST(ST) {
	bool empty = false;
	ifstream fin3;
	if (emptyrun_adress.size()) {
		empty = true;
		fin3.open(emptyrun_adress);
		if (!fin3.is_open()) {
			throw std::invalid_argument("Path to file 3 is incorrect!");
		}
	}
	for (auto now : LN) {
		int st1=now.station1()->get_id();
		int st2 = now.station2()->get_id();
		line new_line(find_station(this->ST, st1), find_station(this->ST, st2), now.length(), now.runtime());
		this->LN.push_back(new_line);
	}
	this->to_graph("length");
	this->to_graph("runtime");
	this->Routes_length = this->all_short_routes();
	this->Routes_runtime = this->all_short_routes("runtime");
	
	if (empty) {
		if (fin3.is_open()) {
			string st_name = "";
			vector <unsigned> stations;
			char ch = '\0';
			while (ch != '\n') {
				fin3 >> st_name;
				unsigned st_code = find_station(this->get_stations(), st_name)->get_id();
				fin3.get(ch);
				stations.push_back(st_code);
			}
			ch = '\0';
			if (!fin3.eof()) {
				ch = '\0';
				for (int i = 0; i < ST.size(); ++i) {
					fin3 >> st_name;
					const station* st1 = find_station(this->get_stations(), st_name);
					fin3.get(ch);
					unsigned cost = 0;
					for (int j = 0; j < ST.size(); ++j) {
						const station* st2 = this->find_st(stations[j]);
						fin3 >> cost;
						fin3.get(ch);
						route* rt = this->get_route(st1, st2, "length");
						rt->set_emptyruncost(cost);
						rt = this->get_route(st1, st2, "runtime");
						rt->set_emptyruncost(cost);
					}
				}
			}

		}
		fin3.close();
	}

}
//????
route* Network::create_routematrix() {
	size_t n = this->ST.size();
	route* m = new route [n];
	return m;
}

Network::~Network() {
	if (!Routes_length.empty()) {
		for (auto now : Routes_length) {
			delete now;
		}
		Routes_length.clear(); 
	}
	if (!Routes_runtime.empty()) {
		for (auto now : Routes_runtime) {
			delete now;
		}
		Routes_runtime.clear();
	}
}
// создание топологий - графов сети
void Network::to_graph(string by) {
	if (by != "length" and by != "runtime") throw std::invalid_argument("Edge weight is not indicated!");
	if ((Topology_length.is_empty() and by == "length") or (Topology_runtime.is_empty() and by == "runtime")) {
		vector <node> Nodes;
		vector <edge> Edges;
		for (auto now : ST) {
			Nodes.push_back(node(now.get_id()));
		}
		for (auto now : LN) {
			float w = 1;
			if (by == "length") w = now.length();
			else if (by == "runtime") w = now.runtime();
			int code1 = now.station1()->get_id();
			int code2 = now.station2()->get_id();
			node* node1 = find(Nodes, code1);
			node* node2 = find(Nodes, code2);
			Edges.push_back(edge(node1, node2, w));
		}
		Graph G(Nodes, Edges);
		if (by == "length") {
			Topology_length = G;
		}
		else if (by == "runtime") {
			Topology_runtime = G;
		}
	}
}
// доступ к топологиям
Graph Network::get_graph(string by) const{
	if (by != "length" and by != "runtime") throw std::invalid_argument("Edge weight is not indicated!");
	if (by == "runtime") {
		return Topology_runtime;
	}
	else {
		return Topology_length;
	}
}

const vector <station>& Network::get_stations() const {
	return this->ST;
}

const vector <line>& Network::get_lines() const {
	return this->LN;
}
// возвращает индекс станции (по ее указателю) в векторе станций сети
int Network::find_posit(station const* st) const {
	for (unsigned i = 0; i < this->ST.size();++i) {
		if (ST[i] == *st) return i;
	}
	return -1;
}

// преоразования от элементов графа к элементам сети
station const* Network::to_obj(node const* N) const {
	return find_station(this->ST, N->get_num());
}

line const* Network::to_obj(edge const* E) const {
	return find(this->LN, to_obj(E->get_node1()), to_obj(E->get_node2()));
}
// поиск перегона между двумя станциями, если нет - нулевой указатель
line const* Network::find_line(station const* A, station const* B) const {
	for (unsigned i = 0; i < this->LN.size(); ++i) {
		if ((LN[i].station1() == A && LN[i].station2() == B) || (LN[i].station1() == B && LN[i].station1() == A)) return &LN[i];
	}
	return nullptr;
}

station const* Network::find_st(unsigned code)
{
	return find_station(this->get_stations(), code);
}

// создает вектор кратчайших маршрутов от заданной станции, by - критерий - по умолчанию длина
vector <route> Network::short_routes(station const* from, string by) const {
	Graph G;
	// определяем критерий краткости
	if (by == "length") G = this->Topology_length;
	else if (by == "runtime") G = this->Topology_runtime;
	else {
		throw std::invalid_argument("Incorrect argument for the shortest paths!");
	}
	G.refresh();
	// индекс начальной станции в векторе станций сети
	int current = this->find_posit(from);
	// вектор расстояний от начальной станции до каждой
	vector<float> dists(this->ST.size(), 50000);
	dists[current] = 0;
	node const* cur_node = &G.nodes()[current];
	station const* cur_st = this->to_obj(cur_node);
	// исключения
	vector <int> exceptions = {};
	// стартовый маршрут из одной станции
	route start_r = route();
	start_r.set_start(cur_st);
	start_r.push_back(cur_st);
	start_r.set_destination(cur_st);
	// вектор маршрутов от начальной станции до каждой
	vector <route> routes(this->ST.size(), start_r);
	while (exceptions.size()!=this->ST.size()) {
		vector <edge> to_visit = G.find_all_to_visit(cur_node);
		if (!to_visit.empty()) {
			for (auto now : to_visit) {
				node const* check_node = nullptr;
				if (now.get_node1() != cur_node) check_node = now.get_node1();
				else check_node = now.get_node2();
				int check = find_pos(G.nodes(), check_node);
				if (dists[check] > dists[current] + now.get_weight()) {
					dists[check] = dists[current] + now.get_weight();
					route temp_route = route();
					temp_route.set_start(cur_st);
					temp_route.push_back(cur_st);
					line const* check_line = this->to_obj(&now);
					temp_route.set_lenght(temp_route.lenght()+check_line->length());
					temp_route.set_runtime(temp_route.runtime() + check_line->runtime());
					temp_route.push_back(check_line);
					station const* const check_st = this->to_obj(check_node);
					temp_route.push_back(check_st);
					temp_route.set_destination(check_st);
					routes[check].empty();
					routes[check] = routes[current] + temp_route;
				}
			}
		}
		cur_node->visit();
		exceptions.push_back(current);
		auto point_min = excep_min(dists, exceptions);
		if (point_min != dists.end()) {
			current = distance(dists.begin(), point_min);
			cur_node = &G.nodes()[current];
			cur_st = this->to_obj(cur_node);
		}
	}
	return routes;
}

// вектор указателей на векторы кратчайших маршрутов: от каждой станции до каждой, by - критерий - по умолчанию длина
vector <vector <route>*> Network::all_short_routes(string by) const {
	vector <vector <route>*> VRoutes;
	for (auto station : this->ST) {
		vector<route>* R = new vector<route>;
		*R = this->short_routes(&station, by);
		VRoutes.push_back(R);
	}
	return VRoutes;
}

// получение маршрута между станциями по указателям на них
route* Network::get_route(station const* st1, station const* st2, string by) const  {
	const vector <vector<route>*>* Routes;
	if (by == "length") Routes = &this->Routes_length;
	else if (by == "runtime") Routes = &this->Routes_runtime;
	else {
		throw std::invalid_argument("Incorrect argument for the shortest paths!");
	}
	vector<route>* VR = (*Routes)[this->find_posit(st1)];
	route* R = &(*VR)[this->find_posit(st2)];
	return R;
}