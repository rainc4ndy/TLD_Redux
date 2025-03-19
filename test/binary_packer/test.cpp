#include <iostream>
#include <filesystem>
#include <set>
#include <istream>

namespace fs = std::filesystem;

#include "CBinaryPacker.h"

int main() {
    CBinaryPacker pck;

    std::set<std::string> allowed_ext = {
        ".png",
        ".jpg"
    };

    pck.open("data.bin");

    fs::path cwd = fs::current_path();
    std::cout << "Files in current working directory (" << cwd << "):\n";

    // Iterate through the directory
    for (const auto& entry : fs::directory_iterator(cwd)) {
        if (fs::is_regular_file(entry.status())) {
            auto s = entry.path().filename();
            if (allowed_ext.count(s.extension().string())) {
                std::cout << "packing " << s.generic_string() << std::endl;

                std::ifstream f(s.generic_string(), std::ios_base::binary);
                f.seekg(0, std::ios::end);
                size_t size = f.tellg();

                auto buffer = new unsigned char[size];

                f.seekg(0);
                f.read(reinterpret_cast<char*>(buffer), size); 

                pck.appendBinary(s.generic_string(), buffer, size);

                delete buffer;
            }
        }
    }

    pck.save(); 

    /* pck.open("data.bin", true);
    
    stBinaryHeader header;
    while (pck.nextHeader(header))
    {
        std::cout << "dumping " << header.fileNameStr << std::endl;
        auto data = new unsigned char[header.size];
        pck.dump(data, header.size);
        delete data;
    }
 */
    return 1;
}