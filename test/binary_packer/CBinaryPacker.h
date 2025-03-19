#ifndef D693DFBB_6D1A_47A2_BFCD_AF3A0A884820
#define D693DFBB_6D1A_47A2_BFCD_AF3A0A884820

#include <fstream>

struct stBinaryHeader {
    size_t size;
    size_t fileName_sz;

    std::string fileNameStr;
};

class CBinaryPacker
{
private:
    std::fstream buffer;

    static std::string XorKey;
    void xorEncryptDecrypt(const unsigned char* raw, unsigned char* out, size_t sz);

public:
    CBinaryPacker(/* args */);
    ~CBinaryPacker();

    void encodeData(const unsigned char* raw, unsigned char* out, size_t sz);
    void decodeData(const unsigned char* raw, unsigned char* out, size_t sz);

    void appendBinary(std::string name, const unsigned char* raw, size_t sz);
    void open(const char *path, bool read = false);
    void save();

    bool nextHeader(stBinaryHeader& header);
    void dump(unsigned char* out, size_t sz);
};

#endif /* D693DFBB_6D1A_47A2_BFCD_AF3A0A884820 */
