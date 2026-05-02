#include "raylib.h"
#include <rlgl.h>

#define GRID_SIZE 100
#define SQUARE_SIZE 1.0f

int main(void) {
  // Initialize the window
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "Raylib 100x100 Plane with Separate Models and Wireframes");

  // Camera setup
  Camera3D camera = {0};
  camera.position = (Vector3){5.0f, 5.0f, 5.0f};                 // Camera position
  camera.target = (Vector3){GRID_SIZE / 2.0f, 0.0f, GRID_SIZE / 2.0f}; // Look at center
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;                    // Field of view
  camera.projection = CAMERA_PERSPECTIVE; // Perspective camera

  // Mesh and Model for a single square element (1x1 unit)
  Mesh squareMesh = GenMeshPlane(SQUARE_SIZE, SQUARE_SIZE, 1, 1); // A plane mesh 1x1 unit
  Model squareModel = LoadModelFromMesh(squareMesh);              // Create model from mesh

  // Main game loop
  while (!WindowShouldClose()) {
    // Update the camera
    UpdateCamera(&camera, CAMERA_FREE);

    // Begin drawing
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Begin 3D mode
    BeginMode3D(camera);

    // Enable wireframe mode
    rlEnableWireMode();

    // Draw each square model in the grid with wireframe
    for (int x = 0; x < GRID_SIZE; x++) {
      for (int z = 0; z < GRID_SIZE; z++) {
        Vector3 position = (Vector3){(float)x * SQUARE_SIZE, 0.0f, (float)z * SQUARE_SIZE};
        DrawModel(squareModel, position, 1.0f, GRAY); // Draw model at its position
      }
    }

    // Disable wireframe mode
    rlDisableWireMode();

    // End drawing in 3D mode
    EndMode3D();

    // Draw UI or other 2D elements if needed
    DrawText("3D Plane with Separate Models", 10, 10, 20, DARKGRAY);

    // End drawing
    EndDrawing();
  }

  // Unload resources
  UnloadModel(squareModel); // Unload the square model
  UnloadMesh(squareMesh);   // Unload the square mesh

  // Close the window and OpenGL context
  CloseWindow();

  return 0;
}