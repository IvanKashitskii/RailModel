#pragma once
#include "Request.h"

unsigned req_counter = 0;

	Request::Request(const station* const ldst, const station* const dest, unsigned rate) : 
	dayly_rate_(rate) 	{
		req_num = req_counter + 1;
		++req_counter;
		loadstation_ = ldst;
		destination_ = dest;
		fulfilled_ = false;
		actuality_ = true;
		linked_ = false;
		loaded_ = false;
		car_to_request_ = nullptr;
	}

	Request::Request(Request const& req) : price_(req.price()) {							// копирование существующей заявки
		Request(req.origin(), req.destination(), req.daily_rate());
	}
	const station* Request::origin() const {
		return this->loadstation_;
	}

	const station* Request::destination() const {
		return this->destination_;
	}

	RailCar* Request::linked_car() const
	{
		return this->car_to_request_;
	}

	unsigned Request::price() const
	{
		return this->price_;
	}

	void Request::set_price(unsigned price)
	{
		this->price_ = price;
	}

	unsigned Request::daily_rate() const
	{
		return this->dayly_rate_;
	}

	unsigned Request::get_number() const {
		return this->req_num;
	}

	void Request::fulfill() {
		this->fulfilled_ = true;
	}

	void Request::loaded() {
		this->loaded_ = true;
		this->actuality_ = false;

	}

	bool Request::is_actual() const {
		return this->actuality_;
	}

	bool Request::is_loaded() const
	{
		return this->loaded_;
	}

	bool Request::is_fulfilled() const
	{
		return this->fulfilled_;
	}

	void Request::refuse() {
		this->actuality_ = false;
		if (this->car_to_request_ != nullptr) {
			this->car_to_request_->release();
		}
	}

	void Request::link(RailCar* car) {
		this->linked_ = true;
		this->car_to_request_ = car;
	}

	void Request::release() {
		this->linked_ = false;
	}