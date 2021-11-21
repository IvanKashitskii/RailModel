#include "node.h"
	
node::node(int a) {
		this->node_num = a;
		this->visited = 0;
	}
// конструктор от номера вершины
node node:: operator=(const node& a)
	{
		this->node_num = a.node_num;
		this->visited = a.visited;
		return *this;
	}
// оператор присваивания
int node::get_num() const {
	return this->node_num;
}
// получение номера вершины
bool node::is_visited() const {
	return this->visited;
}
// посещена ли вершина?
void node::visit() const {
	this->visited = 1;
}
// сделать вершину посещенной
void node::refresh() const {
	this->visited = 0;
}
// сделать вершину непосещенной
bool operator<(const node& a, const node& b)
	{
		return a.node_num < b.node_num;
	}
bool operator>(const node& a, const node& b)
{
	return a.node_num > b.node_num;
}
bool operator==(const node& a, const node& b)
{
	return a.node_num == b.node_num;
}
bool operator!=(const node& a, const node& b)
{
	return !(a.node_num == b.node_num);
}
// операторы сравнения
node* find(vector <node>& N, int a) {
	for (unsigned i = 0; i < N.size(); ++i) {
		if (N[i].get_num() == a) return &(N[i]);
	}
	return nullptr;
}
node* find(vector <node*>& N, int a) {
	for (unsigned i = 0; i < N.size(); i++) {
		if ((*N[i]).get_num() == a) return N[i];
	}
	return nullptr;
}
// поиск вершины в векторе вершин - возвращает указатель
int find_pos(vector <node*>& N, int a) {
	for (unsigned i = 0; i < N.size(); i++) {
		if ((*N[i]).get_num() == a) return i;
	}
	return N.size();
}
int find_pos(const vector <node*>& N, node const* a) {
	for (unsigned i = 0; i < N.size(); i++) {
		if (N[i]== a) return i;
	}
	return N.size();
}
int find_pos(const vector <node>& N, node const* a) {
	for (unsigned i = 0; i < N.size(); i++) {
		if (N[i] == *a) return i;
	}
	return N.size();
}
// поиск вершины в векторе вершин - возвращает позицию вершины в векторе