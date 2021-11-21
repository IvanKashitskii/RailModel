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
	vehicle(vehicle const& RC);					// ������ �����������
	vehicle& operator=(vehicle const& car);		// ������ ������������

protected:

	unsigned vehicle_num_;						//����� ������� �� 
	const RailObject* dislocation_;				// ��������� �� ������� ����������, 0 - ������� �������
	bool in_move_;								// � ��������?
	const RailObject* destination_;				// ��������� �� ������� ����������
	const Network* operated_net_;				// ���� �� ������� ���������� ��	
};

