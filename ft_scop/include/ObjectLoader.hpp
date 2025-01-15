#ifndef OBJECTLOADER_HPP
#define OBJECTLOADER_HPP

#include <vector>
#include <deque>
#include <cstring>
#include <cstdlib>
#include "Shader.hpp"
#include "Object.hpp"
#include "Time.hpp"

//load object data from file
class ObjectLoader
{
private:
    float maxy;
    float miny;
    std::vector<Vertextex> vertices_buffer;
    std::vector<Indice> indices_buffer;
    void parseLine(const std::string& line);
    void parseLine(const char *line);
    void parseIndice(std::istringstream& stream);
    void parseIndice(const char *line);

    void reset();

public:
    Object* parse(const std::string& filePath);
    // Object* parseFast(const std::string &filePath);
    ObjectLoader();
    ~ObjectLoader();
};

#endif