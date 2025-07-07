#include <SDL2/SDL.h>
#include <iostream>
#include "Vec3.h"
#include <vector>
#include "Face.h"
#include <cmath>
#include "OBJParser.h"
#include "Camera.h"


int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Simple Graphics Window", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        1600, 
        900, 
        SDL_WINDOW_SHOWN
    );
    
    if (window == NULL) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    Camera camera = Camera();
    camera.position.z = 50.0f;

    float FOV = 3.14159f / 2.0f;
    float aspect = 16.0f / 9.0f ;
    float f = std::tan(FOV/2.0f);
    float near = 1.0f;
    float far = 250.0f;



    std::vector<std::vector<float>> projectionMatrix(4, std::vector<float>(4, 0.0f));

    projectionMatrix[0][0] = (1/  ( f * aspect ));
    projectionMatrix[1][1] = (1 / f);
    projectionMatrix[2][2] = (far + near) / (near - far);
    projectionMatrix[2][3] = (2 * far * near) / (near - far);
    projectionMatrix[3][2] = -1.0f;

    // std::vector<Vec3> cubeVertices = {
    //     Vec3( 1.0f,  1.0f, -4.0f, 1.0f),
    //     Vec3(-1.0f,  1.0f, -4.0f, 1.0f),
    //     Vec3(-1.0f, -1.0f, -4.0f, 1.0f),
    //     Vec3( 1.0f, -1.0f, -4.0f, 1.0f),
    //     Vec3( 1.0f,  1.0f, -6.0f, 1.0f),
    //     Vec3(-1.0f,  1.0f, -6.0f, 1.0f),
    //     Vec3(-1.0f, -1.0f, -6.0f, 1.0f),
    //     Vec3( 1.0f, -1.0f, -6.0f, 1.0f)
    // };


    // std::vector<Face> faces = {
    // Face({0, 1, 2}), Face({0, 2, 3}),   // front face
    // Face({4, 5, 6}), Face({4, 6, 7}),   // back face
    // Face({0, 4, 7}), Face({0, 7, 3}),   // right face
    // Face({1, 5, 6}), Face({1, 6, 2}),   // left face
    // Face({0, 1, 5}), Face({0, 5, 4}),   // top face
    // Face({3, 2, 6}), Face({3, 6, 7})    // bottom face
    // };

    std::vector<Vec3> vertices;
    std::vector<Face> faces;

    OBJParser::loadOBJ("tank.obj", vertices, faces);



    
    // Set render color to black (background)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Set render color to white for drawing
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for (const Face& face : faces) {

        Vec3 normal = (vertices[face.vecIndices[1]] - vertices[face.vecIndices[0]]) ^ (vertices[face.vecIndices[2]] - vertices[face.vecIndices[0]]);
        
        // Calculate face center (average of vertices)
        Vec3 faceCenter = (vertices[face.vecIndices[0]] + vertices[face.vecIndices[1]] + vertices[face.vecIndices[2]]) * (1.0f/3.0f);
        
        // Vector from camera to face center
        Vec3 cameraToFace = faceCenter - camera.position;
        
        bool isVisible = normal * cameraToFace < 0;
        if (isVisible) {
            face.draw(vertices, projectionMatrix, renderer, camera);
        }
    }
    
    // Update screen
    SDL_RenderPresent(renderer);
    
    // Event loop
    SDL_Event event;
    bool quit = false;
    
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }   
        }
        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_W]) {
            camera.moveForward(0.1f);
        }
        else if (state[SDL_SCANCODE_S]) {
            camera.moveForward(-0.1f);
        }
        else if (state[SDL_SCANCODE_A]) {
            camera.moveRight(-0.1f);            
        }
        else if (state[SDL_SCANCODE_D]) {
            camera.moveRight(0.1f);
        }
        else if (state[SDL_SCANCODE_Q]) {
            camera.moveUp(-0.1f);
        }
        else if (state[SDL_SCANCODE_E]) {
            camera.moveUp(0.1f);
        }
        if (state[SDL_SCANCODE_UP]) {
            camera.rotation.x += 0.01f;
        }
        else if (state[SDL_SCANCODE_DOWN]) {
            camera.rotation.x -= 0.01f;
        }
        else if (state[SDL_SCANCODE_LEFT]) {
            camera.rotation.y += 0.01f;
        }
        else if (state[SDL_SCANCODE_RIGHT]) {
            camera.rotation.y -= 0.01f;
        }
        else if (state[SDL_SCANCODE_R]) {
            camera.rotation.z += 0.01f;
        }
        else if (state[SDL_SCANCODE_F]) {
            camera.rotation.z -= 0.01f;
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Set render color to white for drawing
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Draw the faces
        for (const Face& face : faces) {
            Vec3 normal = (vertices[face.vecIndices[1]] - vertices[face.vecIndices[0]]) ^ (vertices[face.vecIndices[2]] - vertices[face.vecIndices[0]]);
            
            // Calculate face center (average of vertices)
            Vec3 faceCenter = (vertices[face.vecIndices[0]] + vertices[face.vecIndices[1]] + vertices[face.vecIndices[2]]) * (1.0f/3.0f);
            
            // Vector from camera to face center
            Vec3 cameraToFace = faceCenter - camera.position;
            
            bool isVisible = normal * cameraToFace < 0;
            if (isVisible) {
                face.draw(vertices, projectionMatrix, renderer, camera);
            }
        }
    
        // Update screen
        SDL_RenderPresent(renderer);
    }
    
    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
