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

float get_pixel(int i, int j) {
	if (i < 0 || j < 0 || i >= h || j >= w)
		return 0;
	return oldImg[i][j];
}

float is_color(float color, float middle, float half_range) {
	float delta = abs(color - middle);
	if (delta > half_range)
		return 0;
	return 1 - delta / half_range;
}

float is_black(float color) {
	return is_color(color, 0, 0.5);
}

float is_grey(float color) {
	return is_color(color, 0.5, 0.5);
}

float is_white(float color) {
	return is_color(color, 1, 0.5);
}

float resolve_pixel(float cur, float nb) {
	float black = min(is_black(cur), is_black(nb));
	float grey = max(max(
		min(is_grey(cur), is_grey(nb)),
		min(is_black(cur), is_white(nb))),
		min(is_white(cur), is_black(nb))
		);
	float white = min(is_white(cur), is_white(nb));
	return (0 * black + 0.5 * grey + 1 * white) / (black + grey + white);
}

void make_pixel(int i, int j) {
	for (int dy = -1; dy <= 1; ++dy)
		for (int dx = -1; dx <= 1; ++dx) {
			float cur = oldImg[i][j];
			float nb = get_pixel(i + dy, j + dx);
			newImg[i * 3 + dy + 1][j * 3 + dx + 1] = resolve_pixel(cur, nb);
		}
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
