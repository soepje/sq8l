#!/usr/bin/env python3

files = [
    ('images/lcd.gif', 'lcdImageMap'),
    ('images/lcdButtons.gif', 'lcdButtons'),
    ('images/background.jpg', 'backgroundImage'),
    ('images/buttons.gif', 'buttons'),
    ('images/programbank.bin', 'programBank'),
    ('images/toggleButton.gif', 'toggleButton'),
    ('images/toggleButtonLed.gif', 'toggleButtonLed'),
]

with open('src/data/Data.h', 'w') as f:
    f.write('#pragma once\n\n')
    f.write('#include <cstdint>\n\n')
    f.write('#include <cstddef>\n\n')
    f.write('namespace sq8l {\n')
    f.write('namespace data {\n\n')

    for resourceFile, resourceName in files:
        with open(resourceFile, 'rb') as f2:
            size = len(f2.read())

        f.write(f'extern uint8_t {resourceName}[{size}];\n')
        f.write(f'extern size_t {resourceName}Size;\n')

    f.write('\n}\n')
    f.write('}\n')

with open('src/data/Data.cpp', 'w') as f:
    f.write('#include <cstdint>\n\n')
    f.write('#include <cstddef>\n\n')
    f.write('namespace sq8l {\n')
    f.write('namespace data {\n\n')

    for resourceFile, resourceName in files:
        with open(resourceFile, 'rb') as f2:
            data = f2.read()

        f.write(f'uint8_t {resourceName}[] = {{\n')

        for i in range(len(data)):
            if i != 0 and i % 10 == 0:
                f.write('\n')
            f.write(hex(data[i]).rjust(4, ' ') + ', ')

        f.write("\n};\n")
        f.write(f'size_t {resourceName}Size = {len(data)};\n')

    f.write('}\n')
    f.write('}\n')
