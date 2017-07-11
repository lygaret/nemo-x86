AS = clang
CC = clang
PP = clang
LD = ld

SRCDIR       := src
TARGETDIR    := target
BUILDDIR     := target/object
INCLUDEDIRS  := src
INITRD       := src/initrd
ISODIR       := src/isord

KERNEL_NAME  := nemo-x86

IFLAGS   = ${INCLUDEDIRS:%=-I %}
ASFLAGS  = ${IFLAGS}
LDFLAGS  = -m elf_i386 ${IFLAGS}
PPFLAGS  = -E -nostdinc -x c ${IFLAGS}
CFLAGS   = -target i686-pc-none-elf -march=i686
CFLAGS  += -nostdlib -ffreestanding -fPIC
CFLAGS  += -Wall -Wextra ${IFLAGS}
DEPFLAGS = ${IFLAGS} -MT ${@:%.d=%.o} -MM -MP -MF ${BUILDDIR}/$*.d.T

# these are the modules to actually build
modules  = \
    ${patsubst ${SRCDIR}/%.S,%.S,${shell find ${SRCDIR} -name *.S}} \
    ${patsubst ${SRCDIR}/%.c,%.c,${shell find ${SRCDIR} -name *.c}}

# ensure that the build subdirectory exists
dirguard  = @mkdir -p ${@D}

.PHONY: all iso syms clean
.ONESHELL:

all: iso syms
iso: ${TARGETDIR}/${KERNEL_NAME}.iso
syms: ${TARGETDIR}/${KERNEL_NAME}.sym

clean:
	rm -rf ${TARGETDIR}

# builtin rules

${BUILDDIR}/%.d: ${SRCDIR}/%.c
	${dirguard}
	${CC} ${DEPFLAGS} $<
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.T > $@
	rm -f $@.T

${BUILDDIR}/%.c.o: ${SRCDIR}/%.c
	${dirguard}
	${CC} ${CFLAGS} -c $< -o $@

${BUILDDIR}/%.S.o: ${SRCDIR}/%.S
	${dirguard}
	${CC} ${CFLAGS} -c $< -o $@

# grep -v is to pull out the directives included by cpp
${BUILDDIR}/%.ld: ${SRCDIR}/%.ld.in
	${dirguard}
	${PP} ${PPFLAGS} $< | grep -v '^#' > $@

# kernel
${TARGETDIR}/${KERNEL_NAME}: ${BUILDDIR}/link.ld ${modules:%=${BUILDDIR}/%.o}
	${LD} ${LDFLAGS} -T $^ -o $@

# debugging symbols, preprocessed for bochs
${TARGETDIR}/${KERNEL_NAME}.sym: ${TARGETDIR}/${KERNEL_NAME}
	nm $^ | awk '{print 0x$$1" "$$3 }' > $@

# initrd (minus boot stuff)
${TARGETDIR}/${KERNEL_NAME}.initrd: ${shell find ${SRCDIR}/initrd -type f}
	tar -C ${SRCDIR}/initrd -chf $@ ${patsubst ${SRCDIR}/initrd/%,%,$<}

# iso stuff

${ISODIR}/boot/kernel: ${TARGETDIR}/${KERNEL_NAME}
	cp $^ $@

${ISODIR}/boot/initrd: ${TARGETDIR}/${KERNEL_NAME}.initrd
	cp $^ $@

${TARGETDIR}/${KERNEL_NAME}.iso: ${ISODIR}/boot/kernel ${ISODIR}/boot/initrd ${ISODIR}/boot/grub/grub.cfg
	grub-mkrescue -o $@ ${ISODIR}

# include the autogen dependency targets
# wildcard them because we don't do dependency stuff for non-c files
include ${wildcard ${modules:%=${BUILDDIR}/%.d}}
