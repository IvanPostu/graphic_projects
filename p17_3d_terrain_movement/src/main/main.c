#include "raylib.h"
#include "raymath.h"

// Initialization
//--------------------------------------------------------------------------------------
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;

// Function to create a tiled texture by repeating an image X times
Texture2D CreateTiledTexture(Texture2D sourceTexture, int tilesX, int tilesY) {
  // Get the source image from the texture
  Image sourceImage = LoadImageFromTexture(sourceTexture);

  // Calculate new dimensions
  int newWidth = sourceImage.width * tilesX;
  int newHeight = sourceImage.height * tilesY;

  // Create a new image with the tiled dimensions
  Image tiledImage = GenImageColor(newWidth, newHeight, WHITE);

  // Tile the source image across the new image
  for (int y = 0; y < tilesY; y++) {
    for (int x = 0; x < tilesX; x++) {
      Rectangle destRec = {x * sourceImage.width, y * sourceImage.height,
                           sourceImage.width, sourceImage.height};
      ImageDraw(&tiledImage, sourceImage,
                (Rectangle){0, 0, sourceImage.width, sourceImage.height},
                destRec, WHITE);
    }
  }

  // Create texture from the tiled image
  Texture2D tiledTexture = LoadTextureFromImage(tiledImage);

  // Clean up temporary images
  UnloadImage(sourceImage);
  UnloadImage(tiledImage);

  return tiledTexture;
}

// Function to get height from image at a specific world position
float GetHeightFromImage(Image image, Vector3 worldPos, Vector3 mapSize,
                         Vector3 mapPos) {
  // Convert world coordinates to image coordinates
  float u = (worldPos.x - mapPos.x) / mapSize.x; // Normalize x to [0, 1]
  float v = (worldPos.z - mapPos.z) / mapSize.z; // Normalize z to [0, 1]

  // Ensure u, v are within [0, 1]
  if (u < 0.0f || u > 1.0f || v < 0.0f || v > 1.0f)
    return 0.0f;

  // Map to image pixel coordinates
  int pixelX = (int)(u * image.width);
  int pixelY = (int)(v * image.height);

  // Get pixel color (grayscale height value)
  Color pixel = GetImageColor(image, pixelX, pixelY);

  // Debug purposes
  // printf(">>>>>>>>> %i %i\n", pixelX, pixelY);

  // Convert grayscale value (0-255) to height (assuming grayscale image)
  // Scale it to match the heightmap mesh's y-scale (8.0f in GenMeshHeightmap)
  float height = (pixel.r / 255.0f) * mapSize.y;

  // Debug purposes
  // printf(">>>>>>>>>> %i\n", height);

  return height;
}

static void HandleWindowScreenSize(void);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {

  SetConfigFlags(FLAG_VSYNC_HINT);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
             "raylib [models] example - heightmap loading and drawing");
  Quaternion characterRotate =
      QuaternionFromAxisAngle((Vector3){0.0f, 0.0f, 0.0f}, 0.0f);

  // Define our custom camera to look into our 3d world
  Camera camera = {0};
  camera.position = (Vector3){0.5f, 1.5f, -0.5f}; // Camera position
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};    // Camera looking at point
  camera.up =
      (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;                    // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE; // Camera

  Shader lightingShader = LoadShader("resources/shaders/glsl330/lighting.vs",
                                     "resources/shaders/glsl330/lighting.fs");

  int animsCount = 0;
  unsigned int animIndex = 0;
  unsigned int animCurrentFrame = 0;
  Model characterModel = LoadModel("resources/char.glb");

  characterModel.materials[0].shader = lightingShader;

  ModelAnimation *modelAnimations =
      LoadModelAnimations("resources/char.glb", &animsCount);
  Vector3 characterSize = {0.5f, 0.5f, 0.5f};

  Model houseModel = LoadModel("resources/house.glb");

  houseModel.materials[0].shader = lightingShader;

  Vector3 housePosition = {0.5f, 0.03f, 0.93f};
  Vector3 houseSize = {0.005f, 0.005f, 0.005f};

  Image image = LoadImage("resources/map-01.png"); // Load heightmap image (RAM)
  Texture2D texture =
      LoadTextureFromImage(image); // Convert image to texture (VRAM)

  Texture2D terrainTexture01 =
      LoadTexture("resources/map-01-texture-01.png"); // Load base texture
  Texture2D tiledTerrainTexture =
      CreateTiledTexture(terrainTexture01, 16, 16); // Create XxX tiled version
  UnloadTexture(
      terrainTexture01); // Unload original since we're using the tiled version

  Vector3 mapSize = {16.0f, 8.0f, 16.0f}; // Define heightmap size (x, y, z)
  Mesh mesh = GenMeshHeightmap(
      image, mapSize);                   // Generate heightmap mesh (RAM and VRAM)
  Model model = LoadModelFromMesh(mesh); // Load model from generated mesh

  SetMaterialTexture(&model.materials[0], MATERIAL_MAP_DIFFUSE,
                     tiledTerrainTexture);

  model.materials[0].shader = lightingShader;

  Vector3 mapPosition = {-8.0f, 0.0f, -8.0f}; // Define model position

  Vector3 walkingPointSize = {0.2f, 0.2f, 0.2f};
  Vector3 walkingPointPosition = {0.0f, 1.0f, 2.0f};
  Vector3 walkingPointIntentPosition = {0.0f, 1.0f, 2.0f};

  Vector3 sunDirection = Vector3Normalize(
      (Vector3){0.3f, -1.0f, 0.4f});         // Sun coming from upper right
  Vector3 sunColor = {1.0f, 0.95f, 0.8f};    // Warm sunlight color
  Vector3 ambientColor = {0.3f, 0.3f, 0.4f}; // Cooler ambient light

  // Get shader uniform locations
  int lightDirLoc = GetShaderLocation(lightingShader, "lightDir");
  int lightColorLoc = GetShaderLocation(lightingShader, "lightColor");
  int ambientColorLoc = GetShaderLocation(lightingShader, "ambientColor");

  // Set sun lighting uniforms (only need to do this once)
  SetShaderValue(lightingShader, lightDirLoc, &sunDirection,
                 SHADER_UNIFORM_VEC3);
  SetShaderValue(lightingShader, lightColorLoc, &sunColor, SHADER_UNIFORM_VEC3);
  SetShaderValue(lightingShader, ambientColorLoc, &ambientColor,
                 SHADER_UNIFORM_VEC3);

  float characterRotationAngle =
      0.0f;                                           // Add this variable to track rotation angle
  Vector3 characterRotationAxis = {0.0f, 1.0f, 0.0f}; // Y-axis rotation

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT))) {
      HandleWindowScreenSize();
    }

    camera.position.x = walkingPointPosition.x;
    camera.position.y = walkingPointPosition.y + 1.5f;
    camera.position.z = walkingPointPosition.z + 1.50f;
    camera.target.x = walkingPointPosition.x;
    camera.target.y = walkingPointPosition.y;
    camera.target.z = walkingPointPosition.z;
    walkingPointPosition.y =
        GetHeightFromImage(image, walkingPointPosition, mapSize, mapPosition) +
        0.05f;
    walkingPointIntentPosition.x = walkingPointPosition.x;
    walkingPointIntentPosition.y = walkingPointPosition.y;
    walkingPointIntentPosition.z = walkingPointPosition.z;
    bool moving = false;
    bool collided = false;
    Vector3 movementDirection = {0.0f, 0.0f, 0.0f};

    if (IsKeyDown(KEY_RIGHT)) {
      walkingPointIntentPosition.x += 0.01f;
      movementDirection.z = -1.0f;
      characterRotationAngle = 270.0f;
      moving = true;
    } else if (IsKeyDown(KEY_LEFT)) {
      walkingPointIntentPosition.x -= 0.01f;
      movementDirection.z = -1.0f;
      characterRotationAngle = 90.0f;
      moving = true;
    } else if (IsKeyDown(KEY_DOWN)) {
      walkingPointIntentPosition.z += 0.01f;
      movementDirection.z = -1.0f;
      characterRotationAngle = 180.0f;
      moving = true;
    } else if (IsKeyDown(KEY_UP)) {
      walkingPointIntentPosition.z -= 0.01f;
      movementDirection.z = -1.0f;
      characterRotationAngle = 0.0f;
      moving = true;
    }

    ModelAnimation anim = modelAnimations[animIndex];
    if (moving) {
      // Box collision check
      if (CheckCollisionBoxes(
              (BoundingBox){
                  (Vector3){walkingPointIntentPosition.x - characterSize.x / 2,
                            walkingPointIntentPosition.y - characterSize.y / 2,
                            walkingPointIntentPosition.z - characterSize.z / 2},
                  (Vector3){walkingPointIntentPosition.x + characterSize.x / 2,
                            walkingPointIntentPosition.y + characterSize.y / 2,
                            walkingPointIntentPosition.z +
                                characterSize.z / 2}},
              (BoundingBox){(Vector3){housePosition.x - houseSize.x / 2,
                                      housePosition.y - houseSize.y / 2,
                                      housePosition.z - houseSize.z / 2},
                            (Vector3){housePosition.x + houseSize.x / 2,
                                      housePosition.y + houseSize.y / 2,
                                      housePosition.z + houseSize.z / 2}}))
        collided = true;

      animCurrentFrame = (animCurrentFrame + 3) % anim.keyframeCount;
      UpdateModelAnimation(characterModel, anim, animCurrentFrame);
      if (!collided) {
        walkingPointPosition.x = walkingPointIntentPosition.x;
        walkingPointPosition.y = walkingPointIntentPosition.y;
        walkingPointPosition.z = walkingPointIntentPosition.z;
      }
    }

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    DrawModel(model, mapPosition, 1.0f, WHITE);

    // DrawGrid(20, 1.0f);

    // DrawCube(walkingPointPosition, walkingPointSize.x, walkingPointSize.y,
    // walkingPointSize.z, GRAY);
    // UpdateModelAnimation(characterModel, anim, animCurrentFrame);

    // Set the character position using transform matrix
    // ßcharacterModel.transform = MatrixTranslate(walkingPointPosition.x,
    // walkingPointPosition.y, walkingPointPosition.z);

    // Draw the character model at the walking point position
    DrawModelEx(characterModel, walkingPointPosition, characterRotationAxis,
                characterRotationAngle, (Vector3){0.03f, 0.03f, 0.03f}, WHITE);

    // House model
    DrawModelEx(houseModel, housePosition, characterRotationAxis, 0.0f,
                (Vector3){0.05f, 0.05f, 0.05f}, WHITE);

    DrawLine3D((Vector3){0, 0, 0}, (Vector3){9999, 0, 0}, RED);   // X-axis (Red)
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 9999, 0}, GREEN); // Y-axis (Green)
    DrawLine3D((Vector3){0, 0, 0}, (Vector3){0, 0, 9999}, BLUE);  // Z-axis (Blue)
    EndMode3D();

    // DrawFPS(10, 10);

    DrawRectangle(10, 5, 195, 120, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(10, 5, 195, 120, BLUE);

    DrawText("Camera status:", 20, 15, 10, BLACK);
    DrawText(TextFormat("- Projection: %s", (camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" : (camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC"
                                                                                                                                                   : "CUSTOM"),
             20, 45, 10, BLACK);
    DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", camera.position.x, camera.position.y, camera.position.z), 20, 60, 10, BLACK);
    DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 20, 75, 10, BLACK);
    DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", camera.up.x, camera.up.y, camera.up.z), 20, 90, 10, BLACK);
    DrawText(TextFormat("%d FPS", GetFPS()), 20, 105, 10, DARKGRAY);

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  UnloadImage(
      image);                         // Unload heightmap image from RAM, already uploaded to VRAM
  UnloadTexture(texture);             // Unload texture
  UnloadTexture(tiledTerrainTexture); // Unload tiled terrain texture
  UnloadModel(model);                 // Unload model
  UnloadModel(houseModel);

  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

static void HandleWindowScreenSize(void) {
  const int monitorIndex = GetCurrentMonitor();
  const bool isFullscreen = IsWindowFullscreen();
  const int monitorPosX = GetMonitorPosition(monitorIndex).x;
  const int monitorPosY = GetMonitorPosition(monitorIndex).y;

  if (isFullscreen) {
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  } else {
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowSize(GetMonitorWidth(monitorIndex), GetMonitorHeight(monitorIndex));
    SetWindowPosition(monitorPosX, monitorPosY);
  }
}
