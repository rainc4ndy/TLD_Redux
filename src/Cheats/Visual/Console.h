//
// Created by Administrator on 2024/10/10.
//

#ifndef CONSOLE_H
#define CONSOLE_H
#include "CCheatModule.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace CheatModule::Visual {
    struct CommandInfo;
    using t_ArgumentList = std::vector<std::string>;
    using t_CommandFunction = std::function<void(t_ArgumentList&, const CommandInfo&)>;
    struct CommandInfo {
        t_CommandFunction func;
        std::string description;
        CCheatModule* module;
    };

    class Console : public CCheatModule {
    public:
        Console();

        ~Console();
        bool always_on_top;

        void draw();
        void ClearLog();
        void AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void Draw(const char* title, bool* p_open);
        void ExecCommand(const char* command_line);
        void AddCommand(const std::string &cmd, std::string description, const t_CommandFunction &func, CCheatModule* module = nullptr);

        static int Stricmp(const char* s1, const char* s2);
        static int Strnicmp(const char* s1, const char* s2, int n);
        static char* Strdup(const char* s);
        static void Strtrim(char* s);

        void onSave(nlohmann::json &j) override;
        void onLoad(const nlohmann::json &j) override;

    private:
        char                  InputBuf[256];
        std::map<std::string, CommandInfo> Commands;
        ImVector<char*>       Items;
        ImVector<char*>       History;
        int                   HistoryPos;
        ImGuiTextFilter       Filter;
        bool                  AutoScroll;
        bool                  ScrollToBottom;
        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
        int TextEditCallback(ImGuiInputTextCallbackData* data);
    };
}


#endif //CONSOLE_H
