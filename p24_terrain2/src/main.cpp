#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <cmath>
#include <cstdlib>
#include <stdio.h>

int main(void) {

  const int screenWidth = 1200;
  const int screenHeight = 800;

  SetConfigFlags(FLAG_VSYNC_HINT);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  InitWindow(screenWidth, screenHeight, "Demo");
  DisableCursor();

  Camera3D camera = {0}; // init all members to 0 or default

  camera.fovy = 60.f;
  camera.projection = CAMERA_PERSPECTIVE;

  const float terrainWorldWidth = 500.f;
  const float terrainWorldDepth = 500.f;
  const float terrainMaxHeight = 20.f;
  const int terrainImageWidth = 200;  // pixel width of the heightmap image (more = more detail)
  const int terrainImageHeight = 200; // pixel height of the heightmap image (more = more detail)

  Image heightmapImage = GenImageCellular(terrainImageWidth, terrainImageHeight, 16);

  // the vec3 defines {width, max_height, depth}
  Mesh terrainMesh = GenMeshHeightmap(heightmapImage, (Vector3){terrainWorldWidth, terrainMaxHeight, terrainWorldDepth});

  UnloadImage(heightmapImage);

  Model terrainModel = LoadModelFromMesh(terrainMesh);
  terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GRAY; // Give the terrain a simple gray color

  Shader shader = LoadShader("resources/terrain.vs", "resources/terrain.fs");
  terrainModel.materials[0].shader = shader;

  printf("materialCount=%d\n", terrainModel.materialCount);

  // Spaceship Setup (using a simple cube)
  // Create a mesh for a cube. Arguments are width, height, depth.
  Mesh cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f); // A 2x2x2 cube will represent our ship

  // Load the cube mesh into a Model structure.
  Model shipModel = LoadModelFromMesh(cubeMesh); // Using 'shipModel' for clarity

  // Set the ship's color.
  shipModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = MAROON; // A nice maroon color for the ship
  // Alternative: If using a loaded model with its own textures, this might not be needed,
  // or you might use it to tint the existing texture.

  Vector3 shipPosition = {terrainWorldWidth / 2.0f, terrainMaxHeight + 10.0f, terrainWorldDepth / 2.0f};
  float shipYaw = 0.0f;   // Rotation around the ship's local Y-axis (turning left/right). In radians.
  float shipPitch = 0.0f; // Rotation around the ship's local X-axis (looking up/down). In radians.

  const float mouseSensitivity = 0.003f; // How much the ship rotates per pixel of mouse movement. Adjust to taste.
  const float moveSpeed = 25.0f;         // How fast the ship moves in world units per second.

  bool terrainBackfaceCoolingEnabled = true;

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    Vector2 mouseDelta = GetMouseDelta();

    shipYaw -= mouseDelta.x * mouseSensitivity;

    shipPitch -= mouseDelta.y * mouseSensitivity;

    shipPitch = Clamp(shipPitch, -PI / 2.0f + 0.01f, PI / 2.0f - 0.01f);

    Matrix shipRotationMatrix = MatrixMultiply(MatrixRotateX(shipPitch), MatrixRotateY(shipYaw));

    Vector3 shipForward = Vector3Transform({0, 0, 1}, shipRotationMatrix);
    Vector3 shipRight = Vector3Transform({1, 0, 0}, shipRotationMatrix);
    [[maybe_unused]] Vector3 shipUp = Vector3Transform({0, 1, 0}, shipRotationMatrix);

    Vector3 moveDirection = {0}; // Start with no movement

    if (IsKeyDown(KEY_W))
      moveDirection = Vector3Add(moveDirection, shipForward); // Move forward
    if (IsKeyDown(KEY_S))
      moveDirection = Vector3Subtract(moveDirection, shipForward); // Move backward
    if (IsKeyDown(KEY_A))
      moveDirection = Vector3Subtract(moveDirection, shipRight); // Strafe left
    if (IsKeyDown(KEY_D))
      moveDirection = Vector3Add(moveDirection, shipRight); // Strafe right

    if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_E))
      moveDirection.y += 1.0f; // Move up globally
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_Q))
      moveDirection.y -= 1.0f; // Move down globally

    if (IsKeyPressed(KEY_P)) {
      terrainBackfaceCoolingEnabled = !terrainBackfaceCoolingEnabled;
    }

    if (Vector3LengthSqr(moveDirection) > 0.001f) {
      moveDirection = Vector3Normalize(moveDirection);
      shipPosition = Vector3Add(shipPosition, Vector3Scale(moveDirection, moveSpeed * dt));
    }

    // Camera Update (Follow Cam Logic)
    // This creates a third-person camera that follows the ship.
    float cameraDistance = 15.0f;    // How far behind the ship the camera should be.
    float cameraHeightOffset = 5.0f; // How high above the ship's centerline the camera should be.

    Vector3 cameraOffset = Vector3Scale(shipForward, -cameraDistance);
    cameraOffset.y += cameraHeightOffset;

    camera.position = Vector3Add(shipPosition, cameraOffset);
    camera.target = shipPosition;

    camera.up = {0, 1, 0};

    BeginDrawing();           // Marks the beginning of a drawing sequence. Clears buffers.
    ClearBackground(SKYBLUE); // A nice sky blue color
    BeginMode3D(camera);

    // DrawModelWires(terrainModel, Vector3Zero(), 1.0f, LIME); // Draw terrain wireframe

    if (terrainBackfaceCoolingEnabled) {
      DrawModel(terrainModel, Vector3Zero(), 1.0f, WHITE);
    } else {
      rlDisableBackfaceCulling(); // optional, shows hidden edges
      rlEnableWireMode();
      DrawModel(terrainModel, Vector3Zero(), 1.0f, WHITE);
      rlDisableWireMode();
      rlEnableBackfaceCulling();
    }

    shipModel.transform = MatrixMultiply(shipRotationMatrix, MatrixTranslate(shipPosition.x, shipPosition.y, shipPosition.z));
    DrawModel(shipModel, Vector3Zero(), 1.0f, WHITE); // WHITE tint will respect the MAROON material color.

    DrawGrid(100, 10.0f);
    EndMode3D();

    // Draw 2D UI (User Interface) elements on top of the 3D scene
    DrawText("Spaceship Demo - W,A,S,D, Mouse, LMB to Shoot", 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Ship Pos: (%.1f, %.1f, %.1f)", shipPosition.x, shipPosition.y, shipPosition.z), 10, 40, 10, GRAY);
    DrawText(TextFormat("Projectiles: %zu", 0), 10, 60, 10, GRAY);                                                 // %zu is for size_t
    DrawText(TextFormat("Yaw: %.2f Pitch: %.2f (deg)", shipYaw * RAD2DEG, shipPitch * RAD2DEG), 10, 80, 10, GRAY); // Convert radians to degrees
    DrawFPS(screenWidth - 90, 10);                                                                                 // Display current frames per second

    EndDrawing(); // Marks the end of the drawing sequence for this frame.
  }

  UnloadModel(terrainModel);
  UnloadModel(shipModel); // Unload ship model

  CloseWindow();

  return EXIT_SUCCESS;
}
