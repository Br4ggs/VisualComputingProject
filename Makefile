BUILD_DIR=build
SOURCE_DIR=Source
CMAKE_EXPORT_FLAGS=-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE_RELEASE_EXPORT_FLAGS=-DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release
CMAKE_DEBUG_EXPORT_FLAGS=-DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
EXECUTABLE=$(BUILD_DIR)/VisualComputingProject

.PHONY: clean install build run debug run-debug

run: build
	$(EXECUTABLE)

run-release: release
	$(EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)

install: clean build-dir
	cd $(BUILD_DIR) && cmake $(CMAKE_EXPORT_FLAGS) ../$(SOURCE_DIR)

build-dir:
	mkdir -p $(BUILD_DIR)

build:
	cmake --build $(BUILD_DIR)

release: clean build-dir
	cd $(BUILD_DIR) && cmake $(CMAKE_RELEASE_EXPORT_FLAGS) ../$(SOURCE_DIR)
	cmake --build $(BUILD_DIR)

debug: clean build-dir
	cd $(BUILD_DIR) && cmake $(CMAKE_DEBUG_EXPORT_FLAGS) ../$(SOURCE_DIR)
	cmake --build $(BUILD_DIR)
	lldb $(EXECUTABLE)
