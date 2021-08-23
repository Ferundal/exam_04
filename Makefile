# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cjettie <cjettie@21-school.ru>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#                                                      #+#    #+#              #
#                                                     ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL = /bin/sh		#for systems where SHELL variable can be inherited from environment

.SUFFIXES:			#no suffix rules are used

LIBS=

LIBS_EXT=

LIBS_DIR=			${dir ${LIBS}}

LIBS_INC_F=			-L ${dir ${LIBS}} -l${patsubst lib%.a, %, ${notdir ${LIBS}}}

LIBS_INC=			${foreach LIBS, ${LIBS}, ${LIBS_INC_F}}

LIBS_HEADERS=		${patsubst %.a, %.h, ${LIBS}}

HEADERS_MINISHELL=

HEADERS_MINISHELL_B=

HEADERS=			${LIBS_HEADERS} ${HEADERS_MINISHELL}

ifdef COMPILE_BONUS
HEADERS:=			${HEADERS} ${HEADERS_MINISHELL_B}
endif

INC_HEADERS_FORMAT=	-I ${dir ${HEADERS}}

INC_HEADERS_DIR=	${foreach HEADERS, ${HEADERS}, ${INC_HEADERS_FORMAT}}

SRCS_C=				microshell.c

SRCS_C_BONUS=

ifdef COMPILE_BONUS
SRCS_C:=			${SRCS_C} ${SRCS_C_BONUS}
endif

OBJS_S_BONUS=		${SRCS_C_BONUS:.s=.o}

OBJS_C=				${SRCS_C:.c=.o}

NAME=				minishell

CC=					cc
RM=					rm -f
LD=					ld

ALL_CFLAGS=			-Wall -Wextra -Werror -g ${INC_HEADERDIR} ${CFLAGS}
ALL_LDFLAGS=		-g ${LDFLAGS}

NORM=				norminette ${NORMO}

.PHONY:				all clean fclean re bonus libs_make libs_clean

all:				libs_make ${NAME}

libs_make:
					${foreach LIBS_DIR, ${LIBS_DIR}, ${MAKE} -C ${LIBS_DIR} bonus}

%.o:				%.c ${HEADERS}
					${CC} ${ALL_CFLAGS} ${INC_HEADERS_DIR} -c ${<} -o ${<:%.c=%.o}

${NAME}:			${OBJS_C} ${LIBS}
					${CC} ${ALL_LDFLAGS} ${OBJS_C} ${LIBS_INC} ${LIBS_EXT} -o ${NAME}

bonus:
					${MAKE} COMPILE_BONUS=1 all

#$(INSTALL_PROGRAM)=	${TEST_NAME} $(DESTDIR)$(bindir)/foo	#where executeable shoud be placed
#$(INSTALL_DATA)=	${NAME} $(DESTDIR)$(libdir)/libfoo.a		#where libs shoud be placed



clean:
					${foreach LIBS_DIR, ${LIBS_DIR}, ${MAKE} -C ${LIBS_DIR} clean}
					${RM} ${OBJS_C} ${OBJS_C_BONUS}

fclean:
					${foreach LIBS_DIR, ${LIBS_DIR}, ${MAKE} -C ${LIBS_DIR} fclean}
					${RM} ${OBJS_C} ${OBJS_C_BONUS}
					${RM} ${NAME}

norm:		
					${NORM} ${LIBS_DIR} *.c *.h

re:					fclean all

# .DELETE_ON_ERROR