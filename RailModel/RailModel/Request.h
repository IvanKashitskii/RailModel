#pragma once
#include "..\..\RailNetwork2\RailNetwork2\Network.h"
#include "RailCar.h"

struct RailCar;
struct Request
{
	Request(const station* const ldst, const station* const dest, unsigned rate=0);	// создание заявки
	Request(Request const& req);			// копирование существующей заявки (создаст новую с теми же параметрами!)
	unsigned get_number() const;		// доступ к номеру заявки	
	const station* origin() const;		// доступ к станции погрузки
	const station* destination() const; // доступ к станции назначения
	RailCar* linked_car() const;	// доступ к привязанному вагону
	unsigned price() const;			// доступ к стоимости
	void set_price(unsigned price); // установка цены заявки
	unsigned daily_rate() const;		// доступ к суточной ставке
	bool is_actual() const;			// заявка акруальна?
	bool is_loaded() const;			// заявка погружена?
	bool is_fulfilled() const;		// заявка исполнена?
	void refuse();					// отказ от заявки
	void link(RailCar* car);	// привязка под заявку вагона
	
	friend struct RailCar;
private:
	void fulfill();		// исполнение заявки (вагон прибыл на станцию выгрузки)
	void loaded();		// вагон по заявке погружен
	void release();		// отвязка вагона от заявки (без исполнения, заявка остается актуальной!)
	Request& operator=(Request const req);					// запрет присваивания
	unsigned req_num;						// номер заявки
	const station* loadstation_;						// станция погрузки
	const station* destination_;						// станция назначения
	unsigned price_;							// стоимость заявки
	unsigned dayly_rate_;					// суточная ставка			
	bool fulfilled_;							// удовлетворение заявки
	bool actuality_;							// актуальность заявки
	bool linked_;							// привязан ли вагон
	bool loaded_;							// погрузка по заявке исполнена?
	RailCar* car_to_request_;				// указатель на привязанный вагон
};

