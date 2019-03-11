#pragma once

#include <cmath>
#include <fstream>

using namespace std;

typedef unsigned char uc;

const float MAX_DIST = 442;

float sqr(float x) {
	return x * x;
}

float dif(float a, float b) {
	return sqr(a - b);
}

struct pixel {
	uc r, g, b;

	float dist(pixel p) {
		return sqrt(dif(r, p.r) + dif(g, p.g) + dif(b, p.b));
	}

	float equals(pixel p) {
		return min(max((MAX_DIST - dist(p)) / MAX_DIST, 0.0f), 1.0f);
	}

};

struct bitmap {

	int h, w, sz;
	uc *arr;

	void init_size(int _h, int _w) {
		h = _h, w = _w, sz = h * w * 3, arr = new uc[sz];
	}

	bitmap(int h, int w) {
		init_size(h, w);
	}

	bitmap(string path) {
		ifstream in(path, ios::binary);
		in.read((char*)&h, sizeof(h));
		in.read((char*)&w, sizeof(w));
		init_size(h, w);
		in.read((char*)arr, sz);
		in.close();
	}

	void save(string path) {
		ofstream out(path, ios::binary);
		out.write((char*)&h, sizeof(h));
		out.write((char*)&w, sizeof(w));
		out.write((char*)arr, sz);
		out.close();
	}

	pixel get_pixel(int y, int x) {
		int pos = (y * w + x) * 3;
		return { arr[pos], arr[pos + 1], arr[pos + 2] };
	}

	void set_pixel(int y, int x, pixel p) {
		int pos = (y * w + x) * 3;
		arr[pos] = p.r;
		arr[pos + 1] = p.g;
		arr[pos + 2] = p.b;
	}

	~bitmap() {
		delete arr;
	}

};

// int main() {
// 	bitmap *test = new bitmap("arr_normal/1.arr");
// 	bitmap *small = new bitmap(100, 100);
// 	for (int i = 0; i < 100; ++i)
// 		for (int j = 0; j < 100; ++j)
// 			small->set_pixel(i, j, test->get_pixel(i + 100, j + 100));
// 	small->save("test.arr");
// 	return 0;
// }
