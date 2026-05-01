#include <raylib.h>
#include <raymath.h>
#include <cmath>
// dynamic array
#include <vector>
// double linked list
#include <list>
#include <cstdlib>

#include <iostream>

typedef struct {
  Vector3 position; // 3d world position
  Vector3 velocity; // direction and speed of the projectile
  float lifetime;   // how many seconds this projectile will exist before
                    // disappearing
  float radius;
  Color color;
  bool active;
} Projectile;

int main(void) {

  const int screenWidth = 1200;
  const int screenHeight = 800;

  InitWindow(screenWidth, screenHeight, "Demo");

  // hide mouse cursor, locks it to the center of the window
  DisableCursor();
  // HideCursor(); // hides cursor but doesn't lock it
  // ShowCursor(); // make it visible again

  Camera3D camera = {0}; // init all members to 0 or default

  // camera.position  will be dynamically updated to follow the ship
  // camera.target will be dynamically set to the ship's position
  // camera.up the up direction for the camera, usually (0,1,0) for Y up

  // camera.fovy field of view on the Y axis, in degrees
  // common values are 45-90. Larger values give a wider "fish-eye" view
  camera.fovy = 60.f;

  // camera.projection CAMERA_PERSPECTIVE gives a realistic 3d view with depth
  // camera.projection CAMERA_ORTHOGRAPHIC is used for 2D games or isometric
  // views (no perspective)

  camera.projection = CAMERA_PERSPECTIVE;

  const float terrainWorldWidth = 500.f;
  const float terrainWorldDepth = 500.f;
  const float terrainMaxHeight = 20.f;
  const int terrainImageWidth = 200;  // pixel width of the heightmap image (more = more detail)
  const int terrainImageHeight = 200; // pixel height of the heightmap image (more = more detail)

  Image heightmapImage = GenImageCellular(terrainImageWidth, terrainImageHeight, 16);
  // Alternative: GetImagePerlinNoise(); // for smoother more organic terrain
  // Alternative:  LoadImage

  // the vec3 defines {width, max_height, depth}
  Mesh terrainMesh = GenMeshHeightmap(heightmapImage, (Vector3){terrainWorldWidth, terrainMaxHeight, terrainWorldDepth});

  UnloadImage(heightmapImage);

  // A Model can hold multiple meshes and materials, making it easier to draw
  Model terrainModel = LoadModelFromMesh(terrainMesh);

  // Set the terrain's appearance. Models have materials, and materials have maps (textures/colors).
  // terrainModel.materials[0] is the default material for meshes loaded this way.
  // MATERIAL_MAP_DIFFUSE (or MATERIAL_MAP_ALBEDO) is the base color map.
  terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = GRAY; // Give the terrain a simple gray color
  // Alternative for textured terrain:
  // Texture2D terrainTex = LoadTexture("path/to/terrain_texture.png");
  // terrainModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = terrainTex;

  // Spaceship Setup (using a simple cube)
  // Create a mesh for a cube. Arguments are width, height, depth.
  Mesh cubeMesh = GenMeshCube(2.0f, 2.0f, 2.0f); // A 2x2x2 cube will represent our ship
  // Alternative: Load your own 3D model
  // Model shipModel = LoadModel("path/to/your/spacecraft.fbx"); // or .obj, .gltf
  // Make sure to adjust projectile spawn points and camera offsets if using a custom model.

  // Load the cube mesh into a Model structure.
  Model shipModel = LoadModelFromMesh(cubeMesh); // Using 'shipModel' for clarity

  // Set the ship's color.
  shipModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].color = MAROON; // A nice maroon color for the ship
  // Alternative: If using a loaded model with its own textures, this might not be needed,
  // or you might use it to tint the existing texture.

  // Ship's initial state variables
  // Start the ship roughly in the center of the terrain, and elevated.
  Vector3 shipPosition = {terrainWorldWidth / 2.0f, terrainMaxHeight + 10.0f, terrainWorldDepth / 2.0f};
  float shipYaw = 0.0f;   // Rotation around the ship's local Y-axis (turning left/right). In radians.
  float shipPitch = 0.0f; // Rotation around the ship's local X-axis (looking up/down). In radians.

  // We are not implementing roll (rotation around Z-axis) in the example for simplicity

  // Control sensitivity and speed
  const float mouseSensitivity = 0.003f; // How much the ship rotates per pixel of mouse movement. Adjust to taste.
  const float moveSpeed = 25.0f;         // How fast the ship moves in world units per second.

  // Projectile System Setup
  // A list to store all active projectiles.
  std::list<Projectile> projectiles;
  // Alternative: std::vector<Projectile> projectiles; // Vectors can also work, but lists are often
  // better for frequent add/remove in the middle.

  const int MAX_PROJECTILES = 50; // An optional cap on how many projectiles can be on screen at once.
  // Helps prevent performance issues if firing too rapidly.

  const float PROJECTILE_SPEED = 70.f;
  const float PROJECTILE_LIFETIME = 3.f;
  const float PROJECTILE_RADIUS = 0.2f;
  const Color PROJECTILE_COLOR = ORANGE;

  float shootCooldown = 0.f;
  const float SHOOT_RATE = 0.15f;
  // 1.0f / SHOOT_RATE = shots per second 1/0.15 = ~6.6SPS

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    // Get how much mouse has moved since last frame
    Vector2 mouseDelta = GetMouseDelta();

    // Update ship's yaw (left/right rotation) based on horizontal mouse movement.
    // Subtract because typically moving mouse right increases X, which we map to clockwise yaw.
    shipYaw -= mouseDelta.x * mouseSensitivity;

    // Update ship's pitch (up/down rotation) based on vertical mouse movement.
    // Subtract because typically moving mouse up decreases Y (screen coordinates),
    // which we map to pitching up.
    shipPitch -= mouseDelta.y * mouseSensitivity;

    // Clamp the pitch to prevent the ship from flipping upside down.
    // Angles are in radians. PI/2 radians is 90 degrees.
    // We clamp slightly less than 90 degrees to avoid gimbal lock issues or visual glitches.
    shipPitch = Clamp(shipPitch, -PI / 2.0f + 0.01f, PI / 2.0f - 0.01f);
    // Note: PI is defined in raymath.h.

    // Calculate Ship's Orientation Vectors (Forward, Right, Up)
    // These vectors define the ship's local coordinate system in the world.

    // Create a rotation matrix based on the current pitch and yaw.
    // The order of rotations matters:
    // 1. Rotate around Y-axis for yaw.
    // 2. Rotate around the NEW X-axis for pitch
    // MatrixMultiply combines these transformations.
    Matrix shipRotationMatrix = MatrixMultiply(MatrixRotateX(shipPitch), MatrixRotateY(shipYaw));

    // Alternative if roll was included:
    // Matrix shipRotationMatrix = MatrixMultiply(MatrixRotateZ(shipRoll), MatrixMultiply(MatrixRotateX(shipPitch), MatrixRotateY(shipYaw)));

    // Calculate the ship's forward direction in world space.
    // (0, 0, 1) is typically forward in model space (along positive Z).
    // Transform this local forward vector by the ship's rotation matrix.
    Vector3 shipForward = Vector3Transform({0, 0, 1}, shipRotationMatrix);

    // Calculate the ship's right direction in world space.
    // (1, 0, 0) is typically right in model space (along positive X).
    Vector3 shipRight = Vector3Transform({1, 0, 0}, shipRotationMatrix);

    // Calculate the ship's up direction in world space.
    // (0, 1, 0) is typically up in model space (along positive Y).
    [[maybe_unused]] Vector3 shipUp = Vector3Transform({0, 1, 0}, shipRotationMatrix);
    // Alternative, more robust if roll is complex, derive from cross product:
    // shipUp = Vector3Normalize(Vector3CrossProduct(shipRight, shipForward));

    // Ship Control: Movement (Position)
    // Initialize movement direction vector for this frame.
    Vector3 moveDirection = {0}; // Start with no movement

    // Check for movement keys and add to moveDirection along ship's local axes.
    if (IsKeyDown(KEY_W))
      moveDirection = Vector3Add(moveDirection, shipForward); // Move forward
    if (IsKeyDown(KEY_S))
      moveDirection = Vector3Subtract(moveDirection, shipForward); // Move backward
    if (IsKeyDown(KEY_A))
      moveDirection = Vector3Subtract(moveDirection, shipRight); // Strafe left
    if (IsKeyDown(KEY_D))
      moveDirection = Vector3Add(moveDirection, shipRight); // Strafe right

    // Vertical movement (optional, makes it more like a spacecraft)
    // These keys control movement along the GLOBAL Y-axis.
    if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_E))
      moveDirection.y += 1.0f; // Move up globally
    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_Q))
      moveDirection.y -= 1.0f; // Move down globally
    // Alternative for local vertical movement (along ship's up vector):
    // if (IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_E)) moveDirection = Vector3Add(moveDirection, shipUp);
    // if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_Q)) moveDirection = Vector3Subtract(moveDirection, shipUp);

    // If there's any movement input, normalize the direction and scale by speed and delta time.
    // Vector3LengthSqr is used for efficiency (avoids square root) if only checking for non-zero.
    if (Vector3LengthSqr(moveDirection) > 0.001f) {
      // Normalize to get a unit vector (length 1), so speed is consistent regardless of direction.
      moveDirection = Vector3Normalize(moveDirection);
      // Update ship's position: current_pos + direction * speed * time_delta
      shipPosition = Vector3Add(shipPosition, Vector3Scale(moveDirection, moveSpeed * dt));
    }

    // Shooting Projectiles
    // Decrease shoot cooldown timer if it's active.
    if (shootCooldown > 0)
      shootCooldown -= dt;

    // Check if the left mouse button is pressed AND the cooldown has expired.
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && shootCooldown <= 0) {
      // Optional: Check if we haven't exceeded the maximum number of projectiles.
      if (projectiles.size() < MAX_PROJECTILES) {
        Projectile newProjectile; // Create a new projectile instance

        // Set projectile's starting position:
        // Start it slightly in front of the ship's "nose".
        // The ship model is a 2x2x2 cube, so its center is its origin.
        // Its "front" face is 1 unit away along its local Z (shipForward).
        // We use 1.1f to ensure it spawns just outside the ship model.
        newProjectile.position = Vector3Add(shipPosition, Vector3Scale(shipForward, 1.1f));
        // If using a custom model, adjust "1.1f" based on its size and pivot point.

        // Set projectile's velocity:
        // It moves in the direction the ship is facing (shipForward) at PROJECTILE_SPEED.
        newProjectile.velocity = Vector3Scale(shipForward, PROJECTILE_SPEED);

        // Set other projectile properties.
        newProjectile.lifetime = PROJECTILE_LIFETIME;
        newProjectile.radius = PROJECTILE_RADIUS;
        newProjectile.color = PROJECTILE_COLOR;
        newProjectile.active = true; // Mark it as active

        // Add the new projectile to our list of active projectiles.
        projectiles.push_back(newProjectile);

        // Reset the shoot cooldown timer.
        shootCooldown = SHOOT_RATE;
      }
    }

    // Update Projectiles
    // Iterate through all projectiles in the list.
    // The loop structure 'for (auto it = ...; ...; /* manual increment */)' is used
    // because we might remove elements ('projectiles.erase(it)'), which invalidates
    // standard iterators if not handled carefully.
    for (auto it = projectiles.begin(); it != projectiles.end(); /* manual increment below */) {
      if (it->active) // Process only active projectiles
      {
        // Update position based on velocity and delta time.
        it->position = Vector3Add(it->position, Vector3Scale(it->velocity, dt));

        // Decrease lifetime.
        it->lifetime -= dt;

        // If lifetime runs out, mark projectile as inactive.
        if (it->lifetime <= 0) {
          it->active = false;
        }

        // TODO: Add projectile collision detection here (e.g., with terrain or enemies)
        // if a collision occurs, you might set it->active = false;
      }

      if (!it->active) {
        // removes element and returns an iterator to the next element
        it = projectiles.erase(it);
      } else {
        it++;
      }
    }

    // Camera Update (Follow Cam Logic)
    // This creates a third-person camera that follows the ship.
    float cameraDistance = 15.0f;    // How far behind the ship the camera should be.
    float cameraHeightOffset = 5.0f; // How high above the ship's centerline the camera should be.

    // Calculate the desired offset from the ship's position.
    // Start by going backward from the ship along its forward vector.
    Vector3 cameraOffset = Vector3Scale(shipForward, -cameraDistance);
    // Then, add some height (global Y-axis).
    cameraOffset.y += cameraHeightOffset;
    // Alternative: Add height along ship's local up vector for a camera that tilts with the ship
    // cameraOffset = Vector3Add(cameraOffset, Vector3Scale(shipUp, cameraHeightOffset));

    // Set the camera's new position.
    camera.position = Vector3Add(shipPosition, cameraOffset);

    // Make the camera look at the ship's current position.
    camera.target = shipPosition;

    // Set the camera's "up" direction.
    // Using global up (0,1,0) provides a stable horizon.
    camera.up = {0, 1, 0};
    // Alternative: Use ship's local up vector for a camera that rolls with the ship.
    // This can be more immersive for flight sims but also more disorienting.
    // camera.up = shipUp;

    // Draw everything
    BeginDrawing(); // Marks the beginning of a drawing sequence. Clears buffers.

    // Set the background color for this frame.
    ClearBackground(SKYBLUE); // A nice sky blue color

    // Enter 3D mode. All drawing functions between BeginMode3D and EndMode3D
    // will be rendered using the specified camera's perspective.
    BeginMode3D(camera);

    // Draw Terrain
    // The current order draws wires first, then the solid model.
    // This might result in the solid model mostly obscuring the wires,
    // unless there's Z-fighting or the wireframe is slightly offset/larger.

    DrawModelWires(terrainModel, Vector3Zero(), 1.0f, LIME); // Draw terrain wireframe
    DrawModel(terrainModel, Vector3Zero(), 1.0f, WHITE);     // Draw solid terrain (WHITE tint doesn't override material color, just multiplies)

    // Alternative: Draw solid then wireframe for an overlay effect
    // DrawModel(terrainModel, Vector3Zero(), 1.0f, WHITE);
    // DrawModelWires(terrainModel, Vector3Zero(), 1.0f, DARKGRAY); // Darker wireframe for overlay

    // Draw Ship
    // To correctly orient and position the ship model using its pitch and yaw:
    // 1. Create a rotation matrix (shipRotationMatrix, already calculated in Update).
    // 2. Create a translation matrix for the ship's world position.
    // 3. Multiply these to get the final transformation matrix for the model.
    // The model's transform field stores this matrix.
    // Order for model transform: Scale -> Rotate -> Translate
    // Since we're not scaling the model here, it's Rotate -> Translate.
    shipModel.transform = MatrixMultiply(shipRotationMatrix, MatrixTranslate(shipPosition.x, shipPosition.y, shipPosition.z));

    // Draw the ship model.
    // Since its transform is set, we draw it at world origin (0,0,0) with scale 1.0.
    // The 'transform' matrix handles its actual position and orientation.
    DrawModel(shipModel, Vector3Zero(), 1.0f, WHITE); // WHITE tint will respect the MAROON material color.

    // Draw Projectiles
    // Iterate through all active projectiles and draw them.
    for (const auto &proj : projectiles) // Use a const reference for read-only access
    {
      if (proj.active) {
        // Draw each projectile as a simple sphere.
        DrawSphere(proj.position, proj.radius, proj.color);
        // Alternative: DrawBillboard(...) for a 2D sprite that always faces the camera.
        // Alternative: DrawModel(...) if you have a 3D model for projectiles.
      }
    }

    // Draw Helper Grid
    // Draws a 3D grid on the XZ plane for spatial reference.
    // Arguments: number of slices, spacing between slices.
    DrawGrid(100, 10.0f);

    EndMode3D();

    // Draw 2D UI (User Interface) elements on top of the 3D scene
    DrawText("Spaceship Demo - W,A,S,D, Mouse, LMB to Shoot", 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Ship Pos: (%.1f, %.1f, %.1f)", shipPosition.x, shipPosition.y, shipPosition.z), 10, 40, 10, GRAY);
    DrawText(TextFormat("Projectiles: %zu", projectiles.size()), 10, 60, 10, GRAY);                                // %zu is for size_t
    DrawText(TextFormat("Yaw: %.2f Pitch: %.2f (deg)", shipYaw * RAD2DEG, shipPitch * RAD2DEG), 10, 80, 10, GRAY); // Convert radians to degrees
    DrawFPS(screenWidth - 90, 10);                                                                                 // Display current frames per second

    EndDrawing(); // Marks the end of the drawing sequence for this frame.
  }

  // Cleanup
  // Unload models and other resources to free up memory.
  // Models loaded with LoadModelFromMesh also unload their meshes by default.
  UnloadModel(terrainModel);
  UnloadModel(shipModel); // Unload ship model
  // If you loaded textures manually: UnloadTexture(yourTexture);
  // If you loaded sounds: UnloadSound(yourSound); CloseAudioDevice();

  // Close the game window and release all Raylib resources.
  CloseWindow();

  return EXIT_SUCCESS;
}
