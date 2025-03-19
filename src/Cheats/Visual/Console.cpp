//
// Created by Administrator on 2024/10/10.
//

#include "Console.h"

#include "CMenu.h"
#include "Plugin.h"

namespace CheatModule::Visual {

    // Constructor
    Console::Console() : CCheatModule("终端", "Console View"){
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;

        AddCommand("help", "列出所有的命令", [this](t_ArgumentList& args, const CommandInfo& info) {
            // 使用 `this` 指向当前 Console 实例
            for (const auto& command : Commands) {
                AddLog("%s: %s\n", command.first.c_str(), command.second.description.c_str());
            }
        });

        AddCommand("clear", "清理终端", [this](t_ArgumentList& args, const CommandInfo& info) {
            // 使用 `this` 指向当前 Console 实例
            ClearLog();
        });

        AutoScroll = true;
        ScrollToBottom = false;
        AddLog("Console init.");
    }

    // Destructor
    Console::~Console() {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            ImGui::MemFree(History[i]);
    }

    // Clear log
    void Console::ClearLog() {
        for (int i = 0; i < Items.Size; i++)
            ImGui::MemFree(Items[i]);
        Items.clear();
    }

    // Add log entry
    void Console::AddLog(const char *fmt, ...) {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
    }

    // Utility functions
    int Console::Stricmp(const char *s1, const char *s2) {
        int d;
        while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
            s1++;
            s2++;
        }
        return d;
    }

    int Console::Strnicmp(const char *s1, const char *s2, int n) {
        int d = 0;
        while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) {
            s1++;
            s2++;
            n--;
        }
        return d;
    }

    char *Console::Strdup(const char *s) {
        IM_ASSERT(s);
        size_t len = strlen(s) + 1;
        void *buf = ImGui::MemAlloc(len);
        IM_ASSERT(buf);
        return (char *) memcpy(buf, (const void *) s, len);
    }

    void Console::Strtrim(char *s) {
        char *str_end = s + strlen(s);
        while (str_end > s && str_end[-1] == ' ') str_end--;
        *str_end = 0;
    }

    void Console::onSave(nlohmann::json &j) {
        saveData(j, "always_on_top", always_on_top);
    }
    void Console::onLoad(const nlohmann::json &j) {
        loadData(j, "always_on_top", always_on_top);
    }

    // Command execution
    void Console::ExecCommand(const char *command_line) {
        AddLog("# %s\n", command_line);
        HistoryPos = -1;

        // 将命令行分解为命令和参数
        std::istringstream iss(command_line);
        std::string command;
        t_ArgumentList args;

        // 提取第一个单词作为命令
        iss >> command;

        // 提取其余的单词作为参数
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        // 更新历史记录
        for (int i = History.Size - 1; i >= 0; i--) {
            if (Stricmp(History[i], command_line) == 0) {
                ImGui::MemFree(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        }
        History.push_back(Strdup(command_line));

        // 转换命令为小写
        std::ranges::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) { return std::tolower(c); });

        // 查找命令
        auto cmd = Commands.find(command);
        if (cmd != Commands.end()) {
            auto info = cmd->second;
            info.func(args, info); // 调用命令函数，传递参数
        } else {
            AddLog("未知的指令: '%s'\n", command_line);
        }
        ScrollToBottom = true;
    }

    void Console::AddCommand(const std::string &cmd, const std::string description, const t_CommandFunction& func, CCheatModule* module) {
        Commands[cmd] = CommandInfo {
            func, description, module
        };
    }

    void Console::draw() {
        if (!enabled)
            return;

        auto menu = Plugin::getInstance().getMenu();
        if (menu->isEnabled() || always_on_top) {
            static auto open = true;
            Draw("命令终端", &open);
            if (!open) {
                enabled = false;
            }
        }
    }

    // Draw the console
    void Console::Draw(const char *title, bool *p_open) {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar)) {
            ImGui::End();
            return;
        }
        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("关闭"))
                *p_open = false;
            ImGui::EndPopup();
        }
        if (ImGui::SmallButton("清理")) { ClearLog(); }
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::SmallButton("复制");
        ImGui::Separator();

        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
         // Leave room for 1 separator + 1 InputText
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar)) {
            if (copy_to_clipboard)
                ImGui::LogToClipboard();
            for (int i = 0; i < Items.Size; i++) {
                const char *item = Items[i];
                if (!Filter.PassFilter(item))
                    continue;

                ImVec4 color;
                bool has_color = false;
                if (strstr(item, "[error]")) {
                    color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
                    has_color = true;
                } else if (strncmp(item, "# ", 2) == 0) {
                    color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                    has_color = true;
                }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item);
                if (has_color)
                    ImGui::PopStyleColor();
            }

            if (copy_to_clipboard)
                ImGui::LogFinish();

            if (ScrollToBottom)
                ImGui::SetScrollHereY(1.0f);
            ScrollToBottom = false;
        }
        ImGui::EndChild();
        ImGui::Separator();

        bool reclaim_focus = false;
        if (ImGui::InputText("输入", InputBuf, IM_ARRAYSIZE(InputBuf),
                             ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
                             ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void *) this)) {
            char *s = InputBuf;
            ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1);
        ImGui::End();
    }

    // Callback stub
    int Console::TextEditCallbackStub(ImGuiInputTextCallbackData *data) {
        Console *console = (Console *) data->UserData;
        return console->TextEditCallback(data);
    }

    // Text editing callback handler
    int Console::TextEditCallback(ImGuiInputTextCallbackData *data) {
        switch (data->EventFlag) {
            case ImGuiInputTextFlags_CallbackCompletion: {
                // Complete current word logic here
                break;
            }
            case ImGuiInputTextFlags_CallbackHistory: {
                // History navigation logic here
                break;
            }
        }
        return 0;
    }
}
