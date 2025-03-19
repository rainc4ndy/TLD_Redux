#include "CProxyManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

// Helper function to create the key for proxyRecord
std::string makeProxyKey(const std::string& host, unsigned short port) {
    return host + ":" + std::to_string(port);
}

bool CProxyManager::add(std::string host, unsigned short port) {
    std::string proxyKey = makeProxyKey(host, port);

    // Check if proxy already exists in proxyRecord
    if (proxyRecord.find(proxyKey) != proxyRecord.end()) {
        return false; // Proxy already exists
    }

    // Add new proxy to the proxyAuthList and proxyRecord
    ProxyData newProxy = { {host, port}, {"", ""}, 0 };
    proxyAuthList.push_back(newProxy);
    proxyRecord[proxyKey] = proxyAuthList.size() - 1;

    return true;
}

bool CProxyManager::add(std::string host, unsigned short port, std::string user, std::string password) {
    std::string proxyKey = makeProxyKey(host, port);

    // Check if proxy already exists in proxyRecord
    if (proxyRecord.find(proxyKey) != proxyRecord.end()) {
        return false; // Proxy already exists
    }

    // Add new proxy with authentication to the proxyAuthList and proxyRecord
    ProxyData newProxy = { {host, port}, {user, password}, 0 };
    proxyAuthList.push_back(newProxy);
    proxyRecord[proxyKey] = proxyAuthList.size() - 1;

    return true;
}

bool CProxyManager::remove(std::string host, unsigned short port) {
    std::string proxyKey = makeProxyKey(host, port);

    // Find the proxy in the proxyRecord
    auto it = proxyRecord.find(proxyKey);
    if (it == proxyRecord.end()) {
        return false; // Proxy not found
    }

    // Remove the proxy from proxyAuthList
    int index = it->second;
    proxyAuthList.erase(proxyAuthList.begin() + index);
    proxyRecord.erase(it);

    // Update the indices in proxyRecord
    for (auto& record : proxyRecord) {
        if (record.second > index) {
            record.second--; // Adjust indices after removal
        }
    }

    return true;
}

const std::vector<ProxyData>& CProxyManager::getAll() const {
    return proxyAuthList;
}

bool CProxyManager::loadFromFile(std::string file) {
    std::ifstream infile(file);
    if (!infile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string host, user, pass;
        unsigned short port;

        // Split the host and port first
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            continue; // Malformed line, no colon found
        }

        host = line.substr(0, colonPos);
        try {
            port = static_cast<unsigned short>(std::stoi(line.substr(colonPos + 1)));
        } catch (const std::exception&) {
            continue; // Malformed line, invalid port
        }

        // Extract optional user and password (if present)
        if (!(iss >> user >> pass)) {
            // If user/pass are not found, use defaults (empty strings)
            user = "";
            pass = "";
        }

        // Add the proxy, with or without user/pass
        add(host, port, user, pass);
    }

    infile.close();
    return true;
}


ProxyData* CProxyManager::getNextProxy() {
    if (proxyAuthList.empty()) {
        return nullptr;
    }

    // Get the next proxy in the list (round-robin fashion)
    currentProxyID = (currentProxyID + 1) % proxyAuthList.size();
    return &proxyAuthList[currentProxyID];
}

ProxyData* CProxyManager::getNextAvailableProxy() {
    if (proxyAuthList.empty()) {
        return nullptr;
    }

    // Find the next proxy with refCount == 0
    for (size_t i = 0; i < proxyAuthList.size(); i++) {
        currentProxyID = (currentProxyID + 1) % proxyAuthList.size();
        if (proxyAuthList[currentProxyID].refCount == 0) {
            return &proxyAuthList[currentProxyID];
        }
    }

    return nullptr; // No available proxy found
}

void CProxyManager::reduceProxyRef(std::string proxy) {
    auto it = proxyRecord.find(proxy);
    if (it != proxyRecord.end()) {
        int index = it->second;
        if (proxyAuthList[index].refCount > 0) {
            proxyAuthList[index].refCount--;
        }
    }
}

void CProxyManager::reduceProxyRef(std::string host, unsigned short port) {
    std::string proxyKey = makeProxyKey(host, port);
    reduceProxyRef(proxyKey);
}
