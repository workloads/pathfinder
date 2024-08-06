## Makefile for Pathfinder

# configuration
MAKEFILE_TITLE = 🧭 PATHFINDER

include ../tooling/make/configs/shared.mk
include ../tooling/make/functions/shared.mk
include ../tooling/make/functions/snyk.mk
include ../tooling/make/targets/shared.mk
include ./config.mk

# TODO: integrate with `workloads/tooling`
BINARY_ARDUINO_CLI  ?= arduino-cli
BINARY_ARDUINO_LINT ?= arduino-lint
BINARY_CPPLINT      ?= cpplint
BINARY_CLANG_FORMAT ?= clang-format

FORMAT_ARDUINO_LINT ?= text

TARGET_DIR_SRC   = ./src
TARGET_FILES_INO = $(wildcard *.ino)
TARGET_FILES_SRC = $(wildcard $(TARGET_DIR_SRC)/*.cpp $(TARGET_DIR_SRC)/*.h)

# Combine lists
TARGET_FILES = $(TARGET_FILES_INO) $(TARGET_FILES_SRC)

TARGET_FILES = *.ino src/*.{cpp,h}

.SILENT .PHONY: lint
lint: # lint C++ code using cpplint and clang-format [Usage: `make lint`]
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

.SILENT .PHONY: lint-arduino
lint-arduino: # lint Arduino code using arduino-lint [Usage: `make lint-arduino`]
	$(BINARY_ARDUINO_LINT) \
		--compliance strict \
		--format $(FORMAT_ARDUINO_LINT) \
		--project-type all \
		--recursive \
	;

.SILENT .PHONY: board
board: # install Arduino Board Core using arduino-cli [Usage: `make board`]
	$(call print_reference,"$(ARDUINO_CORE)")
	$(BINARY_ARDUINO_CLI) \
		core \
			install \
				$(ARDUINO_CORE) \
	;

.SILENT .PHONY: libs
libs: # install Arduino libraries using arduino-cli [Usage: `make libs`]
	echo "⚠️  Installing and updating upstream libraries"
	$(BINARY_ARDUINO_CLI) \
		lib \
			install \
				$(ARDUINO_LIBRARIES_UPSTREAM) \
	;

	echo

	echo "⚠️  Installing and updating unsafe libraries"
	$(foreach ITEM,$(ARDUINO_LIBRARIES_UNSAFE),$(BINARY_ARDUINO_CLI) lib install --git-url "$(ITEM)";)
	;

.SILENT .PHONY: libs-enable-unsafe
libs-enable-unsafe: # enable installation of unsafe libraries when using Arduino CLI [Usage: `make libs-enable-unsafe`]
	# see https://arduino.github.io/arduino-cli/1.0/configuration/#command-line-flags
	$(BINARY_ARDUINO_CLI) \
		config \
			set \
				library.enable_unsafe_install true \
	;