#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

#include "bitmap.cpp"
#include "fuzzy.cpp"

using namespace std;

const int SAMPLES = 120;
const int PATCHES = 5000;
const int MIN_PATCH = 5;
const int MAX_PATCH = 15;

bitmap *small, *sample_small, *sample_normal;
fuzzy_bitmap *result;

int rnd(int mn, int mx) {
	return mn + rand() % (mx - mn + 1);
}

pixel blend_pixels(pixel a, pixel b, float p) {
	pixel res;
	res.r = a.r * p + b.r * (1 - p);
	res.g = a.g * p + b.g * (1 - p);
	res.b = a.b * p + b.b * (1 - p);
	return res;
}

struct patch_rect {
	int up, left;

	patch_rect(int ph, int pw, int h_mx, int w_mx) {
		up = rnd(0, h_mx - ph);
		left = rnd(0, w_mx - pw);
	}

};

void make_patch() {
	int ph = rnd(MIN_PATCH, MAX_PATCH);
	int pw = rnd(MIN_PATCH, MAX_PATCH);

	patch_rect target_rect = patch_rect(ph, pw, small->h, small->w);

	int h_mx = min(sample_small->h, sample_normal->h / 2);
	int w_mx = min(sample_small->w, sample_normal->w / 2);

	patch_rect sample_rect = patch_rect(ph, pw, h_mx, w_mx);

	float similarity = 1;
	//float similarity_sum = 0;
	for (int i = 0; i < ph; ++i)
		for (int j = 0; j < pw; ++j) {

			int y = target_rect.up + i;
			int x = target_rect.left + j;
			pixel a = small->get_pixel(y, x);

			y = sample_rect.up + i;
			x = sample_rect.left + j;
			pixel b = sample_small->get_pixel(y, x);

			float eq = a.equals(b);
			float mn = 0.9;
			if (eq < mn)
				eq = 0;
			else
				eq = (eq - mn) / (1 - mn);
			//similarity_sum += eq;
			similarity = min(similarity, eq);
		}

	//similarity = similarity_sum / (ph * pw);

	for (int i = 0; i < ph * 2; ++i)
		for (int j = 0; j < pw * 2; ++j) {

			int y = sample_rect.up * 2 + i;
			int x = sample_rect.left * 2 + j;
			pixel p = sample_normal->get_pixel(y, x);

			y = target_rect.up * 2 + i;
			x = target_rect.left * 2 + j;

			fuzzy_pixel fp = result->get_pixel(y, x);
			fp.add(p, similarity);
			result->set_pixel(y, x, fp);
		}
}

int main() {
	srand(time(0));

	small = new bitmap("small.arr");

	result = new fuzzy_bitmap(small->h * 2, small->w * 2);

	for (int i = 0; i < result->h; ++i)
		for (int j = 0; j < result->w; ++j) {
			fuzzy_pixel fp = result->get_pixel(i, j);
			fp.add(small->get_pixel(i / 2, j / 2), 1);
			result->set_pixel(i, j, fp);
		}

	for (int sn = 1; sn <= SAMPLES; ++sn) {

		cout << "Processing sample " << sn << endl;

		string name = to_string(sn) + ".arr";

		sample_small = new bitmap("arr_small/" + name);
		sample_normal = new bitmap("arr_normal/" + name);

		for (int i = 0; i < PATCHES; ++i)
			make_patch();

		delete sample_small;
		delete sample_normal;
	}

	bitmap *res = result->to_bitmap();
	delete result;
	/////
	/*for (int i = 0; i < res->h; ++i)
		for (int j = 0; j < res->w; ++j) {
			pixel a = small->get_pixel(i / 2, j / 2);
			pixel b = res->get_pixel(i, j);

			res->set_pixel(i, j, blend_pixels(a, b, 0.8));
		}*/
	/////
	delete small;
	res->save("result.arr");
	delete res;

	return 0;
}
