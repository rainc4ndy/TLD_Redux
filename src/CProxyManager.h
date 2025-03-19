#ifndef PROXYMANAGER_H
#define PROXYMANAGER_H

#include <string>
#include <vector>
#include <unordered_map>

using ProxyInfo = std::pair<std::string, unsigned short>;
using AuthInfo = std::pair<std::string, std::string>;

struct ProxyData {
    ProxyInfo proxyInfo;
    AuthInfo authInfo;

    /**
     * A proxy is used by a bot if refCount > 0
     */
    int refCount;
};

class CProxyManager {
private:
    std::vector<ProxyData> proxyAuthList;

    /**
     * A list of records of "x.x.x.x:port"
     * Used for checking if a proxy exists in proxyAuthList
     */
    std::unordered_map<std::string, int> proxyRecord;
    int currentProxyID;
public:
    // Add a proxy, return false if a proxy exist already
    bool add(std::string host, unsigned short port);
    bool add(std::string host, unsigned short port, std::string user, std::string password);

    // Remove a proxy, return false if a proxy does not exist
    bool remove(std::string host, unsigned short port);

    // Returning a reference of the container of all proxy
    const std::vector<ProxyData>& getAll() const;

    // Load all proxy of format "x.x.x.x:port user pass" in a file
    bool loadFromFile(std::string file);

    ProxyData* getNextProxy();
    ProxyData* getNextAvailableProxy();

    //reduce a proxy ref count by 1
    void reduceProxyRef(std::string proxy);
    void reduceProxyRef(std::string host, unsigned short port);
};

#endif //PROXYMANAGER_H
