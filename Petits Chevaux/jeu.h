#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define SAVE_FILE "gamesave.dat"
#define TRUE 1
#define FALSE 0

#define DEBUG 0 // DEBUG MODE disable new game creation, and pauses

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
#endif // _WIN32

#ifndef _JEU_H
	#define _JEU_H

	typedef struct Horse {
		int number;
		int pos;
		int hasWalked;
		int inStand;
	} Horse;

	typedef struct Player{
		char* name;
		char* color;
		int   npc;
		Horse horses[4];
	} Player;

	typedef struct Game {
		Player* players;
		int     nbPlayers;
		int     playerPlaying;
	} Game;

	void clearScreen(void);
	void pause(void);
	char* getLine(void);
	void newGame(void);
	void restartGame(void);
	void play(Game game);
	int fileExists(const char* filename);
	Game walk(Game game, int rand);
	Game walkingDeadHorse(Game game, int rand, int horse);
	Game exitHorse(Game game);
	void Watson(Game game, int rand);
	void WatsonWalk(Game game, int r);
	void youAreFreeMyFriend(Game game);

	void printOver(void);
	int menu(void);
	void printEnd(void);
	void printTitle(void);
	void printPause(void);

#endif
