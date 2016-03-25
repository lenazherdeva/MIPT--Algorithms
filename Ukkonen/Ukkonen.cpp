#include<string>
#include<map>
#include<locale>
#include<algorithm>

#include<iostream>

struct vertex;
struct edge {
	size_t beg;
	size_t lim;
	vertex * end;
	edge() :beg(0), lim(0), end(0){}
};
struct vertex {
	std::map<char, edge> child;
	size_t depth_dontuse;
	vertex * suffix_link;
	size_t id_dontUse;

};


void traverse(size_t d, edge * e) {
	size_t i;
	size_t lim;

	if (e->end) {
		if (e->end->depth_dontuse != d - e->lim - e->beg) {
			_asm int 3;
		}
		for (
			auto ch = e->end->child.begin(); // �������� �� �����
			ch != e->end->child.end();
		ch++) {
			traverse(d + e->lim - e->beg,
				&(ch->second));
		}
	}
	else {
		//�������� ������� ���
		printf("%d\n", e->beg - d);
	}
}

void find_substring(
	std::string & s//�����,��� ����
	, edge *e, //���������� ������ ��� ����
	std::string pattern) { //��� ����(������� ����)
	edge * cur_edge;
	size_t cur_edge_pos;
	size_t i = 0;
	cur_edge = e;
	cur_edge_pos = 0;
	while (i < pattern.length()) {
		if (cur_edge_pos < cur_edge->lim) {
			//����� �� �����
			//�������� �������� ��������� �������
			if (s[cur_edge_pos] == pattern[i]) {
				i++;
				cur_edge_pos++;
			}
			else {
				return;
			}
		}
		else {
			//����� � ����� �������� �����, � � � ��� �������� �������
			auto out = cur_edge->end->child.find(pattern[i]);// ���� ���������� ��������� �����
			if (out == cur_edge->end->child.end()) {
				return;
			}
			cur_edge = &(out->second);
			cur_edge_pos = cur_edge->beg;

		}

	}
	//��������� �������. ��������� ��� �������� �������

	traverse(i - cur_edge_pos + cur_edge->beg, cur_edge);
}




void print_edge(size_t shift, std::string &s, edge * e) {
	size_t i;
	size_t lim;
	//������� ������
	for (int i = 0; i < shift; i++) printf(" ");
	//������� ����� ������
	if (e->end)
		lim = e->lim;
	else
		lim = s.length();

	for (i = e->beg; i < lim; i++) printf("%c", s[i]); //������� ������ �����
	//���� �������� ������� ������� �������� �����
	if (e->end) {
		//printf("\n");
		printf("(depth:%d; id: %d; link: %d)\n", e->end->depth_dontuse, e->end->id_dontUse,
			e->end->suffix_link ? (e->end->suffix_link->id_dontUse) : -1);
		for (
			auto ch = e->end->child.begin(); // �������� �� �����
			ch != e->end->child.end();
		ch++) {
			print_edge(
				shift + lim - e->beg, s, &(ch->second));
		}
	}
	else{
		//�������� ������� ���
		printf("$\n");
	}
}

int main() {
	std::string s;
	//s = "abababaab";
	s = "ojojaojojb";
	//setlocale(LC_ALL, "Russian");

	printf("%s\n", s.c_str());
	vertex root;
	edge edge_to_root; //= { 0, 0, &root };
	size_t cur_suf;
	size_t cur_suf_pos;
	edge * cur_edge;
	size_t cur_edge_pos;
	size_t depth; //������� �������
	vertex * no_link_vertex; //��������� �������� �������, � ������� ��� ��� �������
	size_t no_link_depth; //�� �������
	size_t last_linked_depth;
	vertex * last_linked_vertex;
	edge fictive_edge;



	root.depth_dontuse = 0;
	root.suffix_link = 0;
	cur_suf = cur_suf_pos = 0;
	size_t vertex_id = 0;
	cur_edge = &edge_to_root;
	cur_edge_pos = 0;
	edge_to_root.beg = 0;
	edge_to_root.lim = 0;
	edge_to_root.end = &root;
	depth = 0;
	root.id_dontUse = 0;
	no_link_vertex = 0;
	no_link_depth = -1;
	last_linked_depth = -1;
	last_linked_vertex = 0;

	for (size_t len = 1; len <= s.length(); len++) {
		while (cur_suf_pos < len) {
			char ch = s[cur_suf_pos];
			if (cur_edge_pos < cur_edge->lim) { //�� � ����� � ���� �������
				//����� �� ������� �� ���� ���
				if (no_link_vertex) {
					size_t jump;
					jump = std::min(no_link_depth - 1 - depth,
						cur_edge->lim - cur_edge_pos); //������� ���� ���� ����� //�������������� ���������, ����� ���������
					if (jump) {
						cur_suf_pos += jump;
						cur_edge_pos += jump;
						depth += jump;
						continue;
					}

				}

				if (ch == s[cur_edge_pos]) {
					//��������� ������� ���������-������ ���
					cur_suf_pos++;
					cur_edge_pos++;
					depth++;
					continue;
				}
				//����� �� ����� �� ������� ������� �� ���������
				//������� ����� �������
				vertex *nv = new vertex;
				//������������� ������������� ���������� ����� �� ����� �������
				nv->child[s[cur_edge_pos]].beg = cur_edge_pos;
				nv->child[s[cur_edge_pos]].lim = cur_edge->lim;
				nv->child[s[cur_edge_pos]].end = cur_edge->end;
				nv->suffix_link = 0;
				nv->depth_dontuse = depth; //������ ��� ������
				nv->id_dontUse = ++vertex_id;

				//����������� ������ �����
				cur_edge->lim = cur_edge_pos;
				cur_edge->end = nv;
				if (no_link_vertex) {//�� ���� �� ��� �� �������, � ������� ���� ��������� �������
					if (no_link_depth != depth + 1) {
						_asm int 3;
					}
					no_link_vertex->suffix_link = nv;

				}


				no_link_vertex = nv;
				no_link_depth = depth;
			}
			//����� ����, �� ����� � �������� ������� �����
			if (cur_edge->end->suffix_link) {
				last_linked_vertex = cur_edge->end;
				last_linked_depth = depth;
			}

			if (no_link_vertex && (no_link_depth == depth + 1)) {//�� ���� �� ��� �� �������, � ������� ���� ��������� �������
				no_link_vertex->suffix_link = cur_edge->end;
				no_link_vertex = 0;
				no_link_depth = 0;
			}

			cur_edge = &(cur_edge->end->child[ch]); //��������� ����� �� ����� ��������, ���� ��� ���-��� ���������//����� �� ������� �� ch
			if (cur_edge->lim) {
				//������ �� ������������ �����
				cur_edge_pos = cur_edge->beg;
				continue;
			}
			else {
				//������� �����
				cur_edge->beg = cur_suf_pos;
				cur_edge->lim = size_t(-1); //�������������
				cur_edge->end = 0;
				//������� ������� ������� ���� ���� ��������� � ���������� ��������
				cur_suf++;
				cur_suf_pos = cur_suf;
				if (last_linked_vertex) {
					//������� �� ���������� ������� � ������� ���� ��������
					last_linked_vertex = last_linked_vertex->suffix_link;
					last_linked_depth--;

					cur_suf_pos += last_linked_depth;

					fictive_edge.beg = 0;//����� � ������� ����
					fictive_edge.lim = 0;
					fictive_edge.end = last_linked_vertex;

					cur_edge = &fictive_edge;
					cur_edge_pos = 0;

					depth = last_linked_depth;

					//���� � ������� � ������� �� ��������, ��� �������� ������
					last_linked_vertex = 0;
					last_linked_depth = -1;

					continue;
				}

				//� ������ ������
				cur_edge = &edge_to_root;
				cur_edge_pos = 0;
				depth = 0;
				continue;
			}

		}
	}

	//print_edge(0, s, &edge_to_root);

	while (true) {
		std::string p;
		std::cin >> p;
		if (p==std::string("exit")) break;
		find_substring(s, &edge_to_root, p);
	}
	return 0;
}