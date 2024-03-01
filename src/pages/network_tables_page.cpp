#include <HomersDashboard/pages/network_tables_page.h>

void NetworkTablesPage::present(bool* running) {
  if (!m_init) {
    std::string_view prefix = "/";

    // Touch every value so it'll actually be returned by GetKeys().
    m_nt_handler.instance().AddListener(
        std::span<std::string_view> {&prefix, 1}, nt::EventFlags::kValueAll,
        [](const nt::Event&) {});

    // Get the top-most table.
    m_root_table = m_nt_handler.instance().GetTable(prefix);

    m_init = true;
  }

  ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin(name(), running, ImGuiWindowFlags_NoCollapse)) {
    ImGui::End();
    return;
  }

  if (ImGui::BeginTable("data", 2,
                        ImGuiTableFlags_SizingFixedFit |
                            ImGuiTableFlags_BordersInner |
                            ImGuiTableFlags_Resizable)) {

    ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthStretch, 100);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch, 100);
    ImGui::TableHeadersRow();

    show_table("", m_root_table);

    ImGui::EndTable();
  }

  ImGui::End();
}

void NetworkTablesPage::show_value(const nt::Value& value) {
  switch (value.type()) {
  case NT_UNASSIGNED:
    ImGui::TextUnformatted("=== NT_UNASSIGNED ===");
    break;
  case NT_RAW:
    ImGui::TextUnformatted("=== NT_RAW ===");
    break;
  case NT_RPC:
    ImGui::TextUnformatted("=== NT_RPC ===");
    break;
  case NT_STRING:
    ImGui::Text("\"%s\"", value.GetString().data());
    break;
  case NT_INTEGER:
    ImGui::Text("%lld", value.GetInteger());
    break;
  case NT_FLOAT:
    ImGui::Text("%f", value.GetFloat());
    break;
  case NT_DOUBLE:
    ImGui::Text("%f", value.GetDouble());
    break;
  case NT_BOOLEAN:
    ImGui::Text("%s", value.GetBoolean() ? "true" : "false");
    break;
  case NT_STRING_ARRAY:
  case NT_INTEGER_ARRAY:
  case NT_FLOAT_ARRAY:
  case NT_DOUBLE_ARRAY:
  case NT_BOOLEAN_ARRAY:
    show_value_array(value);
    break;
  default:
    assert(false);
    break;
  }
}

void NetworkTablesPage::show_value_array(const nt::Value& value) {
  ImGui::PushID((char*)&value.value());
  if (ImGui::TreeNodeEx("Array")) {
    switch (value.type()) {
    case NT_STRING_ARRAY: {
      std::span<const std::string> strings = value.GetStringArray();
      for (std::size_t i = 0; i < strings.size(); i++) {
        ImGui::Text("\"%s\"", strings[i].data());
      }
      break;
    }
    case NT_INTEGER_ARRAY: {
      std::span<const int64_t> ints = value.GetIntegerArray();
      for (std::size_t i = 0; i < ints.size(); i++) {
        ImGui::Text("%lld", ints[i]);
      }
      break;
    }
    case NT_FLOAT_ARRAY: {
      std::span<const float> floats = value.GetFloatArray();
      for (std::size_t i = 0; i < floats.size(); i++) {
        ImGui::Text("%f", floats[i]);
      }
      break;
    }
    case NT_DOUBLE_ARRAY: {
      std::span<const double> doubles = value.GetDoubleArray();
      for (std::size_t i = 0; i < doubles.size(); i++) {
        ImGui::Text("%f", doubles[i]);
      }
      break;
    }
    case NT_BOOLEAN_ARRAY: {
      std::span<const int> bools = value.GetBooleanArray();
      for (std::size_t i = 0; i < bools.size(); i++) {
        ImGui::Text("%s", bools[i] ? "true" : "false");
      }
      break;
    }
    default:
      assert(false);
      break;
    }

    ImGui::TreePop();
  }

  ImGui::PopID();
}

void NetworkTablesPage::show_table(std::string_view path,
                                   std::shared_ptr<nt::NetworkTable> table) {
  if (!path.empty()) {
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    ImGui::PushFont(m_font_lib.bold_font);
    const bool open =
        ImGui::TreeNodeEx(path.data(), ImGuiTreeNodeFlags_SpanFullWidth |
                                           ImGuiTreeNodeFlags_DefaultOpen);
    ImGui::PopFont();
    if (!open) return;
  }

  std::vector<std::string> keys = table->GetKeys();
  std::sort(keys.begin(), keys.end());

  for (const std::string& key : keys) {
    ImGui::TableNextRow();

    ImGui::TableNextColumn();
    ImGui::TextUnformatted(key.c_str());

    ImGui::TableNextColumn();
    show_value(table->GetEntry(key).GetValue());
  }

  for (const std::string& subtable : table->GetSubTables()) {
    show_table(subtable, table->GetSubTable(subtable));
  }

  if (!path.empty()) {
    ImGui::TreePop();
  }
}

