#!/usr/bin/env python3
from PIL import Image
im = Image.open("prova.png")
print(im.size)
w = im.size[0]
h = im.size[1]
print(w)
print(h)

a = ""
for i in range(0,int(h)):
	for j in range(0,int(w)):
		if(im.getpixel((j,i)) == (0, 0, 0, 255)):
			print("1", end="")
			a+= "1"
		else:
			print("0", end="")
			a+="0"
	print("")

print(len(a))
div = w/8
print(div)
for i in range(0, int(len(a)/div)):
	strg = a[(i*8):(i*8+8)]
	print("B"+strg, end= ", ")
	if(i%div == div-1):
		print("")
for i in range(0, int(len(a)/div)):
	strg = a[(i*8):(i*8+8)]
	print(hex(int(strg, 2)), end= ", ")
	if(i%(div*2) == (div*2)-1):
		print("")
