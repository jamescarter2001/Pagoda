#pragma once

#define ADD_DATA_LENGTH 0x18

#define ENDIAN_FLAG_BIG 'B'
#define ENDIAN_FLAG_LITTLE 'L'

#define BINA_OFFSET_END 0b00000000
#define BINA_OFFSET_6 0b01000000
#define BINA_OFFSET_14 0b10000000
#define BINA_OFFSET_30 0b11000000

const unsigned int binaSig = 0x414E4942;  // BINA
const unsigned int pacSig = 0x78434150;   // PACx

const unsigned int dataSig = 0x41544144;  // DATA

const char pacxVer[4] = "301";

const char binaVer[4] = "210";

namespace Pagoda::Database {

    // Common

    struct Vector3 {
        float X, Y, Z;
    };

    struct Vector4 {
        float X, Y, Z, W;
    };

    struct Vector5 {
        float X, Y, Z, W, A;
    };

    char SwapFlag(char flag);

    // BINA1

    struct BINAV1Header {
        unsigned int fileSize;
        unsigned int offsetTableOffset;
        unsigned int offsetTableLength;
        unsigned int padding;

        unsigned short unknownFlag1;
        unsigned short footerMagicPresent;
        char reservedValues[2];
        unsigned int signature;
        unsigned int padding2;
    };

    // BINA2

    struct BINAHeader {
        unsigned int header;
        char version[3];
        char endianFlag;
        unsigned int fileSize;
        short nodeCount;
        short Unknown1;
    };

    void SwapBINAHeader(BINAHeader* bh);

    struct PACProxyTableEntry {
        char* type;
        char* name;
        unsigned int nodeIndex;
    };

    struct PACProxyTable {
        unsigned int proxyCount;
        PACProxyTableEntry** entries;
    };

    struct NodeHeader {
        unsigned int signature;
        unsigned int length;

        unsigned int stringTableOffset;
        unsigned int stringTableLength;

        unsigned int offsetTableLength;
        short additionalDataLength = ADD_DATA_LENGTH;
        short padding;
        char additionalData[ADD_DATA_LENGTH];
    };

    void SwapNodeHeader(NodeHeader* nh);
    bool VersionEquals(const char ver1[], const char ver2[]);
}