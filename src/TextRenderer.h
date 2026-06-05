#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "raylib.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

struct Glyph
{
    int width;
    vector<string> bitmap;
};

void DrawGlyph(const Glyph& g, int x, int y, int scale, Color color);
void DrawPixelText(string text, int x, int y, int scale, Color color);
void DrawPixelTextStyled(string text, int x, int y, int scale);
void DrawPixelTextTitle(string text, int x, int y, int scale);

#endif // TEXT_RENDERER_H