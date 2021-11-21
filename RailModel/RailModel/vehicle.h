#pragma once
#include "..\..\RailNetwork2\RailNetwork2\Network.h"

struct vehicle
{
	vehicle(const Network* const net, const station* st);
	virtual void move_to(const station*)=0;
	virtual void go_ahead()=0;
	const RailObject* dislocation() const;
	unsigned get_number() const;
	const RailObject* get_destination() const;

private:
	vehicle(vehicle const& RC);					// запрет копировани€
	vehicle& operator=(vehicle const& car);		// запрет присваивани€

protected:

	unsigned vehicle_num_;						//номер единицы ѕ— 
	const RailObject* dislocation_;				// указатель на станцию дислокации, 0 - никака€ станци€
	bool in_move_;								// в движении?
	const RailObject* destination_;				// указатель на станцию назначени€
	const Network* operated_net_;				// сеть на которой обращаетс€ ѕ—	
};

