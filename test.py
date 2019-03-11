import os
from PIL import Image
import matplotlib.pyplot as plt

from funcs import *

folder = ""
modes = {
	"nearest": Image.NEAREST,
	"bilinear": Image.BILINEAR,
	"bicubic": Image.BICUBIC,
}
all_modes = list(modes.keys()) + ["fuzzy_linear", "fuzzy_patterns"]

def produce_resized(path, res_path, factor, mode=Image.ANTIALIAS):
	global folder
	img = load_img(folder + path)
	img = resize_img(img, factor, mode=mode)
	save_img(img, folder + res_path)
	return img	

def run_custom(path, res_path, factor, cmd, mul):
	global folder
	img = load_img(folder + path)
	cur_sz = 1
	while cur_sz < factor:
		arr = img_to_arr(img)
		save_arr(arr, "small.arr")
		os.system(cmd)
		arr = load_arr("result.arr")
		img = arr_to_img(arr)
		cur_sz *= mul
	if cur_sz > factor:
		img = resize_img(img, factor / cur_sz)
	save_img(img, folder + res_path)
	return img

def run_resize(name, path, res_path, factor):
	global modes
	if name in modes:
		return produce_resized(path, res_path, factor, mode=modes[name])
	if name == "fuzzy_linear":
		return run_custom(path, res_path, factor, "./resize_linear", 3)
	if name == "fuzzy_patterns":
		return run_custom(path, res_path, factor, "./resize_patterns", 2)

def make_graph(metrics, folder):
	plt.style.use('seaborn-darkgrid')
	plt.figure(figsize=(8, 5))
	palette = plt.get_cmap('Set1')
	num = 0
	for name, vals in metrics.items():
		num += 1
		plt.plot(
			vals['factor'],
			vals['accuracy'],
			marker='',
			color=palette(num),
			linewidth=2,
			alpha=0.9,
			label=name
		)
		plt.legend(loc='best')
		plt.title("Resizing methods comparison", loc='left', fontsize=12, fontweight=0, color='orange')
		plt.xlabel("Scale factor")
		plt.ylabel("Accuracy")

	plt.savefig(folder + "graph.pdf", format='pdf')

def perform_on_image(original_path):
	global folder
	global all_modes
	
	metrics = {
		name: {
			"factor": [1],
			"accuracy": [1],
		} for name in all_modes
	}

	run_num = 1
	while os.path.exists("run{}".format(run_num)):
		run_num += 1

	folder = "run{}/".format(run_num)

	or_img = load_img(original_path)

	os.mkdir(folder)
	for sz in [2, 3, 4, 5, 6, 7, 8, 9]:
		path = str(sz)
		os.mkdir(folder + path)
		orp = "{}/original.jpg".format(path)
		os.system("cp {} {}{}".format(original_path, folder, orp))
		inp = "{}/small.jpg".format(path)
		produce_resized(orp, inp, 1 / sz)
		for name in all_modes:
			print("Scale factor: {}, Method: {}".format(sz, name))
			res = run_resize(name, inp, "{}/{}.jpg".format(path, name), sz)
			metrics[name]['factor'].append(sz)
			metrics[name]['accuracy'].append(img_similarity(or_img, res))

	make_graph(metrics, folder)

perform_on_image("kek.jpg")
