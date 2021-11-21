#include "edge.h"
	edge::edge(node* const a, node* const b, float w) {
		this->node1 = a;
		this->node2 = b;
		this->weight = w;
	}
	// ����������� �� ���������� �� �������, ��� ����� �� ��������� =1
	edge::edge(int a, int b, float w) {
		node A(a);
		node B(b);
		this->node1 = &A;
		this->node2 = &B;
		this->weight = w;
	}
	// ����������� �� ������� ������ (������� ����� �������!), ��� ����� �� ��������� =1
	node* edge::get_node1() const {
		return this->node1;
	}
	node* edge::get_node2() const {
		return this->node2;
	}
	float edge::get_weight() const {
		return this->weight;
	}
	// ������� �����
	edge edge::operator=(edge a) {
		this->node1 = a.node1;
		this->node2 = a.node2;
		this->weight = a.weight;
		return *this;
	}
	// �������� ������������
	bool operator==(const edge& a, const edge& b)
	{
		if (((a.node1 == b.node1) & (a.node2 == b.node2)) || ((a.node2 == b.node1) & (a.node1 == b.node2))) return 1;
		else return 0;
	}
	bool operator<(const edge& a, const edge& b)
	{
		if (a.node1 < b.node1) return 1;
		else if ((a.node1 == b.node1) & (a.node2 < b.node2)) return 1;
		else return 0;
	}
	bool operator>(const edge& a, const edge& b)
	{
		if (a.node1 > b.node1) return 1;
		else if ((a.node1 == b.node1) & (a.node2 > b.node2)) return 1;
		else return 0;
	}
	// ��������� ��������� (���������� ������ ������!)
	int count(const vector<edge> A, const node* a, const node* b) {
		int counter = 0;
		for (auto now : A) {
			if ((now.get_node1() == a) & (now.get_node2() == b)) counter++;
		}
		return counter;
	}
	// ����� ����� � ������, ����������� ���� ������
	edge* find(vector <edge>& A, node const* a, node const* b) {
			for (unsigned i = 0; i < A.size(); i++) {
				if ((A[i].get_node1() == a) & (A[i].get_node2() == b) || (A[i].get_node2() == a) & (A[i].get_node1() == b)) {
					return &A[i];
				}
				else return nullptr;
			}
		}
	// ���� � ������� ����� ����������� �������� �������� (���������� ������ ��������� ��� ������� ���������)

	int find_pos(const vector <edge>& A, node const* a, node const* b) {
		for (unsigned i = 0; i < A.size(); i++) {
			if ((A[i].get_node1() == a) & (A[i].get_node2() == b) || (A[i].get_node2() == a) & (A[i].get_node1() == b)) {
				return i;
			}
			
		}
		return A.size();
	}
	// ���� � ������� ������� ����� ������������ �������� �������� (���������� ������� ������� ��������� ��� ������ �������)


