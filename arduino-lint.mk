# arduino-lint-specific Make Targets

# TODO: integrate with `workloads/tooling`

BINARY_ARDUINO_LINT ?= arduino-lint

FORMAT_ARDUINO_LINT ?= text


.SILENT .PHONY: lint-arduino
lint-arduino: # lint Arduino code using arduino-lint [Usage: `make lint-arduino`]
	$(BINARY_ARDUINO_LINT) \
		--compliance strict \
		--format $(FORMAT_ARDUINO_LINT) \
		--project-type all \
	;
