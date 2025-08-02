#ifndef SVG_LETTERS_H
#define SVG_LETTERS_H

typedef struct {
    char character;
    const char* svg_path;
    int width;
    int height;
} SvgLetter;

const SvgLetter* get_svg_letter(char ch);

#endif