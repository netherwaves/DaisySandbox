# Project Name
TARGET = Sandbox
APP_TYPE = BOOT_QSPI
# C_INCLUDES += -Iinclude

# Sources
CPP_SOURCES = audio_thru.cpp

# Library Locations
LIBDAISY_DIR = lib/libDaisy
DAISYSP_DIR = lib/DaisySP

# needed by qlib
# C_DEFS += -DQ_DONT_USE_THREADS=1
# needed by qlib (TODO: find a better alternative)		
# CPP_FLAGS += -fexceptions

# Core location, and generic makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/makefile
