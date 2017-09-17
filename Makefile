.POSIX:
.SUFFIXES:

CC := gcc
CFLAGS := -c -std=c99 -pedantic -Wall
LDLIBS := -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lm -ldmatrix
RM := rm -f
MKDIR := mkdir -p
NAME := sub_life

all: obj/ $(NAME)

obj/:
	$(MKDIR) obj/

$(NAME): obj/sub_life.o
	$(CC) $^ $(LDLIBS) -o $@

obj/sub_life.o: src/sub_life.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY:
clean:
	$(RM) $(NAME) obj/*.o

