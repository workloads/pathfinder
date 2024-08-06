# Cpp-specific Make Targets

# TODO: integrate with `workloads/tooling`

BINARY_CPPLINT      ?= cpplint
BINARY_CLANG_FORMAT ?= clang-format

TARGET_DIR_SRC   = ./src
TARGET_FILES_INO = $(wildcard *.ino)
TARGET_FILES_SRC = $(wildcard $(TARGET_DIR_SRC)/*.cpp $(TARGET_DIR_SRC)/*.h)

# Combine lists
TARGET_FILES = $(TARGET_FILES_INO) $(TARGET_FILES_SRC)


.SILENT .PHONY: lint-cpp
lint-cpp: # lint C++ code using cpplint and clang-format [Usage: `make lint-cpp`]
	$(call print_reference,"$(TARGET_FILES_INO)")
	$(BINARY_CPPLINT) \
		$(TARGET_FILES_INO) \
	;

	echo

	$(call print_reference,"$(TARGET_DIR_SRC)")
	$(BINARY_CPPLINT) \
		$(TARGET_FILES_SRC) \
	;

	echo

	$(call print_reference,$(BINARY_CLANG_FORMAT))
	$(BINARY_CLANG_FORMAT) \
		-i \
		$(TARGET_FILES) \
	;

	echo