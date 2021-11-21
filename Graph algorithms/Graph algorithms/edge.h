#pragma once
#include "node.h"

struct edge {
	edge(node* const a, node* const b, float w=1);
	edge(int a, int b, float w=1);
	node* get_node1() const;
	node* get_node2() const;
	float get_weight() const;
	friend bool operator==(const edge& a, const edge& b);
	friend bool operator<(const edge& a, const edge& b);
	friend bool operator>(const edge& a, const edge& b);
	edge operator=(edge a);
	friend int count(const vector<edge> A, const node* a, const node* b);
	friend edge* find(vector <edge>& A, node const* a, node const* b);
	friend int find_pos(const vector <edge>& A, node const* a, node const* b);
private:
	node* node1;
	node* node2;
	float weight;
};
