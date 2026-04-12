let 
  pkgs = import <nixpkgs> { config = { allowUnfree = false; }; };
  PROJECT_ROOT = builtins.toString ./.;
in
pkgs.mkShell {
  name = "app-shell";

  nativeBuildInputs = [
    pkgs.pkg-config # is required for `cargo install sqlx-cli`
  ];

  buildInputs = with pkgs; [
    rustup
   
    cmake
    gcc14
    binutils # provides gprof
    clang-tools
    libx11 # needed to build raylib
    libxrandr # needed to build raylib
    libxinerama # needed to build raylib
    libxcursor # needed to build raylib
    libxi # needed to build raylib
    glfw
    freeglut
    libGLU
    glew
  ];

  LANG = "en_US.UTF-8";
  LC_ALL = "en_US.UTF-8";

  shellHook = ''
      export QQQ=${pkgs.glew}
      export PROJECT_ROOT="${PROJECT_ROOT}"
      export RUSTUP_HOME="$PROJECT_ROOT/.rustup"
      export CARGO_HOME="$PROJECT_ROOT/.cargo"
      export PATH="$CARGO_HOME/bin:$PATH"

      export C_INCLUDE_PATH=$PROJECT_ROOT/external/raylib/build/raylib/include:$C_INCLUDE_PATH
      export CPLUS_INCLUDE_PATH=$PROJECT_ROOT/external/raylib/build/raylib/include:$CPLUS_INCLUDE_PATH

      export C_INCLUDE_PATH=$PROJECT_ROOT/external/raygui/src:$C_INCLUDE_PATH
      export CPLUS_INCLUDE_PATH=$PROJECT_ROOT/external/raygui/src:$CPLUS_INCLUDE_PATH

      export C_INCLUDE_PATH=$PROJECT_ROOT/ogldev/Include:$C_INCLUDE_PATH
      export CPLUS_INCLUDE_PATH=$PROJECT_ROOT/ogldev/Include:$CPLUS_INCLUDE_PATH
  '';
}
