# Graphic Projects

Small projects and experiments for testing ideas and building quick POCs.

## Shell

```sh
nix-shell --run bash
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

- <https://stackoverflow.com/questions/63305865/makefile-create-a-build-directory>
- <https://www.youtube.com/watch?v=z7wVUfnm7M0>
