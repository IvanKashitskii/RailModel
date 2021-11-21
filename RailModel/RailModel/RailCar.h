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
	RailCar(MarketSegment* const mark_segment, const station* st);		// �������� �� ������ ��� ���������� �������� ��������� �����??

	const route* get_route() const;				// ������ � ��������
	const Request* get_request() const;			// ������ � ������
	float empty_run() const;					// ������ � ��������� �������
	float loaded_run() const;					// ������ � ��������� �������
	void move_to(const station*);				// ��������� � �������� � �������
	
	bool is_in_move() const;					// � ��������?
	void load();								// ���������
	void unload();								// ���������
	void dispatch_loaded();						// ��������� ��������
	void link(Request* req);					// ��������� � ������
	bool is_linked() const;						// �������� �� � ������
	map <string, string> status() const;		// ������ ������

	friend struct Request;
	friend struct MarketSegment;
		
private:
	void control_arrive();						// �������� ��������
	void stop();
	void at_point();
	void release();	
	RailCar(RailCar const& RC);					// ������ ����������� ������
	RailCar& operator=(RailCar const& RC);		// ������ ������������ ������
	void go_ahead();							// ����������� �� �������� (�� 1 �����)

	bool load_status_;							// ���������: 1 - ��������, 0 - ��������
	Request* to_request_;						// ��������� �� ����������� ������
	const route* route_;						// ��������� �� ������� �������� ������
	unsigned time_on_line_;						// ����� ���������� �� �������/�������� - ������������� �������
	MarketSegment* const mark_segment_;			// ������ ����� �����������
	float empty_run_;						// �������� ������
	float loaded_run_;						// �������� ������
};

//#endif // !RAILCAR_H

