#include <SDL2/SDL.h>
#include <iostream>
#include "Vec3.h"
#include <vector>
#include "Face.h"
#include <cmath>
#include "OBJParser.h"
#include "Camera.h"
#include "Quaternion.h"
#include <SDL_ttf.h>
#include <string>


int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed! TTF_Error: " << TTF_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/SFNS.ttf", 18);

    float DEG2RAD = M_PI / 180.0f;

    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << '\n';
        SDL_Quit();
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

    OBJParser::loadOBJ("../tank.obj", vertices, faces);



    
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
    bool increased = false;
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
        const float angleDeg = 1.0f;
        const float angleRad = angleDeg * DEG2RAD;

        if (state[SDL_SCANCODE_UP]) {
            // Pitch up (rotate around X-)
            camera.rotation.applyRotation(Quaternion(-angleRad, Vec3(1, 0, 0)));
            // if (!increased){
            //     camera.rotation.applyRotation(Quaternion(90 * DEG2RAD, Vec3(0, 1, 0)));
            //     increased = true;
            // }
            
        }
        else if (state[SDL_SCANCODE_DOWN]) {
            // Pitch down (rotate around X+)
            camera.rotation.applyRotation(Quaternion(angleRad, Vec3(1, 0, 0)));
        }
        else if (state[SDL_SCANCODE_LEFT]) {
            // Yaw left (rotate around Y-)
            camera.rotation.applyRotation(Quaternion(angleRad, Vec3(0, 1, 0)));
        }
        else if (state[SDL_SCANCODE_RIGHT]) {
            // Yaw right (rotate around Y+)
            camera.rotation.applyRotation(Quaternion(-angleRad, Vec3(0, 1, 0)));
        }
        else if (state[SDL_SCANCODE_R]) {
            // Roll right (rotate around Z-)
            camera.rotation.applyRotation(Quaternion(-angleRad, Vec3(0, 0, 1)));
        }
        else if (state[SDL_SCANCODE_F]) {
            // Roll left (rotate around Z+)
            camera.rotation.applyRotation(Quaternion(angleRad, Vec3(0, 0, 1)));
        }
        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Set render color to white for drawing
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        // Assume camera is available
        std::string posText = "Pos: " + camera.position.toString();
        std::string rotText = "Rot: " + camera.rotation.toString();

        SDL_Color white = {255, 255, 255};

        SDL_Surface* posSurface = TTF_RenderText_Blended(font, posText.c_str(), white);
        SDL_Surface* rotSurface = TTF_RenderText_Blended(font, rotText.c_str(), white);

        SDL_Texture* posTexture = SDL_CreateTextureFromSurface(renderer, posSurface);
        SDL_Texture* rotTexture = SDL_CreateTextureFromSurface(renderer, rotSurface);

        SDL_Rect posRect = {10, 10, posSurface->w, posSurface->h};
        SDL_Rect rotRect = {10, 30 + posSurface->h, rotSurface->w, rotSurface->h};

        // Free surfaces after creating textures
        SDL_FreeSurface(posSurface);
        SDL_FreeSurface(rotSurface);

        // Render the textures
        SDL_RenderCopy(renderer, posTexture, nullptr, &posRect);
        SDL_RenderCopy(renderer, rotTexture, nullptr, &rotRect);

        // Free textures after use (or keep and reuse if static)
        SDL_DestroyTexture(posTexture);
        SDL_DestroyTexture(rotTexture);




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
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
