#include "Overlay.h"
#include "TextRenderer.h"
#include "Constants.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void DrawAboutUsOverlay(const AppContext& ctx)
{
    float t = (float)GetTime();
    float glow = (sinf(t * 3.0f) + 1.0f) / 2.0f;

    DrawOverlayTitle(getText("About.title",*ctx.curLanguage), glow);

    Rectangle panel = GetOverlayPanelRect(0.64f, 0.74f);
    DrawClassicPanel(panel, glow);

    float cx = panel.x + panel.width / 2.0f;
    float curY = panel.y + 42.0f;
    float lx = panel.x + 60.0f;

    // ---- Instructor ----
    DrawPixelText(getText("About.instructor",*ctx.curLanguage), (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 120, 70);
    curY += 14;

    {
        const char* name = getText("About.instructor_name",*ctx.curLanguage);
        int nw = (int)(strlen(name) * 8 * FONT_SCALE_SM);
        // Icon hình thầy (dùng ký tự giả lập)
        DrawPixelText("*", (int)(lx), (int)curY, FONT_SCALE_SM, { 255, 200, 80, 180 });
        DrawPixelText(name, (int)(lx + 40), (int)curY, FONT_SCALE_SM, { 255, 230, 140, 255 });
        curY += 40;
    }

    curY += 10;

    // ---- Team Members ----
    DrawPixelText(getText("About.development_team",*ctx.curLanguage), (int)lx, (int)curY, FONT_SCALE_SM, { 255, 200, 80, 220 });
    curY += 32;
    DrawPanelDivider(lx, curY, panel.width - 120, 70);
    curY += 16;

    struct Member {
        const char* name;
        const char* id;
        const char* role;
        const char* roleDetail;
    };

    std::vector<Member> team = {
        { getText("About.member_1_name",*ctx.curLanguage),  getText("About.member_1_id",*ctx.curLanguage), getText("About.member_1_role",*ctx.curLanguage),  getText("About.member_1_roledetail",*ctx.curLanguage)    },
        { getText("About.member_2_name",*ctx.curLanguage),  getText("About.member_2_id",*ctx.curLanguage), getText("About.member_2_role",*ctx.curLanguage),  getText("About.member_2_roledetail",*ctx.curLanguage)         },
        { getText("About.member_3_name",*ctx.curLanguage),  getText("About.member_3_id",*ctx.curLanguage), getText("About.member_3_role",*ctx.curLanguage),  getText("About.member_3_roledetail",*ctx.curLanguage)    }
    };

    Color roleColors[] = {
        { 100, 200, 255, 230 },   // xanh lam nhạt - UI
        { 120, 255, 160, 230 },   // xanh lá       - Logic
        { 255, 180, 100, 230 },   // cam            - Sound
    };

    for (int i = 0; i < 3; i++)
    {
        auto& m = team[i];

        // Số thứ tự + tên
        char numBuf[4];
        snprintf(numBuf, sizeof(numBuf), "%d.", i + 1);
        DrawPixelText(numBuf, (int)lx, (int)curY, FONT_SCALE_SM, { 200, 170, 80, 200 });
        DrawPixelText(m.name, (int)(lx + 40), (int)curY, FONT_SCALE_SM, { 255, 240, 200, 255 });

        curY += 34;

        // ID + Role badge
        char idBuf[32];
        snprintf(idBuf, sizeof(idBuf), "ID: %s", m.id);
        DrawPixelText(idBuf, (int)(lx + 40), (int)curY, 2, { 180, 170, 140, 200 });

        // Role badge (hình chữ nhật nhỏ)
        int roleW = (int)(strlen(m.role) * 8 * 2) + 16;
        int roleX = (int)(lx + 260);
        DrawRectangle(roleX, (int)curY - 2, roleW, 20, { 30, 28, 20, 200 });
        DrawRectangleLinesEx({ (float)roleX, (float)(curY - 2), (float)roleW, 20.0f },
            1, roleColors[i]);
        DrawPixelText(m.role, roleX + 8, (int)curY, 2, roleColors[i]);

        curY += 24;

        // Role detail
        DrawPixelText(m.roleDetail, (int)(lx + 40), (int)curY, 2, { 160, 150, 120, 180 });

        curY += 30;

        // Divider nhỏ giữa các thành viên
        if (i < 2)
            DrawPanelDivider(lx + 20, curY, panel.width - 160, 40);

        curY += 12;
    }

    // ---- Footer / Version ----
    {
        DrawPanelDivider(lx, panel.y + panel.height - 55, panel.width - 120, 50);

        const char* ver = getText("About.version",*ctx.curLanguage);
        int vw = (int)(strlen(ver) * 8 * 2);
        DrawPixelText(ver,
            (int)(panel.x + panel.width / 2 - vw / 2),
            (int)(panel.y + panel.height - 42),
            2, { 160, 150, 100, 160 });
    }
}
