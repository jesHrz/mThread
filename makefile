CC = gcc
LD = gcc
AR = ar
LDFLAGS = -L. -lmthread -Lcontext -lcontext -static 
CFLAGS  = -m32 -g -O2 -Wall -I. -Icontext/include # -DMTHREAD_DEBUG

C_FILES 		= $(wildcard *.c)
HEADER_FILES 	= $(wildcard *.h)
OBJ_FILES 		= ${C_FILES:.c=.o}

LIBMTHREAD_STATIC_NAME 	 	= libmthread.a
LIBMTHREAD_NAME 			= libmthread.so
LIBMTHREAD_SOVERSION 		= 1
LIBMTHREAD_SONAME 	 		= ${LIBMTHREAD_NAME}.$(LIBMTHREAD_SOVERSION)

LIBCONTEXT_STATIC_NAME   = libcontext.a
LIBCONTEXT_NAME 		 = libcontext.so
LIBCONTEXT_SONAME 		 = libcontext.so.1

EXAMPLE_FILE = examples/mthread_test

all: ${LIBMTHREAD_STATIC_NAME} ${LIBMTHREAD_NAME}

${LIBMTHREAD_STATIC_NAME}: ${HEADER_FILES} ${OBJ_FILES} context/${LIBCONTEXT_STATIC_NAME}
	$(AR) rcs ${LIBMTHREAD_STATIC_NAME} ${OBJ_FILES}

${LIBMTHREAD_NAME}: ${HEADER_FILES} ${OBJ_FILES} context/${LIBCONTEXT_SONAME}
	$(CC) -fPIC -o ${LIBMTHREAD_NAME} -Wl,-soname,${LIBMTHREAD_SONAME} -shared ${OBJ_FILES} -m32 -Lcontext -lcontext

${LIBMTHREAD_SONAME}: ${LIBMTHREAD_NAME}
	ln -sf ${LIBMTHREAD_NAME} ${LIBMTHREAD_SONAME}

examples: ${EXAMPLE_FILE}
examples/mthread_test: examples/mthread_test.c ${LIBMTHREAD_SONAME}
	$(CC) -std=gnu99 -m32 -g -Wall -I. -Icontext/include $@.c -o $@ ${LDFLAGS}

.c.o:
	$(CC) -std=gnu99 -fPIC -DPIC ${CFLAGS} -c -o $@ $<

context/${LIBCONTEXT_NAME} context/${LIBCONTEXT_STATIC_NAME}:
	(cd context; make)
context/${LIBCONTEXT_SONAME}:
	(cd context; make ${LIBCONTEXT_SONAME})

clean:
	rm -f ${LIBMTHREAD_STATIC_NAME} ${LIBMTHREAD_NAME} ${OBJ_FILES} ${EXAMPLE_FILE}
	# (cd context; make clean)
	