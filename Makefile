NAME		=	titan

CXXFLAGS	=	-I include/ -Wextra -Wall -Werror

LDFLAGS		=	-lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio

CC		=	g++

RM		=	rm -fv

SRC		=	src/main.cpp		\
			src/Plate.cpp		\
			src/Piece.cpp		\
			src/Titan.cpp		\
			src/Player.cpp		\
			src/Tree.cpp		\
			src/Arbor.cpp		\

OBJ		=	$(SRC:.cpp=.o)

all:	$(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS) $(CXXFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re:	fclean all
