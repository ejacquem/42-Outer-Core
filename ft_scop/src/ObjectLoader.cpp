#include "ObjectLoader.hpp"

ObjectLoader::ObjectLoader(){}
ObjectLoader::~ObjectLoader(){}

std::string readFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }
    std::ostringstream contentStream;
    contentStream << file.rdbuf();
    return contentStream.str();
}

Object ObjectLoader::parse(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    std::ostringstream contentStream;
    std::string line;
    while (std::getline(file, line)) {
        this->parseLine(line);
    }
    std::cout << "number of vertices: " << vertices_buffer.size() << std::endl;
    std::cout << "number of indices: " << indices_buffer.size() << std::endl;

    return Object(vertices_buffer, indices_buffer);
}

void ObjectLoader::parseLine(const std::string& line)
{
    std::istringstream stream(line);
    std::string prefix;
    float x, y, z;
    int i, j, k, l;

    stream >> prefix;
    //example: 
    //v -0.500000 -0.500000 -0.500000
    if (prefix == "v") {
        if (!(stream >> x >> y >> z))
            return;
        vertices_buffer.push_back({x, y, z});
        std::cout << "Vertex: " << x << ", " << y << ", " << z << std::endl;
    }
    else if (prefix == "f") {
        if (!(stream >> i >> j >> k))
            return;
        indices_buffer.push_back({i - 1, j - 1, k - 1});
        std::cout << "Indice: " << i << ", " << j << ", " << k << std::endl;
        if (!(stream >> l))
            return;
        indices_buffer.push_back({i - 1, k - 1, l - 1});
        std::cout << "Indice: " << i << ", " << k << ", " << l << std::endl;
    }
}


