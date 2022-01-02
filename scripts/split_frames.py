#!/usr/bin/env python3
import sys

from PIL import Image

cols = int(sys.argv[2])

with Image.open(sys.argv[1]) as im:
    frames = im.n_frames
    (width, height) = im.size

    rows = frames // cols

    print(rows)

    output = Image.new('RGB', (width * cols, height * rows), color = 'red')

    for i in range(frames):
        row = i // cols
        col = i % cols

        im.seek(i)

        output.paste(im, (width * col, height * row))

    output.save('output.gif')