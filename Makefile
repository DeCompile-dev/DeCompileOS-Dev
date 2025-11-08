# ==============================================================================
# 1. Configuration for AArch64 User Application (First Snippet)
# ==============================================================================

ARCH          ?= aarch64-none-elf
CC            := $(ARCH)-gcc
CXX           := $(ARCH)-g++
LD            := $(ARCH)-ld

# Path to the DeCopmileOS folder
OS_PATH       ?= ../DeCompileOS

# Package name, or executable name if one is not provided in package.info
EXEC_NAME     ?= $(notdir $(CURDIR))

# Path to the system's libshared. Static link only for now
STDINC        ?= $(OS_PATH)/shared/
STDLIB        ?= $(OS_PATH)/shared/libshared.a
CFLAGS        ?= -ffreestanding -nostdlib -std=c99 -I$(STDINC) -O0
FS_PATH       ?= $(OS_PATH)/fs/redos/user/

# Find C source files in the current directory for the AArch64 application
APP_C_SOURCE  ?= $(shell find . -maxdepth 1 -name '*.c')
APP_OBJ       ?= $(APP_C_SOURCE:%.c=%.o)

# ==============================================================================
# 2. Configuration for x86_64 Kernel (Second Snippet)
# ==============================================================================

# Source files for the kernel implementation
kernel_source_files := $(shell find src/impl/kernel -name *.c)
kernel_object_files := $(patsubst src/impl/kernel/%.c, build/kernel/%.o, $(kernel_source_files))

# Source files for the x86_64 specific implementation
x86_64_c_source_files := $(shell find src/impl/x86_64 -name *.c)
x86_64_c_object_files := $(patsubst src/impl/x86_64/%.c, build/x86_64/%.o, $(x86_64_c_source_files))

x86_64_asm_source_files := $(shell find src/impl/x86_64 -name *.asm)
x86_64_asm_object_files := $(patsubst src/impl/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

x86_64_object_files := $(x86_64_c_object_files) $(x86_64_asm_object_files)

# Combined objects for the kernel
KERNEL_OBJECTS := $(kernel_object_files) $(x86_64_object_files)

# ==============================================================================
# 3. x86_64 Kernel Build Rules
# ==============================================================================

.PHONY: build-x86_64 kernel all clean dump

# Default target: Build the x86_64 kernel
all: kernel

kernel: build-x86_64

# Rule for kernel C source files
$(kernel_object_files): build/kernel/%.o : src/impl/kernel/%.c
	@mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I src/intf -ffreestanding $< -o $@

# Rule for x86_64 C source files
$(x86_64_c_object_files): build/x86_64/%.o : src/impl/x86_64/%.c
	@mkdir -p $(dir $@)
	x86_64-elf-gcc -c -I src/intf -ffreestanding $< -o $@

# Rule for x86_64 Assembly source files (NASM)
$(x86_64_asm_object_files): build/x86_64/%.o : src/impl/x86_64/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

# Final linking and ISO creation for x86_64
build-x86_64: $(KERNEL_OBJECTS)
	@mkdir -p dist/x86_64
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(KERNEL_OBJECTS)
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso

# ==============================================================================
# 4. AArch64 User Application Build Rules (Prefixed with 'app-')
# ==============================================================================

# Rule for AArch64 C source files in the current directory
$(APP_OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

app-$(EXEC_NAME): app-package $(APP_OBJ)
	$(LD) -T linker.ld -o $(EXEC_NAME).red/$(EXEC_NAME).elf $(APP_OBJ) $(STDLIB)

app-all: app-$(EXEC_NAME)

app-package:
	@mkdir -p $(EXEC_NAME).red
	@mkdir -p resources
	cp -r resources $(EXEC_NAME).red
	cp package.info $(EXEC_NAME).red

app-copy: app-all
	cp -r $(EXEC_NAME).red $(FS_PATH)

app-run: app-copy
	(cd $(OS_PATH); ./createfs; ./run_virt)

app-dump: app-all
	$(ARCH)-objdump -D $(EXEC_NAME).red/$(EXEC_NAME).elf > dump

# ==============================================================================
# 5. Global Clean Target
# ==============================================================================

clean:
	# Clean x86_64 kernel build artifacts
	rm -rf build
	rm -rf dist/x86_64
	rm -f targets/x86_64/iso/boot/kernel.bin

	# Clean AArch64 user application artifacts
	rm -f $(APP_OBJ)
	rm -rf $(EXEC_NAME).red