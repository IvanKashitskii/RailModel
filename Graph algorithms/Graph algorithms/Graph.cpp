#include "Graph.h"
// ���� �������� 3 �����������: �������� ������ Nodes, �������� ����� Edges, �������� ��������� incid_matrix
// ����� edge �������� 3 �����������: ��������� �� ������� node 1, ��������� �� ������� node 2, ��� ����� - float, ��� = 1 �� ���������
// ������� node: int ����� ������� + ���������� visited (=0 �� ���������)
// ����������� �� ���������
	Graph::Graph(): Nodes(), Edges() {
	this->incid_matrix = nullptr;
}
// ����������� ����� �� ���� ����������: n - ����� ������, m - ����� �����. �������� �������� ������ ������, ����� �������� ���� �����, ��� ����� � ����� 1
	Graph::Graph(int n, int m) {
		this->incid_matrix=nullptr;
		if (n == 0) {}
		else {
			vector <node> N;
			for (int i = 0; i <n; i++) {
				node O(i);
				N.push_back(O);
			}
			this->Nodes = N;
			vector <edge> M = {};
			int a = 0;
			int b = 0;
			for (int i = 1; i <= m; i++) {
				cin >> a >> b;
				node* pA = &Nodes[a-1];
				node* pB = &Nodes[b-1];
				edge K(pA, pB);
				M.push_back(K);
			}
			this->Edges = M;
			this->create_incid_matrix();
		}
	}
// ����������� ������ ��������� ������ � �����
	Graph::Graph(const vector<node>& Nodes, const vector <edge>& Edges)
	{
		this->Nodes = Nodes;
		for (auto now: Edges) {
			node* first=find(this->Nodes, (*now.get_node1()).get_num());
			node* second = find(this->Nodes, (*now.get_node2()).get_num());
			edge temp(first, second, now.get_weight());
			this->Edges.push_back(temp);
		}
		this->create_incid_matrix();
	}
// �������� ������������
	Graph Graph::operator=(const Graph& G) {
		this->incid_matrix = nullptr;
		this->Nodes = G.Nodes;
		this->Edges.clear();
		for (auto now : G.Edges) {
			node* first = find(this->Nodes, (*now.get_node1()).get_num());
			node* second = find(this->Nodes, (*now.get_node2()).get_num());
			edge temp(first, second, now.get_weight());
			this->Edges.push_back(temp);
		}
		this->create_incid_matrix();
		return *this;
	}
// ����������� �����������
	Graph::Graph(Graph const& G)
	{
		this->Nodes = G.Nodes;
		for (auto now : G.Edges) {
			node* first = find(this->Nodes, (*now.get_node1()).get_num());
			node* second = find(this->Nodes, (*now.get_node2()).get_num());
			edge temp(first, second, now.get_weight());
			this->Edges.push_back(temp);
		}
		this->create_incid_matrix();
	}
// ����������
	Graph:: ~Graph() {
		if (incid_matrix != nullptr) {
			delete[] incid_matrix[0];
			delete[] incid_matrix;
		}
		else delete[] incid_matrix;
	}
// ����� �������� ������� ��������� ����� ������� ����� �����
	void Graph::create_incid_matrix() {
		size_t n = Nodes.size();
		if (n == 0) {
			this->incid_matrix = nullptr;
			return;
		}
		int** imatr = new int* [n];
		imatr[0] = new int[n * n];
		for (size_t i = 1; i != n;++i) imatr[i] = imatr[i - 1] + n;
		for (unsigned i = 0; i < n;i++) {
			for (unsigned j = 0; j < n; j++) {
				int k = count(Edges, &(this->Nodes[i]), &(this->Nodes[j])) + count(Edges, &(this->Nodes[j]), &(this->Nodes[i]));
				imatr[i][j] = k;
			}
		}
		this->incid_matrix = imatr;
	}
// ������ � ������� ������
	const vector <node>& Graph::nodes() const {
		return this->Nodes;
	}
// ������ � ������� �����
	const vector <edge>& Graph::edges() const {
		return this->Edges;
	}
// ����� ����� - ���������� ������ �� ������� � �������� ������� num
	node const* Graph::find_node(int num) const  {
		for (unsigned i = 0; i < this->Nodes.size();i++) {
			if (this->Nodes[i].get_num() == num) return &(this->Nodes[i]);
		}
		return nullptr;
	}
// ����� ����� - ����� �� ������� ������� ���������
	void Graph::print() const {
		int** K = this->incid_matrix;
		cout << "  ";
		for (unsigned i = 0; i < Nodes.size(); i++) cout << Nodes[i].get_num() << " ";
		cout << endl;
		for (unsigned i = 0; i < Nodes.size(); i++) {
			cout << this->Nodes[i].get_num() << " ";
			for (unsigned j = 0; j < this->Nodes.size(); j++) {
				cout << K[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
// ����� ����� - ����� � ���� ������� ���������
	void Graph::print_to_fail(ofstream& fout) const {
		int** K = this->incid_matrix;
		fout << "\t";
		for (unsigned i = 0; i < Nodes.size(); i++) fout << Nodes[i].get_num() << "\t";
		fout << endl;
		for (unsigned i = 0; i < Nodes.size(); i++) {
			fout << this->Nodes[i].get_num() << "\t";
			for (unsigned j = 0; j < this->Nodes.size(); j++) {
				fout << K[i][j] << "\t";
			}
			fout << endl;
		}
		fout << endl;
	}
// ����� ����� - ������� ��� ������� "�������������"
	void Graph::refresh() const {
		for (unsigned i = 0; i < Nodes.size(); i++) {
			this->Nodes[i].refresh();
		}
	}
// ����� ����������� ��������� �����
	size_t Graph::con_comp() const {
		size_t comp;
		if (Nodes.size() != 0) comp = 0;
		else return 0;
		for (unsigned now_N = 0; now_N < Nodes.size(); now_N++) {
			if (Nodes[now_N].is_visited() == 0) {
				Graph A = this->tree_from_breadth(&Nodes[now_N]);
				comp++;
			}
		}
		return comp;
	}
// ����� ����� - ������� ������ (�����) �����, ����������� ������ a 
	edge* Graph::find_first(node const* a) const {
		for (auto now : this->Edges) {
			if (now.get_node1() == a || now.get_node2() == a) return &now;
		}
		return nullptr;
	}
// ����� ����� - ���������� ������ ���� �����, ����������� ������� �	
	vector <edge> Graph::find_all(const node* a) const {
		vector <edge> E1;
		for (unsigned i = 0; i < this->Edges.size(); i++) {
			if (Edges[i].get_node1() == a || Edges[i].get_node2() == a) {
				E1.push_back(Edges[i]);
			}
		}
		return E1;
	}												
// ����� ����� - ���������� ������ ���� �����, ����������� ������� �, ������� � ������������ �������
	vector <edge> Graph::find_all_to_visit(node const* a) const {
		vector <edge> E1 = {};
		for (unsigned i = 0; i < Edges.size(); i++) {
			if (((Edges[i].get_node1() == a) & (Edges[i].get_node2()->is_visited() == 0)) || 
				((Edges[i].get_node2() == a) &(Edges[i].get_node1()->is_visited() == 0))) {
				E1.push_back(Edges[i]);
			}
		}
		return E1;
	}
// ���������� ������ ���� �����, ����������� ������� �, ������� � ������������ �������, � ������������ ������� �����
	vector <edge> all_to_visit(node const* a, vector <edge> const& E) {
		vector <edge> E1 = {};
		for (unsigned i = 0; i < E.size(); i++) {
			if (((E[i].get_node1() == a) & (E[i].get_node2()->is_visited() == 0)) ||
				((E[i].get_node2() == a) & (E[i].get_node1()->is_visited() == 0))) {
				E1.push_back(E[i]);
			}
		}
		return E1;
	}
// ���������� ������ ���������� �� ������������ �������, ������� � �
	vector <node*> Graph::adj_nodes_to_visit(node const* a) const {
		vector <edge> incid = this->find_all_to_visit(a);
		vector <node*> adj = {};
		for (unsigned i = 0; i < incid.size(); i++) {
			if (incid[i].get_node1() != a) adj.push_back(incid[i].get_node1());
			else adj.push_back(incid[i].get_node2());
		}
		return adj;
	}
// ���������� ���� - ������ ������ � ������ �� ������� �
	Graph Graph::tree_from_breadth(node const* from) const {
		vector <node> tree_nodes;
		vector <edge> tree_edges;
		Graph tree(0, 0);
		(*from).visit();
		tree.Nodes.push_back(*from);
		tree.incid_matrix = nullptr;
		vector <edge> E_N1 = find_all_to_visit(from);
		stack <edge> E_visited = {};
		if (E_N1.size() != 0) {						// ����� � ������ - ���� ����� ����� ��������� ������� ��������� ����� �����, ������ �� ����� ������� ����� ������
			edge temp_E = E_N1[0];
			tree.Edges.push_back(temp_E);
			E_visited.push(temp_E);					// �������� ���������� ����� ��� ������ ����� ���������
			node* temp_N;
			if (temp_E.get_node1() == from) temp_N = temp_E.get_node2();
			else temp_N = temp_E.get_node1();
			Graph temp_tree = tree_from_breadth(temp_N);
			tree = tree + temp_tree;
		}
		while (!E_visited.empty()) {				// ����� � ������ - �������� ���������� ����� � �������� �������, �������� ������ ������� �� ����� ���������� ������
			edge temp_E = E_visited.top();
			node* temp_N;
			if (temp_E.get_node1() == from) temp_N = temp_E.get_node1();
			else temp_N = temp_E.get_node2();
			E_visited.pop();
			Graph temp_tree = tree_from_breadth(temp_N);
			tree = tree + temp_tree;
		}
		return tree;
	}
// ���������� ���� - ������ ������ � ������� �� ������� �
	Graph Graph::tree_from_depth(node const* from) const {
		Graph tree(0, 0);
		queue <node const*> N_visited = {};
		N_visited.push(from);
		bool go_on = true;
		while (go_on) {
			N_visited.pop();
			(*from).visit();
			tree.Nodes.push_back(*from);
			vector <edge> E_N1 = this->find_all_to_visit(from);
			if (E_N1.size() != 0) {						// ����� � ������� - ��������� ��� ����������� �����, ������� ������� ���������� � �������
				for (unsigned i = 0; i < E_N1.size(); i++) {
					edge temp_E = E_N1[i];
					tree.Edges.push_back(temp_E);
					node* temp_N;
					if (temp_E.get_node1() == from) temp_N = temp_E.get_node2();
					else temp_N = temp_E.get_node1();
					N_visited.push(temp_N);
					(*temp_N).visit();
				}
			}
			if (!N_visited.empty()) from = N_visited.front();	// ����� � ������� - ������������ ������� ����� ���������� ������, �� ������ ������� ����������� ��������
			else go_on = false;
			tree.incid_matrix = nullptr;
		}
		return tree;
	}
// ����������� ����� ����, ������ �������� ��������
	Graph operator+(const Graph& Add1, const Graph& Add2) {
		Graph A (0,0);
		for (unsigned i = 0; i < Add1.Nodes.size(); i++) {
			A.Nodes.push_back(Add1.Nodes[i]);
		}
		for (unsigned i = 0; i < Add2.Nodes.size();i++) {
			const node& temp_N = Add2.Nodes[i];
			bool avail = false;
			for (unsigned j = 0; j < Add1.Nodes.size(); j++) {
				if (temp_N == Add1.Nodes[j]) {
					avail=true;
					break;
				}
			}
			if (!avail) A.Nodes.push_back(temp_N);
		}
		for (unsigned i = 0; i < Add1.Edges.size(); i++) {
			A.Edges.push_back(Add1.Edges[i]);
		}
		for (unsigned i = 0; i < Add2.Edges.size();i++) {
			
			edge temp_E = Add2.Edges[i];
			bool avail = false;
			for (unsigned j = 0; j < Add1.Edges.size();j++) {
				if (temp_E == Add1.Edges[j]) {
					avail = true;
					break;
				}
			}
			if (!avail) A.Edges.push_back(temp_E);
		}
		A.incid_matrix=nullptr;
		return A;
	}
// ����� ����� - ���������� ������ ���������� �� ���� ������ ����� �� �������� �������
	vector <int> Graph::distance_from(node const* from) const {
		Graph A = this->tree_from_depth(from);
		this->refresh();
		vector <int> dist (A.Nodes.size());
		int counter = 0;
		queue <node const*> q = {};
		node const* j=nullptr;
		int k = 0;
		dist[(*from).get_num()] = 0;
		q.push(from);
		while (!q.empty()) {
			j = q.front();
			(*j).visit();
			counter = dist[(*j).get_num()];
			q.pop();
			vector <node*> adj_nodes = adj_nodes_to_visit(j);
			if (adj_nodes.size() != 0) {
				counter++;
				for (unsigned i = 0; i < adj_nodes.size(); i++) {
					q.push(adj_nodes[i]);
					k = (*adj_nodes[i]).get_num();
					dist[k] = counter;
				}
			}
		}
		return dist;
	}
	// ����� - ���������� ������� ������� �
	size_t Graph::node_degree(node* A) const {
		vector <edge> E = this->find_all(A);
		return E.size();
	}
// ����� - ���������� ������ �������� ���� ������ �����
	vector <int> Graph::degrees() const {
		vector <int> D(this->Nodes.size());
		for (unsigned i = 0; i < this->Edges.size(); i++) {
			edge temp_E = this->Edges[i];
			D[temp_E.get_node1()->get_num()]++;
			D[temp_E.get_node2()->get_num()]++;
		}
		return D;
	}
// ����� ����� ������
vector <node*> Graph::Euler_cycle() {
		vector <node*> EC = {};
		if ((this->Edges.size()==0)||(this->con_comp() != 1)) return EC;
		this->refresh();
		vector <int> ND=this->degrees();
		for (unsigned i = 0; i < ND.size(); i++) {
			if (ND[i] % 2 != 0) return EC;
		}
		node* from = &(this->Nodes[0]);
		EC.push_back(from);
		vector <edge> edges_to_visit = this->Edges;
		vector <edge> E_N1 = all_to_visit(from, edges_to_visit);
		while (true) {
			vector <node*> EC_temp = {};
			node* first = from;
			(*from).visit();
			E_N1 = all_to_visit(from, edges_to_visit);
			while (E_N1.size() != 0) {						// ����� � ������� - ���� �� �������� ������ ����
				int k = (*from).get_num();
				ND[k]--;
				edge temp_E = E_N1[0];
				int p = find_pos(edges_to_visit, temp_E.get_node1(), temp_E.get_node2());
				edges_to_visit.erase(edges_to_visit.begin() + p);
				node* temp_N;
				if (temp_E.get_node1() == from) temp_N = temp_E.get_node2();
				else temp_N = temp_E.get_node1();
				from = temp_N;
				(*from).visit();
				EC_temp.push_back(from);
				E_N1 = all_to_visit(from, edges_to_visit);
				k = (*temp_N).get_num();
				ND[k]--;
			}
			EC_temp.push_back(first);
			int p = find_pos(edges_to_visit, from, first);
			edges_to_visit.erase(edges_to_visit.begin() + p);
			int k = (*first).get_num();
			ND[k]--;
			k = (*from).get_num();
			ND[k]--;
			int x = *max_element(ND.begin(), ND.end());
			EC.insert(EC.begin()+find_pos(EC,(*first).get_num())+1, EC_temp.begin(), EC_temp.end());
			if (x != 0) {
				for (unsigned i = 0; i < this->Nodes.size(); i++) {
					if (ND[i] != 0) {
						from = &(this->Nodes[i]);
						break;
					}
				}
			}
			else return EC;
			this->refresh();
		}
	}
// ����� - �������� �� ������� �������� ������
	bool Graph::is_empty() const {
		vector <node> Check= {};
		if (Nodes == Check) return true;
		else return false;
	}

// ���������� ��������. * ������ ����, 2008.
const int inf = numeric_limits<int>::max();

/*
	 * ������ ������ � ����������� ���������� �������.
	 * matrix: ������������� ������� �� ����� ����� (�� ����������� �������������).
	 *         ������ ������� ������ ���� �� ������ ������.
	 * ����������: ������ ��������� ���������, �� ������ �� ������ ������ �������.
	 */
VPInt hungarian(const VVInt& matrix) {

		// ������� �������
		int height = matrix.size(), width = matrix[0].size();

		// ��������, ���������� �� ����� (u) � �������� (v)
		VInt u(height, 0), v(width, 0);

		// ������ ���������� ������ � ������ �������
		VInt markIndices(width, -1);

		// ����� ��������� ������ ������� ���� �� ������
		for (int i = 0; i < height; i++) {
			VInt links(width, -1);
			VInt mins(width, inf);
			VInt visited(width, 0);

			// ���������� �������� (�������� "������������ �������" �� ������� ���������)
			int markedI = i, markedJ = -1, j;
			while (markedI != -1) {
				// ������� ���������� � ��������� � ���������� ������� ������������ ��������
				// ������ �������� � j ������ ������������� ������� � ����� ��������� �� ���
				j = -1;
				for (int j1 = 0; j1 < width; j1++)
					if (!visited[j1]) {
						if (matrix[markedI][j1] - u[markedI] - v[j1] < mins[j1]) {
							mins[j1] = matrix[markedI][j1] - u[markedI] - v[j1];
							links[j1] = markedJ;
						}
						if (j == -1 || mins[j1] < mins[j])
							j = j1;
					}

				// ������ ��� ���������� ������� � ��������� (markIndices[links[j]], j)
				// ���������� ����������� �� �������� � ��������� ���, ����� �� ���������
				int delta = mins[j];
				for (int j1 = 0; j1 < width; j1++)
					if (visited[j1]) {
						u[markIndices[j1]] += delta;
						v[j1] -= delta;
					}
					else {
						mins[j1] -= delta;
					}
				u[i] += delta;

				// ���� �������� �� ��������� - �������� � ��������� ��������
				visited[j] = 1;
				markedJ = j;
				markedI = markIndices[j];
			}

			// ������� �� ��������� ������������ ������� ������, ������ ������� �
			// ���������� ������ � �������� ������� �� ������������
			for (; links[j] != -1; j = links[j])
				markIndices[j] = markIndices[links[j]];
			markIndices[j] = i;
		}

		// ������ ��������� � ������������ �����
		VPInt result;
		for (int j = 0; j < width; j++)
			if (markIndices[j] != -1)
				result.push_back(PInt(markIndices[j], j));
		return result;
	}
