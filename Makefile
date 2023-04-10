NAME = ft_irc

SRCS =	main.cpp \
		Channel.hpp \
		Commands.hpp \
		Server.cpp \
		User.cpp

OBJS = $(SRCS:.cpp=.o)

CC			= c++

RM			= rm -fr

CFLAGS		= -Wall -Wextra -Werror -std=c++98


all: $(NAME)

$(NAME): $(OBJS)
		${CC} ${CFLAGS} $(OBJS) -o $(NAME)

clean:
		${RM} $(OBJS)

fclean: clean
		${RM} ${NAME}

re:		fclean all

.PHONY:	all clean fclean re