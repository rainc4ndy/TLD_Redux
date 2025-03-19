#include "Utils.h"

#include <cstdarg>  // For va_list, va_start, va_end
#include <cstdio>   // For vsnprintf
#include <string>
#include <unordered_map>
#include <vector>

#include <CVector.h>
#include <sampapi/CChat.h>

#include "CCheatManager.h"
#include "Plugin.h"
#include "samp.h"

void CUtils::chatprintf(const char *fmt, ...) {
    // Buffer to hold the formatted string
    std::vector<char> buffer(1024);

    // Initialize variadic arguments
    va_list args;
    va_start(args, fmt);

    // Format the string
    vsnprintf(buffer.data(), buffer.size(), fmt, args);

    // End using variadic arguments
    va_end(args);

    // Pass the formatted string to AddMessage
    samp::RefChat()->AddMessage(-1, buffer.data());
}

void CUtils::logToConsole(const char *fmt, ...) {
	std::vector<char> buffer(1024);

	va_list args;
	va_start(args, fmt);

	vsnprintf(buffer.data(), buffer.size(), fmt, args);

	va_end(args);
	Plugin::getInstance().getCheat()->console.AddLog(buffer.data());
}

void CUtils::errorToConsole(const char *fmt, ...) {
	std::vector<char> buffer(1024);

	const char* errorPrefix = "[error] ";
	std::string errorMessage = errorPrefix;

	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer.data(), buffer.size(), fmt, args);
	va_end(args);

	// Concatenate the prefix and the formatted message
	errorMessage += buffer.data();
	Plugin::getInstance().getCheat()->console.AddLog(errorMessage.c_str());
}

int CUtils::mapStringToVK(const std::string &key) {
    static std::unordered_map<std::string, int> keyMap = {
        // Alphabet keys (represented by ASCII values)
        {"A", 0x41},
        {"B", 0x42},
        {"C", 0x43},
        {"D", 0x44},
        {"E", 0x45},
        {"F", 0x46},
        {"G", 0x47},
        {"H", 0x48},
        {"I", 0x49},
        {"J", 0x4A},
        {"K", 0x4B},
        {"L", 0x4C},
        {"M", 0x4D},
        {"N", 0x4E},
        {"O", 0x4F},
        {"P", 0x50},
        {"Q", 0x51},
        {"R", 0x52},
        {"S", 0x53},
        {"T", 0x54},
        {"U", 0x55},
        {"V", 0x56},
        {"W", 0x57},
        {"X", 0x58},
        {"Y", 0x59},
        {"Z", 0x5A},

        // Number keys (top row)
        {"0", 0x30},
        {"1", 0x31},
        {"2", 0x32},
        {"3", 0x33},
        {"4", 0x34},
        {"5", 0x35},
        {"6", 0x36},
        {"7", 0x37},
        {"8", 0x38},
        {"9", 0x39},

        // Numpad keys
        {"Numpad0", VK_NUMPAD0},
        {"Numpad1", VK_NUMPAD1},
        {"Numpad2", VK_NUMPAD2},
        {"Numpad3", VK_NUMPAD3},
        {"Numpad4", VK_NUMPAD4},
        {"Numpad5", VK_NUMPAD5},
        {"Numpad6", VK_NUMPAD6},
        {"Numpad7", VK_NUMPAD7},
        {"Numpad8", VK_NUMPAD8},
        {"Numpad9", VK_NUMPAD9},

        // Function keys
        {"F1", VK_F1},
        {"F2", VK_F2},
        {"F3", VK_F3},
        {"F4", VK_F4},
        {"F5", VK_F5},
        {"F6", VK_F6},
        {"F7", VK_F7},
        {"F8", VK_F8},
        {"F9", VK_F9},
        {"F10", VK_F10},
        {"F11", VK_F11},
        {"F12", VK_F12},

        // Control keys
        {"Enter", VK_RETURN},
        {"Esc", VK_ESCAPE},
        {"Space", VK_SPACE},
        {"Backspace", VK_BACK},
        {"Tab", VK_TAB},
        {"Shift", VK_SHIFT},
        {"Ctrl", VK_CONTROL},
        {"Alt", VK_MENU},
        {"CapsLock", VK_CAPITAL},

        // Arrow keys
        {"Left", VK_LEFT},
        {"Right", VK_RIGHT},
        {"Up", VK_UP},
        {"Down", VK_DOWN},

        // Other keys
        {"Insert", VK_INSERT},
        {"Delete", VK_DELETE},
        {"Home", VK_HOME},
        {"End", VK_END},
        {"PageUp", VK_PRIOR},
        {"PageDown", VK_NEXT},
        {"PrintScreen", VK_SNAPSHOT},
        {"Pause", VK_PAUSE},

        // Mouse buttons
        {"LButton", VK_LBUTTON},
        {"RButton", VK_RBUTTON},
        {"MButton", VK_MBUTTON},

        // Misc keys
        {"LWin", VK_LWIN},
        {"RWin", VK_RWIN},
        {"Apps", VK_APPS},
        {"Sleep", VK_SLEEP},
        {"NumLock", VK_NUMLOCK},
        {"ScrollLock", VK_SCROLL},
        {"Divide", VK_DIVIDE},
        {"Multiply", VK_MULTIPLY},
        {"Subtract", VK_SUBTRACT},
        {"Add", VK_ADD},
        {"Decimal", VK_DECIMAL},

        // Browser keys
        {"BrowserBack", VK_BROWSER_BACK},
        {"BrowserForward", VK_BROWSER_FORWARD},
        {"BrowserRefresh", VK_BROWSER_REFRESH},
        {"BrowserStop", VK_BROWSER_STOP},
        {"BrowserSearch", VK_BROWSER_SEARCH},
        {"BrowserFavorites", VK_BROWSER_FAVORITES},
        {"BrowserHome", VK_BROWSER_HOME}};

    // Find and return the corresponding VK_Key
    auto it = keyMap.find(key);
    if (it != keyMap.end()) {
        return it->second;
    } else {
        return -1;  // Return -1 for an invalid key
    }
}

void CUtils::vect3_copy(const float in[3], float out[3]) {
    memcpy(out, in, sizeof(float) * 3);
}

uint32_t CUtils::GetFromPool(DWORD value, DWORD Pool, DWORD function) {
    uint32_t retval;
    __asm
    {
		mov ecx, Pool
		mov ecx, [ecx]
		push value
		call function
		mov retval, eax
    }

    return retval;
}

int CUtils::ScriptCarId(struct vehicle_info *car) {
    return (int)GetFromPool((DWORD)car, VEHICLE_POOL_POINTER, FUNC_GET_CAR_ID);
}

int CUtils::ScriptActorId(CPed *ped) {
    return (int)GetFromPool((DWORD)ped, ACTOR_POOL_POINTER, FUNC_GET_ACTOR_ID);
}

int CUtils::ScriptObjectId(struct object_info *object) {
    return (int)GetFromPool((DWORD)object, OBJECT_POOL_POINTER,
                            FUNC_GET_OBJECT_ID);
}

int CUtils::randomInt(int low, int high) {
	return rand() % (high + 1 - low) + low;
}

float CUtils::randomFloat(float low, float high) {
	return low + static_cast<float>(rand()) / RAND_MAX * (high - low);
}

void CUtils::GetMatrixFromQuaternion(float *fQuaternion, MATRIX4X4 *matrix) {
	float xx = fQuaternion[1] * fQuaternion[1];
	float xy = fQuaternion[1] * fQuaternion[2];
	float xz = fQuaternion[1] * fQuaternion[3];
	float xw = fQuaternion[1] * fQuaternion[0];

	float yy = fQuaternion[2] * fQuaternion[2];
	float yz = fQuaternion[2] * fQuaternion[3];
	float yw = fQuaternion[2] * fQuaternion[0];

	float zz = fQuaternion[3] * fQuaternion[3];
	float zw = fQuaternion[3] * fQuaternion[0];

	matrix->right.X = 1.0f - 2.0f * (yy + zz);
	matrix->right.Y = 2.0f * (xy - zw);
	matrix->right.Z = 2.0f * (xz + yw);

	matrix->up.X = 2.0f * (xy + zw);
	matrix->up.Y = 1.0f - 2.0f * (xx + zz);
	matrix->up.Z = 2.0f * (yz - xw);

	matrix->at.X = 2.0f * (xz - yw);
	matrix->at.Y = 2.0f * (yz + xw);
	matrix->at.Z = 1.0f - 2.0f * (xx + yy);
}


void CUtils::GetOffsetedPosition(float * result, float *input, float *offset, float ang) {
	result[2] = input[2] + offset[2]; // z

	result[0] = input[0] - sin(ang) *offset[1];
	result[1] = input[1] + cos(ang) *offset[1];

	result[0] = result[0] + cos(ang) *offset[0];
	result[1] = result[1] + sin(ang) *offset[0];
}

CVector CUtils::GetOffsetedPosition(const CVector &in, const CVector &offset, float ang) {
	float z = in.z + offset.z; // z coordinate
	float x = in.x - sin(ang) * offset.y + cos(ang) * offset.x;
	float y = in.y + cos(ang) * offset.y + sin(ang) * offset.x;
	return {x, y, z};
}