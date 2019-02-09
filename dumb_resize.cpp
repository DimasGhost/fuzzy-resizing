#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

ifstream fin("image.txt");
ofstream fout("image_new.txt");

const int MAXD = 1000;

int h, w;
float oldImg[MAXD][MAXD];
float newImg[MAXD * 3][MAXD * 3];

void make_pixel(int i, int j) {
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx)
			newImg[i * 3 + dy + 1][j * 3 + dx + 1] = oldImg[i][j];
}

int main() {
	fin >> h >> w;
	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j)
			fin >> oldImg[i][j];
	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j)
			make_pixel(i, j);
	fout << h * 3 << ' ' << w * 3 << endl;
	for (int i = 0; i < h * 3; ++i)
		for (int j = 0; j < w * 3; ++j)
			fout << newImg[i][j] << ' ';
	cout << endl;
	return 0;
}
