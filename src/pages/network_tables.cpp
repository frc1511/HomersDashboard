#include <pages/network_tables.h>
#include <imgui_internal.h>

NetworkTablesPage::NetworkTablesPage()
: sd_table(nt::NetworkTableInstance::GetDefault().GetTable("SmartDashboard")) { }

NetworkTablesPage::~NetworkTablesPage() { }

void NetworkTablesPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Network Tables", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();
  
  if (ImGui::BeginTable("Network Tables", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
    ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthStretch, 100);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 100);
    ImGui::TableHeadersRow();

    std::vector<std::string> keys(sd_table->GetKeys());
    // Sort the keys alphabetically.
    std::sort(keys.begin(), keys.end());
    
    for (const std::string& key : keys) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);

      // The key.
      ImGui::TextUnformatted(key.c_str());

      ImGui::TableSetColumnIndex(1);
      
      std::shared_ptr<nt::Value> value(sd_table->GetEntry(key).GetValue());
      
      // The value.
      switch (value->type()) {
        case NT_UNASSIGNED:
          ImGui::TextUnformatted("=== Invalid Value ===");
          break;
        case NT_BOOLEAN:
          ImGui::Text("%s", value->GetBoolean() ? "true" : "false");
          break;
        case NT_DOUBLE:
          ImGui::Text("%f", value->GetDouble());
          break;
        case NT_STRING:
          ImGui::Text("%s", value->GetString().data());
          break;
        case NT_RAW:
          ImGui::Text("%s", value->GetRaw().data());
          break;
        case NT_RPC:
          ImGui::Text("%s", value->GetRpc().data());
          break;
        case NT_BOOLEAN_ARRAY:
          for (const auto& v : value->GetBooleanArray()) {
            ImGui::Text("%s", v ? "true" : "false");
          }
          break;
        case NT_DOUBLE_ARRAY:
          for (const auto& v : value->GetDoubleArray()) {
            ImGui::Text("%f", v);
          }
          break;
        case NT_STRING_ARRAY:
          for (const auto& v : value->GetStringArray()) {
            ImGui::Text("%s", v.data());
          }
          break;
      }
    }
    
    ImGui::EndTable();
  }
  
  ImGui::End();
}

NetworkTablesPage NetworkTablesPage::instance;
