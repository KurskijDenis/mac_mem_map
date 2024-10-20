SED_BIN = gsed
# Use gnu sed if we have it.
# sed on mac may work incorrectly.
ifeq (, $(shell which gsed))
	SED_BIN = sed
endif

.DEFAULT_GOAL := default

.PHONY: generate-cmake
generate-cmake: generate-cmake-debug generate-cmake-release generate-cmake-rel-with-deb-info # Generate cmake files for debug, release, rel-with-deb-info.

generate-cmake-debug: CMAKE_DVARIABLES=-DCMAKE_CXX_FLAGS_DEBUG="-ggdb3" -DCMAKE_EXPORT_COMPILE_COMMANDS=1

generate-cmake-%: # Generate cmake files for selected target.
	cmake -B build/$(subst -,_,$(patsubst generate-cmake-%,%,$@)) \
		-S . \
		-DCMAKE_BUILD_TYPE=$(shell echo "$@" | $(SED_BIN) -e 's/^generate-cmake-\([a-z]\)/\U\1/' -e 's/-\([a-z]\)/\U\1/g') $(CMAKE_DVARIABLES)

.PHONY: build
build: build-debug build-release build-rel-with-deb-info

build-%: generate-cmake-%
	cmake --build build/$(subst -,_,$(patsubst generate-cmake-%,%,$?)) -j 8
#		--verbose


.PHONY: clean
clean: # Clean built clients.
	rm -rf build/debug
	rm -rf build/release
	rm -rf build/rel_with_deb_info

.PHONY: default
default: build-debug build-release # Build project from scratch.
