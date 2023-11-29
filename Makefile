
TARGETS= main.c hunter.c room.c evidence.c house.c logger.c utils.c ghost.c multithread.c


multithread: 
		-gcc -g -fsanitize=thread pthread -o project ${TARGETS}
		
singlethread: 
		gcc -g -o project ${TARGETS}

		
all:		${TARGETS}

# Normally we would do gcc -c p1-prime.c to create an object file
# But since these are single-file projects, we can just compile+link in one step

run: 
		./project

leakcheck: 
		valgrind --leak-check=full ./project

main: 	main.c defs.h
			gcc -g -fsanitize=thread -pthread -o project main.c

clean:
		rm -f ${TARGETS}

		

#TARGETS = final
#all:		${TARGETS}
#main.o: main.c
#			gcc -Wall -Wextra -Werror -g -c main.c
#hunter.o: hunter.c
#			gcc -Wall -Wextra -Werror -g -c hunter.c
#ghost.o: ghost.c
#			gcc -Wall -Wextra -Werror -g -c ghost.c
#evidence.o: evidence.c
#			gcc -Wall -Wextra -Werror -g -c evidence.c
#room.o: room.c
#			gcc -Wall -Wextra -Werror -g -c room.c
#house.o: house.c
#			gcc -Wall -Wextra -Werror -g -c house.c
#multithread.o: multithread.c
#			gcc -Wall -Wextra -Werror -g -c multithread.c
#logger.o: logger.c
#			gcc -Wall -Wextra -Werror -g -c logger.c
#utils.o: utils.c
#			gcc -Wall -Wextra -Werror -g -c utils.c
#final: main.o hunter.o ghost.o evidence.o room.o house.o multithread.o logger.o utils.o
#			gcc -o project main.o hunter.o ghost.o evidence.o room.o house.o multithread.o logger.o utils.o

#clean:
#			rm -f ${TARGETS}