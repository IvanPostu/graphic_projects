#include <cstdio>
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

static void HandleWindowScreenSize();

int main(void) {

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib - WASD camera + grid");

  // Define a 3D camera
  Camera3D camera = {0};
  camera.position = (Vector3){2.0f, 2.0f, 2.0f}; // Camera position
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};   // Where it looks
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};       // Up vector
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  SetTargetFPS(60);

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

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
      HandleWindowScreenSize();
    }

    // Update camera (WASD + mouse)
    UpdateCamera(&camera, CAMERA_FREE);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
    {
      DrawModel(model, (Vector3){-25, 0, -25}, 1.0f, WHITE);
      // Draw a simple grid (size, spacing)
      DrawGrid(50, 1.0f);
    }
    EndMode3D();

    DrawText("Use WASD to move, mouse to look", 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Camera Pos: (%.1f, %.1f, %.1f)", camera.position.x, camera.position.y, camera.position.z), 10, 40, 10, GRAY);

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