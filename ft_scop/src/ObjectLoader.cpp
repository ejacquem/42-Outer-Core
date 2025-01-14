#include "ObjectLoader.hpp"
#include <algorithm>
#include <iostream>
#include <cstring>

// for mmap:
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

ObjectLoader::ObjectLoader() {}
ObjectLoader::~ObjectLoader() {}

std::string readFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: " + filePath);
    }
    std::ostringstream contentStream;
    contentStream << file.rdbuf();
    return contentStream.str();
}

Object* ObjectLoader::parse(const std::string &filePath)
{
    vertices_buffer.clear();
    indices_buffer.clear();
    std::cout << "parsing: " << filePath << std::endl;
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "Unable to open file: " << filePath << std::endl;
        return NULL;
    }

    Timer timer;
    timer.start("object parsing");
    char line_buffer[1024];
    while (file.getline(line_buffer, sizeof(line_buffer)))
    {
        this->parseLine(line_buffer);
    }
    std::cout << "number of vertices: " << vertices_buffer.size() << std::endl;
    std::cout << "number of indices: " << indices_buffer.size() << std::endl;
    timer.stop();

    float radius = (maxy - miny) / 2;

    for (auto &v : vertices_buffer)
    {
        // align object to center
        v.y -= (maxy + miny) / 2;
        v.u = 0.5 - atan2(v.z, v.x) / (2 * M_PI);
        v.v = (0.5 + asin(v.y / radius) / M_PI);
    }

    timer.start("creating object buffer");
    Object* a = new Object(vertices_buffer, indices_buffer);
    timer.stop();
    return a;
}

// parse the indice line in thread
void ObjectLoader::parseIndice(std::istringstream& stream)
{
    int i, j, k;
    Timer t1;
    Timer t2;
    Timer t3;

    // t1.start("stream parsing");
    if (stream >> i && stream.ignore(1000, ' ') && stream >> j)
    {
        // t1.stop();
        // t3.start("stream parsing 2");
        while(stream.ignore(1000, ' ') && stream >> k)
        {
            // t3.stop();
            // t2.start("vector pushback");
            indices_buffer.push_back({i - 1, j - 1, k - 1});
            // t2.stop();
            // std::cout << "Indice: " << i << ", " << j << ", " << k << std::endl;
            j = k;
        }
    }
}

void ObjectLoader::parseIndice(const char *line)
{
    int i, j, k;
    if((line = strchr(line, ' ')))
        i = atoi(line);
    if((line = strchr(line + 1, ' ')))
        j = atoi(line);
    while((line = strchr(line + 1, ' ')))
    {
        k = atoi(line);
        indices_buffer.push_back({i - 1, j - 1, k - 1});
        j = k;
    }
}

void ObjectLoader::parseLine(const char *line)
{
    float x = 0, y = 0, z = 0, u = 0, v = 0;
    // int i, j, k;

    // example:
    // v -0.500000 -0.500000 -0.500000
    if (line[0] == 'v' && line[1] == ' ')
    {
        char* ptr = (char *)line + 2;
        x = strtof(ptr, &ptr);
        y = strtof(ptr, &ptr);
        z = strtof(ptr, &ptr);
        u = 0.5 - atan2(z, x) / (2 * M_PI);
        v = 0.5 + asin(y) / M_PI;
        vertices_buffer.push_back({x, y, z, u, v});

        maxy = std::max(maxy, y);
        miny = std::min(miny, y);
        // std::cout << "Vertex: "
        //   << std::setw(9) << x << ", "
        //   << std::setw(9) << y << ", "
        //   << std::setw(9) << z << ", "
        //   << std::setw(9) << u << ", "
        //   << std::setw(9) << v << std::endl;
    }
    // f 1/a/b 2// 3 4 5 turns into 1 2 3 | 1 3 4 | 1 4 5
    else if (line[0] == 'f' && line[1] == ' ')
    {
        // std::thread t(parseIndice, std::ref(stream), std::ref(indices_buffer));
        // t.join();
        // parseIndice(stream);
        parseIndice(line);
    }
}

// const char* map_file(const char* fname, size_t& length);

// Object ObjectLoader::parseFast(const std::string &filePath)
// {
//     Timer timer;
//     size_t length;
//     timer.start("mapping file");
//     auto f = map_file(filePath.c_str(), length);
//     timer.stop();
//     auto l = f + length;

//     timer.start("object parsing");
//     std::vector<std::string> lines;
//     while (f && f != l)
//     {
//         const char* next = static_cast<const char*>(memchr(f, '\n', l - f));
//         if (!next)
//             next = l;  // Handle case where last line may not end with newline
//         if(*f == 'v' || *f == 'f')
//             this->parseLine(std::string(f, next));
//         f = next + 1;
//     }
//     std::cout << "number of vertices: " << vertices_buffer.size() << std::endl;
//     std::cout << "number of indices: " << indices_buffer.size() << std::endl;
//     timer.stop();

//     timer.start("creating object buffer");
//     Object a = Object(vertices_buffer, indices_buffer);
//     timer.stop();
//     return a;
// }

// void handle_error(const char* msg) {
//     perror(msg); 
//     exit(255);
// }

// const char* map_file(const char* fname, size_t& length)
// {
//     int fd = open(fname, O_RDONLY);
//     if (fd == -1)
//         handle_error("open");

//     // obtain file size
//     struct stat sb;
//     if (fstat(fd, &sb) == -1)
//         handle_error("fstat");

//     length = sb.st_size;

//     const char* addr = static_cast<const char*>(mmap(NULL, length, PROT_READ, MAP_PRIVATE, fd, 0u));
//     if (addr == MAP_FAILED)
//         handle_error("mmap");

//     // TODO close fd at some point in time, call munmap(...)
//     return addr;
// }