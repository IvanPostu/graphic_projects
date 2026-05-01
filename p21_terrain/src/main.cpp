#include <cstdio>
#include <raylib.h>
#include <raymath.h>
#include <cmath>
#include <cstdlib>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

static void HandleWindowScreenSize();

int main(void) {

  SetConfigFlags(FLAG_MSAA_4X_HINT); // optional, smoother edges
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

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
      HandleWindowScreenSize();
    }

    // Update camera (WASD + mouse)
    UpdateCamera(&camera, CAMERA_FREE);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // Draw a simple grid (size, spacing)
    DrawGrid(20, 1.0f);

    EndMode3D();

    DrawText("Use WASD to move, mouse to look", 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Camera Pos: (%.1f, %.1f, %.1f)", camera.position.x, camera.position.y, camera.position.z), 10, 40, 10, GRAY);

    EndDrawing();
  }

  // Close the game window and release all Raylib resources.
  CloseWindow();

  return EXIT_SUCCESS;
}

static void HandleWindowScreenSize() {
  const int monitorIndex = GetCurrentMonitor();
  const bool isFullscreen = IsWindowFullscreen();

  printf("isFullscreen=%d, monitorIndex=%d\n", isFullscreen, monitorIndex);
  if (isFullscreen) {
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  } else {

    // Workaround due to the bug that turns monitor index=1 OFF on Linux - X11
    if (GetMonitorPosition(monitorIndex).x == 0 && GetMonitorPosition(monitorIndex).y == 0 &&
        GetMonitorWidth(monitorIndex) == GetScreenWidth() && GetMonitorHeight(monitorIndex) == GetScreenHeight()) {
      SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
      return;
    }

    SetWindowSize(GetMonitorWidth(monitorIndex), GetMonitorHeight(monitorIndex));
    SetWindowState(FLAG_WINDOW_UNDECORATED);

    SetWindowPosition(GetMonitorPosition(monitorIndex).x,
                      GetMonitorPosition(monitorIndex).y);

    // BUG: turns monitor index=1 OFF on Linux - X11
    // ToggleFullscreen();
  }
}