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

#  new Makefile
make -f ./p18_minecraft_cpp/Makefile print
make -f ./p18_minecraft_cpp/Makefile buildAndRun

# cmake
cd p19_game1
# first run will take some time
cmake -S . -B build
cmake --build build -- -j$(nproc)

# cmake2
cd ogldev/tutorial1
cmake -S . -B build
cmake --build build
./build/tutorial1

# better Makefile config: playground.core.test
make clean all BUILD_TYPE=release
make clean all BUILD_TYPE=debug
# "run" is linux only
make clean all BUILD_TYPE=debug run
```

## Build and Run

```sh
# Build p1
cd p1_load_obj
make opt=0 # 0=debug, 1=release

cd $PROJECT_ROOT/p1_load_obj/build

./main.out
```

## Rust setup + run

```sh
# on Windows via git bash: https://rust-lang.org/tools/install/

rustup default stable
rustup component add rust-analyzer # install analyzer

# VSCodium + rust-analyzer plugin + codeLLB(debugger) plugin
# set in settings.json "rust-analyzer.server.path" path to rust-analyzer executable

cargo run
cargo run --bin=cfgen --package=r_cfgen
cargo test
cargo build --release \
  --bin cfgen \
  --package r_cfgen

# --target x86_64-unknown-linux-gnu \

mkdir -p misc

cp target/release/cfgen misc/
```

## Inspect symbols

- `readelf -Ws ./bin/libplayground.core.so`
- `nm ./bin/libplayground.core.so`
- `ldd ./bin/playground.core.tests` - show required dynamic libs
- run `LD_LIBRARY_PATH=../playground.core/bin:$LD_LIBRARY_PATH ./bin/playground.core.tests`

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
- p10 - cubic map
  - <img style="width: 450px" src="./p10_models_cubicmap_rendering/image.png" />
- p11 - models decals
  - <img style="width: 450px" src="./p11_models_decals/image.png" />
- p12 - directional billboard
  - <img style="width: 450px" src="./p12_models_directional_billboard/image.png" />
- p13 - first person maze
  - <img style="width: 450px" src="./p13_models_first_person_maze/image.png" />
- p14 - geometric shapes
  - <img style="width: 450px" src="./p14_models_geometric_shapes/image.png" />
- p15 - height map rendering
  - <img style="width: 450px" src="./p15_models_heightmap_rendering/image.png" />
- p16 - model loading + animations
  - <img style="width: 450px" src="./p16_models_loading_gltf/image.png" />
- p17 - 3d terrain movement
  - <img style="width: 450px" src="./p17_3d_terrain_movement/image.png" />
- p18 - minecraft (see reference)
  - <img style="width: 450px" src="./p18_minecraft_cpp/image.png" />
- p19 - game example (see reference)
  - <img style="width: 450px" src="./p19_game1/image.png" />
- p20
  - <img style="width: 450px" src="./p20_spaceship_flight/image.png" />
- p21
  - <img style="width: 450px" src="./p21_terrain_raylib_image_mesh_model/image.png" />
- p22
  - <img style="width: 450px" src="./p22_terrain_2d_arr_vec3/image.png" />
- p25
  - <img style="width: 450px" src="./p25_model_collision/image.png" />
- tower-blocks -  (see references)
  - <img style="width: 450px" src="./tower-blocks/image.png" />
- p26
  - <img style="width: 450px" src="./p26_text_on_top_of_model/image.png" />
- p27 - camera FPS
  - <img style="width: 450px" src="./p27_core_3d_camera_fps/image.png" />
- p28 FPS
  - <img style="width: 450px" src="./p28_core_3d_camera_fps/image.png" />
- p29 shader base lighting
  - <img style="width: 450px" src="./p29_shaders_base_lighting/image.png" />
- p30 shader pbr lighting
  - <img style="width: 450px" src="./p30_shaders_basic_pbr_lighting/image.png" />
- p30 shader pbr lighting
  - <img style="width: 450px" src="./p30_shaders_basic_pbr_lighting/image2.png" />
- p31
  - <img style="width: 450px" src="./p31_textures_particles_blending/image.png" />
- p32
  - <img style="width: 450px" src="./p32_textures_fog_of_war/image.png" />
- p33
  - <img style="width: 450px" src="./p33_textures_framebuffer_rendering/image.png" />
- p34
  - <img style="width: 450px" src="./p34_shapes_top_down_lights/image.png" />

## References

1. <https://stackoverflow.com/questions/63305865/makefile-create-a-build-directory>
1. Git submodules: <https://www.w3schools.com/git/git_submodules.asp>
1. <https://github.com/raysan5/raylib/wiki/Working-with-CMake>
1. glb viewer: <https://sandbox.babylonjs.com/>
1. <https://github.com/raysan5/raygui>
1. <https://github.com/misabitencourt/3d-terrain-movement>
1. <https://github.com/LelsersLasers/Minecraft/tree/main>
1. <https://github.com/Cakez77/MyFirstGame>
1. <https://github.com/emeiri/ogldev>
1. <https://www.youtube.com/playlist?list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA>
1. <https://github.com/travisvroman/kohi/blob/main/utils/Makefile>
1. [Raylib C++ 3D Tutorial: Spaceship Flight, Terrain Gen & Projectile System](https://www.youtube.com/watch?v=xWYcA1RZwXU)
1. [Tower Blocks in C and Raylib: Tower Collapse](https://www.youtube.com/watch?v=vFQlTZ1J_E0)
