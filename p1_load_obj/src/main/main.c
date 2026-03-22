#include <raylib.h>
#include <rcamera.h>
#include <stdio.h>

#define model_path "resources/plane/plane.obj"
#define texture_path "resources/plane/plane_diffuse.png"

int main() {
  SetTraceLogLevel(LOG_ALL);
  InitWindow(1280, 720, "Model Loading");

  Model model = LoadModel(model_path);
  Image image = LoadImage(texture_path);
  Texture2D texture = LoadTextureFromImage(image);
  UnloadImage(image);
  printf("Material count=%d\n", model.materialCount);
  for (int i = 0; i < model.materialCount; i++) {
    model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
  }

  Camera cam = {0};
  cam.position = (Vector3){50.0f, 50.0f, 50.0f};
  cam.target = (Vector3){0.0f, 0.0f, 0.0f};
  cam.up = (Vector3){0.0f, 1.0f, 0.0f};
  cam.fovy = 90.f;
  cam.projection = CAMERA_PERSPECTIVE;

  Vector3 pos = {0.0f, 0.0f, 0.0f};
  Vector3 pos2 = {200.0f, 1.0f, 0.0f};
  BoundingBox bounds = GetMeshBoundingBox(model.meshes[0]);

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    UpdateCamera(&cam, CAMERA_THIRD_PERSON);
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(cam);
    DrawModel(model, pos, 1.0f, WHITE);
    DrawModel(model, pos2, 1.0f, WHITE);
    DrawGrid(20, 10.0f);
    DrawBoundingBox(bounds, GREEN);
    EndMode3D();
    DrawText("Loading obj file", 10, GetScreenHeight() - 25, 25, DARKGRAY);
    DrawFPS(10, 10);
    EndDrawing();
  }

  UnloadTexture(texture);
  UnloadModel(model);
  CloseWindow();
  return 0;
}
