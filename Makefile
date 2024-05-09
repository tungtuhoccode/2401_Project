# Directories
Build_Dir = ./build
Source_Dir = ./src
Bin_Dir = ./bin

# Targets and dependencies
TARGETS = final
OBJS = ${Build_Dir}/main.o ${Build_Dir}/hunter.o ${Build_Dir}/ghost.o ${Build_Dir}/evidence.o ${Build_Dir}/room.o ${Build_Dir}/house.o ${Build_Dir}/multithread.o ${Build_Dir}/logger.o ${Build_Dir}/utils.o
EXCUTABLE = ${Bin_Dir}/simulation

# Compiler and flags  
CC = gcc
CFLAGS = -Wall -Wextra -g

# Build rules
all: ${TARGETS}

${Build_Dir}/main.o: ${Source_Dir}/main.c
	${CC} ${CFLAGS} -c $< -o $@

${Build_Dir}/hunter.o: ${Source_Dir}/hunter.c
	${CC} ${CFLAGS} -c $< -o $@

${Build_Dir}/ghost.o: ${Source_Dir}/ghost.c
	${CC} ${CFLAGS} -c $< -o $@

${Build_Dir}/evidence.o: ${Source_Dir}/evidence.c
	${CC} ${CFLAGS} -c $< -o $@

${Build_Dir}/room.o: ${Source_Dir}/room.c
	${CC} ${CFLAGS} -c $< -o $@

${Build_Dir}/house.o: ${Source_Dir}/house.c
	${CC} ${CFLAGS} -c $< -o $@

${Build_Dir}/multithread.o: ${Source_Dir}/multithread.c
	${CC} ${CFLAGS} -c $< -o $@

${Build_Dir}/logger.o: ${Source_Dir}/logger.c
	${CC} ${CFLAGS} -c $< -o $@

${Build_Dir}/utils.o: ${Source_Dir}/utils.c
	${CC} ${CFLAGS} -c $< -o $@

final: ${OBJS}
	${CC} -o ${EXCUTABLE} ${OBJS}

# Run the executable
run: ${EXCUTABLE}
	${EXCUTABLE}

# Memory leak check
leakcheck: ${EXCUTABLE}
	valgrind --leak-check=full ./${EXCUTABLE}

# Clean up object files and executable
clean:
	rm -f ${OBJS} ${EXCUTABLE}

# Create necessary directories
directories:
	mkdir -p ${Build_Dir} ${Bin_Dir}

