#pragma once
#include <stack>
#include <queue>
#include <algorithm>
#include <fstream>
#include "edge.h"
using namespace std;

struct Graph {

	Graph();
	Graph(int n, int m);
	Graph(const vector<node>& Nodes, const vector <edge>& Edges);
	~Graph();
	const vector <node>& nodes() const;
	const vector <edge>& edges() const;
	node const* find_node(int num) const;
	void print() const;
	void print_to_fail(ofstream& fout) const;
	void refresh() const;
	size_t con_comp() const;
	size_t node_degree(node* A) const;
	vector <int> degrees() const;
	friend Graph operator+(const Graph& Add1, const Graph& Add2);
	Graph operator=(const Graph& G);
	Graph(Graph const&);
	Graph tree_from_breadth(node const* from) const;
	Graph tree_from_depth(node const* from) const;
	vector <int> distance_from(node const* from) const;
	edge* find_first (node const* a) const;
	vector <edge> find_all(node const* a) const;
	vector <edge> find_all_to_visit(node const* a) const;
	vector <node*> adj_nodes_to_visit(node const* a) const;
	vector <node*> Euler_cycle();
	bool is_empty() const;
private:
	void create_incid_matrix();
	vector <node> Nodes;
	vector <edge> Edges;
	int** incid_matrix;
};

// Венгерский алгоритм. * Даниил Швед, 2008.
#include <limits>
typedef pair<int, int> PInt;
typedef vector<int> VInt;
typedef vector<VInt> VVInt;
typedef vector<PInt> VPInt;
VPInt hungarian(const VVInt& matrix);