#include <cstdio>
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <cstdlib>
#include <terrainChunking.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

static void HandleWindowScreenSize();

int main(void) {

  // stop targeting 60 FPS, so frame rate is uncapped.
  // Before making a new build, enable vsync so it matches the users monitor.
  SetConfigFlags(FLAG_VSYNC_HINT);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - WASD camera + grid");

  Vector3 startPosition = {5475.0f, 300.0f, -5665.0f}; // middle island start pos
  // Vector3 boatPosition = {-3727.55f, -20.0f, 5860.31f}; // set in level not here

  // Define a 3D camera
  Camera3D camera = {0};
  camera.position = (Vector3){startPosition.x, startPosition.y + 1000, startPosition.z}; // Camera position
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};                                           // Where it looks
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};                                               // Up vector
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // SetTargetFPS(60);

  // Enable mouse look
  DisableCursor();
  // ShowCursor();

  Image heightmap = GenImageColor(256, 256, BLACK);

  // Generate procedural heightmap
  for (int y = 0; y < heightmap.height; y++) {
    for (int x = 0; x < heightmap.width; x++) {
      float fx = (float)x / heightmap.width;
      float fy = (float)y / heightmap.height;

      float h =
          sinf(fx * 10.0f) * 0.5f +
          cosf(fy * 10.0f) * 0.5f;

      unsigned char value = (unsigned char)((h + 1.0f) * 0.5f * 255);

      ImageDrawPixel(&heightmap, x, y, (Color){value, value, value, 255});
    }
  }

  Mesh mesh = GenMeshHeightmap(heightmap, (Vector3){50, 10, 50});
  Model model = LoadModelFromMesh(mesh);

  Texture2D texture = LoadTextureFromImage(heightmap);
  model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

  Vector3 terrainScale = {16000.0f, 200.0f, 16000.0f};                         // very large x and z,
  terrain = BuildTerrainGridFromHeightmap(heightmap, terrainScale, 193, true); // 193 bigger chunks less draw calls.

  Shader sh = LoadShader("resources/shaders/lighting_vertex.glsl",
                         "resources/shaders/lighting_fragment.glsl");

  for (auto &c : terrain.chunks) {
    if (c.model.materialCount == 0)
      continue;
    Material &m = c.model.materials[0];
    m.shader = sh;
    printf("1\n");
    // SetMaterialTexture(&m, MATERIAL_MAP_ALBEDO, grass);
    // SetMaterialTexture(&m, MATERIAL_MAP_METALNESS, sand);
    // SetMaterialTexture(&m, MATERIAL_MAP_OCCLUSION, gTreeShadowMask.rt.texture);
  }

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
      HandleWindowScreenSize();
    }

    // Update camera (WASD + mouse)
    UpdateCamera(&camera, CAMERA_FREE);

    BeginDrawing();
    ClearBackground(SKYBLUE);

    BeginMode3D(camera);
    {
      float maxDrawDist = 10.0f; // lowest it can be before terrain popping in is noticable.

      DrawTerrainGrid(terrain, camera, maxDrawDist); // draw the chunks
      // DrawModel(model, (Vector3){-25, 0, -25}, 1.0f, WHITE);
      // Draw a simple grid (size, spacing)
      // DrawGrid(50, 1.0f);
    }
    EndMode3D();

    DrawText("Use WASD to move, mouse to look", 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Camera Pos: (%.1f, %.1f, %.1f)", camera.position.x, camera.position.y, camera.position.z), 10, 40, 10, GRAY);

    DrawText(TextFormat("%d FPS", GetFPS()), 10, 70, 10, DARKGRAY);
    EndDrawing();
  }

  UnloadTexture(texture);
  UnloadModel(model);
  UnloadImage(heightmap);

  CloseWindow();

  return EXIT_SUCCESS;
}

static void HandleWindowScreenSize() {
  const int monitorIndex = GetCurrentMonitor();
  const bool isFullscreen = IsWindowFullscreen();
  const int monitorPosX = GetMonitorPosition(monitorIndex).x;
  const int monitorPosY = GetMonitorPosition(monitorIndex).y;

  printf("isFullscreen=%d, monitorIndex=%d, monitorPosX=%d, monitorPosY=%d\n", isFullscreen, monitorIndex, monitorPosX, monitorPosY);
  if (isFullscreen) {
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  } else {

    // Workaround due to the bug that turns monitor index=1 OFF on Linux - X11
    if (GetMonitorWidth(monitorIndex) == GetScreenWidth() && GetMonitorHeight(monitorIndex) == GetScreenHeight()) {
      ClearWindowState(FLAG_WINDOW_UNDECORATED);
      SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

      return;
    }

    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowSize(GetMonitorWidth(monitorIndex), GetMonitorHeight(monitorIndex));
    SetWindowPosition(monitorPosX, monitorPosY);

    // BUG: turns monitor index=1 OFF on Linux - X11
    // ToggleFullscreen();
  }
}