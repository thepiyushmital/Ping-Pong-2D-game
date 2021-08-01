/*****************************************************************************
 * PONG MASTER 2015 
 * A revamped version of the classic game pong
 *
 * Copyright (C) 2015 PIYUSH KUSHAL MITAL piyushhcool@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#define _BSB_SOURCE
#include<unistd.h>
#include<stdlib.h>
#include <menu.h>
#include<stdio.h>
#include<string.h>
#include<curses.h>
#include"listscore.h"
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 	4
char name2[16];
char name1[16];
int loader = 1;
void play_game1(int, int, int);
void highscore1();
void enter_name(int);
typedef struct  save{
	int go_balls;
	int cpu;
	int score_left;
	int score_right;
	int score_mid;
	char name1[16];
	char name2[16];	
	int space;
        int stop ,stop_1;
	int next_x[2];
	int next_y[2];
	int x[2], y[2];
	int dir1[2], dir2[2];
	int angle_1[2];
	int angle_2[2];
}save;
char *choices[] = {
                        "Multiplayer 2 Ball",
                        "Multiplayer 1 Ball",
                        "Single Player",
			"Load: Multi 2 Ball",                        
			"Load: Multi 1 Ball",                        
			"Load: Single Player",                        			
			"Highscore",
                        "Exit",
                        (char *)NULL,
                  };
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);
int main(){
	ITEM **my_items;
	int c, max_y, max_x, out_key = 1;				
	MENU *my_menu;
        WINDOW *my_menu_win, *start;
        int n_choices, go_name, load_game, i, position = 0, go_balls = 0, cpu = 0;

	/* Initialize curses */
	initscr();
	start_color();
        cbreak();
        noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_YELLOW);
	getmaxyx(stdscr, max_y, max_x);
	start = newwin( max_y, max_x, 0, 0);
	wbkgd(start, COLOR_PAIR(2));
	mvwprintw(start, max_y / 2 - 1, max_x / 2 - 17, "PongMaster 2015!!!");
	mvwprintw(start, max_y / 2 + 1, max_x / 2 - 30,"Revamped  version of the classic game pong");
	mvwprintw(start, max_y / 2 + 6, max_x / 2 - 22," Press any key to continue! ");
	wrefresh(start);
	wgetch(start);
	/* Create items */
        n_choices = ARRAY_SIZE(choices);
        my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
        for(i = 0; i < n_choices; ++i)
                my_items[i] = new_item(choices[i], NULL);

	/* Create menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
        my_menu_win = newwin(12, 40, 4,  4);
        keypad(my_menu_win, TRUE);
     
	/* Set main window and sub window */
        set_menu_win(my_menu, my_menu_win);
        set_menu_sub(my_menu, derwin(my_menu_win, 8, 38, 3, 1));

	/* Set menu mark to the string " * " */
        set_menu_mark(my_menu, " * ");

	/* Print a border around the main window and print a title */
        box(my_menu_win, 0, 0);
	print_in_middle(my_menu_win, 1, 0, 40, "Pong Master", COLOR_PAIR(2));
	mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
	mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
	mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
	refresh();
        
	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);
	
	/*Post instructions to use menu*/
	mvprintw(max_y - 1, 0,"< PRESS 'i' FOR INSTRUCTIONS >");
	mvprintw(max_y - 3, 0,"< PRESS ARROW KEYS TO MOVE UP AND DOWN >");
	mvprintw(max_y - 2, 0,"< PRESS ENTER TO SELECT >");
  	
	/*Access menu by arrow keys and select with enter*/
	while(out_key){	 
		c = wgetch(my_menu_win);
	 	switch(c){
			case 'i':
				mvprintw(max_y / 2, 0, "< INSTRUCTIONS >\n\n< 2 & 1 BALL GAME >\n< IF YOU FAIL TO HIT THE BALL, THE OPPONENT GAINS A POINT >\n< PLAYER ON RIGHT USES ARROW KEYS TO PLAY GAME >\n< PLAYER ON LEFT USES 'W' & 'S' TO PLAY GAME >\n< GAME OF 15 POINTS >\n\n< SINGLE PLAYER GAME\n< IF YOU MISS EVEN ONCE, YOU LOSE >\n< USE 'W' & 'S' FOR MOVEMENT >");
				break;
	        	case KEY_DOWN:
				position = position + 1;
				if(position > 7)
					position = 7;
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				position = position - 1;
				if(position < 0)
					position = 0;
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case 10: 
				/*Game conditions for multiplayer 2 ball game*/
				if(position == 0){
					go_balls = 2;	
					cpu = 0;
					go_name = 1;
					load_game = 0;
					enter_name(go_name);
					play_game1(go_balls, cpu, load_game);
				}

				/*Game conditions for multiplayer 1 ball game*/
				if(position == 1){
					go_balls = 1;
					cpu = 0;
					go_name = 1;
					load_game = 0;
					enter_name(go_name);
					play_game1(go_balls, cpu, load_game);
				}

				/*Game conditions for single player game*/
				if(position == 2){
					go_balls = 1;
					cpu = 1;
					load_game = 0;
					go_name = 0;
					enter_name(go_name);
					play_game1(go_balls, cpu, load_game);
				}	

				/*Function to access highscore*/
				if(position == 6){
					highscore1();
				}
			
				/*end game condition*/
				if(position == 7){
					out_key = 0;		
				}
			
				/*Load 2 ball multiplayer game last saved*/
				if(position == 3){
					load_game = 1;
					cpu = 0;
					go_balls = 1;
					play_game1(go_balls, cpu, load_game);
				}

				/*Load 1 ball multiplayer game last saved*/
				if(position == 4){
					load_game = 2;
					cpu = 0;
					go_balls = 1;
					play_game1(go_balls, cpu, load_game);
				}

				/*load 1 ball single player game last saved*/
				if(position == 5){
					load_game = 3;
					cpu = 0;
					go_balls = 1;
					play_game1(go_balls, cpu, load_game);
				}				
				break; 
			default: 
				break;
				
		}

		/*post menu everytime returning from a game*/
		box(my_menu_win, 0, 0);
		print_in_middle(my_menu_win, 1, 0, 40, "Pong Master", COLOR_PAIR(2));
		mvprintw(max_y - 1, 0,"< PRESS 'i' FOR INSTRUCTIONS >");
		mvprintw(max_y - 3, 0,"< PRESS ARROW KEYS TO MOVE UP AND DOWN >");
		mvprintw(max_y - 2, 0,"< PRESS ENTER TO SELECT >");
		mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
		mvwhline(my_menu_win, 2, 1, ACS_HLINE, 38);
		mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
		post_menu(my_menu);
                wrefresh(my_menu_win); 
	}	

	/* Unpost and free all the memory taken up */
        unpost_menu(my_menu);
        free_menu(my_menu);
        for(i = 0; i < n_choices; ++i)
                free_item(my_items[i]);
	endwin();
	return 0;
}
/*funtion to post the name of the game in the middle of the menu title bar*/
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
void play_game1(int go_balls, int cpu, int load_game){
	int q = 0,score_left = 0, counter, score_right = 0;
       	int end_num = 15, FINISH = 1;
	int char_l, char_r, color_1, color_2;
	int stop = 0,stop_1 = 0;
	int x[2] = {24, 24}, y[2] = {16, 17}, a = 25, i = -1, k = -1, j = 1;
        int max_y = 0, max_x = 0;
        int next_x[2] = {0, 0};
	int next_y[2] = {0, 0}; 
        int direction1[2] = {1, 1}, angle_1[2] = {1, 1}, angle_2[2] = {1, 1};
	int press_y = 0, count = 0;
	int direction2[2] = {1, 1}, end_game = 0;		
	int DELAY = 40000;
	int space = 1, score_mid = 0;
	int leveler1 = 25, /*leveler2 = 0,*/ level1 = 1;
	FILE *fp;
	info store;
	/*structure to define bar display*/
        typedef struct bar{
		char *cha;
		int num;
	}bar;
	bar pongbar[7];
	for(j = 0; j < 7; j++){
		pongbar[j].cha = "H";
		pongbar[j].num = j;
	}
	//starting with the initialization of ncurses  
	initscr();
        noecho();
       	start_color();
	//color pairs set
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_WHITE, COLOR_RED);
	curs_set(FALSE);
	//keypad input set to true
	keypad(stdscr, TRUE);
	//max values of x axis and y axis found
        getmaxyx(stdscr, max_y, max_x);
	/*Load game conditons*/
	if(load_game){
		save save1;
		/*if load game fails then program stays on the menu page itself*/
		if(load_game == 1){
			/*load 2 ball game*/
			fp = fopen("gamesave.c", "r");
			if(fp == NULL){
				attron(A_STANDOUT);
				mvprintw(max_y / 2 - 10, max_x / 2 - 10, "Game not saved or cant be loaded");
				attroff(A_STANDOUT);				
				return;
			}				
		}else if(load_game == 2){
			/*load 1 ball multiplayer game*/
			fp = fopen("gamesave1.c", "r");
			if(fp == NULL){
				attron(A_STANDOUT);
				mvprintw(max_y / 2 - 10, max_x / 2 - 10, "Game not saved or cant be loaded");
				attroff(A_STANDOUT);
				return;
			}
		}else if(load_game == 3){
			/*load 1 ball single player game*/
			fp = fopen("gamesave2.c", "r");
			if(fp == NULL){
				attron(A_STANDOUT);
				mvprintw(max_y / 2 - 10, max_x / 2 - 10, "Game not saved or cant be loaded");
				attroff(A_STANDOUT);				
				return;
			}		
		}
		/*Read from a file and store in a structure*/
		fread(&save1, 1, sizeof(save), fp);
		go_balls = save1.go_balls;
		cpu = save1.cpu;
		score_left = save1.score_left;
		score_right = save1.score_right;	
		space = save1.space; 
	        stop = save1.stop;
		stop_1 = save1.stop_1;
		for(counter = 0; counter < go_balls; counter++){
			next_x[counter] = save1.next_x[counter];
			next_y[counter] = save1.next_y[counter];
			x[counter] = save1.x[counter];
			y[counter] = save1.y[counter];
			direction1[counter] = save1.dir1[counter];
			direction2[counter] = save1.dir2[counter];
			angle_2[counter] = save1.angle_2[counter];
			angle_1[counter] = save1.angle_1[counter];
		}	
		for(counter = 0; counter < 16; counter++){
			name1[counter] = save1.name1[counter];
			name2[counter] = save1.name2[counter];
		}
		mvprintw(max_y / 2, max_x / 2 - 20, "		GAME LOADED		");
		mvprintw(max_y/2 + 1, max_x / 2 - 20, " Press 'o' to resume, esc to exit");			
	}
	if(go_balls == 1 && !cpu){
		score_left = score_left + 1;
		score_right = score_right + 1;
	}
        while(1 && loader){
		clear();
		attron(COLOR_PAIR(2));
		attroff(COLOR_PAIR(2));
		attron(COLOR_PAIR(1));
		//play area given a certain color
		for(color_1 = 5; color_1 < max_y - 4; color_1++)
			for(color_2 = a - 2; color_2 < max_x - a + 3; color_2++)
			  	mvprintw(color_1, color_2, " ");
		refresh();
		attron(COLOR_PAIR(4));

		/*General gameplay conditons*/
                mvprintw(2, 0, "Use arrow keys and (w or s) to play");
                mvprintw(3, 0, "Press esc to stop");
                mvprintw(1, 0, "Press 'p' to pause and 'o' to resume");
                mvprintw(0, 0, "Press '1' for level down and '2' for level up");
		attroff(COLOR_PAIR(4));		
		attron(COLOR_PAIR(3));
		
		/*Level and score calculation in the game*/
		if(cpu)
			mvprintw(3, max_x / 2 - 6, "LEVEL : %d", level1 / 2 + 1);
		if(!space && FINISH){
			mvprintw( max_y / 2 - 5, max_x / 2 - 13, "press space to continue");
		}
		if(!cpu){
			mvprintw(max_y - 3, max_x / 2 - 15, "%s : %d\tSCORE\t%s : %d  ", name1, score_left / 2, name2, score_right / 2);
        	}
		if(cpu){			
			mvprintw(max_y - 2, a - 2,"%s", name1); 
			mvprintw(max_y - 2, max_x / 2 - 6, "SCORE:  %d", score_mid / 2);
			mvprintw(max_y - 2, max_x - 26, "CPU");
		}
		attroff(COLOR_PAIR(3));

		/*Game over conditions*/
		if(cpu){
			if(score_right != 0)
			FINISH = 0;	
		}
		if(FINISH == 0 && cpu){		
			int counter;			

		/*score to be entered in highscore table if it comes under tp 10 scores*/			
			for(counter = 0; counter < 16; counter++)			
				store.name[counter] = name1[counter];
			store.level = level1 / 2 + 1;
			store.score = score_mid / 2;			
			includenew(store);			
			refresh();
			
		/*single player game over condition*/			
			mvprintw(max_y / 2, max_x / 2 - 5, "GAME OVER");
			mvprintw(max_y / 2 + 1, max_x / 2 - 5, "SCORE: %d", score_mid / 2);
			attron(A_STANDOUT);
			for(j = 1; j < 8; j++){				
		               	mvprintw(  13 + i + j, a - 1 , pongbar[j -1].cha);
				mvprintw(  13 + k + j, max_x - a + 1 , pongbar[j - 1].cha);
			}
			mvprintw( max_y / 2 - 5, max_x / 2 - 13, "press escape to continue");
			attroff(A_STANDOUT);
			for(q = 0; q < go_balls; q++){
				attron(COLOR_PAIR(2));
				mvprintw(y[q], x[q], "O");
				attroff(COLOR_PAIR(2));			
			}
			stop = 1;
			while(wgetch(stdscr) != 27)
				;
			end_game = 1;
		}
		attron(COLOR_PAIR(4));
		if(score_left / 2 == end_num && !cpu){
	
		/*player left game over conditons*/
			mvprintw(max_y / 2 - 1, max_x / 2 - 10, "%s won", name1);
			stop = 1;
			//mvprintw(max_y / 2 + 2, max_x / 2 - 10, "Press esc key to go back");
			attron(A_STANDOUT);
			for(j = 1; j < 8; j++){				
		               	mvprintw(  13 + i + j, a - 1 , pongbar[j -1].cha);
				mvprintw(  13 + k + j, max_x - a + 1 , pongbar[j - 1].cha);
			}
			mvprintw( max_y / 2 - 5, max_x / 2 - 13, "press escape to continue");
			attroff(A_STANDOUT);			
			for(q = 0; q < go_balls; q++){
				attron(COLOR_PAIR(2));
				mvprintw(y[q], x[q], "O");
				attroff(COLOR_PAIR(2));			
			}			
			while(wgetch(stdscr) != 27)
				;			
			end_game = 1;
		}
		if(score_right / 2 == end_num && !cpu){
			
		/*player right game over condition*/
			mvprintw(max_y / 2 - 1, max_x / 2 - 10, "%s won", name2);
			stop = 1;
			//mvprintw(max_y / 2 + 2, max_x / 2 - 10, "Press esc key to go back");
			attron(A_STANDOUT);
			for(j = 1; j < 8; j++){				
		               	mvprintw(  13 + i + j, a - 1 , pongbar[j -1].cha);
				mvprintw(  13 + k + j, max_x - a + 1 , pongbar[j - 1].cha);
			}
			attroff(A_STANDOUT);
			for(q = 0; q < go_balls; q++){
				attron(COLOR_PAIR(2));
				mvprintw(y[q], x[q], "O");
				attroff(COLOR_PAIR(2));			
			}
			attron(A_STANDOUT);
			mvprintw( max_y / 2 - 5, max_x / 2 - 13, "press escape to continue");			
			attroff(A_STANDOUT);
			while(wgetch(stdscr) != 27)
				;
			end_game = 1;
		}
		
		/*Deuce condition*/
		if(score_left / 2 == end_num - 1 && score_right / 2 == end_num - 1)
			end_num = end_num + 1;
		
		/*Game pause condition*/
		if(stop_1){
			mvprintw(max_y / 2 - 1, max_x / 2 - 10, "   GAME PAUSED    ");
			mvprintw(max_y / 2, max_x / 2 - 10,      "Want to save game?");
			mvprintw(max_y / 2 + 1, max_x / 2 - 10, " Press 'y' or 'n' ");	
		attroff(COLOR_PAIR(4));
		}
			
		/*Game save conditons*/
		if(press_y ){		
			save save1;
			save1.go_balls = go_balls;
			save1.cpu = cpu;
			save1.score_left = score_left;
			save1.score_right = score_right;	
			save1.space = space;
			save1.score_mid = score_mid;
		        save1.stop = stop;
			save1.stop_1 = stop_1;
			for(counter = 0; counter < go_balls; counter++){
				save1.next_x[counter] = next_x[counter];
				save1.next_y[counter] = next_y[counter];
				save1.x[counter] = x[counter];
				save1.y[counter] = y[counter];
				save1.dir1[counter] = direction1[counter];
				save1.dir2[counter] = direction2[counter];
				save1.angle_2[counter] = angle_2[counter];
				save1.angle_1[counter] = angle_1[counter];
			}	
			for(counter = 0; counter < 16; counter++){
				save1.name1[counter] = name1[counter];
				save1.name2[counter] = name2[counter];
			}
		/*Game save for multiplayer 1 , 2 ball and single player mode respetively*/			
			if(go_balls == 1 && !cpu){
				fp = fopen("gamesave1.c", "w");
				fwrite(&save1, 1, sizeof(save), fp);
				fclose(fp);
			}else if(go_balls == 2){
				fp = fopen("gamesave.c", "w");
				fwrite(&save1, 1, sizeof(save), fp);
				fclose(fp);
			}else if(go_balls == 1 && cpu){
				fp = fopen("gamesave2.c", "w");
				fwrite(&save1, 1, sizeof(save), fp);
				fclose(fp);
			}							
			mvprintw(max_y/2 , max_x /2 - 20,"            GAME SAVED           ");
                        mvprintw(max_y/2 + 1, max_x / 2 - 20, " Press 'o' to resume, esc to exit");
		}	
		// printing of playing bars after every iteration
		attron(A_STANDOUT);
		for(j = 1; j < 8; j++){				
                	mvprintw(  13 + i + j, a - 1 , pongbar[j -1].cha);
			mvprintw(  13 + k + j, max_x - a + 1 , pongbar[j - 1].cha);
		}
		attroff(A_STANDOUT);
		if(go_balls == 2){
			timeout(0);
			char_l = wgetch(stdscr);
		}
		if(go_balls == 1){
			if( direction1[0] == -1){
			//timeout used to eliminated time waited by wgetch for input
				timeout(0);
				char_l = wgetch(stdscr);
			}else{
				timeout(0);
				char_r = wgetch(stdscr);
			}
		}
		switch(char_l){
			/*movement of bars on left and right by arrow keys and w & s*/
			case 'w':
				if(!stop){
					i = i - 2; 
					if(i < (- 8) )
						i = (- 8);	
					attron(A_STANDOUT);
					for(j = 1; j < 8; j++)
                				mvprintw( 13 + i + j, (a - 1), pongbar[j - 1].cha);
					attroff(A_STANDOUT);
				} 
			 	break;
			case 's':	
				if(!stop){
					i = i + 2;
					if(i + 	11 > ( max_y - 15))
						i  = max_y - 26;
					attron(A_STANDOUT);
					for(j = 1; j < 8; j++)							
						mvprintw( 13 + i + j ,( a - 1), pongbar[j - 1].cha);
					attroff(A_STANDOUT);
				}
				break;
         		case KEY_UP :
				if(!stop && go_balls == 2){
					k = k - 2;
					if(k < (- 8))
						k = (- 8); 
					attron(A_STANDOUT);
					for(j = 1; j < 8; j++)
						mvprintw((13 + j + k), max_x - a + 1, pongbar[j - 1].cha);
					attroff(A_STANDOUT);
				}				
				break;
			case KEY_DOWN:
				if(!stop && go_balls == 2){
					k = k + 2;
					if(k + 11 > max_y - 15)
						k = max_y - 26;
					attron(A_STANDOUT);
					for(j = 1; j < 8; j++)
						mvprintw((13 + j + k), max_x - a + 1, pongbar[j - 1].cha);
					attroff(A_STANDOUT);
				} 
				break; 

			case 'p'://pauses game
				stop = 1;
				stop_1 = 1;			
				break;
	
			case 'y'://saves game		
				press_y = 1;
				break;
						
			case 'n'://resumes game after a 'y'  rejection	
				stop = 0;
				stop_1 = 0;
				break;

			case 'o'://resumes game
				stop = 0;
				stop_1 = 0;
				press_y = 0;
				break;
				
			case '1'://increased delay to ease level
				if(!cpu){						
					if(DELAY < 40000)
						DELAY = DELAY + 5000;
					level1 = level1 - 1;
					if(level1 == 0)
						level1 = 1; 
				}				
				break;

			case '2'://decrease delay to difficult the level	
				if(!cpu){
					if(DELAY > 15000)
						DELAY = DELAY - 5000;
					level1 = level1 + 1;
				}				
				break;

			case ' '://use space to contiue came when opponent wins a point in single player and multiplayer 1 ball games	
				space = 1;
				break;

			case 27 ://press escape to end game 
				clear();
				end_game = 1;
				break;
	
			default:
				break;
		}
		if(go_balls)
			switch(char_r){	
				/*movement of bars on left and right by arrow keys and w & s*/
				case KEY_UP :
					if(!stop && !cpu){
						k = k - 2;
						if(k < (- 8))
							k = (- 8); 
						attron(A_STANDOUT);
						for(j = 1; j < 8; j++)
							mvprintw((13 + j + k), max_x - a + 1, pongbar[j - 1].cha);
						attroff(A_STANDOUT);
					}	
					break;
				case KEY_DOWN:
					if(!stop && !cpu){
						k = k + 2;
						if(k + 11 > max_y - 15)
							k = max_y - 26;
						attron(A_STANDOUT);
						for(j = 1; j < 8; j++)
							mvprintw((13 + j + k), max_x - a + 1, pongbar[j - 1].cha);
						attroff(A_STANDOUT);
					} 
					break;

				case 'p'://pauses game 	
					stop = 1;
					stop_1 = 1;
					break;

				case 'o'://resumes game	
					stop = 0;
					stop_1 = 0;
					press_y = 0;
					break;

				case 'n'://resumes game after 'y' declined	
					stop = 0;
					stop_1 = 0;
					break;

				case '1':
					if(!cpu){
                                        	if(DELAY < 40000)
                                                	DELAY = DELAY + 1000;
						level1 = level1 - 1;
						if(level1 == 0)
							level1 = 1; 
					}					
					break;

               		        case '2':
					if(!cpu){
               	                        	if(DELAY > 15000)
                                                	DELAY = DELAY - 1000;
						level1 = level1 + 1;                                        
					}					
					break;

				case 'y'://saves game		
					press_y = 1;
					break;
			
				case ' '://resume game after each point won
					space = 1;
					break;

				case 27 ://end game condition 	
					clear();
					end_game = 1;
					break;

				default: 
					break;
		}
		attron(COLOR_PAIR(1));
		attroff(COLOR_PAIR(1));
     		refresh();
		//delay used to increase time between every iteration in while loop
       		//direction changed after the ball hits the wall of each edge, with the same angle
		for(q = 0; q < go_balls; q++){
		/*Printing the ball in the new positon after every loop ccompletion*/			
			attron(COLOR_PAIR(2));
			mvprintw(y[q], x[q], "O");
			attroff(COLOR_PAIR(2));
			if(cpu && space){			
				if( x[q] > max_x / 2 + leveler1){
					count = count + 1;
					if(count % 1 == 0){
                        	        	if(y[q] < 14 + k)
                       	                		k = k - 1;
                        	       		else if( y[q] > 20 + k)
                        	        	        k = k + 1;
                        	       		if(k < -8)
                       		         	        k = -8;
                       		      	  	if( k  > max_y - 26)
                       	                		k = max_y - 26;
                       		 	}
				}
			}
		}		
		refresh();
		//delay used to increase time between every iteration in while loop
       		usleep(DELAY);
		if((!stop || !stop_1 * FINISH) && space){
		/*Direction changed after the ball hits the wall of each edge, with the same angle
		 *For different parts of the paddle different angles are created*/
			for(q = 0; q < go_balls; q++){			
				next_x[q] = x[q] + angle_1[q] * direction1[q];
				next_y[q] = y[q] + angle_2[q] * direction2[q];
				if((y[q] == 14 + i || y[q] == 20 + i || y[q] == 14 + k || y[q] == 20 + k) && (x[q] == a || x[q] == max_x - a)){
					angle_2[q] = 1;
					angle_1[q] = 2;
				}else if((y[q] == 15 + i || y[q] == 19 + i || y[q] == 15 + k || y[q] == 19 + k) && (x[q] == a || x[q] == max_x - a)){		
					angle_2[q] = 1;
					angle_1[q] = 1;
				}else if((y[q] == 16 + i || y[q] == 18 + i || y[q] == 16 + k || y[q] == 18 + k) && (x[q] == a || x[q] == max_x - a)){
					angle_2[q] = 1;
					angle_1[q] = 2;
				}else if((y[q] == 17 + i) && (x[q] == a)){
					angle_2[q] = 0;
					angle_1[q] = 2;
				} 
				else if(( y[q] == 17 + k) && ( x[q] == max_x - a)){
					angle_2[q] = 0;
					angle_1[q] = 2;
				}else if(( y[q] > (20 + i) || y[q] < (14 + i) || y[q] > (20 + k) || y[q] < (14 + k)) && (x[q] == a || x[q] == max_x - a)){	
					angle_2[q] = 1;
					angle_1[q] = 1;
				}
				/*Basic ball bouncing algorithm
				 *Direction changes when the ball hits a wall*/
				if(next_y[q] > (max_y - 6) || next_y[q] < 6){
				 	direction2[q] = direction2[q] * -1;
				}else{ 
				 	y[q] = y[q] + angle_2[q] * direction2[q];
				}
				if (next_x[q] > (max_x - a) || next_x[q] < a){ 
        		 	 	direction1[q] = direction1[q] * -1;
					/*Automatic level increament in game against cpu*/
       			 	}else {
        		 	 	 x[q] = x[q] + angle_1[q] * direction1[q];
			   	}
			}
		}
		/*Calculating score for all three cases
		 *Bonus points for player in cpu mode everytime it beats the cpu*/
		if(FINISH && space){	
			for(q = 0; q < go_balls; q++){
				if((( y[q] > (21 + i) || y[q] < (13 + i)) && x[q] == a)){
					score_right++;
					if(go_balls != 2)				
						space = 0;
					leveler1 = 25;
				}
				if((( y[q] > (21 + k) || y[q] < (13 + k)) && x[q] == max_x - a)){ 
					if(go_balls != 2)
						space = 0;
					score_left++;
				
					/*Automatic level increament in game against cpu*/
					if(cpu){					
						DELAY = DELAY - 1000;
						if(DELAY < 20000)
							DELAY = 20000;
						leveler1 = leveler1 - 1;
						score_mid = score_mid + 5;
						level1++;
						if(level1 > 10)
							level1 = 10;
					}
				}
				if((( y[q] <= (21 + i) || y[q] >= (13 + i)) && x[q] == a)){ 			
					score_mid++;	
				}
			}
		}
		attroff(COLOR_PAIR(1));		
		if(end_game == 1)
			break;	
		refresh();	
	}	
	endwin();
	clear();
	refresh();
}	
/*Function for highscore displayed called by menu*/
void highscore1(){
	WINDOW *start1;
	initscr();
	raw();
	cbreak();
	noecho();	
	info store;
	int max_x, max_y;	
	int i = 0, n = 0;
	init_pair(2, COLOR_CYAN, COLOR_BLACK);
	getmaxyx(stdscr, max_y, max_x);
	start1 = newwin( max_y, max_x, 0, 0);
	wbkgd(start1, COLOR_PAIR(2));	
	FILE *fp;
	fp = fopen("highscore.c", "r");
	fread(&n, 1, sizeof(int), fp);
	wattron(start1, A_STANDOUT);
	mvwprintw(start1, 11, max_x / 2 - 5 , "HIGHSCORE");
	wattroff(start1, A_STANDOUT);
	wattron(start1, A_UNDERLINE);
	mvwprintw(start1, 13, max_x / 2 - 20 + 2, "SR.");	
	mvwprintw(start1, 13, max_x / 2 - 20 + 8, "NAME");
	mvwprintw(start1, 13, max_x / 2 - 20 + 22,"LEVEL");
	mvwprintw(start1, 13, max_x / 2 - 20 + 36,"SCORE");	
	wattroff(start1, A_UNDERLINE);
	for(i = 0; i < n; i++){
		fread(&store, 1, sizeof(struct info), fp);
		mvwprintw(start1, 15 + i, max_x / 2 - 20 + 2, "%d", i + 1);		
		mvwprintw(start1, 15 + i, max_x / 2 - 20 + 8, "%s", store.name);
		mvwprintw(start1, 15 + i, max_x / 2 - 20 + 22,"%d", store.level);
		mvwprintw(start1, 15 + i, max_x / 2 - 20 + 36,"%d", store.score);	
	}
	fclose(fp);
	wattron(start1, A_STANDOUT);
	mvwprintw(start1, 35, max_x / 2 - 20 + 2, "Press any key to go back");
	wattroff(start1, A_STANDOUT);
	wrefresh(start1);	
	wgetch(start1);
	endwin();
	clear();
	refresh();
	return;
}
/*Function to enter the names of the player playing the game*/
void enter_name(int go_name){
	WINDOW *start1;
	initscr();
	echo();
	int max_x, max_y;
	init_pair(2, COLOR_BLUE, COLOR_YELLOW);
	getmaxyx(stdscr, max_y, max_x);
	start1 = newwin(max_y, max_x, 0, 0);
	wbkgd(start1, COLOR_PAIR(2));
	mvwprintw(start1, max_y / 2, max_x / 2 - 15, "ENTER NAME OF PLAYER 1 : ");
	wgetstr(start1, name1);
	if(go_name){
		mvwprintw(start1, max_y / 2 + 2, max_x / 2 - 15	, "ENTER NAME OF PLAYER 2: ");
		wgetstr(start1, name2);
	}
	mvwprintw(start1, max_y / 2 + 6, max_x / 2 - 15, "Press any key to proceed");
	wgetch(start1);
	endwin();
	clear();
	refresh();
	return;
}
