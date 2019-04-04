#include"fire.h"
#include<iostream>
#include<fstream>
using namespace std;


int main() {
	ifstream infile;
	string filename = "test_06.ini";
	infile.open(filename.data());

	string input;
	infile >> noskipws;
	while (!infile.eof()) {
		char c;
		infile >> c;
		input.push_back(c);
	}
	infile.close();
	string output;
	fire(input, output);
	cout << output << endl;

	int m;
	cin >> m;

	return 0;
}