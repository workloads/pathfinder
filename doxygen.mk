# Doxygen-specific Make Targets

# TODO: integrate with `workloads/tooling`

BINARY_DOXYGEN ?= doxygen
BINARY_GIT     ?= git

CONFIG_DOXYGEN ?= Doxyfile
FORMAT_DOXYGEN ?= html

# see https://jothepro.github.io/doxygen-awesome-css/
DOXYGEN_THEME_URL  ?= "https://github.com/jothepro/doxygen-awesome-css.git"
DOXYGEN_THEME_PATH ?= ".doxygen/theme"


.SILENT .PHONY: docs
docs: # create docs using doxygen [Usage: `make docs`]
	$(BINARY_DOXYGEN) \
		#-w $(FORMAT_DOXYGEN) \
		$(CONFIG_DOXYGEN) \
	;


docs-get-theme: # retrieve Doxygen theme using git [Usage: `make docs-get-theme`]
	if [ ! -d "$(DOXYGEN_THEME_PATH)" ]; then \
  		echo "$(DOXYGEN_THEME_PATH) does not exist, cloning repository." \
  		; \
		$(BINARY_GIT) \
			clone \
				$(DOXYGEN_THEME_URL) \
				$(DOXYGEN_THEME_PATH) \
		; \
	else \
		echo "$(DOXYGEN_THEME_PATH) already exists, pulling latest changes." \
		; \
		cd $(DOXYGEN_THEME_PATH) \
		; \
		$(BINARY_GIT) \
			pull \
		; \
	fi