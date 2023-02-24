PROGNAME = main.c
OBG = main
SHNAME = proga.sh

#Program compile (C)
compile: $(PROGNAME)
	gcc -O -o $(OBG) $(PROGNAME)
#Execute the program (C)
start: $(OBG)
	./$(OBG)
#Execute the program (Shell)
shstart: $(SHNAME)
	sh $(SHNAME)

