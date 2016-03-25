#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

class Aho_Corasick {
public:
	struct Vertex {
		std::map <char, int > children;
		std::map <char, int > go;
		int suff_link;
		bool leaf; //true ���� � ���� ������� ������������ �����-���� ������ �� ������� ������
		int parent;
		char parent_char;
		int up; //������ ���������� ������ -��� ��������� �������, ��������� � ����, ��� �������� leaf=true
		int pat_num;  //����� ������ �������

		Vertex(int _parent, char _c): 
			suff_link(-1), leaf(false), parent(_parent), parent_char(_c), up(-1), pat_num(-1) {
		}

		Vertex() :suff_link(-1), leaf(false), parent(-1), parent_char((char)0), up(-1), pat_num(-1) {
		}	
	};
	std::vector <Vertex> vertices; //���
	std::vector <std::string> patterns; // ������, ����������� �� ����

	Aho_Corasick() {
		vertices.resize(1);
		vertices[0] = Vertex();
		vertices[0].suff_link = 0;	
	}

	int getSuffLink(int v) {
		if (vertices[v].suff_link == -1) { //���� ���������� ������ ��� �� ���������
			if (v == 0 || vertices[v].parent == 0) {
				vertices[v].suff_link = 0;
			}
			else {
				vertices[v].suff_link = getGo(getSuffLink(vertices[v].parent), vertices[v].parent_char);
			}
		}
		return vertices[v].suff_link;
	}

	int getGo(int  v, char c) {
		if (!vertices[v].go[c]) {  //���� ������� �� ������� c ��� �� �������� 
			if (vertices[v].children.find(c)!=vertices[v].children.end()) {
				vertices[v].go[c] = vertices[v].children[c];
			}
			else if (v == 0) {
				vertices[v].go[c] = 0;
			}
			else {
				vertices[v].go[c] = getGo(getSuffLink(v), c);
			}
		}
		return vertices[v].go[c];
	}

	void Aho_Corasick::addString (const std::string & str) {
		int curr = 0;
		for (int i = 0; i < (int)str.length(); ++i) {
			char c = str[i];
			if (!vertices[curr].children[c]) { // ���� �� ������� �� ���� �����
				vertices.push_back(Vertex(curr, c));
				vertices[curr].children[c] = vertices.size() - 1;
			}
			curr = vertices[curr].children[c];
		}
		vertices[curr].leaf = true;
		patterns.push_back(str);
		vertices[curr].pat_num = patterns.size() - 1;	
	}

	int getUp(int v) { 
		if (vertices[v].up == -1) {
			int link = getSuffLink(v);
			if (link == 0) {  //���� v - ������, ���� ���. ������ v ��������� �� ������ 
				vertices[v].up = 0;
			}
			else
				vertices[v].up = (vertices[link].leaf) ? link : getUp(link);
		}
		return vertices[v].up;
	}
	
	void check(int v, int i) { //��������� �������� �� ���������� �������
		for (int u = v; u != 0; //�� ������� ������� �������� v ��������, ��� ��� ������� ������������ �� i-�� ����� � ����� s.
			u = getUp(u)) {
			if (vertices[u].leaf) {
				//std::cout << i - patterns[vertices[u].pat_num].length() + 1 << " " << patterns[vertices[u].pat_num] << std::endl;
				std::cout << i - patterns[vertices[u].pat_num].length() + 1 << std::endl;
			}
		}
	}

	void find_all_pos (const std::string & s) {
		int u = 0;
		for (int i = 0; i < (int)s.length(); i++) {
			u = getGo(u, s[i]);
			check(u, i + 1);
		}
	}
};

std::string read(const std::string & filename) {
	std::string str;
	std::ifstream file;
	file.open(filename);
	while (!file.eof()) {
		std::string tmp;
		std::getline(file, tmp);
		str += tmp;
	}
	file.close();
	return str;
}

int main() {
	std::string str = read("text.txt");
	Aho_Corasick A;
	A.addString("her");
	A.addString("she");
	A.addString("son");
	A.find_all_pos(str);
	return 0;
}