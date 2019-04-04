#include "fire.h"
#include<string>
#include<iostream>
#include<fstream>
using namespace std;

void trim(string &s)
{

	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
		s.erase(s.find_last_not_of("\n") + 1);
		s.erase(s.find_last_not_of("\t") + 1);
	}

}
void initialize(string filename, int moisture, int burn, int& row, int& col, char grid[30][30], int moistureLevel[30][30], int burnLevel[30][30]) {
	ifstream infile;
	infile.open(filename.data());

	string line;

	getline(infile, line);
	string::size_type colon = line.find(":");
	int rows = atoi(line.substr(colon + 1).c_str());
	row = rows<30 ? rows : 30;

	getline(infile, line);
	colon = line.find(':');
	int cols = atoi(line.substr(colon + 1).c_str());
	col = cols<30 ? cols : 30;

	int count = 0;
	while (getline(infile, line) && count<30) {
		trim(line);
		if (line != "") {
			for (int i = 0; i < line.length() && i<59; i++) {
				if (line[i] != ' ') {
					int m = i / 2;
					grid[count][m] = line[i];
					if (line[i] == 'n') {
						moistureLevel[count][m] = moisture;
						burnLevel[count][m] = 0;
					}
					else if (line[i] == 'b') {
						moistureLevel[count][m] = 0;
						burnLevel[count][m] = burn;
					}
					else {
						moistureLevel[count][m] = burnLevel[count][m] = 0;
					}
				}
			}
			count++;
		}

	}

	infile.close();
}

string generateOutput(int row, int col, char grid[30][30], int moistureLevel[30][30], int burnLevel[30][30]) {
	string res;
	for (int i = 0; i < row; i++) {


		for (int j = 0; j < col; j++) {
			res.push_back(grid[i][j]);
		}
		res += "\t\t";
		for (int j = 0; j < col; j++) {
			res += to_string(moistureLevel[i][j]);
		}
		res += "\t\t";
		for (int j = 0; j < col; j++) {
			res += to_string(burnLevel[i][j]);
		}
		res += "\n";

	}
	return res;
}

bool simulation(int moisture, int burn, int row, int col, char grid[30][30], int moistureLevel[30][30], int burnLevel[30][30]) {

	bool flag = false;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (grid[i][j] == 'n') {

				if (i > 0 && grid[i - 1][j] == 'b') {
					moistureLevel[i][j]--;
				}
				if (j > 0 && grid[i][j - 1] == 'b') {

					moistureLevel[i][j]--;

				}

				if (i < row - 1 && grid[i + 1][j] == 'b') {

					moistureLevel[i][j]--;

				}
				if (j < col - 1 && grid[i][j + 1] == 'b') {

					moistureLevel[i][j]--;

				}

				if (moistureLevel[i][j] < 0) {
					grid[i][j] = 'b';
					moistureLevel[i][j] = 0;
					burnLevel[i][j] = burn;
				}
			}
			else if (grid[i][j] == 'b') {
				burnLevel[i][j]--;
				if (burnLevel[i][j] == 0) {
					grid[i][j] = 't';
				}
				flag = true;
			}
		}
	}
	return flag;
}
void fire(string input, string& output)
{

	string::size_type newline = input.find('\n');
	string mo = input.substr(0, newline);
	string::size_type colon = mo.find(':');
	string moi = mo.substr(colon + 1);
	int moisture = atoi(moi.c_str());
	output += "Moisture: " + to_string(moisture) + "\n";

	input = input.substr(newline + 1);
	newline = input.find('\n');
	string bu = input.substr(0, newline);
	colon = bu.find(':');
	string bur = bu.substr(colon + 1);
	int burn = atoi(bur.c_str());
	output += "Burn: " + to_string(burn) + "\n";

	input = input.substr(newline + 1);
	colon = input.find(':');
	string filename = input.substr(colon + 1);
	trim(filename);
	output += "Grid_file: " + filename + "\n";

	char grid[30][30] = {};
	int moistureLevel[30][30] = {};
	int burnLevel[30][30] = {};
	int rows = 0;
	int cols = 0;

	initialize(filename, moisture, burn, rows, cols, grid, moistureLevel, burnLevel);

	output += "Initial State:\n";
	output += generateOutput(rows, cols, grid, moistureLevel, burnLevel);


	int step = 1;

	while (1) {
		if (simulation(moisture, burn, rows, cols, grid, moistureLevel, burnLevel)) {
			output += "Step: " + to_string(step) + "\n";
			output += generateOutput(rows, cols, grid, moistureLevel, burnLevel);
		}
		else break;
		step++;
	}

	output += "Final State:\n";
	output += generateOutput(rows, cols, grid, moistureLevel, burnLevel);
}