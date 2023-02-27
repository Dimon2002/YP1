PROGNAME = main.c
OBG = main
SHNAME = proga.sh
MISS = error.txt
SHERROR = sherror.txt

#Program compile (C)
compile: $(PROGNAME)
	@gcc -O -o $(OBG) $(PROGNAME)
#Execute the program (C)
start: $(OBG)
	-@./$(OBG) $(PATH) 2> $(MISS)
	@./printError $(MISS)
#Execute the program (Shell)
shstart: $(SHNAME)
	sh $(SHNAME) $(SHPATH) 2> $(SHERROR)
	@./printError $(SHERROR)
