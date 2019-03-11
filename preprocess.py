from funcs import *

n = 120

for i in range(61, n + 1):

	print("Preprocessing {}.jpg".format(i))
	img = load_img('img_normal/{}.jpg'.format(i))

	print("\tConverting to numpy array")
	arr = img_to_arr(img)
	print("\tImage shape is {}".format(arr.shape[:-1]))

	print("\tSaving normal size arr")
	save_arr(arr, 'arr_normal/{}.arr'.format(i))

	print("\tResizing image (50%)")
	img = resize_img(img, 0.5)

	print("\tSaving small image...")
	save_img(img, 'img_small/{}.jpg'.format(i))

	print("\tConverting small version to numpy array")
	arr = img_to_arr(img)

	print("\tSmall image shape is {}".format(arr.shape[:-1]))

	print("\tSaving small size arr")
	save_arr(arr, 'arr_small/{}.arr'.format(i))
