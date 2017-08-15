AS = clang
CC = clang
PP = clang
LD = ld

SRCDIR       := src
TESTDIR      := test
TARGETDIR    := target
BUILDDIR     := build
INITRDDIR    := initrd
INCLUDEDIRS  := src

KERNEL_NAME  := nemo-x86

IFLAGS   = ${INCLUDEDIRS:%=-I%}
ASFLAGS  = ${IFLAGS}
LDFLAGS  = -m elf_i386 ${IFLAGS}
PPFLAGS  = -E -nostdinc -x c ${IFLAGS}
CFLAGS   = -target i686-pc-none-elf -march=i686
CFLAGS  += -nostdlib -ffreestanding -g
CFLAGS  += -Wall -Wextra ${IFLAGS}
DEPFLAGS = ${IFLAGS} -MT ${@:%.d=%.o} -MM -MP -MF ${BUILDDIR}/$*.d.T

# these are the modules to actually build
modules  = \
    ${patsubst ${SRCDIR}/%.S,%.S,${shell find ${SRCDIR} -name *.S}} \
    ${patsubst ${SRCDIR}/%.c,%.c,${shell find ${SRCDIR} -name *.c}}

# ensure that the build subdirectory exists
dirguard  = @mkdir -p ${@D}

.PHONY: all iso syms test clean
.ONESHELL:

all:  iso syms
iso:  ${TARGETDIR}/${KERNEL_NAME}.iso
syms: ${TARGETDIR}/${KERNEL_NAME}.sym
test: ${TESTDIR}/tests
	${TESTDIR}/tests
	echo $$?

clean:
	rm -rf ${TARGETDIR} ${BUILDDIR}

# builtin rules

${BUILDDIR}/%.d: ${SRCDIR}/%.c
	${dirguard}
	${CC} ${DEPFLAGS} $<
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.T > $@
	rm -f $@.T

${BUILDDIR}/%: ${SRCDIR}/%.in
	${dirguard}
	${PP} ${PPFLAGS} $< | grep -v '^#' > $@

${BUILDDIR}/%.c.o: ${SRCDIR}/%.c
	${dirguard}
	${CC} ${CFLAGS} -c $< -o $@

${BUILDDIR}/%.S.o: ${SRCDIR}/%.S
	${dirguard}
	${CC} ${CFLAGS} -c $< -o $@

# test targets

${TESTDIR}/tests: | ${TARGETDIR}/${KERNEL_NAME}
	${CC} ${IFLAGS}	${TESTDIR}/main.c -o $@

# targets

${TARGETDIR}/${KERNEL_NAME}: ${BUILDDIR}/link.ld ${modules:%=${BUILDDIR}/%.o}
	${dirguard}
	${LD} ${LDFLAGS} -T $^ -o $@

${TARGETDIR}/${KERNEL_NAME}.sym: ${TARGETDIR}/${KERNEL_NAME}
	${dirguard}
	nm $^ | awk '{print 0x$$1" "$$3 }' > $@

${TARGETDIR}/${KERNEL_NAME}.initrd: ${shell find ${INITRDDIR} -type f}
	${dirguard}
	tar -C ${INITRDDIR} -chf $@ ${patsubst ${INITRDDIR}/%,%,$<}

${TARGETDIR}/boot/grub/grub.cfg: ${BUILDDIR}/grub.cfg
	${dirguard}
	cp $^ $@

# the actual iso
${TARGETDIR}/${KERNEL_NAME}.iso: \
	${TARGETDIR}/boot/grub/grub.cfg \
	${TARGETDIR}/${KERNEL_NAME} \
	${TARGETDIR}/${KERNEL_NAME}.initrd
	grub-mkrescue -o $@ -m objects ${TARGETDIR}

# include the autogen dependency targets
# wildcard them because we don't do dependency stuff for non-c files
include ${wildcard ${modules:%=${BUILDDIR}/%.d}}
