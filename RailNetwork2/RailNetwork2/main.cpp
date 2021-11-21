#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include "Network.h"

using namespace std;

Network& Create_Net() {
	int n = 0;
	cout << "—читать из файла - F, ввод с консоли - C" << endl << endl;
	char choise;
	cin >> choise;
	cout << endl;
	vector <station> ST;
	vector <line> LN;
	if (choise == 'C') {
		cout << "¬ведите число станций:\t";
		cin >> n;
		cout << endl;
		for (int i = 0; i < n; i++) {
			string temp_name = "";
			cout << "¬ведите название станции:\t";
			cin >> temp_name;
			ST.push_back(station(temp_name));
		}
		cout << endl;
		cout << "—танции и коды" << endl;
		for (auto now : ST) {
			cout << now.get_name() << "\t" << now.get_id() << endl;
		}
		cout << endl;
		int m = 0;
		cout << "¬ведите число перегонов:\t";
		cin >> m;
		int code1 = 0;
		int code2 = 0;
		float lenght = 1;
		cout << endl;
		for (int i = 0; i < m; i++) {
			string temp_name = "";
			cout << "¬ведите коды станций и длину перегонов:\t";
			cin >> code1 >> code2 >> lenght;
			LN.push_back(line(find_station(ST, code1), find_station(ST, code2), lenght));
		}
		cout << endl << "ѕеречень перегонов:" << endl;
		for (auto now : LN) {
			cout << (*now.station1()).get_name() << " - " << (*now.station2()).get_name() << endl;
		}
		cout << endl;
		cout << endl;
	}
	else if (choise == 'F') {
		string path;
		cout << "¬ведите адрес файла: " << '\t';
		cin >> path;
		cout << endl;
		try {
			Network& RailNet = Network::instance(path);
		}
		catch (std::exception err) {
			cout << err.what() << '\n';
			cout << "¬ведите корректный адрес файла" << '\t';
			cin >> path;
		}

	}
	Network& RailNet = Network::instance(ST, LN);
	return RailNet;
}


int main() {
	setlocale(LC_CTYPE, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	Network&  RailNet = Create_Net();
	// int num;
	// cout << "¬ведите код станции:\t ";
	// cin >> num;
	// cout << endl;
	// vector <route> routes = RailNet.short_routes(find_station(RailNet.get_stations(), num));
	// cout << " ратчайшие маршруты от станции " << find_station(RailNet.get_stations(), num)->get_name() << ":" << endl << endl;
	// for (auto now : routes) {
		// now.print();
		// cout << '\t' << now.lenght() << endl;
	//}

	for (auto st1:RailNet.get_stations()) {
		cout << " ратчайшие маршруты от станции " << st1.get_name() << ":" << endl << endl;
		for (auto st2 : RailNet.get_stations()) {
			const route* R=RailNet.get_route(&st1, &st2);
			R->print();
			cout << '\t' << R->lenght() << endl;
		}
		cout << endl;
	}
	system("pause");
	return 0;
}