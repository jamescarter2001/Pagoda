#include "pgpch.h"
#include "BinaWriterV2.h"

namespace Pagoda::Database {

    BinaWriterV2::BinaWriterV2() {
        m_structInsert = m_structs.begin();
    }

    BinaWriterV2::~BinaWriterV2() {

    }

    Pagoda::Database::StructHandle BinaWriterV2::CreateHandle(void* const pStruct, const size_t offset, const size_t size, const size_t padding, const StructHandleType handleType) {
        return StructHandle{pStruct, offset, size, padding, handleType};
    }

	void BinaWriterV2::AddStruct(void* const pStruct, size_t size) {
        StructHandle h = CreateHandle(pStruct, m_currentOffset, size, GetAlignment(size, 4), STRUCT_HANDLE_TYPE_STANDARD);
        AddStruct(h);
	}

	void BinaWriterV2::AddString(char* const pStr) {
        // Get string size, accounting for null termination.
        size_t size = strlen(pStr) + 1;

        StructHandle h = CreateHandle(pStr, m_currentOffset, size, GetAlignment(size, 4), STRUCT_HANDLE_TYPE_STR);
        AddStruct(h);
	}

    void BinaWriterV2::AddStruct(const StructHandle h) {
        const size_t actualSize = h.size + h.padding;

        if (h.handleType == STRUCT_HANDLE_TYPE_STANDARD) {
            m_structInsert = m_structs.emplace(m_structInsert, h);
            m_structsSize += actualSize;
        } else if (h.handleType == STRUCT_HANDLE_TYPE_STR) {
            m_structs.emplace_back(h);
            m_stringsSize += actualSize;
        }

        m_currentOffset += actualSize;
    }

	void BinaWriterV2::Write(std::string& destFile) {
        std::ofstream outFile;
        outFile.open(destFile, std::ios::out | std::ios::binary);

        // Allocate enough bytes for bina header and node data.
        size_t heapSize = sizeof(BINAHeader) + sizeof(NodeHeader) + m_structsSize;
        char* pBinaNode = new char[heapSize];

        BINAHeader* bh = reinterpret_cast<BINAHeader*>(pBinaNode);
        NodeHeader* nh = reinterpret_cast<NodeHeader*>(pBinaNode + sizeof(NodeHeader));

        // Set bina header information.
        bh->header = binaSig;
        memcpy(bh->version, binaVer, 3);
        bh->endianFlag = 'L';

        // Set node header information.
        nh->signature = dataSig;
        nh->additionalDataLength = ADD_DATA_LENGTH;
	}

}