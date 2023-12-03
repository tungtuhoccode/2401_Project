
TARGETS = final
OBJS = main.o hunter.o ghost.o evidence.o room.o house.o multithread.o logger.o utils.o
EXCUTABLE = project
all:		${TARGETS}
main.o: main.c
			gcc -Wall -Wextra -g -c main.c
hunter.o: hunter.c
			gcc -Wall -Wextra -g -c hunter.c
ghost.o: ghost.c
			gcc -Wall -Wextra -g -c ghost.c
evidence.o: evidence.c
			gcc -Wall -Wextra -g -c evidence.c
room.o: room.c
			gcc -Wall -Wextra -g -c room.c
house.o: house.c
			gcc -Wall -Wextra -g -c house.c
multithread.o: multithread.c
			gcc -Wall -Wextra -g -c multithread.c
logger.o: logger.c
			gcc -Wall -Wextra -g -c logger.c
utils.o: utils.c
			gcc -Wall -Wextra -g -c utils.c
final: ${OBJS}
			gcc -o project ${OBJS}
run: 
			./${EXCUTABLE} 
runWithInput: 
			./${EXCUTABLE} 
leakcheck:
			valgrind --leak-check=full ./project
clean:
			rm -f ${OBJS} ${EXCUTABLE}
