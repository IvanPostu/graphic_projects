#ifndef WORLD_GUARD
#define WORLD_GUARD

// #include "raylib.h"

#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <functional>

#include "Atlas.h"
#include "Block.h"
#include "BlockPlaceRequest.h"
#include "BlockType.h"
#include "CameraController.h"
#include "RaycastRequest.h"

#include "NoCopy.h"
#include "PerlinNoise.h"

class Chunk; // forward declaration to avoid circular dependency (effectively:
             // #include "chunk.h")

using std::optional;
using std::pair;
using std::reference_wrapper;
using std::string;
using std::tuple;
using std::unordered_map;
using std::vector;

class World : public NoCopy {
public:
  unordered_map<string, Chunk> chunks;

  vector<reference_wrapper<Chunk>> nearbyChunks;
  vector<reference_wrapper<Chunk>> chunksToRender;

  vector<pair<tuple<int, int, int>, float>> chunksToGenerate;

  bool shouldSortChunksToRender;

  vector<BlockPlaceRequest> blockPlaceRequests;

  std::function<bool(const reference_wrapper<Chunk> &,
                     const reference_wrapper<Chunk> &)>
      compareChunksByDistance;

  World();
  // ~World();

  optional<reference_wrapper<Chunk>> getChunkAt(tuple<int, int, int> chunkPos);
  Block getBlockAt(tuple<int, int, int> chunkPos, int x, int y, int z);

  void generateChunk(PerlinNoise &pn, Atlas &atlas);

  int getHeightAt(PerlinNoise &pn,
                  vector<pair<Vector2, float>> &lowerResolutions, int x, int y);
  void createBlockPlaceRequestAt(Chunk &chunk, int x, int y, int z, Block block,
                                 vector<BlockType> canOverwrite);
  void createChunkData(PerlinNoise &pn, Chunk &chunk);

  void updateChunkModels(Atlas &atlas);

  void dirtyNeighbors(tuple<int, int, int> srcChunk,
                      tuple<int, int, int> srcBlock);

  bool cameraIsSubmerged(const CameraController &cameraController);

  void sortKeysToRender();
  void sortChunksToGenerate();

  void cameraMoved(const CameraController &cameraController, PerlinNoise &pn);

  optional<Vector3>
  handleRaycastRequest(const CameraController &cameraController,
                       RaycastRequest &raycastRequest, Block selectedBlock);
};

#endif