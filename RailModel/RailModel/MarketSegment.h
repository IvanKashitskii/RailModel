#ifndef MARKETSEGMENT_H
#define MARKETSEGMENT_H

#pragma once
#include "..\..\RailNetwork2\RailNetwork2\Network.h"
#include "Request.h"
#include <random>				// ����������� � �������������� ����������

struct RailCar;
struct Request;

struct MarketSegment
{
	MarketSegment(unsigned fleet, unsigned day_requests, const Network* Net, string optimizer = "random", float req_irr = 1, vector <const station*> LoadStations = {}, bool redistribute = false);
	~MarketSegment();

	const Network* const get_net() const;
	const float requests_irregularity() const;
	const unsigned day_requests() const;
	const vector <RailCar*> Fleet() const;
	const vector <Request*> Requests() const;
	const vector <RailCar*> Available_RailCars() const;
	const vector <Request*> Actual_Requests() const;
	const vector <RailCar*> Empty_RailCars() const;
	string get_optimizer() const;
	const vector <vector<int>> to_simplymatrix(vector <RailCar*>& Distr_RailCars, string optimizer="shortest_empty_runs");		// ������� ������� ��������� ������ � ������ � ������ �� �������� optimizer
	void distribute(string optimizer="random");			// ����������� ������ �� �������� optimizer: "random", "shortest_empty_runs", "shortest_empty_runtime" 
	void new_day();								// ��������� ����� ������ � ������ ���, ������������ �������� �������, �� ����������� �� ���������
	map <string, string> status() const;		// ������
	float empty_run() const;					// �������� ������ �� �������� �����
	float loaded_run() const;				// �������� ������ �� �������� �����
	unsigned loaded_requests() const;			// ����������� ������ �����
	unsigned fulfilled_requests() const;		// ����������� ������ �����

	friend struct RailCar;

private:
	unsigned day_requests_;
	float requests_irregularity_;
	vector <RailCar*> Fleet_;
	vector <Request*> Requests_;
	vector <Request*> Actual_Requests_;
	vector <RailCar*> Available_RailCars_;
	vector <RailCar*> Empty_RailCars_;
	vector <const station*> Load_stations_;
	vector <const station*> Unload_stations_;
	const Network* const operated_net_;
	string optimizer_;
	bool redistribute_;
};

#endif // !MARKETSEGMENT_H