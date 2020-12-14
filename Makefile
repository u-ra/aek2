TARGET = aek2

CSRC = \
	pins.c
CXXSRC = \
	adb.cc \
	buttons.cc \
	cplusplus_helpers.cc \
	keyboard_matrix.cc \
	keymap.cc \
	main.cc \
	timer.cc \
	usb_callbacks.cc \
	usb_device.cc \
	usb_helpers.cc \
	usb_hid_interface.cc \
	usb_hid_keyboard.cc \
	usb_hid_keyboard_extension.cc \
	usb_hid_mouse.cc


# MCU name.
#     you MUST set this to match the board you are using type "make clean"
#     after changing this, so all files will be rebuilt
#MCU = at90usb162
#MCU = atmega32u4
#MCU = at90usb646
MCU = at90usb1286

# Processor frequency.
#    Normally the first thing your program should do is set the clock
#    prescaler, so your program will run at the correct speed. You should also
#    set this variable to same clock speed. The _delay_ms() macro uses this,
#    and many examples use this variable to calculate timings. Do not add a
#    "UL" here.
F_CPU = 16000000

# Optimization level, can be [0, 1, 2, 3, s]. 
#    0 = turn off optimization. s = optimize for size.
#    (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s


# Options for both C and C++
CFLAGS := -mmcu=$(MCU)
CFLAGS += -DF_CPU=$(F_CPU)UL
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -ffunction-sections
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wundef
CFLAGS += -fno-exceptions

CXXFLAGS := $(CFLAGS)
CXXFLAGS += -fno-threadsafe-statics

# Options for C only
CFLAGS += -Wstrict-prototypes
CFLAGS += -std=gnu99

# Options for C++ only
CXXFLAGS += -std=c++0x


LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += -Wl,--relax
LDFLAGS += -Wl,--gc-sections
#LDFLAGS += -Wl,-u,vfprintf -lprintf_min
#LDFLAGS += -Wl,-u,vfscanf -lscanf_min


#============================================================================


# Define programs and commands.
SHELL = sh
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AR = avr-ar rcs
NM = avr-nm
REMOVE = rm -f
REMOVEDIR = rm -rf
COPY = cp
DOXYGEN = doxygen
LOADER = ./teensy_loader_cli
CHECKER = ./avr-mem.sh


# Define all object files.
OBJ = $(CSRC:%.c=%.o) $(CXXSRC:%.cc=%.o)


# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d


# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = $(CFLAGS) $(GENDEPFLAGS)
ALL_CXXFLAGS = $(CXXFLAGS) $(GENDEPFLAGS)


# Default target.
all: $(TARGET).hex

docs: $(CSRC) $(CXXSRC)
	@echo "----  Building documentation  ----"
	$(DOXYGEN)
	@echo

# install on the device
load: $(TARGET).hex
	@echo "----  Loading \"$<\" onto device  ----"
	$(LOADER) -mmcu=$(MCU) -w -v $<
	@echo

check: $(TARGET).elf
	@echo "----  Reporting statistics of \"$<\"  ----"
	$(CHECKER) $< $(MCU)
	@echo

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo "----  Building \"$@\" from \"$<\"  ----"
	$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@
	@echo

# Link: create ELF output file from object files.
%.elf: $(OBJ)
	@echo "----  Building \"$@\" from \"$<\"  ----"
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)
	@echo

# Compile: create object files from C source files.
%.o: %.c
	@echo "----  Building \"$@\" from \"$<\"  ----"
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 
	@echo

# Compile: create object files from C++ source files.
%.o: %.cc
	@echo "----  Building \"$@\" from \"$<\"  ----"
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 
	@echo

# Target: clean project.
clean:
	@echo "----  Cleaning  ----"
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(CSRC:%.c=%.o) $(CXXSRC:%.cc=%.o)
	$(REMOVEDIR) .dep
	$(REMOVEDIR) html
	@echo


# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Listing of phony targets.
.PHONY: all load check hex clean
