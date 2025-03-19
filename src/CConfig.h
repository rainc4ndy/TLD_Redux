#ifndef D35BD2F4_25FB_4E86_98E0_A6C6C9C529E9
#define D35BD2F4_25FB_4E86_98E0_A6C6C9C529E9

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

#define PLUGIN_CONFIG_FILE "/config.json"

class CConfig {
public:
    CConfig(/* args */);
    ~CConfig();

    void load(const std::string file);
    static void to_json(nlohmann::json& j, const CConfig& p);
    static void from_json(const nlohmann::json& j, CConfig& p);
    void save(const std::string file);

    void applyGocVariables();

    std::string menu_key_str;
    int menu_key;

    std::string font_str;
    int font_size;
    int color_theme;

    float gocRange;
    int gocDelay;
    int gocMode;
    int gocVehicle;
    float gocSpeed;
    bool gocFix;
};

void to_json(nlohmann::json& j, const CConfig& p);
void from_json(nlohmann::json& j, const CConfig& p);

#endif /* D35BD2F4_25FB_4E86_98E0_A6C6C9C529E9 */
