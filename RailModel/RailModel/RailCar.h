//#ifndef RAILCAR_H
//#define RAILCAR_H

#pragma once
#include <map>
#include "vehicle.h"
#include "Request.h"
#include "MarketSegment.h"

struct Request;
struct MarketSegment;

struct RailCar: vehicle
{
	RailCar(MarketSegment* const mark_segment, const station* st);		// защитить от вызова вне начального создания вагонного парка??

	const route* get_route() const;				// доступ к маршруту
	const Request* get_request() const;			// доступ к заявке
	float empty_run() const;					// доступ к порожнему пробегу
	float loaded_run() const;					// доступ к груженому пробегу
	void move_to(const station*);				// отправить в движение к станции
	
	bool is_in_move() const;					// в движении?
	void load();								// погрузить
	void unload();								// выгрузить
	void dispatch_loaded();						// отправить груженый
	void link(Request* req);					// привязать к заявке
	bool is_linked() const;						// привязан ли к заявке
	map <string, string> status() const;		// статус вагона

	friend struct Request;
	friend struct MarketSegment;
		
private:
	void control_arrive();						// контроль прибытия
	void stop();
	void at_point();
	void release();	
	RailCar(RailCar const& RC);					// запрет копирования вагона
	RailCar& operator=(RailCar const& RC);		// запрет присваивания вагона
	void go_ahead();							// продвижение по маршруту (за 1 сутки)

	bool load_status_;							// состояние: 1 - груженый, 0 - порожний
	Request* to_request_;						// указатель на исполняемую заявку
	const route* route_;						// указатель на маршрут движения вагона
	unsigned time_on_line_;						// время нахождения на участке/перегоне - накопительный счетчик
	MarketSegment* const mark_segment_;			// какому парку принадлежит
	float empty_run_;						// порожний пробег
	float loaded_run_;						// груженый пробег
};

//#endif // !RAILCAR_H

