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
	;


.SILENT .PHONY: attach
attach: # attach Arduino Board using arduino-cli [Usage: `make attach`]
	$(call print_reference,"$(ARDUINO_SKETCH_CONFIG)")

	echo

	# see https://arduino.github.io/arduino-cli/1.0/commands/arduino-cli_board_attach/
	$(BINARY_ARDUINO_CLI) \
		board \
			attach \
				--config-file "$(ARDUINO_SKETCH_CONFIG)" \
	;


.SILENT .PHONY: compile
compile: # compile Arduino Sketch using arduino-cli [Usage: `make compile`]
	$(call print_reference,"$(ARDUINO_SKETCH_FILE)")

	echo

	# see https://arduino.github.io/arduino-cli/1.0/commands/arduino-cli_compile/
	$(BINARY_ARDUINO_CLI) \
		compile \
			--clean \
			--config-file="$(ARDUINO_SKETCH_CONFIG)" \
			--discovery-timeout "$(ARDUINO_CLI_COMPILE_DISCOVERY_TIMEOUT)" \
			--export-binaries \
			--jobs="$(ARDUINO_CLI_COMPILE_JOBS)" \
			--output-dir "$(ARDUINO_CLI_COMPILE_OUTPUT_DIRECTORY)" \
			--profile "$(ARDUINO_SKETCH_PROFILE)" \
			--protocol "$(ARDUINO_CLI_COMPILE_PROTOCOL)" \
			--quiet \
			--verify \
			--warnings "$(ARDUINO_CLI_COMPILE_WARNINGS)" \
			$(ARDUINO_SKETCH_FILE) \
	;


.SILENT .PHONY: upload
upload: # upload binary artifact using arduino-cli [Usage: `make upload`]
	$(call print_reference,"$(ARDUINO_CLI_COMPILE_OUTPUT_DIRECTORY)")

	echo

	# see https://arduino.github.io/arduino-cli/1.0/commands/arduino-cli_compile/
	$(BINARY_ARDUINO_CLI) \
		upload \
			--config-file="$(ARDUINO_SKETCH_CONFIG)" \
			--discovery-timeout "$(ARDUINO_CLI_COMPILE_DISCOVERY_TIMEOUT)" \
			--input-dir "$(ARDUINO_CLI_COMPILE_OUTPUT_DIRECTORY)" \
			--profile "$(ARDUINO_SKETCH_PROFILE)" \
			--protocol="$(ARDUINO_CLI_COMPILE_PROTOCOL)" \
			--verify
	;


.SILENT .PHONY: install-core
install-core: # install Arduino Board Core using arduino-cli [Usage: `make install-core`]
	$(call print_reference,"$(ARDUINO_CORE)")

	# see https://arduino.github.io/arduino-cli/1.0/commands/arduino-cli_core_install/
	$(BINARY_ARDUINO_CLI) \
		core \
			install \
				$(ARDUINO_CORE) \
	;


.SILENT .PHONY: install-libs
install-libs: # install Arduino libraries using arduino-cli [Usage: `make install-libs`]
	echo "⚠️  Installing and updating upstream libraries"

	# see https://arduino.github.io/arduino-cli/1.0/commands/arduino-cli_lib_install/
	$(BINARY_ARDUINO_CLI) \
		lib \
			install \
				$(ARDUINO_LIBRARIES_UPSTREAM) \
	;

	echo

	echo "⚠️  Installing and updating unsafe libraries"
	$(foreach ITEM,$(ARDUINO_LIBRARIES_UNSAFE),$(BINARY_ARDUINO_CLI) lib install --git-url $(ITEM))


.SILENT .PHONY: enable-unsafe-libs
enable-unsafe-libs: # enable installation of unsafe libraries when using arduino-cli [Usage: `make enable-unsafe-libs`]
	# see https://arduino.github.io/arduino-cli/1.0/commands/arduino-cli_config_set/
	$(BINARY_ARDUINO_CLI) \
		config \
			set \
				library.enable_unsafe_install true \
	;