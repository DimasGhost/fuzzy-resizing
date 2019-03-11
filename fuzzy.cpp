#pragma once

#include <cmath>

#include "bitmap.cpp"

using namespace std;

const int CD = 3;
const float TERMS_R_INTERSECT = 1.15;
const float TERM_RADIUS = 1.0f / (CD - 1) / (2 - TERMS_R_INTERSECT);
const float EPS = 1e-9;

float d_to_val(int d) {
	return 255.0f / (CD - 1) * d;
}

float get_ch_dist(int d, uc val) {
	float df = d / (CD - 1.0f) - val / 255.0f;
	return df * df;
}

int eval_fuzzy_channel(float sum, float mass) {
	if (mass < EPS)
		return 128;
	return max(min(int(sum / mass), 255), 0);
}

struct fuzzy_pixel {
	float colors[CD][CD][CD];

	fuzzy_pixel() {
		for (int r = 0; r < CD; ++r)
			for (int g = 0; g < CD; ++g)
				for (int b = 0; b < CD; ++b)
					colors[r][g][b] = 0;
	}

	fuzzy_pixel(pixel p) {
		for (int r = 0; r < CD; ++r)
			for (int g = 0; g < CD; ++g)
				for (int b = 0; b < CD; ++b) {
					float cr = get_ch_dist(r, p.r);
					float cg = get_ch_dist(g, p.g);
					float cb = get_ch_dist(b, p.b);
					float dist = sqrt(cr + cg + cb);
					float proximity = max((TERM_RADIUS - dist) / TERM_RADIUS, 0.0f);
					colors[r][g][b] = proximity;
				}
	}

	void add(pixel p, float prob) {
		fuzzy_pixel fp = fuzzy_pixel(p);
		for (int r = 0; r < CD; ++r)
			for (int g = 0; g < CD; ++g)
				for (int b = 0; b < CD; ++b)
					colors[r][g][b] = max(colors[r][g][b], fp.colors[r][g][b] * prob);
	}

	pixel to_pixel() {
		float r_sum = 0, g_sum = 0, b_sum = 0, mass = 0;
		float r_mass = 0, g_mass = 0, b_mass = 0;
		for (int r = 0; r < CD; ++r)
			for (int g = 0; g < CD; ++g)
				for (int b = 0; b < CD; ++b) {
					float ms = colors[r][g][b];
					r_sum += d_to_val(r) * ms;
					g_sum += d_to_val(g) * ms;
					b_sum += d_to_val(b) * ms;
					mass += ms;
				}
		pixel res;
		res.r = eval_fuzzy_channel(r_sum, mass);
		res.g = eval_fuzzy_channel(g_sum, mass);
		res.b = eval_fuzzy_channel(b_sum, mass);
		return res;
	}

};

struct fuzzy_bitmap {
	int h, w;
	fuzzy_pixel *matrix;

	fuzzy_bitmap(int h, int w): h(h), w(w) {
		matrix = new fuzzy_pixel[h * w];
		for (int i = 0; i < h * w; ++i)
			matrix[i] = fuzzy_pixel();
	}

	fuzzy_pixel get_pixel(int y, int x) {
		return matrix[y * w + x];
	}

	void set_pixel(int y, int x, fuzzy_pixel fp) {
		matrix[y * w + x] = fp;
	}

	bitmap* to_bitmap() {
		bitmap *b = new bitmap(h, w);
		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j)
				b->set_pixel(i, j, get_pixel(i, j).to_pixel());
		return b;
	}

	~fuzzy_bitmap() {
		delete matrix;
	}

};
