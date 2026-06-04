# name of the program (Minix service)
PROG=proj

# source code files to be compiled

SRCS = proj.c queue.c ui.c sprite.c init.c font.c menu.c menu_draw.c mouse_trail.c sushi.c game.c rtc.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -I../lab1 -I../lab2 -I../lab3 -I../lab4 -I../lab5
CFLAGS += -I./include -I./include/devices -I./include/game

# Toggle debug logging globally (uncomment to enable)
# CFLAGS += -DDEBUG_LOG

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf
LDADD += -L../lab1 -L../lab2 -L../lab3 -L../lab4 -L../lab5
LDADD += -lmouse -lkbd -ltimer -lgraphics

.PATH: ${.CURDIR}/src ${.CURDIR}/src/ui ${.CURDIR}/../lab1

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
