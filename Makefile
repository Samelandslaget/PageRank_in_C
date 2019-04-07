CC := gcc

.PHONY : all clean

PROGRAMS = pagerank.exe test.exe time.exe
FLAGS = -Wall -fopenmp 

all: $(PROGRAMS)

pagerank.exe: PE_main_15220.c PE_functions_15220.c
	$(CC) -o $@ $^ $(FLAGS)

test.exe: PE_test_15220.c PE_functions_15220.c
	$(CC) -o $@ $^ $(FLAGS)

time.exe: PE_time_15220.c PE_functions_15220.c
	$(CC) -o $@ $^ $(FLAGS)

clean :
	rm -f *.o $(PROGRAMS)

print-% : ; @echo $* = $($*)

# all:
# 	$(CC) -o pagerank.exe PE_main_15220.c PE_functions_15220.c