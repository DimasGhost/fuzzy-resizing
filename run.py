from matplotlib.image import imread
from matplotlib import pyplot as plt
import numpy as np
from os import system

image = imread("image.png")
h, w = image.shape[0], image.shape[1]
new_image = np.zeros((h * 3, w * 3, 4))
dumb_image = np.zeros((h * 3, w * 3, 4))


def read_channel(img, ch):
	txt = open("image_new.txt", 'r')
	nh, nw = map(int, txt.readline().split())
	tmp = np.array(list(map(float, txt.readline().split()))).reshape((nh, nw))
	for i in range(nh):
		for j in range(nw):
			img[i, j, ch] = tmp[i, j]
	txt.close()


for ch in range(3):
	print("writing channel {}".format(ch))
	txt = open("image.txt", 'w')
	txt.write("{} {}\n".format(h, w))
	for i in range(h):
		for j in range(w):
			txt.write('{} '.format(image[i, j, ch]))
	txt.close()

	print("resizing channel {}".format(ch))
	system("./resize")
	read_channel(new_image, ch)

	print("dumb resizing channel {}".format(ch))
	system("./dumb_resize")
	read_channel(dumb_image, ch)

	print("channel {} is done".format(ch))

for i in range(h * 3):
	for j in range(w * 3):
		new_image[i, j, 3] = 1
		dumb_image[i, j, 3] = 1

print("Saving images...")

plt.imshow(new_image)
plt.savefig("new_image")

plt.imshow(dumb_image)
plt.savefig("dumb_image")

print("Done!")
