# Graphic Projects

Small projects and experiments for testing ideas and building quick POCs.

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
make run
```

## Rust setup + run

```sh
rustup default stable
rustup component add rust-analyzer # install analyzer

# VSCodium + rust-analyzer plugin
# set in settings.json "rust-analyzer.server.path" path to rust-analyzer executable

cargo run
cargo run --bin=dsa_rust --package=dsa_rust
cargo test
```

## C language setup + run

```sh
# VSCodium + clangd(LSP) + codeLLB(debugger)
# add clang-tools as a nix dependency

# C project
cd dsa_c
make clean # clean dsa_c/build folder
make opt=0 # debug build
make opt=1 # release build
make run

# debugger usage: 
# call compact_8b_bool_to_char_bits(actual)
# print a
# print a ? b
```

## References

1. <https://stackoverflow.com/questions/63305865/makefile-create-a-build-directory>
1. <https://www.youtube.com/watch?v=z7wVUfnm7M0>
1. Git submodules: <https://www.w3schools.com/git/git_submodules.asp>
1. <https://github.com/raysan5/raylib/wiki/Working-with-CMake>
1. <https://free3d.com/>
