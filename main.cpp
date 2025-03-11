#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

struct Vec3 {
    float x, y, z;
};

struct Face {
    int v1, v2, v3;
};

std::vector<Vec3> vertices;
std::vector<Face> faces;

bool loadOBJ(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            Vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (type == "f") {
            Face face;
            char slash;  // For skipping texture/normal indices (v/vt/vn)
            int ignore;
            iss >> face.v1 >> slash >> ignore >> slash >> ignore;
            iss >> face.v2 >> slash >> ignore >> slash >> ignore;
            iss >> face.v3 >> slash >> ignore >> slash >> ignore;
            face.v1--; face.v2--; face.v3--;  // OBJ indices start at 1, C++ arrays at 0
            faces.push_back(face);
        }
    }

    file.close();
    return true;
}

// Convert 3D to 2D (simple orthographic projection)
Vec3 projectTo2D(Vec3 v, int screenWidth, int screenHeight) {
    float scale = 100.0f;
    return {
        v.x * scale + screenWidth / 2,
        -v.y * scale + screenHeight / 2,
        v.z
    };
}

void drawWireframe(SDL_Renderer* renderer) {
    for (const auto& face : faces) {
        Vec3 v1 = projectTo2D(vertices[face.v1], 800, 600);
        Vec3 v2 = projectTo2D(vertices[face.v2], 800, 600);
        Vec3 v3 = projectTo2D(vertices[face.v3], 800, 600);

        SDL_RenderDrawLine(renderer, v1.x, v1.y, v2.x, v2.y);
        SDL_RenderDrawLine(renderer, v2.x, v2.y, v3.x, v3.y);
        SDL_RenderDrawLine(renderer, v3.x, v3.y, v1.x, v1.y);
    }
}

int main() {
    if (!loadOBJ("10370_Bongo_v1_L3.obj")) {
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("OBJ Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        drawWireframe(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
