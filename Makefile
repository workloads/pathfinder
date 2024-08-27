## Makefile for Pathfinder

# configuration
MAKEFILE_TITLE = 🧭 PATHFINDER

include ../tooling/make/configs/shared.mk
include ../tooling/make/functions/shared.mk
include ../tooling/make/targets/arduino-lint.mk
include ../tooling/make/targets/cpp.mk
include ../tooling/make/targets/yamllint.mk
include ../tooling/make/targets/arduino.mk
include ../tooling/make/targets/doxygen.mk
include ../tooling/make/targets/shared.mk
include ../tooling/make/targets/vale.mk
include ./config.mk
