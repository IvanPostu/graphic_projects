// world.h
#pragma once
#include "raylib.h"
#include <vector>
#include "list"

enum class GameState {
  Menu,
  Playing,
  LoadingLevel,
  Quit
};

// Globals or in a FadeController:
enum class FadePhase { Idle,
                       FadingOut,
                       Swapping,
                       FadingIn };

extern Image heightmap;
extern Vector3 terrainScale;

// gobal vars
extern Vector3 boatPosition;
extern Vector3 startPosition;
extern Vector3 playerSpawnPoint;

extern Vector3 waterPos;
extern Vector3 bottomPos;

extern bool controlPlayer;
extern bool isDungeon;
extern float dungeonPlayerHeight;
extern float floorHeight;
extern float wallHeight;
extern unsigned char *heightmapPixels;
extern float vignetteStrengthValue;
extern float bloomStrengthValue;
extern int selectedOption; // 0 = Start, 1 = Quit
extern int levelIndex;
extern int previousLevelIndex;
extern int pendingLevelIndex;
extern float fadeToBlack;
extern float vignetteIntensity;
extern float vignetteFade;
extern int vignetteMode;
extern float boatSpeed;
extern float waterHeightY;
extern float ceilingHeight;
extern bool switchFromMenu;
extern float tileSize;

extern bool first;
extern float dungeonEnemyHeight;
extern float ElapsedTime;
extern bool debugInfo;
extern bool isLoadingLevel;
extern float weaponDarkness;
extern bool drawCeiling;
extern bool unlockEntrances;
extern bool playerInit;
extern float fade;
extern bool hasStaff;
extern bool hasBlunderbuss;
extern bool hasCrossbow;
extern bool hasHarpoon;
extern bool drawCeiling;
extern bool levelLoaded;
extern bool isFullscreen;
extern bool hasIslandNav;
extern int gEnemyCounter;
extern int gCurrentLevelIndex; // for resuming game
extern bool quitQued;
extern float lavaOffsetY;
extern bool enteredDungeon1;
// extern float muzzleFlashTimer;
extern Model oceanModel;
extern GameState currentGameState;
extern FadePhase gFadePhase;

void ClearLevel();
void InitDungeonLights();
void UpdateFade(Camera &camera, float deltaTime);
void removeAllCharacters();
void generateRaptors(int amount, Vector3 centerPos, float radius);
void generateDactyls(int amount, Vector3 centerPos, float radius);
void generateTrex(int amount, Vector3 centerPos, float radius);
// void BeginCustom3D(Camera3D camera, float farClip);
void GenerateEntrances();
void HandleWaves(Camera &camera);
void UpdateEnemies(float deltaTime);
void UpdateNPCs(float deltaTime);
void DrawEnemyShadows();
void UpdateMuzzleFlashes(float deltaTime);
void UpdateBullets(Camera &camera, float deltaTime);
void EraseBullets();
void ApplyWeaponTint(Model &weapon, float darkness);
void HandleWeaponTints();
void UpdateDecals(float deltaTime);
void UpdateCollectables(float deltaTime);
void DrawBullets(Camera &camera);
void DrawBloodParticles(Camera &camera);
void DrawOverworldProps();
void DrawCannons();
void UpdateCannons(float deltaTime);
void EventLockAllDoors(bool lock);
float GetHeightAtWorldPosition(Vector3 position, Image &heightmap, Vector3 terrainScale);
void PlayerSwipeDecal(Camera &camera);
void StartFadeOutToLevel(int levelIndex);
void StartFadeOutFromTeleport();
void StartFadeInFromBlack();
void OpenEventLockedDoors();
void InitOverworldWeapons();
void InitRaftCollectables();
void MovePlayerToFreeCam();
void InitShaders();
void EnterMenu();
void UpdateShadersPerFrame(float deltaTime, float ElapsedTime, Camera &camera);
void InitNPCs();
void eraseCharacters();
void DrawWaterPlane();
void ActivatePowerUp();

void UpdateSlashEffects(float deltaTime);
void UpdateKraken(float deltaTime);
void DrawKraken();
void SpawnTentacle(Vector3 startPos, bool onRight);