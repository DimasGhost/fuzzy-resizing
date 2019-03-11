from PIL import Image
import numpy

def load_img(path):
	return Image.open(path)

def save_img(img, path):
	img.save(path)

def load_arr(path):
	with open(path, 'rb') as f:
		h = int.from_bytes(f.read(4), byteorder='little')
		w = int.from_bytes(f.read(4), byteorder='little')
		arr = numpy.empty([h, w, 3], dtype=int)
		for i in range(h):
			for j in range(w):
				for c in range(3):
					arr[i][j][c] = int.from_bytes(f.read(1), byteorder='little')
		return arr

def save_arr(arr, path):
	with open(path, 'wb') as f:
		h, w = arr.shape[0], arr.shape[1]
		f.write(h.to_bytes(4, byteorder='little'))
		f.write(w.to_bytes(4, byteorder='little'))
		for i in range(h):
			for j in range(w):
				for c in range(3):
					f.write(int(arr[i][j][c]).to_bytes(1, byteorder='little'))

def img_to_arr(img):
	return numpy.asarray(img)

def arr_to_img(arr):
	return Image.fromarray(numpy.uint8(arr))

def resize_img(img, scale, mode=Image.ANTIALIAS):
	sz = (int(img.size[0] * scale), int(img.size[1] * scale))
	return img.resize(sz, mode)

def img_similarity(img1, img2):
	mxd = ((255 ** 2) * 3) ** 0.5
	arr1 = img_to_arr(img1)
	arr2 = img_to_arr(img2.resize(img1.size, Image.ANTIALIAS))
	arr1 = arr1.astype(numpy.float32, copy=False)
	arr2 = arr2.astype(numpy.float32, copy=False)
	arr3 = (arr1 - arr2) ** 2
	arr3 = numpy.sum(arr3, axis=2) ** 0.5
	arr3 = (mxd - arr3) / mxd
	#arr3[arr3 < 0.5] = 0
	#arr3 = (arr3 - 0.5) * 2
	arr3 = arr3 ** 10
	s = numpy.sum(arr3)
	h, w = arr1.shape[0], arr1.shape[1]
	return s / (h * w)
