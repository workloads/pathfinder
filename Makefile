## Makefile for Pathfinder

# configuration
MAKEFILE_TITLE = 🧭 PATHFINDER

include ../tooling/make/configs/shared.mk
include ../tooling/make/functions/shared.mk
include ./arduino-lint.mk
include ./cpp.mk
include ../tooling/make/targets/yamllint.mk
include ./arduino.mk
include ./doxygen.mk
include ../tooling/make/targets/shared.mk
include ./config.mk
