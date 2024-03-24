CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
GCC = gcc $(CFLAGS)
EXEC = hw14
OBJS =  hw14.o utility.o
VALGRIND = valgrind --tool=memcheck --leak-check=yes --verbose

$(EXEC): $(OBJS)
	$(GCC) $(OBJS) -o $(EXEC)

test: $(EXEC)
	./$(EXEC)  inputs/prideandprejudice output1 output2 output3
	diff -w output3 expected/prideandprejudice_header
	diff -w output1 expected/prideandprejudice_sorted
	diff -w output2 expected/prideandprejudice_huffman

all: $(EXEC)

memory: $(EXEC)
	$(VALGRIND) --log-file=log1 ./$(EXEC) inputs/prideandprejudice output1 output2 output3



%.o : %.c
	$(GCC) -c $< 

clean:
	/bin/rm -f *.o
	/bin/rm -f $(EXEC)
	/bin/rm -f output? log?
	clear