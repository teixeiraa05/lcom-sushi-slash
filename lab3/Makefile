# name of the program (Minix service)
PROG=lab3 

# source code files to be compiled
SRCS = lab3.c kbc.c ../lab2/utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic
CFLAGS += -DLAB3 	# define the macro LAB3 to be used in the code to conditionally compile code specific for Lab 3

# list of library dependencies (for Lab 3, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
