# Graphic Projects

Small projects and experiments for testing ideas and building quick POCs.

- p1..p30 - play with **raylib** examples from: <https://github.com/raysan5/raylib/tree/master/examples>

## Shell

```sh
nix-shell --run bash
```

## Prerequisites

```sh
# Setup external dependencies
git submodule init
git submodule update

cd external/raylib
cmake -B build && cmake --build build
```

## Build and Run

```sh
# Build p1
cd p1_load_obj
make opt=0 # 0=debug, 1=release

cd $PROJECT_ROOT/p1_load_obj/build

./main.out
```

## Rust setup + run (isn't used yet)

```sh
rustup default stable
rustup component add rust-analyzer # install analyzer

# VSCodium + rust-analyzer plugin
# set in settings.json "rust-analyzer.server.path" path to rust-analyzer executable

cargo run
cargo run --bin=dsa_rust --package=dsa_rust
cargo test
```

## Overview

- p1 - load obj file with png texture and render it
  - <img style="width: 450px" src="./p1_load_obj/image.png" />
- p2 - transition between two animations using per-bone animation blending
  - <img style="width: 450px" src="./p2_models_animation_blend_custom/image.png" />
- p3 - animation blending (transition between animations)
  - <img style="width: 450px" src="./p3_models_animation_blending/image.png" />
- p4 - GPU skinning
  - <img style="width: 450px" src="./p4_models_animation_gpu_skinning/image.png" />
- p5 - animation timing
  - <img style="width: 450px" src="./p5_models_animation_timing/image.png" />
- p6 - simple voxel
  - <img style="width: 450px" src="./p6_models_basic_voxel/image.png" />
- p7 - billboard - logic that decides what is on front/back
  - <img style="width: 450px" src="./p7_models_billboard_rendering/image.png" />
  - <img style="width: 450px" src="./p7_models_billboard_rendering/image copy.png" />
- p8 - model + animation + bone socket (model + external things like weapon or shield)
  - <img style="width: 450px" src="./p8_models_bone_socket/image.png" />
- p9 - collision detection
  - <img style="width: 450px" src="./p9_models_box_collisions/image.png" />

## References

1. <https://stackoverflow.com/questions/63305865/makefile-create-a-build-directory>
1. Git submodules: <https://www.w3schools.com/git/git_submodules.asp>
1. <https://github.com/raysan5/raylib/wiki/Working-with-CMake>
1. glb viewer: <https://sandbox.babylonjs.com/>
1. <https://github.com/raysan5/raygui>
