#include "raylib.h"

#include "include/BlockType.h"

#include "include/Block.h"

Block::Block(const BlockType &blockType) {
  this->blockType = blockType;
  this->transparent = getTransparent(blockType);
  this->solid = getSolid(blockType);
  this->chunkModelInfo = getChunkModelInfo(blockType);
  this->getTexcoords = getTexcoordsFn(blockType);
}