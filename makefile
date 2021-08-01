project: pong_master.o listscore.o
	 cc pong_master.o listscore.o -o project -lncurses -lmenu
pong_master.o: pong_master.c listscore.h
	cc -Wall -c  pong_master.c  
listscore.o: listscore.c listscore.h
	cc -Wall -c listscore.c listscore.h
