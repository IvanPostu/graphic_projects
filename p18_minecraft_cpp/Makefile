opt=0
CXX = g++

# Get absolute directory where current Makefile lives
MAKEFILE_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

RAYLIB_FLAGS = -I$(MAKEFILE_DIR)/../external/raylib/build/raylib/include -I$(MAKEFILE_DIR)/../external/raygui/src
RAYLIB_LDFLAGS = -L$(MAKEFILE_DIR)/../external/raylib/build/raylib -lraylib -lglfw -lGL -lm -lpthread -ldl -lrt -lX11

CXXFLAGS = -Wall
LDFLAGS = $(RAYLIB_LDFLAGS)

OUT_DIR=$(MAKEFILE_DIR)/build

SRC_RESOURCES_DIR := $(MAKEFILE_DIR)/resources
TARGETS=print

ifeq ($(opt),0)
    CXXFLAGS += -g -Wextra
else
    CXXFLAGS += -O3 -Ofast
endif

.phony: all
all: $(TARGETS)

buildAndRun: compile run

print:
	echo $(MAKEFILE_DIR)

makeDirs:
	mkdir -vp $(OUT_DIR)

copyAssets:
	cp -vr $(SRC_RESOURCES_DIR) $(OUT_DIR)

compile: makeDirs copyAssets
	$(CXX) -o $(OUT_DIR)/main $(MAKEFILE_DIR)/src/*.cpp $(LDFLAGS) $(RAYLIB_FLAGS) $(CXXFLAGS)

compileWithProfile: makeDirs copyAssets
	$(CXX) -o $(OUT_DIR)/mainWithProfile $(MAKEFILE_DIR)/src/*.cpp $(LDFLAGS) $(RAYLIB_FLAGS) $(CXXFLAGS) -pg

run:
	$(OUT_DIR)/main

runWithProfile:
	$(OUT_DIR)/mainWithProfile
	gprof $(OUT_DIR)/mainWithProfile > $(OUT_DIR)/gprofOutput.txt

clean:
	rm -vr $(OUT_DIR)

cleanProfiles:
	rm -v gmon.out gprofOutput.txt
