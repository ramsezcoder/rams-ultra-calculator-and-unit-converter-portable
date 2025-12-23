#include "engine/history_manager.h"
#include <imgui.h>

void HistoryManager::Render() {
    ImGui::Text("History (placeholder)");
    for (int i = 0; i < 5; i++)
        ImGui::BulletText("Result #%d", i + 1);
}
