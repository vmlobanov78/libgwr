#	Core vars
OBJ_DIR	= obj

SRC_DIR_ARRAYS	= src/C/arrays
SRC_DIR_BUFFERS	= src/C/buffers
SRC_DIR_GTK		= src/C/gtk/GtkFastTextView
SRC_DIR_DEMO	= src/GtkFastTextView-demo
SRC_DIR_OPTIMS	= src/GtkFastTextView-optims

OBJ_DIR			= Make

OBJ_FILES_DEMO	=	libgwrc-array-data-block16.o	\
					libgwrc-array-data-block24.o	\
					libgwrc-array-data-block32.o	\
					libgwrc-array-equal-simple.o	\
					libgwrc-array-equal.o			\
					libgwrc-array-data-multi.o		\
													\
					libgwrc-fast-text-buffer.o		\
													\
					gtkfasttextview.o				\
													\
					GtkFastTextView-demo.o

OBJ_TARGETS_DEMO= $(patsubst %,${OBJ_DIR}/%,${OBJ_FILES_DEMO})

OBJ_FILES_OPTIMS=	libgwrc-array-data-block24.o	\
					libgwrc-array-equal-simple.o	\
					libgwrc-array-equal.o			\
					libgwrc-array-data-multi.o		\
													\
					libgwrc-fast-text-buffer.o		\
													\
					gtkfasttextview.o				\
													\
					GtkFastTextView-optims.o

OBJ_TARGETS_OPTIMS= $(patsubst %,${OBJ_DIR}/%,${OBJ_FILES_OPTIMS})

#	C compiler
CC=gcc

#	C compiler options & includes
CFLAGS_DEBUG_OPT	=-std=gnu99 -g -ggdb -Wno-unused-label -Wno-unused-but-set-variable
CFLAGS_RELEASE_OPT	=-std=gnu99 -O2 -Wno-unused-label -Wno-unused-but-set-variable -fno-unwind-tables -fno-dwarf2-cfi-asm
CFLAGS_GTK		=`pkg-config --cflags gtk+-3.0`
CFLAGS_INCLUDE	=-I./src

#	C link flags
LFLAGS			=`pkg-config --libs gtk+-3.0`

#	Rules ( one rule for each subdir )
$(OBJ_DIR)/%.o: $(SRC_DIR_ARRAYS)/%.c
	@echo "[CC] "$<
	@${CC} -S -o Make/$(notdir $<).s -c $< ${CFLAGS} 1>>make.log.txt 2>&1
	@${CC} -c -o $@ $< ${CFLAGS} 1>>Make/make.log.txt 2>&1

$(OBJ_DIR)/%.o:	$(SRC_DIR_BUFFERS)/%.c
	@echo "[CC] "$<
	@${CC} -c -o $@ $< ${CFLAGS} 1>>Make/make.log.txt 2>&1

$(OBJ_DIR)/%.o:	$(SRC_DIR_GTK)/%.c
	@echo "[CC] "$<
	@${CC} -c -o $@ $< ${CFLAGS} 1>>Make/make.log.txt 2>&1

$(OBJ_DIR)/%.o:	$(SRC_DIR_DEMO)/%.c
	@echo "[CC] "$<
	@${CC} -c -o $@ $< ${CFLAGS} 1>>Make/make.log.txt 2>&1

$(OBJ_DIR)/%.o:	$(SRC_DIR_OPTIMS)/%.c
	@echo "[CC] "$<
	@${CC} -c -o $@ $< ${CFLAGS} 1>>Make/make.log.txt 2>&1

#	#############################################################################
#	TARGETS
#	#############################################################################
Clean:
	@reset
	@echo "-------------------------------------------"
	@echo "Cleaning..."
	rm -f Make/*.o
	rm -f Make/*.s
	rm -f Make/GtkFastTextView-demo
	rm -f Make/GtkFastTextView-optims
	@echo "" > Make/make.log.txt
	@echo "(make logs will be put in make.log.txt)"
	@echo "-------------------------------------------"

Log:
	@echo "-------------------------------------------"
	@echo "Using CFLAGS:\""${CFLAGS}"\""
	@echo "Using LFLAGS:\""${LFLAGS}"\""
	@echo "-------------------------------------------"
#	=============================================================================
DDemo:	CFLAGS=${CFLAGS_DEBUG_OPT} ${CFLAGS_GTK} ${CFLAGS_INCLUDE}
DDemo:	Clean Log GtkFastTextViewDemo

RDemo:	CFLAGS=${CFLAGS_RELEASE_OPT} ${CFLAGS_GTK} ${CFLAGS_INCLUDE}
RDemo:	Clean Log GtkFastTextViewDemo

GtkFastTextViewDemo:	${OBJ_TARGETS_DEMO}
	gcc  -o Make/GtkFastTextView-demo ${OBJ_TARGETS_DEMO} ${LFLAGS}
#	=============================================================================
DOptims:	CFLAGS=${CFLAGS_DEBUG_OPT} ${CFLAGS_GTK} ${CFLAGS_INCLUDE}
DOptims:	Clean Log GtkFastTextViewOptims

ROptims:	CFLAGS=${CFLAGS_RELEASE_OPT} ${CFLAGS_GTK} ${CFLAGS_INCLUDE}
ROptims:	Clean Log GtkFastTextViewOptims

GtkFastTextViewOptims:	${OBJ_TARGETS_OPTIMS}
	gcc  -o Make/GtkFastTextView-optims ${OBJ_TARGETS_OPTIMS} ${LFLAGS}
