#include <HomersDashboard/pages/network_tables.h>
#include <imgui_internal.h>

#define INDENT_LEVEL 16.0f

NetworkTablesPage::NetworkTablesPage() = default;

NetworkTablesPage::~NetworkTablesPage() = default;

void NetworkTablesPage::init() {
  root_table = frc1511::NTHandler::get()->get_instance().GetTable("/");
}

void NetworkTablesPage::present(bool* running) {
  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("Network Tables", running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }
  
  focused = ImGui::IsWindowFocused();

  auto root = frc1511::NTHandler::get()->get_instance().GetTable("/");
  std::vector<std::string> _tables = root_table->GetSubTables();
  std::set<std::string> tables(_tables.begin(), _tables.end());

  for (const std::string& table : tables) {
    show_table(fmt::format("/{}", table));
  }
  
  ImGui::End();
}

void NetworkTablesPage::show_table(std::string name) {
  if (!ImGui::CollapsingHeader(name.c_str())) return;
  ImGui::Indent(INDENT_LEVEL);
  auto table = frc1511::NTHandler::get()->get_instance().GetTable(name);

  std::vector<std::string> keys = table->GetKeys();
  // Sort the keys alphabetically.
  std::sort(keys.begin(), keys.end());

  if (keys.size() && ImGui::BeginTable(name.c_str(), 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable)) {
    ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthStretch, 100);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 100);
    ImGui::TableHeadersRow();
    
    for (const std::string& key : keys) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);

      // The key.
      ImGui::TextUnformatted(key.c_str());

      ImGui::TableSetColumnIndex(1);
      
      auto value = table->GetEntry(key).GetValue();

      // The value.
      switch (value.type()) {
        case NT_UNASSIGNED:
          ImGui::TextUnformatted("=== NT_UNASSIGNED ===");
          break;
        case NT_BOOLEAN:
          ImGui::Text("%s", value.GetBoolean() ? "true" : "false");
          break;
        case NT_DOUBLE:
          ImGui::Text("%f", value.GetDouble());
          break;
        case NT_STRING:
          ImGui::Text("%s", value.GetString().data());
          break;
        case NT_RAW:
          ImGui::TextUnformatted("=== NT_RAW ===");
          break;
        case NT_BOOLEAN_ARRAY:
          for (const auto& v : value.GetBooleanArray()) {
            ImGui::Text("%s, ", v ? "true" : "false");
          }
          break;
        case NT_DOUBLE_ARRAY:
          for (const auto& v : value.GetDoubleArray()) {
            ImGui::Text("%f, ", v);
          }
          break;
        case NT_STRING_ARRAY:
          for (const auto& v : value.GetStringArray()) {
            ImGui::Text("\"%s\", ", v.data());
          }
          break;
        case NT_RPC:
          ImGui::TextUnformatted("=== NT_RPC ===");
          break;
        case NT_INTEGER:
          ImGui::Text("%lld", value.GetInteger());
          break;
        case NT_FLOAT:
          ImGui::Text("%f", value.GetFloat());
          break;
        case NT_INTEGER_ARRAY:
          for (const auto& v : value.GetIntegerArray()) {
            ImGui::Text("%lld, ", v);
          }
          break;
        case NT_FLOAT_ARRAY:
          for (const auto& v : value.GetFloatArray()) {
            ImGui::Text("%f, ", v);
          }
          break;
      }
    }
    ImGui::EndTable();
  }

  std::vector<std::string> _subtables = table->GetSubTables();
  std::set<std::string> subtables(_subtables.begin(), _subtables.end());
  for (const std::string& subtable : subtables) {
    show_table(fmt::format("{}/{}", name, subtable));
  }

  ImGui::Unindent(INDENT_LEVEL);
}

NetworkTablesPage NetworkTablesPage::instance;
