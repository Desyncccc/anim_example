#define IMGUI_DEFINE_MATH_OPERATORS

#include "custom.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <unordered_map>
#include <string>
 
bool custom::Checkbox(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);

    const float square_sz = ImGui::GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    ImVec2 label_pos = ImVec2(pos.x + square_sz + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y);
    const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + square_sz));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    const float ANIM_DURATION = 0.6f;

    float anim_time = g.LastActiveIdTimer / ANIM_DURATION;
    float t = *v ? 1.0f : 0.0f;
    if (g.LastActiveId == id) {
        t = Animator::Animate(anim_time, ANIM_DURATION, Animator::SmoothStep);
        if (!*v) {

            
            t = 1.0f - t;
        }
    }

    const ImU32 col_checked_bg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.45f, 0.59f, 1.00f, 1.00f));
    const ImU32 col_unchecked_bg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.170f, 0.170f, 0.170f, 1.00f));
    const ImU32 col_border_unchecked_bg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.200f, 0.200f, 0.200f, 1.00f));
    const ImU32 col_marker = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.00f)); // #b5b5b5

    const float rounding = 3.0f; 

    // render bg
    ImGui::RenderFrame(pos, ImVec2(pos.x + square_sz, pos.y + square_sz), col_unchecked_bg, true, rounding);

    // Draw filling with animation
    if (*v) {
        const ImVec2 center = ImVec2(pos.x + square_sz * 0.5f, pos.y + square_sz * 0.5f);
        const float rect_sz = square_sz * t; // The size of the filled square
        const float offset = (square_sz - rect_sz) * 0.5f; // Calculate the offset from the center

        const ImVec2 rect_min = ImVec2(pos.x + offset, pos.y + offset);
        const ImVec2 rect_max = ImVec2(rect_min.x + rect_sz, rect_min.y + rect_sz);

        window->DrawList->AddRectFilled(rect_min, rect_max, col_checked_bg, rounding);

        float checkSize = rect_sz * 0.5f; 
        float checkThickness = rect_sz * 0.1f; 
        checkThickness = (checkThickness < 1.0f) ? 1.0f : checkThickness;

        ImVec2 shortP1 = ImVec2(center.x - checkSize * 0.4f, center.y);
        ImVec2 shortP2 = ImVec2(center.x - checkSize * 0.1f, center.y + checkSize * 0.3f);
        window->DrawList->AddLine(shortP1, shortP2, col_marker, checkThickness);

        ImVec2 longP1 = ImVec2(center.x - checkSize * 0.2f, center.y + checkSize * 0.2f);
        ImVec2 longP2 = ImVec2(center.x + checkSize * 0.4f, center.y - checkSize * 0.3f);
        window->DrawList->AddLine(longP1, longP2, col_marker, checkThickness);
    }

    ImGui::RenderText(label_pos, label);

    return pressed;
}

bool custom::CheckboxFade(const char* label, bool* v) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, nullptr, true);

    const float square_sz = ImGui::GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    ImVec2 label_pos = ImVec2(pos.x + square_sz + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y);
    const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), pos.y + square_sz));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id)) return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    const float ANIM_DURATION = 0.6f;
    float anim_time = g.LastActiveIdTimer / ANIM_DURATION;
    float t = *v ? 1.0f : 0.0f;
    if (g.LastActiveId == id) {
        t = Animator::Animate(anim_time, ANIM_DURATION, Animator::FadeIn);
        if (g.LastActiveId == id && !*v) {
            t = 1.0f - t;
        }
    }

    // colors || don't look at this there are cleaner ways of doing it
    const ImU32 col_checked_bg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.45f, 0.59f, 1.00f, 1.00f));
    const ImU32 col_unchecked_bg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.170f, 0.170f, 0.170f, 1.00f));
    const ImU32 col_border_unchecked_bg = ImGui::ColorConvertFloat4ToU32(ImVec4(0.200f, 0.200f, 0.200f, 1.00f));

    const float rounding = 3.0f;

    // Draw background
    ImGui::RenderFrame(pos, ImVec2(pos.x + square_sz, pos.y + square_sz), col_unchecked_bg, true, rounding);

    // Draw filling with animation
    if (*v) {
        const ImVec2 center = ImVec2(pos.x + square_sz * 0.5f, pos.y + square_sz * 0.5f);
        const float rect_sz = square_sz; // The size of the filled square
        const float offset = (square_sz - rect_sz) * 0.5f; // Calculate the offset from the center

        const ImVec2 rect_min = ImVec2(pos.x + offset, pos.y + offset);
        const ImVec2 rect_max = ImVec2(rect_min.x + rect_sz, rect_min.y + rect_sz);
        const ImU32 col_checked_bg2 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.45f, 0.59f, 1.00f, 1.00f * t));

        window->DrawList->AddRectFilled(rect_min, rect_max, col_checked_bg2, rounding);
        const ImU32 col_marker = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.00f * t)); // #b5b5b5

        float checkSize = rect_sz * 0.5f;
        float checkThickness = rect_sz * 0.1f;
        checkThickness = (checkThickness < 1.0f) ? 1.0f : checkThickness;
        
        ImVec2 shortP1 = ImVec2(center.x - checkSize * 0.4f, center.y);
        ImVec2 shortP2 = ImVec2(center.x - checkSize * 0.1f, center.y + checkSize * 0.3f);
        window->DrawList->AddLine(shortP1, shortP2, col_marker, checkThickness);
        
        ImVec2 longP1 = ImVec2(center.x - checkSize * 0.2f, center.y + checkSize * 0.2f);
        ImVec2 longP2 = ImVec2(center.x + checkSize * 0.4f, center.y - checkSize * 0.3f);
        window->DrawList->AddLine(longP1, longP2, col_marker, checkThickness);
    }

    ImGui::RenderText(label_pos, label);

    return pressed;
}
