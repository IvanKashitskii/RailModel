#include "MarketSegment.h"
#include <algorithm>
//RAILCAR_H

MarketSegment::MarketSegment(unsigned fleet, unsigned day_requests, const Network* Net, string optimizer,float req_irr, vector <const station*> LoadStations, bool redistribute):
	day_requests_(day_requests), operated_net_(Net), optimizer_(optimizer), requests_irregularity_(req_irr), redistribute_(redistribute)
{
	for (unsigned i = 1; i <= fleet; ++i) {
		unsigned step1 = rand() % this->get_net()->get_stations().size();
		const station* st1 = &(*(this->get_net()->get_stations().begin() + step1));
		RailCar* car1 = new RailCar(this, st1);								
		this->Fleet_.push_back(car1);
	}
	this->Available_RailCars_=Fleet_;
	this->Empty_RailCars_ = Fleet_;
	this->Requests_ = {};
	this->Actual_Requests_ = {};

	if (LoadStations.size() != 0) {
		this->Load_stations_ = LoadStations;
	}
	else {
		for (unsigned i = 1; i < this->get_net()->get_stations().size() / 2; ++i) {
			unsigned step2 = rand() % this->get_net()->get_stations().size();
			const station* st = &(*(this->get_net()->get_stations().begin() + step2));
			this->Load_stations_.push_back(st);
		}
	}

	for (unsigned i = 0; i < this->get_net()->get_stations().size(); ++i) {
		if (find(this->Load_stations_.begin(), this->Load_stations_.end(), &(*(this->get_net()->get_stations().begin()+i)))== this->Load_stations_.end()) {
			this->Unload_stations_.push_back(&(*(this->get_net()->get_stations().begin() + i)));
		}
	}
}

MarketSegment::~MarketSegment()
{
	for (auto now: this->Fleet_) {
		delete now;
	}
	for (auto now : this->Requests_) {
		delete now;
	}
}

const Network* const MarketSegment::get_net() const
{
	return operated_net_;
}

const float MarketSegment::requests_irregularity() const
{
	return this->requests_irregularity_;
}

const unsigned MarketSegment::day_requests() const
{
	return this->day_requests_;
}

const vector<RailCar*> MarketSegment::Fleet() const
{
	return this->Fleet_;
}

const vector<Request*> MarketSegment::Requests() const
{
	return this->Requests_;
}

const vector<RailCar*> MarketSegment::Available_RailCars() const
{
	return this->Available_RailCars_;
}

const vector<Request*> MarketSegment::Actual_Requests() const
{
	return this->Actual_Requests_;
}

const vector<RailCar*> MarketSegment::Empty_RailCars() const
{
	return this->Empty_RailCars_;
}

string MarketSegment::get_optimizer() const
{
	return this->optimizer_;
}

const vector<vector<int>> MarketSegment::to_simplymatrix(vector <RailCar*>& Distr_RailCars, string optimizer)
{
	vector<vector<int>> matrix = {};
	if (optimizer == "shortest_empty_runs") {
		for (auto car : Distr_RailCars) {
			if (dynamic_cast <const station*> (car->dislocation())) {
				const station* disloc = dynamic_cast <const station*> (car->dislocation());
					vector <int> str = {};
					for (auto request : this->Actual_Requests_) {
						str.push_back(this->operated_net_->get_route(disloc, request->origin(), "length")->lenght());
					}
				matrix.push_back(str);
			}
		}
	}
	else if (optimizer == "shortest_empty_runtimes") {
		for (auto car : Distr_RailCars) {
			const station* disloc = dynamic_cast <const station*> (car->dislocation());
			vector<int> str = {};
			float runtime = 0;
			int int_runtime = 0;
			for (auto request : this->Actual_Requests_) {
				runtime = this->operated_net_->get_route(disloc, request->origin(), "runtime")->runtime();
				// увеличиваем во избежание обнуления маленьких значений
				int_runtime = (int)(runtime * 10000);
				str.push_back(int_runtime);
			}
			matrix.push_back(str);
		}
	}
	else if (optimizer == "cheepest_empty_runtimes") {
		for (auto car : Distr_RailCars) {
			const station* disloc = dynamic_cast <const station*> (car->dislocation());
			vector <int> str = {};
			for (auto request : this->Actual_Requests_) {
				str.push_back(this->operated_net_->get_route(disloc, request->origin(), "length")->emptyrun_cost());
			}
			matrix.push_back(str);
		}
	}
	else if (optimizer == "max_profit") {
		for (auto car : Distr_RailCars) {
			const station* disloc = dynamic_cast <const station*> (car->dislocation());
			vector <int> str = {};
			for (auto request : this->Actual_Requests_) {
				str.push_back(this->operated_net_->get_route(disloc, request->origin(), "length")->emptyrun_cost()-request->price());
			}
			matrix.push_back(str);
		}
	}
	return matrix;
}

void MarketSegment::distribute(string optimizer) {
	
	// выбираем какой парк вагонов распраделять в зависимости от возможности перераспределния
	vector <RailCar*> &Distr_RailCars = this->Available_RailCars_;
	vector <RailCar*> Available_EmptyCars = {};
	if (this->redistribute_) {
		for (auto car : this->Empty_RailCars()) {
			if (dynamic_cast <const station*> (car->dislocation())) {
				Available_EmptyCars.push_back(car);
			}
		}
		Distr_RailCars = Available_EmptyCars;
	}

	if (Distr_RailCars.empty() || this->Actual_Requests_.empty()) return;
	vector<pair <int, int>> distribution = {};

	if (optimizer == "shortest_empty_runs"|| optimizer == "shortest_empty_runtimes" || optimizer == "cheepest_empty_runtimes"||optimizer=="max_profit") {
		
		vector<vector<int>> matrix = this->to_simplymatrix(Distr_RailCars, optimizer);
		size_t height = matrix.size(), width = matrix[0].size();
		bool added = false; // наличие фиктивных столбцов
		if (height > width) {
			added = true;
			for (size_t i = 0; i < height; ++i) {
				for (size_t j = width; j < height; ++j) {
					matrix[i].push_back(0);
				}
			}
		}
		distribution = hungarian(matrix);
		if (added) {
			distribution.erase(distribution.begin() + width, distribution.end());
		}
		for (auto now : distribution) {
			int car_num = now.first;
			int req_num = now.second;
			RailCar* car = *(Distr_RailCars.begin() + car_num);
			Request* request = *(this->Actual_Requests_.begin() + req_num);
			bool if_link = true;
			if (optimizer == "max_profit") {
				const station* disloc = dynamic_cast <const station*> (car->dislocation());
				int profit = request->price() - this->operated_net_->get_route(disloc, request->origin(), "length")->emptyrun_cost();
				if (profit<0) {
					if_link = false;
				}
			}
			if (if_link) {
				// обнуляем указатели на привязанные элементы
				*(Distr_RailCars.begin() + car_num) = nullptr;
				*(this->Actual_Requests_.begin() + req_num) = nullptr;
				car->link(request);
			}
		}
		//перемещаем все обнуленные в конец
		auto newEnd1 = remove_if(Distr_RailCars.begin(), Distr_RailCars.end(), [](RailCar* pointer) {
			return pointer == nullptr;
			});
		// Удаляем все элементы, следующие после
		Distr_RailCars.erase(newEnd1, Distr_RailCars.end());

		auto newEnd2 = remove_if(this->Actual_Requests_.begin(), this->Actual_Requests_.end(), [](Request* pointer) {
			return pointer == nullptr;
			});
		// Удаляем все элементы, следующие после
		this->Actual_Requests_.erase(newEnd2, this->Actual_Requests_.end());

	}

	else {
		auto j = this->Actual_Requests_.begin();
		while (j < this->Actual_Requests_.end()) {
			if (Distr_RailCars.size() != 0) {
				RailCar* car = Distr_RailCars.back();
				Distr_RailCars.pop_back();
				car->link(*j);
				this->Actual_Requests_.erase(j);
				j = this->Actual_Requests_.begin();
			}
			else break;
		}
	}
}

void MarketSegment::new_day() {
	std::random_device rd;						// разобраться для чего это нужно
	std::mt19937 gen(rd());
	const station* st1 = nullptr;
	const station* st2 = nullptr;
	std::bernoulli_distribution bern1(0.8);		// распределение Бернулли T|F с указанной вероятность T
	std::bernoulli_distribution bern2(0.8);
	std::uniform_real_distribution<float> dist3(0, 0.05);	//доля исчезающих заявок
	std::bernoulli_distribution bern3(dist3(gen));
	unsigned i = 0;
	for (Request* req : this->Requests()) {
		if (!req->is_loaded() && !req->is_fulfilled()) {
			if (bern3(gen)) {
				req->refuse();
				this->Requests_.erase(this->Requests_.begin() + i);
				--i;
				unsigned j = 0;
				for (auto av_req : this->Actual_Requests_) {
					if (av_req == req) {
						this->Actual_Requests_.erase(this->Actual_Requests_.begin() + j);
						--j;
					}
					++j;
				}
			}
		}
		++i;
	}
	// распределение числа заявок в диапазоне от (2-к)*ср до к*ср
	std::uniform_real_distribution<float> dist((2-this->requests_irregularity())*this->day_requests(),this->requests_irregularity()*this->day_requests());
	unsigned num_requests = (unsigned) dist(gen);

	for (unsigned i=0; i!=num_requests; ++i){
		while (st1 == st2) {
			if (bern1(gen)) {
				unsigned step1 = rand() % this->Load_stations_.size();
				st1 = *(this->Load_stations_.begin() + step1);
			}
			else {
				unsigned step1 = rand() % this->Unload_stations_.size();
				st1 = *(this->Unload_stations_.begin() + step1);
			}
			if (bern2(gen)) {
				unsigned step2 = rand() % this->Unload_stations_.size();
				st2 = *(this->Unload_stations_.begin() + step2);
			}
			else {
				unsigned step2 = rand() % this->Load_stations_.size();
				st2 = *(this->Load_stations_.begin() + step2);
			}
		}
		unsigned rate = 4000;
		std::uniform_real_distribution<float> dist2(0.7, 1.5); // распределение коэф к начальной ставке за вагон в сутки
		float k = dist2(gen);
		Request* req = new Request(st1, st2, rate);
		float runtime = this->operated_net_->get_route(st1, st2, "runtime")->runtime();
		req->set_price(rate * runtime*k);
		this->Requests_.push_back(req);
		this->Actual_Requests_.push_back(req);
	}

	for (auto now : this->Fleet()) {
		now->go_ahead();
	}

	this->distribute(this->get_optimizer());
}

map<string, string> MarketSegment::status() const
{
	map<string, string> status;
	status["Вагонов в оперировании"] = to_string(this->Fleet().size());
	status["Заявок принято всего"] = to_string(this->Requests().size());
	status["Свободных вагонов"] = to_string(this->Available_RailCars_.size());
	status["Доступных заявок"] = to_string(this->Actual_Requests_.size());
	status["Общий порожний пробег"] = to_string(this->empty_run());
	status["Общий груженый пробег"] = to_string(this->loaded_run());
	status["Погружено заявок"] = to_string(this->loaded_requests());
	status["Исполнено заявок"] = to_string(this->fulfilled_requests());
	status["Cредняя дальность перевозки"] = to_string(this->loaded_run() / this->fulfilled_requests());
	status["Cредний порожний пробег"] = to_string(this->empty_run() / this->loaded_requests());
	status["Коэффициент порожнего пробега"] = to_string(this->empty_run() / (this->loaded_run()+this->empty_run()));
	status["Погрузок на 1 вагон за период"] = to_string(float(this->loaded_requests()) / this->Fleet().size());
	return status;
}

float MarketSegment::empty_run() const
{
	float empty_run = 0;
	for (auto car : this->Fleet()) {
		empty_run += car->empty_run();
	}
	return empty_run;
}

float MarketSegment::loaded_run() const
{
	float loaded_run = 0;
	for (auto car : this->Fleet()) {
		loaded_run += car->loaded_run();
	}
	return loaded_run;
}

unsigned MarketSegment::loaded_requests() const
{
	unsigned n = 0;
	for (auto request : this->Requests()) {
		n += request->is_loaded();
	}
	return n;
}

unsigned MarketSegment::fulfilled_requests() const
{
	unsigned n = 0;
	for (auto request : this->Requests()) {
		n += request->is_fulfilled();
	}
	return n;
}
