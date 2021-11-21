#include "vehicle.h"

int fleet = 0;									// парк вагонов (счетчик)
void FleetUp()									// увеличение парка вагонов
{
	fleet++;
}

vehicle::vehicle(const Network* const net, const station* st):
	operated_net_(net), dislocation_(st), destination_(st)
{
	FleetUp();
	this->vehicle_num_ = 50000000+fleet;
	in_move_ = false;
}

const RailObject* vehicle::dislocation() const {
	return this->dislocation_;
}

unsigned vehicle::get_number() const
{
	return this->vehicle_num_;
}

const RailObject* vehicle::get_destination() const
{
	return this->destination_;
}