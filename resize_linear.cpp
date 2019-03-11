#include <iostream>
#include <cstdlib>

#include "bitmap.cpp"

using namespace std;

const int TERMS = 5;
const float TERMS_R_INTERSECT = 1;

const float TERM_RADIUS = 1.0f / (TERMS - 1) / (2 - TERMS_R_INTERSECT);

// GRAY SCALE
struct fuzzy_color_gs {
	float grades[TERMS];

	fuzzy_color_gs(uc val) {
		float f_val = val / 255.0f;
		for (int i = 0; i < TERMS; ++i) {
			float center = 1.0f / (TERMS - 1) * i;
			float dist = abs(center - f_val);
			grades[i] = max((TERM_RADIUS - dist) / TERM_RADIUS, 0.0f);
		}
	}

	fuzzy_color_gs(fuzzy_color_gs a, fuzzy_color_gs b) {
		for (int i = 0; i < TERMS; ++i) {
			grades[i] = 0;
			int md = min(i, TERMS - i - 1);
			for (int j = -md; j <= md; ++j)
				grades[i] = max(grades[i], min(a.grades[i - j], b.grades[i + j]));
		}
	}

	uc to_val() {
		float sum = 0, mass = 0;
		for (int i = 0; i < TERMS; ++i) {
			sum += grades[i] * (255.0f / (TERMS - 1) * i);
			mass += grades[i];
		}
		return min(max(int(sum / mass), 0), 255);
	}
};

bitmap *small, *big;

pixel scan_pixel(int y, int x) {
	if (y < 0 || x < 0 || y >= small->h || x >= small->w)
		return {128, 128, 128};
	return small->get_pixel(y, x);
}

uc mix(uc a, uc b) {
	return fuzzy_color_gs(fuzzy_color_gs(a), fuzzy_color_gs(b)).to_val();
}

pixel merge_pixels(pixel a, pixel b) {
	return {mix(a.r, b.r), mix(a.g, b.g), mix(a.b, b.b)};
}

void resolve_pixel(int y, int x) {
	pixel a = small->get_pixel(y, x);
	for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j) {
			pixel b = scan_pixel(y + i, x + j);
			pixel p = merge_pixels(a, b);
			big->set_pixel(y * 3 + 1 + i, x * 3 + 1 + j, p);
		}
}

int main() {
	small = new bitmap("small.arr");
	big = new bitmap(small->h * 3, small->w * 3);

	for (int i = 0; i < small->h; ++i)
		for (int j = 0; j < small->w; ++j)
			resolve_pixel(i, j);

	big->save("result.arr");

	return 0;
}
