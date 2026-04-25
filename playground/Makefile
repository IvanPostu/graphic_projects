# All compiler flags/rules used across the board.
CFLAGS =-std=gnu11 -Wall -Wextra -Werror -Wno-error=deprecated-declarations -Wno-error=unused-function 
CFLAGS +=-Wvla -Werror=vla -Wno-missing-braces -Wstrict-prototypes -Wno-unused-parameter 
CFLAGS +=-Wno-missing-field-initializers -Wno-tautological-compare 
# NOTE: -fvisibility=hidden hides all symbols by default
CFLAGS +=-fvisibility=hidden

# clang specific flag
# CFLAGS += -Wgnu-folding-constant

LDFLAGS = -Lbin -Llib
INCLUDE_FLAGS = -Isrc
DEFINES :=

MAIN_ASSEMBLY_NAME =main1

MAIN_OUTPUT_NAME =

UNAME_S := 
ifneq ($(OS),Windows_NT)
	UNAME_S = $(shell uname -s)
endif
ifeq ($(OS),Windows_NT)
	PLATFORM := win32
else ifeq ($(UNAME_S),Linux)
	PLATFORM := linux
else ifeq ($(UNAME_S),Darwin)
    PLATFORM := macos
else
    $(error Unsupported platform)
endif

.PHONY: all-release all-debug flags-debug flags-release 
.PHONY: scaffold scaffold-win32 scaffold-linux scaffold-macos 
.PHONY: link link-win32 link-linux link-macos

# don't allow running just 'make'
default:
	@echo "Error: you must run either 'make all-release' or 'make all-debug'"
	@exit 1

all-release: scaffold flags-release compile link
all-debug: scaffold flags-debug compile link

flags-debug: DEFINES += -D_DEBUG
flags-debug: CFLAGS += -g -MD -O0 -fno-omit-frame-pointer
flags-debug: LDFLAGS += -g
# flags-debug: link compile

flags-release: DEFINES += -D_RELEASE
flags-release: CFLAGS += -MD -O2
flags-release: LDFLAGS += 
# flags-release: link compile

scaffold: scaffold-$(PLATFORM)

.NOTPARALLEL: scaffold

scaffold-win32:
	@if not exist obj mkdir obj
	@if not exist bin mkdir bin

scaffold-linux scaffold-macos:
	@mkdir -p bin
	@mkdir -p obj

.PHONY: clean
clean:    clean-$(PLATFORM)

clean-win32:
	if exist bin del /s /q bin\*.*
	if exist obj del /s /q obj\*.*

clean-linux clean-macos:
	@rm -rf bin/*
	@rm -rf obj/*

link: link-$(PLATFORM)

link-win32: LDFLAGS += -fdeclspec -Wno-cast-function-type-mismatch -Xlinker /INCREMENTAL 

# TBD if needed
# link-win32: LDFLAGS += -luser32 -lgdi32 -lwinmm 
link-win32: DEFINES += -D_CRT_SECURE_NO_WARNINGS -DUNICODE
link-win32: MAIN_OUTPUT_NAME = bin\$(MAIN_ASSEMBLY_NAME).exe

# 		NOTE: --no-undefined and --no-allow-shlib-undefined ensure that symbols linking against are resolved.
# 		These are linux-specific, as the default behaviour is the opposite of this, allowing code to compile 
# 		here that would not on other platforms from not being exported (i.e. Windows)
# 		Discovered the solution here for this: https://github.com/ziglang/zig/issues/8180
link-linux: CFLAGS += -Wno-cast-function-type-mismatch -fPIC

# TBD if needed
# link-linux: LDFLAGS += -lxcb -lX11 -lX11-xcb -lxkbcommon -lxcb-xkb -lm -ldl -L/usr/X11R6/lib -Wl,
link-linux: LDFLAGS += -Wl,--no-undefined -Wl,--no-allow-shlib-undefined -Wl,-rpath='$$ORIGIN'
link-linux: MAIN_OUTPUT_NAME = bin/$(MAIN_ASSEMBLY_NAME)

link-macos: LDFLAGS += -lobjc -framework AppKit -framework QuartzCore -framework DiskArbitration -framework CoreFoundation
link-macos: MAIN_OUTPUT_NAME = bin/$(MAIN_ASSEMBLY_NAME)

link-win32 link-linux link-macos: link-common

link-common: obj/main.c.o
	gcc obj/main.c.o -o $(MAIN_OUTPUT_NAME) $(LDFLAGS)

.PHONY: compile
compile: compile-$(PLATFORM)

compile-macos compile-linux: compile-common
# 	echo '*nix (Unix-like)'

compile-win32: compile-common
# 	echo 'win32'

compile-common:
	gcc src/main.c $(CFLAGS) -c -o obj/main.c.o $(DEFINES) $(INCLUDE_FLAGS)
