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

static Font unicodeFont;
static bool unicodeFontLoaded = false;


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

// Unicode fallback font state (defined below)

void DrawPixelText(string text, int x, int y, int scale, Color color)
{
    int cursor = x;
    const char *c_str = text.c_str();
    int bytesProcessed = 0;
    // Iterate through the UTF-8 string byte by byte
    while (*c_str != '\0')
    {
        // Decode the UTF-8 sequence into a single Unicode codepoint
        int codepoint = GetCodepoint(c_str, &bytesProcessed);

                float fontSize = (float)scale * 10.0f;
                Vector2 pos = { (float)cursor, (float)y };

                // Draw the specific Unicode glyph texture from the font atlas
                DrawTextCodepoint(unicodeFont, codepoint, pos, fontSize, color);

                // Advance cursor based on the TTF font's specific glyph width
                GlyphInfo info = GetGlyphInfo(unicodeFont, codepoint);
                cursor += 6 * scale;
            
        

        // Advance the string pointer by the number of bytes this codepoint occupied
        c_str += bytesProcessed;
    }
}

void DrawPixelTextStyled(string text, int x, int y, int scale)
{
    Color shadow = { 0,0,0,180 };
    Color gold = { 235,200,120,255 };

    DrawPixelText(text, x + 3, y + 3, scale, shadow);
    DrawPixelText(text, x, y, scale, Color { 92, 64, 51, 255 }); 
}

// --------------------------------------------------
// Unicode fallback font support
// --------------------------------------------------

void InitTextRenderer()
{
    if (unicodeFontLoaded) return;
    
    // LƯU Ý: Đảm bảo file .ttf này thực sự có hỗ trợ tiếng Việt (như Arial, Roboto, NotoSans...)
    const char* path = "assets/Fairfax.ttf"; 
    
    std::vector<int> codepoints;

    // 1. Basic Latin & Latin-1 Supplement (0x0020 - 0x00FF)
    // Bao gồm các ký tự tiêu chuẩn và các dấu cơ bản: á, à, ã, ó, ò...
    for (int i = 0x0020; i <= 0x00FF; i++) codepoints.push_back(i);

    // 2. Latin Extended-A (0x0100 - 0x017F)
    // Chứa chữ đ, Đ, và một số ký tự như ă, â, ê, ô...
    for (int i = 0x0100; i <= 0x017F; i++) codepoints.push_back(i);

    // 3. Latin Extended-B (0x0180 - 0x024F) 
    // QUAN TRỌNG: Chứa các ký tự Ơ, ơ, Ư, ư và các biến thể mở rộng khác
    for (int i = 0x0180; i <= 0x024F; i++) codepoints.push_back(i);

    // 4. Latin Extended Additional (0x1E00 - 0x1EFF)
    // Chứa các ký tự tổ hợp phức tạp nhất: ắ, ằ, ẳ, ẵ, ặ, ế, ề, ể, ễ, ệ, ố, hồ...
    for (int i = 0x1E00; i <= 0x1EFF; i++) codepoints.push_back(i);

    // Load font với danh sách các codepoint đã gom lại
    unicodeFont = LoadFontEx(path, 60, codepoints.data(), codepoints.size());
    
    // Filter texture để chữ mượt hơn (tuỳ chọn)
    SetTextureFilter(unicodeFont.texture, TEXTURE_FILTER_BILINEAR);

    if (unicodeFont.glyphCount > 0) unicodeFontLoaded = true;
}

void UnloadTextRenderer()
{
    if (!unicodeFontLoaded) return;
    UnloadFont(unicodeFont);
    unicodeFontLoaded = false;
}

