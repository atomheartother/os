LD =	bin/i386-elf-ld
LD_BUILD = ld-build
LD_MAKEFILE = $(LD_BUILD)/Makefile
BINUTILS_VERS=binutils-2.33.1
BINUTILS_TAR=$(BINUTILS_VERS).tar.gz
BINUTILS_URL=http://ftp.gnu.org/gnu/binutils/$(BINUTILS_TAR)

CC_TARGET =	bin/i386-elf-gcc
CC_BUILD= gcc-build
CC_MAKEFILE= $(CC_BUILD)/Makefile
CC_VERS=gcc-9.2.0
CC_TAR=$(CC_VERS).tar.gz
CC_URL= https://ftp.gnu.org/gnu/gcc/$(CC_VERS)/$(CC_TAR)

CURRENT_DIR = $(shell pwd)
TARGET=i386-elf

all: $(LD) $(CC_TARGET)

$(LD): $(LD_MAKEFILE)
	$(MAKE) -C $(LD_BUILD) all install

$(LD_MAKEFILE): $(BINUTILS_VERS)
	@mkdir -p $(LD_BUILD)
	cd $(LD_BUILD) && ../$(BINUTILS_VERS)/configure --target=$(TARGET) --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$(CURRENT_DIR)

$(BINUTILS_VERS): $(BINUTILS_TAR)
	tar -xf $<

$(BINUTILS_TAR):
	curl -o $@ $(BINUTILS_URL)

$(CC_TARGET): $(CC_MAKEFILE)
	$(MAKE) -C $(CC_BUILD) all-gcc
	$(MAKE) -C $(CC_BUILD) all-target-libgcc 
	$(MAKE) -C $(CC_BUILD) install-gcc 
	$(MAKE) -C $(CC_BUILD) install-target-libgcc

$(CC_MAKEFILE): $(CC_VERS)
	@mkdir -p $(CC_BUILD)
	cd $(CC_BUILD) && ../$(CC_VERS)/configure --target=$(TARGET) --prefix="$(CURRENT_DIR)" --disable-nls --disable-libssp --enable-languages=c --without-headers

$(CC_VERS): $(CC_TAR)
	tar -xf $<

$(CC_TAR):
	curl -o $@ $(CC_URL)

