#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <SDL2/SDL.h>

struct Vec3 { float x, y, z; };
struct Vec2 { float u, v; };

std::vector<Vec3> vertices;
std::vector<Vec2> textCoords;
std::vector<Vec3> normals;

int main()
{
    std::ifstream file("10370_Bongo_v1_L3.obj");
    if (!file) {
        std::cerr << "Failed to open obj file" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            Vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        } else if (prefix == "vt") {
            Vec2 textCoord;
            iss >> textCoord.u >> textCoord.v;
            textCoords.push_back(textCoord);
        } else if (prefix == "vn") {
            Vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } else if (prefix == "f") {

        }
    }

    file.close();

    std::cout << "Parsed " << vertices.size() << " vertices, "
    << textCoords.size() << " texture coordinates, "
    << normals.size() << " normals.\n";
}
