# This is a Makefile wrapper for CMake, to make it easier to build cmake from
# within Eclipse.  This does an out-of-source build in the 'build' directory.
# CMake is run automatically when necessary!
#
# Useful targets:
#   cmake, ccmake, cmake_gui:  Run this variant of cmake from the 'build' directory
#   configure:  Just runs cmake
#   distclean:  Clean out the build directory, including removing the CMake cache
# All other targets are just passed to the Makefile created by CMake.
#
# If CMake is not finding the packages it needs, set CMAKE_PREFIX_PATH in the
# environment, e.g. CMAKE_PREFIX_PATH=/opt/local:/opt/local/libexec/qt4-mac for
# MacPorts.
#
# Another feature if you need to set CMake options, but never want to run it by
# hand, is to put the CMake command line options in the environment variable
# $CMAKE_OPTIONS, and this Makefile will pass it on!  You can do this in Eclipse.
#
# Nov 24, 2009, Richard Roberts
#

# Out-of-source build directory name 
BUILD=bin

# Avoid conflicts with environment variable
CMAKE_OPTIONS_INT=${CMAKE_OPTIONS}
# If the install prefix isn't specified, default it to $HOME
ifeq ($(strip $(findstring CMAKE_INSTALL_PREFIX, ${CMAKE_OPTIONS})), )
	CMAKE_OPTIONS_INT += -DCMAKE_INSTALL_PREFIX:path=${HOME}
endif

# Default target - have to make 'all' the first listed target, but we also want
# to pass 'all' to the CMake-generated Makefile.
d_all: all ;
.PHONY: d_all

##### Special targets #####

# Run cmake
.PHONY: configure
configure: cmake ;

# Run cmake variants
.PHONY: cmake ccmake cmake_gui
cmake ccmake cmake_gui: builddir_exists
	cd ${BUILD}; $@ ${CMAKE_OPTIONS_INT} ..

# Re-run CMake if the Makefile is non-existant.  The CMake-generated makefile
# will run CMake if the Makefile is out-of-date.
.PHONY: ${BUILD}/Makefile
${BUILD}/Makefile:
	@if [ ! -d ${BUILD} ]; then \
		echo "[cmake_wrapper]: mkdir ${BUILD}"; \
		mkdir ${BUILD}; \
	fi
	cd ${BUILD}; cmake ${CMAKE_OPTIONS_INT} ..

# Clear the cache, totally remove everything
.PHONY: distclean
distclean:
	@if [ -d ${BUILD} ]; then \
		if [ -d ${BUILD}/CMakeFiles -a -f ${BUILD}/CMakeCache.txt ]; then \
			echo "[cmake_wrapper]: rm -r ${BUILD}"; \
			rm -r ${BUILD}; \
		else \
			echo "[cmake_wrapper]: *** Did not find one of ${BUILD}/CMakeFiles or ${BUILD}/CMakeCache.txt," >&2; \
			echo "[cmake_wrapper]: *** so not attempting to remove ${BUILD}, please remove manually." >&2; \
			exit 1; \
		fi \
	else \
		echo "[cmake_wrapper]: Did not find ${BUILD} - nothing to clean or in the wrong directory" >&2; \
	fi

# Make sure the build directory exists
.PHONY: builddir_exists
builddir_exists:
	@if [ ! -d ${BUILD} ]; then \
		echo "[cmake_wrapper]: mkdir ${BUILD}"; \
		mkdir ${BUILD}; \
	fi

# Hack to make sure "make install" does something when there is an INSTALL file
.PHONY: install
install:
	make -f$(lastword ${MAKEFILE_LIST}) ${BUILD}/Makefile
	VERBOSE=1 make -C${BUILD} $@

# Default rule to pass target to CMake Makefile
%:
	make -f$(lastword ${MAKEFILE_LIST}) ${BUILD}/Makefile
	VERBOSE=1 make -C${BUILD} $@
	

