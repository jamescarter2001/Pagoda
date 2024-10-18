#include "pgpch.h"

#include "database/common/pg_bina.h"
#include "database/common/pg_pacx.h"
#include "database/common/pg_bina_reader.h"
#include "database/common/pg_bina_writer.h"
#include "database/common/pg_bina_template_converter.h"

#include "database/hedgehog/set/pg_set.h"
#include "database/model/pg_model.h"
#include "database/hedgehog/lightfield/pg_lightfield.h"

struct Test {
    int a;
    int b;
    float c;
    int padding1 = 0;
    char* d;
    char* e;
    char* f;
    char* g;
};

int main() {
    std::string s1 = "HELLO";
    std::string s2 = "WORLD";
    std::string s3 = "BINA";
    std::string s4 = "HELLO";

    Test test = {1, 2, 3.0f, 0, (char*)s1.c_str(), (char*)s2.c_str(), (char*)s3.c_str(), (char*)s4.c_str()};

    Pagoda::Database::BinaWriter bw;

    bw.BeginNode();
    bw.AddStruct(&test, sizeof(test));

    bw.AddString((char*)s1.c_str());
    bw.AddString((char*)s2.c_str());
    bw.AddString((char*)s3.c_str());
    bw.AddString((char*)s4.c_str());
    bw.EndNode();

    bw.BeginNode(Pagoda::Database::OffsetType::OFFSET_TYPE_ABSOLUTE);
    bw.AddStruct(&test, sizeof(test));

    bw.AddString((char*)s1.c_str());
    bw.AddString((char*)s2.c_str());
    bw.AddString((char*)s3.c_str());
    bw.AddString((char*)s4.c_str());
    bw.EndNode();

    bw.Write("../output/test_struct_2.orc");

    Pagoda::Database::BinaReader binaReader;
    std::vector<data_t*> testFile = binaReader.Read("../input/w1f01_trr_heightfield.pac");
    Pagoda::Database::PACV3NodeTree* s = reinterpret_cast<Pagoda::Database::PACV3NodeTree*>(testFile[0]);

    std::vector<Pagoda::Database::PACV3TreeNode<void>> treeNodes(s->nodes, s->nodes + s->nodeCount);

    std::cout << "Done!" << std::endl;

    /* Pagoda::Database::BINATemplateConverter conv(PTR_SIZE_64);
    conv.ConvertTemplateAndSave("res/basic.bt", "../output/test.orc", false);

    std::string stg = "901";
    std::vector<data_t*> lfFile = binaReader.Read("C:/sonic2010/dvd_builder_image/sonic2010/stg" + stg + "/stg" + stg + "_obj/arc/stg" + stg + "_lfield.orc");
    Pagoda::Database::LightFieldData lightFieldData = Pagoda::Database::LightFieldData::LightFieldDataFromNodeData(lfFile[0]);
    lightFieldData.Print();

    std::ifstream orcFile("C:/sonic2010/dvd_builder_image/sonic2010/stg901/stg901_obj/arc/stg901_grass.orc", std::ios::binary | std::ios::in);
    Pagoda::Database::BINAV1Header orcHeader;

    READ_STRUCT(orcFile, orcHeader);
    orcFile.seekg(0, std::ios::beg);

    if (orcHeader.signature == binaSig) {
        char* data = new char[orcHeader.fileSize];
        orcFile.read(data, orcHeader.fileSize);

        Pagoda::Database::Node::PrintOffsets(sizeof(orcHeader), data + sizeof(orcHeader) + orcHeader.offsetTableOffset, orcHeader.offsetTableLength);

        delete[] data;
    }

    float cubeArray[] = {
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7
    };

    char en1[4] = "POS";
    char en2[4] = "COL";

    Pagoda::Database::VertexBufferElement e1 = {en1, 1};
    Pagoda::Database::VertexBufferElement e2 = {en2, 2};

    std::vector<Pagoda::Database::VertexBufferElement*> elements;
    elements.push_back(&e1);
    elements.push_back(&e2);

    Pagoda::Database::ModelHeader cubeHeader = {0};

    cubeHeader.vertexBuffer = cubeArray;
    cubeHeader.indexBuffer = indices;
    cubeHeader.elementArray = &elements[0];
    cubeHeader.vertexBufferSize = (unsigned int)sizeof(cubeArray);
    cubeHeader.indexBufferSize = (unsigned int)sizeof(indices);
    cubeHeader.elementCount = (unsigned int)elements.size();

    Pagoda::Database::BinaWriter bw;

    bw.AddStruct(&cubeHeader, sizeof(cubeHeader));

    bw.AddStruct(cubeArray, sizeof(cubeArray));
    bw.AddStruct(indices, sizeof(indices));
    bw.AddStructVector<Pagoda::Database::VertexBufferElement>(elements);

    bw.AddString(en1);
    bw.AddString(en2);
    bw.Write("../output/dummy_cube.orc");

    std::vector<data_t*> binaFile = binaReader.Read("C:/w1r03_gedit/w1r03_autotest.gedit");
    Pagoda::Database::SetData setData = Pagoda::Database::SetData::SetDataFromNodeData(binaFile[0]);
    setData.Print();*/

    return 0;
}