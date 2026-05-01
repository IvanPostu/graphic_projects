ASSEMBLY_NAME=tower-blocks

CXX := g++

# All compiler flags/rules used across the board.
CFLAGS =-Wall -Wextra -Werror -Wno-error=deprecated-declarations -Wno-error=unused-function 
CFLAGS +=-Wvla -Werror=vla -Wno-missing-braces -Wno-unused-parameter 
CFLAGS +=-Wno-missing-field-initializers -Wno-tautological-compare 
# NOTE: -fvisibility=hidden hides all symbols by default, and only those that explicitly say otherwise are exported (i.e. via _API).
CFLAGS +=-fvisibility=hidden

# clang specific flag
# CFLAGS += -Wgnu-folding-constant

CFLAGS += -I../external/raylib/build/raylib/include

# Base linker flags
# LDFLAGS = -Lbin -Llib -L../playground.core/bin -lplayground.core
LDFLAGS = -Lbin -Llib
LDFLAGS += -L../external/raylib/build/raylib -lraylib -lglfw -lGL -lm -lpthread -ldl -lrt -lX11

# Base include flags
# INCLUDE_FLAGS = -Isrc -I../playground.core/src 
INCLUDE_FLAGS = -Isrc

# not really needed yet
DEFINES =-DAPI_EXPORT

OUTPUT_NAME =
BUILD_TYPE =

UNAME_S := 
ifneq ($(OS),Windows_NT)
	UNAME_S = $(shell uname -s)
endif
ifeq ($(OS),Windows_NT)
	PLATFORM := win32

# 	Make does not offer a recursive wildcard function, and Windows needs one, so here it is:
	rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
	DIR := $(subst /,\,${CURDIR})
	SRC_FILES := $(call rwildcard,src,*.cpp)
	DIRECTORIES := \src $(subst $(DIR),,$(shell dir src /S /AD /B | findstr /i src)) 
	OBJ_FILES := $(SRC_FILES:%=obj/%.o)
else ifeq ($(UNAME_S),Linux)
	PLATFORM := linux
	SRC_FILES := $(shell find src -name "*.cpp")
	DIRECTORIES := $(shell find src -type d)
	OBJ_FILES := $(SRC_FILES:%=obj/%.o)
else ifeq ($(UNAME_S),Darwin)
  PLATFORM := macos
	SRC_FILES := $(shell find src -type f \( -name "*.cpp" -o -name "*.m" \))
	DIRECTORIES := $(shell find src -type d)
	OBJ_FILES := $(SRC_FILES:%=obj/%.o)
else
    $(error Unsupported platform)
endif

SRC_DIR = resources
DST_DIR = bin/resources

ifeq ($(PLATFORM),win32)
    COPY_DIR = xcopy /E /I /Y $(SRC_DIR) $(DST_DIR)
else
    COPY_DIR = cp -r $(SRC_DIR) $(DST_DIR)
endif

ifeq ($(filter $(BUILD_TYPE),debug release),)
$(error Invalid BUILD_TYPE=$(BUILD_TYPE). Must be 'debug' or 'release')
endif

# =========================================================
# BEGIN TARGET RECIPES
# =========================================================
.PHONY: all flags-debug flags-release
.PHONY: scaffold scaffold-win32 scaffold-linux scaffold-macos 
.PHONY: link link-win32 link-macos link-linux link-linux-release link-linux-debug

# should be first to be as default
all: scaffold flags-$(BUILD_TYPE) compile link 
#copy_resources

flags-debug: DEFINES += -D_DEBUG
flags-debug: CFLAGS += -g -MD -O0 -fno-omit-frame-pointer
flags-debug: LDFLAGS += -g

flags-release: DEFINES += -D_RELEASE
flags-release: CFLAGS += -MD -O2
flags-release: LDFLAGS +=

scaffold: scaffold-$(PLATFORM)

.NOTPARALLEL: scaffold

scaffold-win32:
	-@mkdir $(addprefix obj\\, $(DIRECTORIES)) 2>nul
	@if not exist bin mkdir bin

scaffold-linux scaffold-macos:
	@mkdir -p bin
	@mkdir -p $(addprefix obj/,$(DIRECTORIES))
	

.PHONY: clean
clean:    clean-$(PLATFORM)

clean-win32:
	if exist bin del /s /q bin\*.*
	if exist obj del /s /q obj\*.*

clean-linux clean-macos:
	@rm -rf bin/*
	@rm -rf obj/*

# TODO: do same for mac and windows
link: link-$(PLATFORM)-$(BUILD_TYPE)

link-win32: LDFLAGS += -fdeclspec -Wno-cast-function-type-mismatch -Xlinker /INCREMENTAL 
link-win32: DEFINES += -D_CRT_SECURE_NO_WARNINGS -DUNICODE
link-win32: OUTPUT_NAME = bin\$(ASSEMBLY_NAME).exe

link-linux-debug: LDFLAGS += -Wl,-rpath,$(abspath ../playground.core/bin)
link-linux-debug: link-linux
link-linux-release: LDFLAGS += -Wl,-rpath='$$ORIGIN'
link-linux-release: link-linux


# 		NOTE: --no-undefined and --no-allow-shlib-undefined ensure that symbols linking against are resolved.
# 		These are linux-specific, as the default behaviour is the opposite of this, allowing code to compile 
# 		here that would not on other platforms from not being exported (i.e. Windows)
# 		Discovered the solution here for this: https://github.com/ziglang/zig/issues/8180
link-linux: CFLAGS += -Wno-cast-function-type-mismatch -fPIC
link-linux: LDFLAGS += -lm
link-linux: LDFLAGS += -Wl,--no-undefined,--no-allow-shlib-undefined

# TODO: big workaround in order to not move .so file in the same dir with exe, required smarter approach like debug only
# -rpath='$$ORIGIN' - allows exe to look in the same dir for *.so dynamic lib
link-linux: OUTPUT_NAME = bin/$(ASSEMBLY_NAME)

link-macos: LDFLAGS += 
link-macos: OUTPUT_NAME = bin/$(ASSEMBLY_NAME)

link-win32 link-linux link-macos: link-common

link-common: $(OBJ_FILES)
	@echo   $<...
	$(CXX) $(OBJ_FILES) -o $(OUTPUT_NAME) $(LDFLAGS)


.PHONY: compile
compile: gen_compile_flags-$(BUILD_TYPE)
-include $(OBJ_FILES:.o=.d)


# compile .cpp to .o object for windows, linux and mac
obj/%.cpp.o: %.cpp 
	@echo   $<...
	$(CXX) $< $(CFLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)

# compile .m to .o object only for macos
obj/%.m.o: %.m
	@echo   $<...
	$(CXX) $< $(CFLAGS) -c -o $@ $(DEFINES) $(INCLUDE_FLAGS)

-include $(OBJ_FILES:.o=.d)

.PHONY: gen_compile_flags-debug
gen_compile_flags-debug: gen_compile_flags

gen_compile_flags-release: gen_compile_flags

SEPARATOR := '|------------'

.PHONY: gen_compile_flags
gen_compile_flags:
ifeq ($(PLATFORM),win32)
	@..\misc\cfgen -outfile=compile_flags.txt \
		'INCLUDE_FLAGS:' $(INCLUDE_FLAGS) \
		$(SEPARATOR) 'DEFINES:' $(DEFINES) \
		$(SEPARATOR) 'CFLAGS:' $(CFLAGS) \
		$(SEPARATOR) 'LDFLAGS:' $(LDFLAGS)
else
	@../misc/cfgen -outfile=compile_flags.txt \
		'INCLUDE_FLAGS:' $(INCLUDE_FLAGS) \
		$(SEPARATOR) 'DEFINES:' $(DEFINES) \
		$(SEPARATOR) 'CFLAGS:' $(CFLAGS) \
		$(SEPARATOR) 'LDFLAGS:' $(LDFLAGS)
endif

copy_resources:
	$(COPY_DIR)

.ONESHELL:

run:
	set -e
	cd bin
	./$(ASSEMBLY_NAME)
