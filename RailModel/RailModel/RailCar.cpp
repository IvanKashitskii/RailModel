#include "RailCar.h"

	RailCar::RailCar(MarketSegment* const mark_segment_, const station* st):
		vehicle(mark_segment_->get_net(), st), mark_segment_(mark_segment_)
	{									// создание нового вагона
		load_status_ = false;
		to_request_ = nullptr;
		route_ = nullptr;
		time_on_line_ = 0;
		empty_run_ = 0;
		loaded_run_ = 0;
		}
	
	void RailCar::load() {								// погрузить
		this->load_status_ = true;
		unsigned i = 0;
		for (auto car: this->mark_segment_->Empty_RailCars()) {
			if (car == this) {
				this->mark_segment_->Empty_RailCars_.erase(this->mark_segment_->Empty_RailCars_.begin() + i);
				return;
			}
			++i;
		}
	}

	void RailCar::unload() {								// выгрузить
		this->load_status_ = false;
		this->mark_segment_->Empty_RailCars_.push_back(this);
	 
	}

	const route* RailCar::get_route() const
	{
		return this->route_;
	}

	const Request* RailCar::get_request() const
	{
		return this->to_request_;
	}

	float RailCar::empty_run() const
	{
		return this->empty_run_;
	}

	float RailCar::loaded_run() const
	{
		return this->loaded_run_;
	}

	void RailCar::move_to(const station* st) {
		if (const station* pos = dynamic_cast<const station*>(this->dislocation()))
		{
			const station* disl = (const station*)this->dislocation();
			this->destination_ = st;
			this->in_move_ = true;
			this->route_ = this->operated_net_->get_route(disl, st);
		}
	}

	void RailCar::go_ahead()
	{
		if (!this->is_in_move()) return;
		if (const station* st = dynamic_cast<const station*> (this->dislocation())) {
			this->control_arrive();
			if (!this->is_in_move()) return;
			auto ptr = this->route_->find(this->dislocation());
			++ptr;
			this->dislocation_ = *ptr;
			++this->time_on_line_;
		}
		else {
			const line* this_line = static_cast<const line*> (this->dislocation());
			if (this_line->runtime() <= this->time_on_line_) {
				auto ptr = this->route_->find(this->dislocation());
				++ptr;
				this->dislocation_ = *ptr;
				this->control_arrive();
				this->time_on_line_=0;
			}
			else ++this->time_on_line_;
		}
	}

	bool RailCar::is_in_move() const
	{
		return this->in_move_;
	}

	void RailCar::dispatch_loaded() {		// погрузить и отправить по назначению завки
		if (!load_status_) {
			load();
			move_to(this->to_request_->destination());
			this->to_request_->loaded();
		}
	}

	void RailCar::link(Request* req) {					// привязать порожний к заявке
		if (!load_status_) {
			this->to_request_ = req;
			move_to(req->origin());
			req->link(this);
		}
	}

	bool RailCar::is_linked() const
	{
		return this->to_request_;
	}

	map<string, string> RailCar::status() const
	{
		map<string, string> status;
		status["Вагон №"] = to_string(this->get_number());
		status["Дислокация"] = this->dislocation()->print();
		status["Груженый"] = this->load_status_ ? "Да" : "Нет";
		status["Под заявку №"] = this->to_request_ ? to_string(this->get_request()->get_number()) : " - ";
		status["Станция назначения"] = this->get_destination()->print();

		return status;
	}


	void RailCar::control_arrive() {						// контроль прибытия на станцию назначения
		if (this->dislocation() == this->destination_) this->at_point();
	}

	void RailCar::release() {								// отвязать от заявки
		if (!load_status_&& this->to_request_ != nullptr) {
			if (!this->to_request_->is_actual()) {
				this->to_request_ = nullptr;
			}
			else {
				this->to_request_->release();
				this->to_request_ = nullptr;
			}
		}
	}

	void RailCar::stop() {
		if (this->to_request_ == nullptr) {
			this->in_move_ = false;
			this->destination_ = this->dislocation();
		}
	}

	void RailCar::at_point() {
		this->time_on_line_ = 0;
		if (load_status_) {
			this->loaded_run_ += this->get_route()->lenght();
			this->unload();
			this->to_request_->fulfill();
			this->release();
			this->stop();
			this->mark_segment_->Available_RailCars_.push_back(this);
		}
		else {				
			this->empty_run_ += this->get_route()->lenght();
			if (this->to_request_ != nullptr) {
				this->dispatch_loaded();
			}
			else {
				this->stop();
				this->mark_segment_->Available_RailCars_.push_back(this);
			}
		}
	}


