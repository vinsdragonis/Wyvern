import freetype

font_filename = 'fonts/arial.ttf'
font_name = 'Arial'

font_width = 10
font_height = 15

face = freetype.Face(font_filename)
face.set_pixel_sizes(font_width, font_height)

characters = []

for i in range(ord(' '), 127):
    face.load_char(chr(i), freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_MONO)

    # Unpack the font bitmap
    data = bytearray(face.glyph.bitmap.rows * face.glyph.bitmap.width)

    for y in range(face.glyph.bitmap.rows):
        for b in range(face.glyph.bitmap.pitch):
            byte_val = face.glyph.bitmap.buffer[y * face.glyph.bitmap.pitch + b]

            bits_traversed = b * 8
            row = y * face.glyph.bitmap.width + bits_traversed

            for b_index in range(min(8, face.glyph.bitmap.width - bits_traversed)):
                bit = byte_val & (1 << (7 - b_index))
                data[row + b_index] = 1 if bit else 0

    representation = ''
    for y in range(face.glyph.bitmap.rows):
        if y < font_height:
            # representation += ''
            representation += '\t\t0b'
            for x in range(face.glyph.bitmap.width):
                if x < font_width:
                    representation += '1' if data[y * face.glyph.bitmap.width + x] else '0'
                # representation += ','

            if face.glyph.bitmap.width < font_width:
                representation += ''.join(['0' for i in range(font_width - face.glyph.bitmap.width)]) + ','
            # representation += '"\n'
            representation += '\n'

    extra = ''
    if face.glyph.bitmap.rows < font_height:
        for i in range(font_height - face.glyph.bitmap.rows):
            # extra += '\t\t\t"' + ''.join(['0,' for j in range(font_width)]) + '"\n'
            extra += '\t\t0b' + ''.join(['0' for j in range(font_width)]) + ',\n'

    representation = extra + representation
    characters.append(representation)

with open('characters_' + font_name + '.c', 'w') as file:
    count = 0
    array = 0
    file.write('int get' + font_name + 'Character(int index, int y) {\n')
    for i in range(len(characters)):
        if count == 0:
            file.write('unsigned int characters_' + font_name.lower() + '_' + str(array) + '[][150] = {')
        file.write('\t{\n')
        # file.write('\t\t//' + chr(i + ord(' ')) + '\n')
        file.write(characters[i])
        # file.write('\t\t\n')
        file.write('\t},\n')

        count += 1

        if count >= 13:
            array += 1
            count = 0
            file.write('};')
            file.write('//################################################################################\n')

    file.write('};\n')

    file.write('\tint start = (int)(\' \');\n')
    file.write('\tif (index >= start && index < start + 13) {\n')
    file.write('\t\treturn characters_' + font_name.lower() + '_0[index - start][y];\n')
    file.write('\t}\n')
    
    file.write('\telse if (index >= start + 13 && index < start + 13 * 2) {\n')
    file.write('\t\treturn characters_' + font_name.lower() + '_1[index - (start + 13)][y];\n')
    file.write('\t}\n')

    file.write('\telse if (index >= start + 13 * 2 && index < start + 13 * 3) {\n')
    file.write('\t\treturn characters_' + font_name.lower() + '_2[index - (start + 13 * 2)][y];\n')
    file.write('\t}\n')

    file.write('\telse if (index >= start + 13 * 3 && index < start + 13 * 4) {\n')
    file.write('\t\treturn characters_' + font_name.lower() + '_3[index - (start + 13 * 3)][y];\n')
    file.write('\t}\n')

    file.write('\telse if (index >= start + 13 * 4 && index < start + 13 * 5) {\n')
    file.write('\t\treturn characters_' + font_name.lower() + '_4[index - (start + 13 * 4)][y];\n')
    file.write('\t}\n')

    file.write('\telse if (index >= start + 13 * 5 && index < start + 13 * 6) {\n')
    file.write('\t\treturn characters_' + font_name.lower() + '_5[index - (start + 13 * 5)][y];\n')
    file.write('\t}\n')

    file.write('\telse if (index >= start + 13 * 6 && index < start + 13 * 7) {\n')
    file.write('\t\treturn characters_' + font_name.lower() + '_6[index - (start + 13 * 6)][y];\n')
    file.write('\t}\n')

    file.write('\telse if (index >= start + 13 * 7 && index < start + 13 * 8) {\n')
    file.write('\t\treturn characters_' + font_name.lower() + '_7[index - (start + 13 * 7)][y];\n')
    file.write('\t}\n')

    file.write('}\n')



    file.write('\n')
    file.write('const int font_' + font_name.lower() + '_width = ' + str(font_width) + ';\n')
    file.write('const int font_' + font_name.lower() + '_height = ' + str(font_height) + ';\n')
