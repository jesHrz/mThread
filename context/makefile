CC = gcc
LD = gcc
AR = ar
CFLAGS = -m32 -g -O2 -Wall -I. -Iinclude

AS_FILES     = $(wildcard *.S)
C_FILES      = $(wildcard *.c)
HEADER_FILES = $(wildcard *.h)
OBJ_FILES    = ${AS_FILES:.S=.o} ${C_FILES:.c=.o}

EXAMPLE_FILE = examples/context_test

LIBCONTEXT_STATIC_NAME      = libcontext.a
LIBCONTEXT_NAME 			= libcontext.so
LIBCONTEXT_SOVERSION 		= 1
LIBCONTEXT_SONAME    		= ${LIBCONTEXT_NAME}.$(LIBCONTEXT_SOVERSION)

all: ${LIBCONTEXT_STATIC_NAME} ${LIBCONTEXT_NAME}

${LIBCONTEXT_STATIC_NAME}: ${HEADER_FILES} ${OBJ_FILES}
	$(AR) rcs ${LIBCONTEXT_STATIC_NAME} ${OBJ_FILES}

${LIBCONTEXT_NAME}: ${HEADER_FILES} ${OBJ_FILES}
	$(CC) -fPIC -o ${LIBCONTEXT_NAME} -Wl,-soname,${LIBCONTEXT_SONAME} \
		-shared ${OBJ_FILES} -m32

${LIBCONTEXT_SONAME}: ${LIBCONTEXT_NAME}
	ln -sf ${LIBCONTEXT_NAME} ${LIBCONTEXT_SONAME}

examples: ${EXAMPLE_FILE}
examples/context_test: examples/context_test.c ${LIBCONTEXT_STATIC_NAME}
	$(CC) -std=gnu99 ${CFLAGS} $@.c -o $@ -L. -lcontext

.c.o:
	$(CC) -std=gnu99 -fPIC -DPIC ${CFLAGS} -c -o $@ $<

.S.o:
	$(CC) ${CFLAGS} -c -o $@ $<

clean:
	rm -f ${LIBCONTEXT_STATIC_NAME} ${LIBCONTEXT_NAME} $(OBJ_FILES) $(EXAMPLE_FILE)

.PHONY: clean
