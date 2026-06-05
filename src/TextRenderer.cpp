#include "raylib.h"
#include <map>
#include <vector>
#include <string>

using namespace std;

struct Glyph
{
    int width;
    vector<string> bitmap;
};

map<char, Glyph> font =
{

{'A',{5,{
" ### ",
"#   #",
"#   #",
"#####",
"#   #",
"#   #",
"#   #"}}},

{'B',{5,{
"#### ",
"#   #",
"#   #",
"#### ",
"#   #",
"#   #",
"#### "}}},

{'C',{5,{
" ### ",
"#   #",
"#    ",
"#    ",
"#    ",
"#   #",
" ### "}}},

{'D',{5,{
"#### ",
"#   #",
"#   #",
"#   #",
"#   #",
"#   #",
"#### "}}},

{'E',{5,{
"#####",
"#    ",
"#    ",
"#### ",
"#    ",
"#    ",
"#####"}}},

{'F',{5,{
"#####",
"#    ",
"#    ",
"#### ",
"#    ",
"#    ",
"#    "}}},

{'G',{5,{
" ### ",
"#   #",
"#    ",
"# ###",
"#   #",
"#   #",
" ### "}}},

{'H',{5,{
"#   #",
"#   #",
"#   #",
"#####",
"#   #",
"#   #",
"#   #"}}},

{'I',{5,{
"#####",
"  #  ",
"  #  ",
"  #  ",
"  #  ",
"  #  ",
"#####"}}},

{'J',{5,{
"#####",
"    #",
"    #",
"    #",
"#   #",
"#   #",
" ### "}}},

{'K',{5,{
"#   #",
"#  # ",
"# #  ",
"##   ",
"# #  ",
"#  # ",
"#   #"}}},

{'L',{5,{
"#    ",
"#    ",
"#    ",
"#    ",
"#    ",
"#    ",
"#####"}}},

{'M',{5,{
"#   #",
"## ##",
"# # #",
"#   #",
"#   #",
"#   #",
"#   #"}}},

{'N',{5,{
"#   #",
"##  #",
"# # #",
"#  ##",
"#   #",
"#   #",
"#   #"}}},

{'O',{5,{
" ### ",
"#   #",
"#   #",
"#   #",
"#   #",
"#   #",
" ### "}}},

{'P',{5,{
"#### ",
"#   #",
"#   #",
"#### ",
"#    ",
"#    ",
"#    "}}},

{'Q',{5,{
" ### ",
"#   #",
"#   #",
"#   #",
"# # #",
"#  # ",
" ## #"}}},

{'R',{5,{
"#### ",
"#   #",
"#   #",
"#### ",
"# #  ",
"#  # ",
"#   #"}}},

{'S',{5,{
" ####",
"#    ",
"#    ",
" ### ",
"    #",
"    #",
"#### "}}},

{'T',{5,{
"#####",
"  #  ",
"  #  ",
"  #  ",
"  #  ",
"  #  ",
"  #  "}}},

{'U',{5,{
"#   #",
"#   #",
"#   #",
"#   #",
"#   #",
"#   #",
" ### "}}},

{'V',{5,{
"#   #",
"#   #",
"#   #",
"#   #",
"#   #",
" # # ",
"  #  "}}},

{'W',{5,{
"#   #",
"#   #",
"#   #",
"# # #",
"# # #",
"## ##",
"#   #"}}},

{'X',{5,{
"#   #",
"#   #",
" # # ",
"  #  ",
" # # ",
"#   #",
"#   #"}}},

{'Y',{5,{
"#   #",
"#   #",
" # # ",
"  #  ",
"  #  ",
"  #  ",
"  #  "}}},

{'Z',{5,{
"#####",
"    #",
"   # ",
"  #  ",
" #   ",
"#    ",
"#####"}}},

{ 'a',{5,{
"     ",
" ### ",
"    #",
" ####",
"#   #",
"#   #",
" ####"}} },

{ 'b',{5,{
"#    ",
"#    ",
"# ## ",
"##  #",
"#   #",
"#   #",
"#### "}} },

{ 'c',{5,{
"     ",
" ### ",
"#   #",
"#    ",
"#    ",
"#   #",
" ### "}} },

{ 'd',{5,{
"    #",
"    #",
" ## #",
"#  ##",
"#   #",
"#   #",
" ####"}} },

{ 'e',{5,{
"     ",
" ### ",
"#   #",
"#####",
"#    ",
"#   #",
" ### "}} },

{ 'f',{5,{
"  ## ",
" #  #",
" #   ",
"#### ",
" #   ",
" #   ",
" #   "}} },

{ 'g',{5,{
"     ",
" ####",
"#   #",
"#   #",
" ####",
"    #",
" ### "}} },

{ 'h',{5,{
"#    ",
"#    ",
"# ## ",
"##  #",
"#   #",
"#   #",
"#   #"}} },

{ 'i',{5,{
"  #  ",
"     ",
" ##  ",
"  #  ",
"  #  ",
"  #  ",
" ### "}} },

{ 'j',{5,{
"   # ",
"     ",
"  ## ",
"   # ",
"   # ",
"#  # ",
" ##  "}} },

{ 'k',{5,{
"#    ",
"#    ",
"#  # ",
"# #  ",
"##   ",
"# #  ",
"#  # "}} },

{ 'l',{5,{
" ##  ",
"  #  ",
"  #  ",
"  #  ",
"  #  ",
"  #  ",
" ### "}} },

{ 'm',{5,{
"     ",
"## # ",
"# # #",
"# # #",
"#   #",
"#   #",
"#   #"}} },

{ 'n',{5,{
"     ",
"# ## ",
"##  #",
"#   #",
"#   #",
"#   #",
"#   #"}} },

{ 'o',{5,{
"     ",
" ### ",
"#   #",
"#   #",
"#   #",
"#   #",
" ### "}} },

{ 'p',{5,{
"     ",
"#### ",
"#   #",
"#   #",
"#### ",
"#    ",
"#    "}} },

{ 'q',{5,{
"     ",
" ####",
"#   #",
"#   #",
" ####",
"    #",
"    #"}} },

{ 'r',{5,{
"     ",
"# ## ",
"##  #",
"#    ",
"#    ",
"#    ",
"#    "}} },

{ 's',{5,{
"     ",
" ####",
"#    ",
" ### ",
"    #",
"    #",
"#### "}} },

{ 't',{5,{
" #   ",
" #   ",
"#### ",
" #   ",
" #   ",
" #  #",
"  ## "}} },

{ 'u',{5,{
"     ",
"#   #",
"#   #",
"#   #",
"#   #",
"#  ##",
" ## #"}} },

{ 'v',{5,{
"     ",
"#   #",
"#   #",
"#   #",
"#   #",
" # # ",
"  #  "}} },

{ 'w',{5,{
"     ",
"#   #",
"#   #",
"# # #",
"# # #",
"## ##",
"#   #"}} },

{ 'x',{5,{
"     ",
"#   #",
" # # ",
"  #  ",
" # # ",
"#   #",
"#   #"}} },

{ 'y',{5,{
"     ",
"#   #",
"#   #",
" ####",
"    #",
"   # ",
" ##  "}} },

{ 'z',{5,{
"     ",
"#####",
"   # ",
"  #  ",
" #   ",
"#    ",
"#####"}} },

{ '0',{5,{
" ### ",
"#   #",
"#  ##",
"# # #",
"##  #",
"#   #",
" ### "}} },

{ '1',{5,{
"  #  ",
" ##  ",
"# #  ",
"  #  ",
"  #  ",
"  #  ",
"#####"}} },

{ '2',{5,{
" ### ",
"#   #",
"    #",
"   # ",
"  #  ",
" #   ",
"#####"}} },

{ '3',{5,{
" ### ",
"#   #",
"    #",
" ### ",
"    #",
"#   #",
" ### "}} },

{ '4',{5,{
"#   #",
"#   #",
"#   #",
"#####",
"    #",
"    #",
"    #"}} },

{ '5',{5,{
"#####",
"#    ",
"#    ",
"#### ",
"    #",
"#   #",
" ### "}} },

{ '6',{5,{
" ### ",
"#   #",
"#    ",
"#### ",
"#   #",
"#   #",
" ### "}} },

{ '7',{5,{
"#####",
"    #",
"   # ",
"  #  ",
" #   ",
" #   ",
" #   "}} },

{ '8',{5,{
" ### ",
"#   #",
"#   #",
" ### ",
"#   #",
"#   #",
" ### "}} },

{ '9',{5,{
" ### ",
"#   #",
"#   #",
" ####",
"    #",
"#   #",
" ### "}} },

{' ',{3,{
"   ",
"   ",
"   ",
"   ",
"   ",
"   ",
"   "}}},

};

void DrawGlyph(const Glyph& g, int x, int y, int scale, Color color)
{
    for (int r = 0; r < g.bitmap.size(); r++)
    {
        for (int c = 0; c < g.bitmap[r].size(); c++)
        {
            if (g.bitmap[r][c] != ' ')
            {
                DrawRectangle(
                    x + c * scale,
                    y + r * scale,
                    scale,
                    scale,
                    color
                );
            }
        }
    }
}

void DrawPixelText(string text, int x, int y, int scale, Color color)
{
    int cursor = x;
    for (char c : text)
    {
        char up = toupper(c);

        if (font.find(up) == font.end())
        {
            cursor += scale * 6;
            continue;
        }

        Glyph g = font[up];

        DrawGlyph(g, cursor, y, scale, color);

        cursor += (g.width + 1) * scale;
    }
}

void DrawPixelTextStyled(string text, int x, int y, int scale)
{
    Color shadow = { 0,0,0,180 };
    Color gold = { 235,200,120,255 };

    DrawPixelText(text, x + 3, y + 3, scale, shadow);
    DrawPixelText(text, x, y, scale, Color { 92, 64, 51, 255 }); 
}
void DrawPixelTextTitle(string text, int x, int y, int scale)
{
    Color highlightColor = {255, 242, 181, 255}; 
    Color fillTop        = {244, 215, 123, 255}; 
    Color fillBottom     = {184, 125, 35, 255};  

    Color innerBorder    = {214, 176, 74, 255}; 
    Color outerBorder    = {75, 50, 21, 255};    

    Color shadowColor    = {42, 22, 8, 255};     

    auto DrawLayer = [&](int offsetX,
                         int offsetY,
                         Color color)
    {
        int cursor = x + offsetX;

        for (char ch : text)
        {
            char up = toupper(ch);

            if (font.find(up) == font.end())
            {
                cursor += scale * 6;
                continue;
            }

            Glyph g = font[up];

            for (int r = 0; r < (int)g.bitmap.size(); r++)
            {
                for (int c = 0; c < (int)g.bitmap[r].size(); c++)
                {
                    if (g.bitmap[r][c] != ' ')
                    {
                        DrawRectangle(
                            cursor + c * scale,
                            y + offsetY + r * scale,
                            scale,
                            scale,
                            color
                        );
                    }
                }
            }

            cursor += (g.width + 1) * scale;
        }
    };

    for (int step = 4; step >= 1; step--)
    {
        DrawLayer(step, step, shadowColor);
    }

    for (int radius = 2; radius >= 1; radius--)
    {
        for (int dx = -radius; dx <= radius; dx++)
        {
            for (int dy = -radius; dy <= radius; dy++)
            {
                if (dx == 0 && dy == 0) continue;

                DrawLayer(dx, dy, outerBorder);
            }
        }
    }

    const int ix[8] = {-1,0,1,-1,1,-1,0,1};
    const int iy[8] = {-1,-1,-1,0,0,1,1,1};

    for (int i = 0; i < 8; i++)
    {
        DrawLayer(ix[i], iy[i], innerBorder);
    }

    int cursor = x;

    for (char ch : text)
    {
        char up = toupper(ch);

        if (font.find(up) == font.end())
        {
            cursor += scale * 6;
            continue;
        }

        Glyph g = font[up];

        int rows = (int)g.bitmap.size();

        for (int r = 0; r < rows; r++)
        {
            float t = (float)r / (rows - 1);

            Color rowColor;

            if (t < 0.35f)
            {
                float k = t / 0.35f;

                rowColor.r =
                    (unsigned char)(highlightColor.r +
                    (fillTop.r - highlightColor.r) * k);

                rowColor.g =
                    (unsigned char)(highlightColor.g +
                    (fillTop.g - highlightColor.g) * k);

                rowColor.b =
                    (unsigned char)(highlightColor.b +
                    (fillTop.b - highlightColor.b) * k);

                rowColor.a = 255;
            }
            else
            {
                float k = (t - 0.35f) / 0.65f;

                rowColor.r =
                    (unsigned char)(fillTop.r +
                    (fillBottom.r - fillTop.r) * k);

                rowColor.g =
                    (unsigned char)(fillTop.g +
                    (fillBottom.g - fillTop.g) * k);

                rowColor.b =
                    (unsigned char)(fillTop.b +
                    (fillBottom.b - fillTop.b) * k);

                rowColor.a = 255;
            }

            for (int c = 0; c < (int)g.bitmap[r].size(); c++)
            {
                if (g.bitmap[r][c] != ' ')
                {
                    DrawRectangle(
                        cursor + c * scale,
                        y + r * scale,
                        scale,
                        scale,
                        rowColor
                    );
                }
            }
        }

        cursor += (g.width + 1) * scale;
    }

    cursor = x;

    for (char ch : text)
    {
        char up = toupper(ch);

        if (font.find(up) == font.end())
        {
            cursor += scale * 6;
            continue;
        }

        Glyph g = font[up];

        for (int c = 0; c < (int)g.bitmap[0].size(); c++)
        {
            if (g.bitmap[0][c] != ' ')
            {
                DrawRectangle(
                    cursor + c * scale,
                    y,
                    scale,
                    max(1, scale / 3),
                    highlightColor
                );
            }
        }

        cursor += (g.width + 1) * scale;
    }
}