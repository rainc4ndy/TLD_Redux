#include "CNicknameGenerator.h"
#include <fstream>     // For file input
#include <random>      // For random number generation
#include <ctime>       // For time-based seeding
#include <string>      // For std::string
#include <vector>      // For std::vector

#include "CBotManager.h"
#include "Plugin.h"
#include "samp.h"

namespace CheatModule {
    namespace Bot {
        static size_t roundRobinIndex = 0;
        std::vector<std::string> CNicknameGenerator::nameList;
        std::default_random_engine CNicknameGenerator::randomEngine(static_cast<unsigned>(std::time(nullptr)));
        // Initialize the random engine once with the current time as the seed

        // Load names from a file into the name list
        bool CNicknameGenerator::LoadFromList(std::string file) {
            std::ifstream input(file);
            if (!input.is_open()) {
                return false; // File could not be opened
            }

            nameList.clear(); // Clear existing name list
            std::string name;
            while (std::getline(input, name)) {
                if (!name.empty()) {
                    nameList.push_back(name); // Add each name from the file
                }
            }

            input.close();
            return true;
        }

        // Generate a random binary string
        std::string CNicknameGenerator::FromRandomBinary(int length) {
            std::string result;
            result.reserve(length);

            std::uniform_int_distribution<int> distribution(0, 1); // Binary values 0 and 1

            for (int i = 0; i < length; ++i) {
                result += std::to_string(distribution(randomEngine)); // Use the static random engine
            }

            return result;
        }

        // Generate a random ASCII string
        std::string CNicknameGenerator::FromRandomASCII(int length) {
            std::string result;
            result.reserve(length);

            std::uniform_int_distribution<int> distribution(0, 61); // 62 possible characters
            static const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

            for (int i = 0; i < length; ++i) {
                result += characters[distribution(randomEngine)]; // Use the static random engine
            }

            return result;
        }

        // Generate a string from the player's own name with a random suffix
        std::string CNicknameGenerator::FromSelf(int suffixLength) {
            std::string selfName = samp::RefNetGame()->GetPlayerPool()->m_localInfo.m_szName;
            std::string suffix = FromRandomASCII(suffixLength);
            return selfName + "_" + suffix;
        }

        // Generate a string from another player's name with a random suffix
        std::string CNicknameGenerator::FromOther(int suffixLength) {
            std::string otherName = samp::RefNetGame()->GetPlayerPool()->m_localInfo.m_szName;

            std::vector<int> sel;
            auto pool = samp::RefNetGame()->GetPlayerPool();
            for (int i = 0; i < samp::CPlayerPool::MAX_PLAYERS; i++) {
                if (!pool->m_bNotEmpty[i] || (pool->IsNPC(i)))
                    continue;
                sel.push_back(i);
            }
            if (!sel.empty()) {
                otherName = pool->GetAt(sel[rand() % sel.size()])->m_szNick;
            }
            std::string suffix = FromRandomASCII(suffixLength);
            return otherName + "_" + suffix;
        }

        // Return a random name from the list
        // Return a name from the list in round-robin order
        std::string CNicknameGenerator::FromList() {
            if (nameList.empty()) {
                return ""; // Default name if the list is empty
            }
            size_t attempts = 0;
            while (attempts < nameList.size()) {
                std::string selectedName = nameList[roundRobinIndex];

                // Check if the name is available using the bot manager
                if (Plugin::getInstance().getBotManager()->isNicknameAvailable(selectedName)) {
                    // Update the index for the next call
                    roundRobinIndex = (roundRobinIndex + 1) % nameList.size();
                    return selectedName; // Return the name if it is available
                }

                // Move to the next name in the round-robin sequence
                roundRobinIndex = (roundRobinIndex + 1) % nameList.size();
                attempts++;
            }
            // If no available name is found, return a default value
            return ""; // You can change "root" to a more suitable default if necessary
        }

    } // namespace Bot
} // namespace CheatModule
