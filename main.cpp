#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Face {
    std::vector<int> vertexIndices;
    std::vector<int> texCoordIndices;
    std::vector<int> normalIndices;
};

int main() {
    std::ifstream file("10370_Bongo_v1_L3.obj");
    if (!file) {
        std::cerr << "Failed to open obj file" << std::endl;
        return 1;
    }

    std::string line;
    std::vector<Face> faces;  // Vector to store parsed faces
    std::vector<std::string> vertices;   // Store vertices (v)
    std::vector<std::string> texCoords;  // Store texture coords (vt)
    std::vector<std::string> normals;    // Store normals (vn)

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // Read vertex data
            std::string vertex;
            iss >> vertex;
            vertices.push_back(vertex);  // Store the vertex position
        } else if (prefix == "vt") {
            // Read texture coordinate data
            std::string texCoord;
            iss >> texCoord;
            texCoords.push_back(texCoord);  // Store the texture coordinate
        } else if (prefix == "vn") {
            // Read normal data
            std::string normal;
            iss >> normal;
            normals.push_back(normal);  // Store the normal vector
        } else if (prefix == "f") {
            // Parse faces
            Face face;
            std::string vertexData;
            while (iss >> vertexData) {
                std::istringstream vertexStream(vertexData);
                std::string vertexIndex, texCoordIndex, normalIndex;
                
                // Split by '/' and store the respective indices
                std::getline(vertexStream, vertexIndex, '/');
                std::getline(vertexStream, texCoordIndex, '/');
                std::getline(vertexStream, normalIndex, '/');
                
                // Store the parsed indices
                face.vertexIndices.push_back(std::stoi(vertexIndex));
                
                // If texture coordinate exists, store it; otherwise, store -1
                if (!texCoordIndex.empty()) {
                    face.texCoordIndices.push_back(std::stoi(texCoordIndex));
                } else {
                    face.texCoordIndices.push_back(-1);  // No texture coordinates
                }
                
                // If normal exists, store it; otherwise, store -1
                if (!normalIndex.empty()) {
                    face.normalIndices.push_back(std::stoi(normalIndex));
                } else {
                    face.normalIndices.push_back(-1);  // No normal
                }
            }

            faces.push_back(face);  // Store the parsed face
        }
    }

    file.close();

    // Output to verify parsing
    for (const auto& face : faces) {
        std::cout << "Face with " << face.vertexIndices.size() << " vertices:" << std::endl;
        for (size_t i = 0; i < face.vertexIndices.size(); ++i) {
            std::cout << "Vertex: " << face.vertexIndices[i] << ", ";
            if (face.texCoordIndices[i] != -1)
                std::cout << "TexCoord: " << face.texCoordIndices[i] << ", ";
            if (face.normalIndices[i] != -1)
                std::cout << "Normal: " << face.normalIndices[i];
            std::cout << std::endl;
        }
    }

    return 0;
}
