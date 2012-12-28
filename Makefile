# sxkbl - toggle synaptics touchpad
# See LICENSE file for copyright and license details.

include config.mk

SRC = util.c sxkbl.c
OBJ = ${SRC:.c=.o}

all: options sxkbl

options:
	@echo sxkbl build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

sxkbl: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f sxkbl ${OBJ} sxkbl-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p sxkbl-${VERSION}
	@cp -R LICENSE Makefile README arg.h config.mk \
		sxkbl.1 ${SRC} sxkbl-${VERSION}
	@tar -cf sxkbl-${VERSION}.tar sxkbl-${VERSION}
	@gzip sxkbl-${VERSION}.tar
	@rm -rf sxkbl-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f sxkbl ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/sxkbl
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < sxkbl.1 > ${DESTDIR}${MANPREFIX}/man1/sxkbl.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/sxkbl.1

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/sxkbl
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/sxkbl.1

.PHONY: all options clean dist install uninstall
