#include "RailCar.h"
#include "MarketSegment.h"
#include <iostream>
#include <Windows.h>
using namespace std;

Network& Create_Net() {
	int n = 0;
	cout << "Считать из файла - F, ввод с консоли - C" << endl << endl;
	char choise;
	cin >> choise;
	cout << endl;
	vector <station> ST;
	vector <line> LN;
	if (choise == 'C') {
		cout << "Введите число станций:\t";
		cin >> n;
		cout << endl;
		for (int i = 0; i < n; i++) {
			string temp_name = "";
			cout << "Введите название станции:\t";
			cin >> temp_name;
			ST.push_back(station(temp_name));
		}
		cout << endl;
		cout << "Станции и коды" << endl;
		for (auto now : ST) {
			cout << now.get_name() << "\t" << now.get_id() << endl;
		}
		cout << endl;
		int m = 0;
		cout << "Введите число перегонов:\t";
		cin >> m;
		int code1 = 0;
		int code2 = 0;
		float lenght = 1;
		cout << endl;
		for (int i = 0; i < m; i++) {
			string temp_name = "";
			cout << "Введите коды станций и длину перегонов:\t";
			cin >> code1 >> code2 >> lenght;
			LN.push_back(line(find_station(ST, code1), find_station(ST, code2), lenght));
		}
		cout << endl << "Перечень перегонов:" << endl;
		for (auto now : LN) {
			cout << (*now.station1()).get_name() << " - " << (*now.station2()).get_name() << endl;
		}
		cout << endl;
		cout << endl;
	}
	else if (choise == 'F') {
		char runtime_fail = 'Y';
		// cout << "Времена хода заданы в отдельном файле? Y | N\t";
		// cin >> runtime_fail;
		//string path;
		string path = "RZDNet2_distances.txt";
		//cout << "Введите адрес файла с расстояниями: " << '\t';
		//cin >> path;
		cout << endl;
		string path2 = "";
		string path3 = "";
		if (runtime_fail == 'Y') {
			//string path2;
			path2 = "RZDNet2_runtimes.txt";
			//cout << "Введите адрес файла с временами хода: " << '\t';
			//cin >> path2;
			cout << endl;
		}
		char emptycost_fail = 'Y';
		// cout << "Стоимость порожних пробегов задана в отдельном файле? Y | N\t";
		// cin >> emptycosts_fail;
		//string path3;
		path3 = "RZDNet2_emptycosts.txt";
		// if (emptycost_fail == 'Y') {
			//cout << "Введите адрес файла со стоимостью порожних пробегов: " << '\t';
			//cin >> path3;
			//}
		cout << endl;
		try {
			Network& RailNet = Network::instance(path, path2, path3);
		}
		catch (std::exception err) {
			cout << err.what() << '\n';
			cout << "Введите корректный адрес файла" << '\t';
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
	Network& RailNet = Create_Net();
	unsigned days=1000;
	//cout << "Число дней : " << '\t';
	//cin >> days;
	//cout << '\n';
	unsigned fleet=240;
	unsigned requests=15;
	//cout << "Число вагонов : " << '\t';
	//cin >> fleet;
	//cout << "Число заявок в день: " << '\t';
	//cin >> requests;
	cout << '\n';
	//char req_irregularity;
	float req_irr=2;
	//cout << "Погрузки нерегулярны?" << '\t';
	//cin >> req_irregularity;
	//cout << '\n';
	//if (req_irregularity == 'Y') {
		//cout << "Введите коэффициент нераномерности (от 1 до 2)" << '\t';
		//cin >> req_irr;
		//cout << '\n';
	//}
	char create_l_st='Y';
	map <char, string> Optimizer_Map = { {'L', "shortest_empty_runs"}, {'T', "shortest_empty_runtimes"}, {'C',"cheepest_empty_runtimes"}, {'P',"max_profit"} };
	char optim_char='L';
	cout << "Выберитие праметр оптимизации" << '\n';
	cout << "Мнимизация пробега - L" << '\n';
	cout << "Мнимизация времени подачи - T" << '\n';
	cout << "Мнимизация затрат на пробег - C" << '\n';
	cout << "Максимизация прибыли - P" << '\n';
	cin >> optim_char;
	string optimizer = Optimizer_Map[optim_char];
	//cout << "Задать станции погрузки? Y | N" << '\n';
	//cin >> create_l_st;
	MarketSegment* Market1 = nullptr;
	MarketSegment* Market2 = nullptr;
	MarketSegment* Market3 = nullptr;
	if (create_l_st == 'Y') {
		vector <const station*> LoadStations = {};
		//unsigned st_num = 0;
		//cout << "Введите число станций погрузки" << '\n';
		//cin >> st_num;
		//cout << "Введите коды станций погрузки" << '\n';
		//for (auto now : RailNet.get_stations()) {
			//cout << now.get_id() << '\t'<< now.get_name()  ;
			//cout << '\n';
		//}
		cout << '\n';
		//for (unsigned i = 1; i <= st_num; ++i) {
			//int code = 0;
			//cout << "Станция" << i << ":" << '\t';
			//cin >> code;
			//cout << '\n';
			//LoadStations.push_back(RailNet.find_st(code));
		LoadStations.push_back(RailNet.find_st(100010));
		LoadStations.push_back(RailNet.find_st(100011));
		LoadStations.push_back(RailNet.find_st(100012));
		//}
		bool redistribute = false;
		Market1 = new MarketSegment(fleet/3, requests/3, &RailNet, "shortest_empty_runtimes", req_irr, LoadStations, redistribute);
		Market2 = new MarketSegment(fleet / 3, requests / 3, &RailNet, "cheepest_empty_runtimes", req_irr, LoadStations, redistribute);
		Market3 = new MarketSegment(fleet / 3, requests / 3, &RailNet, "max_profit", req_irr, LoadStations, redistribute);
	}
	else {
		Market1 = new MarketSegment(fleet, requests, &RailNet, optimizer, req_irr);
	}
	

	for (unsigned i = 1; i <= days; ++i) {
		cout << '\n';
		cout << "День " << i << '\n';
		Market1->new_day();
		cout << "Фрагмент1" << '\n';
		cout << "Свободных вагонов:" << '\t' << Market1->Available_RailCars().size() << '\t';
		cout << "Свободных заявок:" << '\t' << Market1->Actual_Requests().size() << '\n';
		Market2->new_day();
		cout << "Фрагмент2" << '\n';
		cout << "Свободных вагонов:" << '\t' << Market2->Available_RailCars().size() << '\t';
		cout << "Свободных заявок:" << '\t' << Market2->Actual_Requests().size() << '\n';
		Market3->new_day();
		cout << "Фрагмент3" << '\n';
		cout << "Свободных вагонов:" << '\t' << Market3->Available_RailCars().size() << '\t';
		cout << "Свободных заявок:" << '\t' << Market3->Actual_Requests().size() << '\n';
		// for (auto car : Market->Fleet()) {
			// for (auto now : car->status()) {
			// cout << now.second << '\t';
			// } 
			//cout << '\n';
		//}
	}
		
	cout << '\n';
	cout << "Дней анализа: " << days << '\n';
	cout << '\n';
	cout << "Фрагмент1" << '\n';
	for (auto now : Market1->status()) {
		cout << now.first << '\t' << now.second << '\n';
	}
	cout << "Оборот вагона" << '\t' << days / stof(Market1->status()["Погрузок на 1 вагон за период"])<<'\n';
	cout << '\n';
	cout << "Фрагмент2" << '\n';
	for (auto now : Market2->status()) {
		cout << now.first << '\t' << now.second << '\n';
	}
	cout << "Оборот вагона" << '\t' << days / stof(Market2->status()["Погрузок на 1 вагон за период"]) << '\n';
	cout << '\n';
	cout << "Фрагмент3" << '\n';
	for (auto now : Market3->status()) {
		cout << now.first << '\t' << now.second << '\n';
	}
	cout << "Оборот вагона" << '\t' << days / stof(Market3->status()["Погрузок на 1 вагон за период"]) << '\n';
	cout << '\n';
	cout << '\n';
	cout << "Общие данные" << '\n';
	cout << "Коэфф. пор. пробега" << '\t' << (float)(Market1->empty_run() + Market2->empty_run() + Market3->empty_run()) / (Market1->loaded_run() + Market2->loaded_run() + Market3->loaded_run()+ Market1->empty_run() + Market2->empty_run() + Market3->empty_run()) << '\n';
	unsigned loaded_requests= Market1->loaded_requests() + Market2->loaded_requests() + Market3->loaded_requests();
	cout << "Всего погружено вагонов" << '\t' << loaded_requests << '\n';
	cout << "Всего принято заявок" << '\t' << Market1->Requests().size() + Market2->Requests().size() + Market3->Requests().size() << '\n';
	float loads_per_car = (float)loaded_requests / fleet;
	float turnover = days / loads_per_car;
	cout << "Оборот вагона" << '\t' << turnover;
	cout << '\n';
	cout << "Tubli. Proovi edasi!"<< '\n';
	cout << "Hurra! kõik on korras!" << '\n';
	cout << '\n';
	system("pause");
	return 0;
}