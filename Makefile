BUILD_DIR = build

build:
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake ../ -G Ninja && ninja -v

.PHONY: build
