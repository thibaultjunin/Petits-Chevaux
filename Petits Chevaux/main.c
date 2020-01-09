#include "jeu.h"

void printTitle() {
	printf(" ____        __        __             \n");
	printf("/\\  _`\\     /\\ \\__  __/\\ \\__          \n");
	printf("\\ \\ \\L\\ \\ __\\ \\ ,_\\/\\_\\ \\ ,_\\   ____  \n");
	printf(" \\ \\ ,__/'__`\\ \\ \\/\\/\\ \\ \\ \\/  /',__\\ \n");
	printf("  \\ \\ \\/\\  __/\\ \\ \\_\\ \\ \\ \\ \\_/\\__, `\\ \n");
	printf("   \\ \\_\\ \\____\\\\ \\__\\\\ \\_\\ \\__\\/\\____/\n");
	printf("    \\/_/\\/____/ \\/__/ \\/_/\\/__/\\/___/ \n");
	printf("                                      \n");
	printf("                                      \n");
	printf(" ____     __                                                 \n");
	printf("/\\  _`\\  /\\ \\                                                \n");
	printf("\\ \\ \\/\\_\\\\ \\ \\___      __   __  __     __     __  __  __  _  \n");
	printf(" \\ \\ \\/_/_\\ \\  _ `\\  /'__`\\/\\ \\/\\ \\  /'__`\\  /\\ \\/\\ \\/\\ \\/'\\ \n");
	printf("  \\ \\ \\L\\ \\\\ \\ \\ \\ \\/\\  __/\\ \\ \\_/ |/\\ \\L\\.\\_\\ \\ \\_\\ \\/>  </ \n");
	printf("   \\ \\____/ \\ \\_\\ \\_\\ \\____\\\\ \\___/ \\ \\__/.\\_\\\\ \\____//\\_/\\_\\ \n");
	printf("    \\/___/   \\/_/\\/_/\\/____/ \\/__/   \\/__/\\/_/ \\/___/ \\//\\/_/\n");
}

void printEnd() {
	printf(" ______                                                       \n");
	printf("/\\  _  \\                                           __        \n");
	printf("\\ \\ \\L\\ \\  __  __      _ __    __   __  __    ___ /\\_\\  _ __  \n");
	printf(" \\ \\  __ \\/\\ \\/\\ \\    /\\`'__\\/'__`\\/\\ \\/\\ \\  / __`\\/\\ \\/\\`'__\\ \n");
	printf("  \\ \\ \\/\\ \\ \\ \\_\\ \\   \\ \\ \\//\\  __/\\ \\ \\_/ |/\\ \\L\\ \\ \\ \\ \\ \\/ \n");
	printf("   \\ \\_\\ \\_\\ \\____/    \\ \\_\\\\ \\____\\\\ \\___/ \\ \\____/\\ \\_\\ \\_\\ \n");
	printf("    \\/_/\\/_/\\/___/      \\/_/ \\/____/ \\/__/   \\/___/  \\/_/\\/_/ \n");
}

void printOver() {
	printf("           _____          __  __ ______    ______      ________ _____  \n");
	printf("          / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\ \n");
	printf("         | |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |\n");
	printf("         | | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  / \n");
	printf("         | |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\ \n");
	printf("          \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\\n");
	printf("                                                                       \n");
}

void printRules() {
	printf(" ____                    ___                    \n");
	printf("/\\  _`\\                 /\\_ \\                   \n");
	printf("\\ \\ \\L\\ \\     __     __ \\//\\ \\      __    ____  \n");
	printf(" \\ \\ ,  /   /'__`\\ /'_ `\\ \\ \\ \\   /'__`\\ /',__\\ \n");
	printf("  \\ \\ \\\\ \\ /\\  __//\\ \\L\\ \\ \\_\\ \\_/\\  __//\\__, `\\\n");
	printf("   \\ \\_\\ \\_\\ \\____\\ \\____ \\/\\____\\ \\____\\/\\____/\n");
	printf("    \\/_/\\/ /\\/____/\\/___L\\ \\/____/\\/____/\\/___/ \n");
	printf("                     /\\____/                    \n");
	printf("                     \\_/__/                     \n");
}

void printPause() {
	printf("                    ____                                    \n");
	printf("                   /\\  _`\\                                  \n");
	printf("                   \\ \\ \\L\\ \\ __     __  __    ____     __   \n");
	printf("                    \\ \\ ,__/'__`\\  /\\ \\/\\ \\  /',__\\  /'__`\\ \n");
	printf("                     \\ \\ \\/\\ \\L\\.\\_\\ \\ \\_\\ \\/\\__, `\\/\\  __/ \n");
	printf("                      \\ \\_\\ \\__/.\\_\\\\ \\____/\\/\\____/\\ \\____\\ \n");
	printf("                       \\/_/\\/__/\\/_/ \\/___/  \\/___/  \\/____/ \n");
	printf("                                                            \n");
}


int main(int argc, char* argv[]) {
	srand(time(NULL)); // https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c // C4244

	return menu();
}

int menu() {
	char* input;
	clearScreen();
	printTitle();
	if (DEBUG == 1) {
		printf("\n\033[31;103mDEBUG MODE\033[0m");
	}
	char* con = "\n\t[2] Continuer la derniere partie";
	if (!fileExists(SAVE_FILE)) {
		con = "\n\t\033[90m[2] Continuer la derniere partie\033[0m";
	}
	printf("\n\nBienvenue! \n\nVoici les options a votre disposition:\n\t[1] Nouvelle Partie%s\n\t[3] Afficher les regles\n\t[4] Quitter\n\nFaites votre choix: ", con);
	input = getLine();
	while (strcmp(input, "1") && strcmp(input, "2") && strcmp(input, "3") && strcmp(input, "4")) {
		printf("Je ne reconnait pas cette option... Veuillez reessayer:");
		free(input);
		input = getLine();
	}
	if (strncmp(input, "4", 1) == 0) {
		clearScreen();
		printEnd();
		free(input);
		return 0;
	}
	else if (strncmp(input, "3", 1) == 0) {
		free(input);
		clearScreen();
		printRules();
		printf("\n\nDeux, trois ou quatre joueurs disposent de deux pions-chevaux, (quelquefois trois, voire quatre). Pour sortir de l'ecurie, il faut faire un 6 avec le de. Le parcours se fait d'abord sur la peripherie, celle-ci est commune a tous les joueurs. Lorsqu'un cheval arrive sur une case occupee par un concurrent, il le renvoie dans son ecurie (le depart). Le jeu se joue avec un seul de, les chevaux circulent dans le sens des aiguilles d'une montre. Un 6 permet de rejouer.\n\nApres avoir fait un tour complet, le joueur doit faire le chiffre exact pour s'arreter devant son escalier. Si le chiffre est trop grand, il avance jusqu'a la case puis recule du nombre de cases correspondant a la difference entre le chiffre a faire et le chiffre fait.\n\nUne fois que le joueur se trouve dans la case situee juste devant l'escalier de sa couleur, il doit remonter marche par marche jusqu'au centre du jeu pour rentrer a l'ecurie. Pour cela, le joueur doit obtenir a chaque fois le chiffre exact inscrit dans la case, et encore un 6 pour arriver a la coupe (il suffit qu'un seul cheval y arrive pour faire gagner le joueur).\n\nChacun est libre de faire sortir le nombre de chevaux qu'il desire mais il ne peut jouer qu'un seul cheval par tour, d'ou la strategie du jeu.\n\n\t- Wikipedia (https://fr.wikipedia.org/wiki/Jeu_des_petits_chevaux)\n\n\n");
		pause();
		menu();
	}
	else if (strncmp(input, "1", 1) == 0) {
		// New Game
		free(input);
		newGame();
	}
	else if (strncmp(input, "2", 1) == 0) {
		// Load Game
		free(input);
		restartGame();
	}
	return 0;
}