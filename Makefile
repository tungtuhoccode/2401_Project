
TARGETS= main.c hunter.c room.c evidence.c house.c logger.c utils.c ghost.c 


current: 
		gcc -g -o project ${TARGETS}
later: 
		gcc -g -fsanitize=thread -pthread -o project ${TARGETS}
		
all:		${TARGETS}

# Normally we would do gcc -c p1-prime.c to create an object file
# But since these are single-file projects, we can just compile+link in one step

r: 
		./project

lc: 
		valgrind --leak-check=full ./project
main: 	main.c defs.h
			gcc -g -fsanitize=thread -pthread -o project main.c


