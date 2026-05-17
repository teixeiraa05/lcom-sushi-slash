# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = main.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -I../lab1 -I../lab2 -I../lab3 -I../lab4 -I../lab5
CFLAGS += -I. -I./controllers -I./game

# list of library dependencies
DPADD += ${LIBLCF}
LDADD += -llcf 
LDADD += -L../lab1 -L../lab2 -L../lab3 -L../lab4 -L../lab5  # Tell the compiler to look in the lab directories for libraries
LDADD += -ltimer -lkbd -lmouse -lgraphics


# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
