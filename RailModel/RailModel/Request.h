#pragma once
#include "..\..\RailNetwork2\RailNetwork2\Network.h"
#include "RailCar.h"

struct RailCar;
struct Request
{
	Request(const station* const ldst, const station* const dest, unsigned rate=0);	// �������� ������
	Request(Request const& req);			// ����������� ������������ ������ (������� ����� � ���� �� �����������!)
	unsigned get_number() const;		// ������ � ������ ������	
	const station* origin() const;		// ������ � ������� ��������
	const station* destination() const; // ������ � ������� ����������
	RailCar* linked_car() const;	// ������ � ������������ ������
	unsigned price() const;			// ������ � ���������
	void set_price(unsigned price); // ��������� ���� ������
	unsigned daily_rate() const;		// ������ � �������� ������
	bool is_actual() const;			// ������ ���������?
	bool is_loaded() const;			// ������ ���������?
	bool is_fulfilled() const;		// ������ ���������?
	void refuse();					// ����� �� ������
	void link(RailCar* car);	// �������� ��� ������ ������
	
	friend struct RailCar;
private:
	void fulfill();		// ���������� ������ (����� ������ �� ������� ��������)
	void loaded();		// ����� �� ������ ��������
	void release();		// ������� ������ �� ������ (��� ����������, ������ �������� ����������!)
	Request& operator=(Request const req);					// ������ ������������
	unsigned req_num;						// ����� ������
	const station* loadstation_;						// ������� ��������
	const station* destination_;						// ������� ����������
	unsigned price_;							// ��������� ������
	unsigned dayly_rate_;					// �������� ������			
	bool fulfilled_;							// �������������� ������
	bool actuality_;							// ������������ ������
	bool linked_;							// �������� �� �����
	bool loaded_;							// �������� �� ������ ���������?
	RailCar* car_to_request_;				// ��������� �� ����������� �����
};

