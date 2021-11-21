#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

	struct node {

		node(int a);
		node operator=(const node& a);
		int get_num() const;
		bool is_visited() const;
		void visit() const;
		void refresh() const;
		friend bool operator<(const node& a, const node& b);
		friend bool operator>(const node& a, const node& b);
		friend bool operator==(const node& a, const node& b);
		friend bool operator!=(const node& a, const node& b);


	private:
		int node_num;
		mutable bool visited;
	};

	node* find(vector <node>& N, const int a);
	node* find(const vector <node*>& N, const int a);
	int find_pos(vector <node*>& N, const int a);
	int find_pos(const vector <node*>& N, node const* a);
	int find_pos(const vector <node>& N, node const* a);

