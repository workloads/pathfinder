# Arduino-specific Make Targets

# TODO: integrate with `workloads/tooling`

BINARY_ARDUINO_CLI  ?= arduino-cli


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

.SILENT .PHONY: clean
clean: # clean Arduino cache using arduino-cli [Usage: `make clean`]
	# see https://arduino.github.io/arduino-cli/1.0/commands/arduino-cli_cache_clean/
	$(BINARY_ARDUINO_CLI) \
		cache \
			clean \
				--config-file="$(ARDUINO_SKETCH_CONFIG)" \
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
			--verbose \
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


.SILENT .PHONY: monitor
monitor: # monitor binary output using arduino-cli [Usage: `make monitor`]
	$(call print_arg,"baudrate",$(ARDUINO_CLI_MONITOR_BAUDRATE))

	echo

	# see https://arduino.github.io/arduino-cli/1.0/commands/arduino-cli_monitor/
	$(BINARY_ARDUINO_CLI) \
		monitor \
			--config "$(ARDUINO_CLI_MONITOR_BAUDRATE)" \
			--config-file="$(ARDUINO_SKETCH_CONFIG)" \
			--discovery-timeout "$(ARDUINO_CLI_COMPILE_DISCOVERY_TIMEOUT)" \
			--timestamp \
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