#include "jeu.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif // _WIN32 
}

char* toLowerCase(char str[]) {
    int i = 0;
    while (str[i]) {
        str[i] = tolower(str[i]);
        i++;
    }
    return str;
}

void delay(int s) {
    #ifdef _WIN32
        // Sleep(s * 1000);
    #else
        sleep(s * 1000);
    #endif
}

void pause() {
    #ifdef _WIN32
        if (DEBUG == 0) {
            system("pause");
        }
    #endif // _WIN32
}

void saveGame(Game game) { // https://www.tutorialspoint.com/read-write-structure-to-a-file-using-c https://www.tutorialspoint.com/c_standard_library/c_function_fwrite.htm
    FILE* of;
    of = fopen(SAVE_FILE, "w");
    if (of == NULL) {
        return;
    }

    // Sauvegarde du nombre de joueurs
    fwrite(&game.nbPlayers, sizeof(int), 1, of);

    // Sauvegarde du joueur entrain de jouer
    fwrite(&game.playerPlaying, sizeof(int), 1, of);

    // Sauvegarde des joueurs
    for (int i = 0; i < game.nbPlayers; i++)
    {
        // Sauvegarde du nom du joueur
        int NameSize = strlen(game.players[i].name);
        fwrite(&NameSize, sizeof(int), 1, of);
        fwrite(game.players[i].name, sizeof(char), NameSize, of);

        // Sauvegarde de la couleur du joueur
        int ColorSize = strlen(game.players[i].color);
        fwrite(&ColorSize, sizeof(int), 1, of);
        fwrite(game.players[i].color, sizeof(char), ColorSize, of);

        // Sauvegarde du type de joueur
        fwrite(&game.players[i].npc, sizeof(int), 1, of);

        // Sauvegarde des chevaux du joueur
        for (int j = 0; j < 4; j++)
        {
            // Sauvegarde du numéro
            fwrite(&game.players[i].horses[j].number, sizeof(int), 1, of);

            // Sauvegarde de la position
            fwrite(&game.players[i].horses[j].pos, sizeof(int), 1, of);

            // Sauvegarde du nombre de pas
            fwrite(&game.players[i].horses[j].hasWalked, sizeof(int), 1, of);

            // Sauvegarde de l'écurie
            fwrite(&game.players[i].horses[j].inStand, sizeof(int), 1, of);
        }
    }

    fclose(of);
}

Game loadGame(Game game) { // https://www.tutorialspoint.com/read-write-structure-to-a-file-using-c https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm
    FILE* inf;

    inf = fopen(SAVE_FILE, "r");
    if (inf == NULL) {
        printf("\n\n\tImpossible d'ouvrir le fichier de sauvergarde...");
        exit(1);
    }
    
    // Ecriture du nombre de joueurs
    fread(&game.nbPlayers, sizeof(int), 1, inf);

    // Ecriture du joueur en train de jouer
    fread(&game.playerPlaying, sizeof(int), 1, inf);

    game.players = (Player*)calloc(game.nbPlayers, sizeof(Player));
    if (game.players == NULL) {
        printf("\n\n\tImpossible d'allouer la memoire necessaire...");
        exit(1);
    }

    for (int i = 0; i < game.nbPlayers; i++)
    {
        int NameSize;
        fread(&NameSize, sizeof(int), 1, inf);
        game.players[i].name = calloc(sizeof(char), NameSize+1);
        if (game.players[i].name == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        fread(game.players[i].name, sizeof(char), NameSize, inf);
        strncpy(game.players[i].name, game.players[i].name, NameSize); // https://stackoverflow.com/questions/16348512/getting-the-first-10-characters-of-a-string
        game.players[i].name[NameSize] = '\0';

        int ColorSize;
        fread(&ColorSize, sizeof(int), 1, inf);
        game.players[i].color = calloc(sizeof(char), ColorSize+1);
        if (game.players[i].color == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        fread(game.players[i].color, sizeof(char), ColorSize, inf);
        strncpy(game.players[i].color, game.players[i].color, ColorSize);
        game.players[i].color[ColorSize] = '\0';

        fread(&game.players[i].npc, sizeof(int), 1, inf);

        for (int j = 0; j < 4; j++)
        {
            fread(&game.players[i].horses[j].number, sizeof(int), 1, inf);
            fread(&game.players[i].horses[j].pos, sizeof(int), 1, inf);
            fread(&game.players[i].horses[j].hasWalked, sizeof(int), 1, inf);
            fread(&game.players[i].horses[j].inStand, sizeof(int), 1, inf);
        }
    }

    fclose(inf);
    return game;
}

int fileExists(const char* filename) { // https://www.zentut.com/c-tutorial/c-file-exists/
    FILE* file;
    if (file = fopen(filename, "r")) {
        fclose(file);
        return 1;
    }
    return 0;
}

char* getLine(void) { // Source https://stackoverflow.com/questions/314401/how-to-read-a-line-from-the-console-in-c and fixed by myself
    char* line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if (line == NULL)
        return NULL;

    for (;;) {
        c = fgetc(stdin);
        if (c == EOF || c == '\n')
            break;

        if (--len == 0) {
            len = lenmax;
            char* linen = realloc(linep, lenmax *= 2);

            if (linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if ((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;

}

char colors[4][8] = { "\033[34m", "\033[91m", "\033[93m", "\033[32m" }; // https://en.wikipedia.org/wiki/ANSI_escape_code

void newGame() {

    clearScreen();
    char NPCNames[4][8] = { "WALL-E", "X-29488", "K-2SO", "T-800" }; // https://en.wikipedia.org/wiki/List_of_fictional_robots_and_androids

    Game game;
    game.playerPlaying = 0;

    if (DEBUG == 1) {
        game.nbPlayers = 4;
        game.players = (Player*)calloc(game.nbPlayers, sizeof(Player));

        game.players[0].name = calloc(6, sizeof(char));
        if (game.players[0].name == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        strcpy(game.players[0].name, "DEBUG");

        game.players[0].color = calloc(strlen(colors[0]) + 1, sizeof(char));
        if (game.players[0].color == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        strcpy(game.players[0].color, colors[0]);

        game.players[1].name = calloc(6, sizeof(char));
        if (game.players[1].name == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        strcpy(game.players[1].name, "MODE");

        game.players[1].color = calloc(strlen(colors[1]) + 1, sizeof(char));
        if (game.players[1].color == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        strcpy(game.players[1].color, colors[1]);

        game.players[2].npc = 1;
        game.players[2].name = calloc(strlen(NPCNames[2]) + 1, sizeof(char));
        if (game.players[2].name == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        strcpy(game.players[2].name, NPCNames[2]);

        game.players[2].color = calloc(strlen(colors[2]) + 1, sizeof(char));
        if (game.players[2].color == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        strcpy(game.players[2].color, colors[2]);

        game.players[3].npc = 1;
        game.players[3].name = calloc(strlen(NPCNames[3]) + 1, sizeof(char));
        if (game.players[3].name == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        strcpy(game.players[3].name, NPCNames[3]);

        game.players[3].color = calloc(strlen(colors[3]) + 1, sizeof(char));
        if (game.players[3].color == NULL) {
            printf("\n\n\tImpossible d'allouer la memoire necessaire...");
            exit(1);
        }
        strcpy(game.players[3].color, colors[3]);

        for (int i = 0; i < game.nbPlayers; i++) {
            for (int j = 0; j < 4; j++) {
                game.players[i].horses[j].number = j + 1;
                game.players[i].horses[j].inStand = 1;
                game.players[i].horses[j].pos = -1;
            }
        }

        play(game);
        return;
    }

    char* input;
    printf("Combien de joueurs ? (Entre 2 et 4): ");
    input = getLine();
    while (strcmp(input, "2") && strcmp(input, "3") && strcmp(input, "4")) {
        printf("\nJe ne reconnait pas ce nombre... Veuillez reessayer:\n");
        printf("Combien de joueurs ? (Entre 2 et 4): ");
        free(input);
        input = getLine();
    }

    game.nbPlayers = input[0] - '0'; // https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
    game.players = (Player*)calloc(game.nbPlayers, sizeof(Player));

    free(input);

    printf("\n\nParfait, maintenant passons a la configuration des joueurs!");
    for (int i = 0; i < game.nbPlayers; i++) {
        printf("\n\nQuel est le prenom du joueur %d (Entrez \"PNJ\" pour que l'ordinateur joue): ", i+1);
        input = getLine();
        while (!strcmp(input, "")) { // Check if empty, if I've got time, will also check for only spaces
            printf("Avoir un joueur avec un nom est franchement mieux ;)\nVeuillez entrer le prenom du joueur:");
            free(input);
            input = getLine();
        }
        if (strcmp(toLowerCase(input), "pnj") == 0) {
            game.players[i].npc = 1;

            game.players[i].name = calloc(strlen(NPCNames[i]) + 1, sizeof(char));
            if (game.players[i].name == NULL) {
                printf("\n\n\tImpossible d'allouer la memoire necessaire...");
                exit(1);
            }
            strcpy(game.players[i].name, NPCNames[i]);

            game.players[i].color = calloc(strlen(colors[i]) + 1, sizeof(char));
            if (game.players[i].color == NULL) {
                printf("\n\n\tImpossible d'allouer la memoire necessaire...");
                exit(1);
            }
            strcpy(game.players[i].color, colors[i]);

            printf("Pret pour vous servir, je suis %s%s\033[0m", game.players[i].color, game.players[i].name);
            free(input);
        }
        else {

            game.players[i].color = calloc(strlen(colors[i]) + 1, sizeof(char));
            if (game.players[i].color == NULL) {
                printf("\n\n\tImpossible d'allouer la memoire necessaire...");
                exit(1);
            }
            strcpy(game.players[i].color, colors[i]);

            game.players[i].name = input;
            printf("Bienvenue %s%s\033[0m", game.players[i].color, input);
        }
    }

    printf("\n\nQue les meilleurs qui jouent!\nLaissez moi un instant je termine la configuration...");

    for (int i = 0; i < game.nbPlayers; i++) {
        for (int j = 0; j < 4; j++) {
            game.players[i].horses[j].number = j + 1;
            game.players[i].horses[j].inStand = 1;
            game.players[i].horses[j].pos = -1;
        }
    }

    saveGame(game);

    delay(4);

    play(game);

}

void restartGame() {

    if (fileExists(SAVE_FILE)) {
        Game game;
        game.nbPlayers = 0;
        game = loadGame(game);

        play(game);
    }
    else {
        
        clearScreen();
        printf("\n\n\tJe n'arrive pas a trouver la derniere sauvegarde...\n\nVoulez-vous creer une nouvelle partie? [O]ui / [N]on :\n\t");

        char* input;
        input = getLine();
        if (strncmp(toLowerCase(input), "o", 1) == 0) {
            newGame();
            return;
        }
        else {
            exit(1);
        }

    }  

    
}

int startPos[4] = { 3, 17, 31, 45 };
int endPos[4] = { 2, 16, 30, 44 };

void youAreFreeMyFriend(Game game) {
    for (int i = 0; i < game.nbPlayers; i++)
    {
        free(game.players[i].name);
        free(game.players[i].color);
    }
    free(game.players);
}

void play(Game game) {
    clearScreen();

    for (int i = 0; i < game.nbPlayers; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game.players[i].horses[j].pos == 9999) {
                printOver();
                printf("\n\n\t%s%s\033[0m gagne!\n\n\n", game.players[i].color, game.players[i].name);
                youAreFreeMyFriend(game);
                printf("\t");
                pause();
                menu();
                return;
            }
        }
    }

    char* d = "";
    if (DEBUG == 1) {
        d = "\033[31;103mDEBUG MODE\033[0m";
    }

    printf(" Ecuries: %s\n", d);
    for (int i = 0; i < game.nbPlayers; i++) {

        if (i > 0 && i < game.nbPlayers) {
            printf("\t\t");
        }
        else {
            printf("\t");
        }
        printf("%s%s\033[0m: ", game.players[i].color, game.players[i].name);
        for (int j = 0; j < 4; j++) {
            if (game.players[i].horses[j].inStand) {
                printf("%s%d\033[0m ", game.players[i].color, game.players[i].horses[j].number);
            }
        }
    }

    printf("\n\n\n ");

    /*
     * 0 = Space
     * -1 = New Line
     * Everything else = Playable position
     */
    int plate[444] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 0, 0, 3, -1,

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 56, 0, 0, 101, 0, 0, 4, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 55, 0, 0, 102, 0, 0, 5, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 103, 0, 0, 6, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 53, 0, 0, 104, 0, 0, 7, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 52, 0, 0, 105, 0, 0, 8, -1,

        45, 0, 0, 46, 0, 0, 47, 0, 0, 48, 0, 0, 49, 0, 0, 50, 0, 0, 51, 0, 0, 106, 0, 0, 9, 0, 0, 10, 0, 0, 11, 0, 0, 12, 0, 0, 13, 0, 0, 14, 0, 0, 15, -1,
        44, 0, 0, 401, 0, 0, 402, 0, 0, 403, 0, 0, 404, 0, 0, 405, 0, 0, 406, 0, 0, 9999, 0, 0, 206, 0, 0, 205, 0, 0, 204, 0, 0, 203, 0, 0, 202, 0, 0, 201, 0, 0, 16, -1,
        43, 0, 0, 42, 0, 0, 41, 0, 0, 40, 0, 0, 39, 0, 0, 38, 0, 0, 37, 0, 0, 306, 0, 0, 23, 0, 0, 22, 0, 0, 21, 0, 0, 20, 0, 0, 19, 0, 0, 18, 0, 0, 17, -1,

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 0, 0, 305, 0, 0, 24, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 304, 0, 0, 25, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 303, 0, 0, 26, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33, 0, 0, 302, 0, 0, 27, -1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 301, 0, 0, 28, -1,

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 31, 0, 0, 30, 0, 0, 29, -1
    };


    for (int i = 0; i < 444; i++)
    {
        if (plate[i] == 0) {
            printf(" ");
        }
        else if (plate[i] == -1) {
            printf("\n ");
        }
        else {
            /*
                C'EST HORRIBLE, A CORRIGER SI Y'A LE TEMPS
            */
            int chevoxOnPos = 0; // Désolé pour le nom de la variable, j'ai craqué
            int player = 0;
            int horse = 0;
            for (int k = 0; k < game.nbPlayers; k++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (game.players[k].horses[j].pos == plate[i] && !game.players[k].horses[j].inStand) {
                        chevoxOnPos = 1;
                        player = k;
                        horse = j;
                    }
                }
            }
            if (chevoxOnPos) {
                printf("%s%d\033[0m", game.players[player].color, game.players[player].horses[horse].number);
            }
            else {
                if (plate[i] > 9998) {
                    printf("%sX\033[0m", game.players[game.playerPlaying].color);
                }
                else if (plate[i] > 400) {
                    printf("\033[32m~\033[0m");
                }
                else if (plate[i] > 300) {
                    printf("\033[93m~\033[0m");
                }
                else if (plate[i] > 200) {
                    printf("\033[91m~\033[0m");
                }
                else if (plate[i] > 100) {
                    printf("\033[34m~\033[0m");
                }
                else {
                    if (plate[i] > 43 || plate[i] == 1) {
                        printf("\033[32m#\033[0m");
                    }
                    else if (plate[i] > 29 && plate[i] < 44) {
                        printf("\033[93m#\033[0m");
                    }
                    else if (plate[i] > 15 && plate[i] < 30) {
                        printf("\033[91m#\033[0m");
                    }
                    else if (plate[i] > 1 && plate[i] < 16) {
                        printf("\033[34m#\033[0m");
                    }
                }
            }
        }
    }

    delay(2);

    printf("\n\n\tC'est au tour de %s%s\033[0m !\n\t",
        game.players[game.playerPlaying].color,
        game.players[game.playerPlaying].name
    );
    delay(1);
    printf("%s%s\033[0m lance le de...\n\t", game.players[game.playerPlaying].color, game.players[game.playerPlaying].name);
    int r = (rand() % 6) + 1; // https://stackoverflow.com/questions/822323/how-to-generate-a-random-int-in-c https://www.dummies.com/programming/c/how-to-generate-random-numbers-in-c-programming/
    if (game.players[game.playerPlaying].npc == 0) {
        pause();
        printf("\tet obtient %d !\n", r);
    }
    else {
        delay(1);
        printf("et obtient %d !\n", r);
    }

    if (game.players[game.playerPlaying].npc == 1) {
        delay(1);
        printf("\t%s%s\033[0m reflechi sur son action...", game.players[game.playerPlaying].color, game.players[game.playerPlaying].name);
        delay(1);

        Watson(game, r);

        if (r < 6) {
            game.playerPlaying++;
            if (game.playerPlaying > game.nbPlayers - 1) {
                game.playerPlaying = 0;
            }
        }

        saveGame(game);
    }
    else {
        
        if (r == 6) {

            printf("\tQue voulez-vous faire %s%s\033[0m ?\n\t\t[1] Sortir un nouveau cheval\n\t\t[2] Faire avance un cheval deja sorti\n\t\t", game.players[game.playerPlaying].color, game.players[game.playerPlaying].name);
            char* input;
            input = getLine();
            while (strcmp(input, "1") && strcmp(input, "2")) {
                printf("\n\t\tJe ne reconnait pas cette option... Veuillez reessayer:\n\t\t");
                free(input);
                input = getLine();
            }
            if (!strncmp(input, "1", 1)) {
                // Sortir cheval
                game = exitHorse(game);
                free(input);
            }
            if (!strncmp(input, "2", 1)) { // C6001	Utilisation de la mémoire non initialisée 'input'. Ne comprend pas le warning...

                if (game.players[game.playerPlaying].horses[0].inStand &&
                    game.players[game.playerPlaying].horses[1].inStand &&
                    game.players[game.playerPlaying].horses[2].inStand &&
                    game.players[game.playerPlaying].horses[3].inStand) {
                    printf("\n\t%s%s\033[0m n'a aucun cheval sur le plateau ! J'en sort donc un.", game.players[game.playerPlaying].color, game.players[game.playerPlaying].name);
                    // Si on fait sortir un chevox en tapant 2. La position n'est pas bonne !
                    game = exitHorse(game);
                }else{
                    game = walk(game, r);
                }

                free(input);
                
            }

        }
        else {

            if (game.players[game.playerPlaying].horses[0].inStand &&
                game.players[game.playerPlaying].horses[1].inStand &&
                game.players[game.playerPlaying].horses[2].inStand &&
                game.players[game.playerPlaying].horses[3].inStand) {
                printf("\n\t%s%s\033[0m vous n'avez pas de cheval de sorti... Vous ne pouvez donc pas encore jouer.", game.players[game.playerPlaying].color, game.players[game.playerPlaying].name);
            }
            else {
                game = walk(game, r);
            }

            game.playerPlaying++;
            if (game.playerPlaying > game.nbPlayers-1) {
                game.playerPlaying = 0;
            }
            
        }

        
    }

    delay(3);
    play(game);

}

Game exitHorse(Game game) {

    for (int i = 0; i < 4; i++) {
        if (game.players[game.playerPlaying].horses[i].inStand) {
            game.players[game.playerPlaying].horses[i].inStand = 0;
            game.players[game.playerPlaying].horses[i].pos = startPos[game.playerPlaying];
            game.players[game.playerPlaying].horses[i].hasWalked = 0;
            printf("\n\tLe cheval %s%d\033[0m est desormais sur le plateau", game.players[game.playerPlaying].color, game.players[game.playerPlaying].horses[i].number);

            for (int k = 0; k < game.nbPlayers; k++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if (game.players[k].horses[j].pos == game.players[game.playerPlaying].horses[i].pos && k != game.playerPlaying) {
                        game.players[k].horses[j].inStand = 1;
                        game.players[k].horses[j].pos = -1;
                        printf("\n\t%s%s\033[0m a manger le cheval %s%d\033[0m de %s%s\033[0m",
                            game.players[game.playerPlaying].color,
                            game.players[game.playerPlaying].name,
                            game.players[k].color,
                            game.players[k].horses[j].number,
                            game.players[k].color,
                            game.players[k].name
                        );
                    }
                }
            }

            saveGame(game);

            return game;
        }
    }

    return game;
}

Game walk(Game game, int rand) {

    int nbOut = 0;

    if (!game.players[game.playerPlaying].horses[0].inStand) {
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[1].inStand) {
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[2].inStand) {
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[3].inStand) {
        nbOut++;
    }

    if (nbOut == 0) {

    }

    if (nbOut == 1) {

        for (int i = 0; i < 4; i++)
        {
            if (!game.players[game.playerPlaying].horses[i].inStand) {
                // walk
                game = walkingDeadHorse(game, rand, i);
            }
        }

    }
    else {

        // Si plusieurs chevaux sortie, demander quel chevox faire avancé
        printf("\n\n\tQuel cheval voulez-vous faire avancer?");
        if (!game.players[game.playerPlaying].horses[0].inStand) {
            printf("\n\t\t[1] - Le cheval numero %d", game.players[game.playerPlaying].horses[0].number);
        }
        if (!game.players[game.playerPlaying].horses[1].inStand) {
            printf("\n\t\t[2] - Le cheval numero %d", game.players[game.playerPlaying].horses[1].number);
        }
        if (!game.players[game.playerPlaying].horses[2].inStand) {
            printf("\n\t\t[3] - Le cheval numero %d", game.players[game.playerPlaying].horses[2].number);
        }
        if (!game.players[game.playerPlaying].horses[3].inStand) {
            printf("\n\t\t[4] - Le cheval numero %d", game.players[game.playerPlaying].horses[3].number);
        }
        printf("\n\t\t");
        char* input;
        input = getLine();
        while (strncmp(input, "1", 1) && strncmp(input, "2", 1) && strncmp(input, "3", 1) && strncmp(input, "4", 1)) {
            printf("\n\tJe ne reconnait pas ce cheval... Veuillez reessayer:\n\t\t");
            free(input);
            input = getLine();
        }
        while (game.players[game.playerPlaying].horses[(input[0] - '0') - 1].inStand) {
            printf("\n\tCe cheval est toujours dans l'ecurie... Veuillez choisir un cheval en jeu:\n\t\t");
            free(input);
            input = getLine();
        }

        // walk
        game = walkingDeadHorse(game, rand, (input[0] - '0') - 1);

    }

    saveGame(game);
    return game;
}

Game walkingDeadHorse(Game game, int rand, int horse) {
    // Si le joueur est sur sa case de fin
    if (game.players[game.playerPlaying].horses[horse].pos == endPos[game.playerPlaying]) {
        if (rand == 1) {
            game.players[game.playerPlaying].horses[horse].pos = ((game.playerPlaying + 1) * 100) + 1;
            printf("\n\tLe cheval %s%d\033[0m monte la premiere marche de son escalier", game.players[game.playerPlaying].color, game.players[game.playerPlaying].horses[horse].number);
            return game;
        }
        printf("\n\tLe cheval %s%d\033[0m ne bouge pas etant donne qu'il se trouve en bas de son escalier", game.players[game.playerPlaying].color, game.players[game.playerPlaying].horses[horse].number);
        return game; // Si il fait qqch d'autre que 1, il bouge pas. (non préciser sur wikipedia)
    }

    // Si le joueur est sur un escalier
    if (game.players[game.playerPlaying].horses[horse].pos > 100) {

        int cleanedPos = game.players[game.playerPlaying].horses[horse].pos - ((game.playerPlaying + 1) * 100);
        // Si le joueur fait le chiffre exacte pour passer a la case suivante
        if (cleanedPos + 1 == rand) {
            game.players[game.playerPlaying].horses[horse].pos = game.players[game.playerPlaying].horses[horse].pos + 1;
            printf("\n\tLe cheval %s%d\033[0m monte une marche de son escalier", game.players[game.playerPlaying].color, game.players[game.playerPlaying].horses[horse].number);
            return game;
        }
        // Si le joueur est sur la dernière case de l'escalier et qu'il fait 6, il gagne
        if (cleanedPos == 6) {
            if (rand == 6) {
                game.players[game.playerPlaying].horses[horse].pos = 9999;
                return game;
            }
        }

        printf("\n\tLe cheval %s%d\033[0m ne bouge pas sur son escalier", game.players[game.playerPlaying].color, game.players[game.playerPlaying].horses[horse].number);
        return game;

    }

    // Calcul de la prochaine position
    int newTheoricalPosition = game.players[game.playerPlaying].horses[horse].pos + rand;
    if (newTheoricalPosition > 56) {
        newTheoricalPosition = (newTheoricalPosition - 56); // On utilise donc, le dépassement comme nouvelle position
    }

    printf("\n\tLe cheval %s%d\033[0m avance de %d cases", game.players[game.playerPlaying].color, game.players[game.playerPlaying].horses[horse].number, rand);


    if(newTheoricalPosition > endPos[game.playerPlaying] &&
        game.players[game.playerPlaying].horses[horse].hasWalked+rand > 55
        ){

        printf("... mais il recule de %d cases", (newTheoricalPosition - endPos[game.playerPlaying]));

        newTheoricalPosition = newTheoricalPosition - ((newTheoricalPosition - endPos[game.playerPlaying]) *2);
        if (newTheoricalPosition < 1) {
            newTheoricalPosition = 56 + newTheoricalPosition;
        }
        int walked = newTheoricalPosition - startPos[game.playerPlaying];
        if (walked < 0) {
            walked += 56;
        }
        game.players[game.playerPlaying].horses[horse].hasWalked = walked - rand;
    }

    game.players[game.playerPlaying].horses[horse].pos = newTheoricalPosition; // On defini la nouvelle position du chevox

    game.players[game.playerPlaying].horses[horse].hasWalked += rand; // On ajoute le nombre de case a celui qui a été marché

    for (int i = 0; i < game.nbPlayers; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (game.players[i].horses[j].pos == game.players[game.playerPlaying].horses[horse].pos && !(i == game.playerPlaying && j == horse)) {
                game.players[i].horses[j].inStand = 1;
                game.players[i].horses[j].pos = -1;
                printf("\n\t%s%s\033[0m a manger le cheval %s%d\033[0m de %s%s\033[0m",
                    game.players[game.playerPlaying].color,
                    game.players[game.playerPlaying].name,
                    game.players[i].color,
                    game.players[i].horses[j].number,
                    game.players[i].color,
                    game.players[i].name
                );
            }
        }
    }

    return game;
}

void Watson(Game game, int rand) { // https://fr.wikipedia.org/wiki/Watson_(intelligence_artificielle)

    int nbOut = 0;

    if (!game.players[game.playerPlaying].horses[0].inStand) {
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[1].inStand) {
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[2].inStand) {
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[3].inStand) {
        nbOut++;
    }

    if (rand == 6) {

        for (int k = 0; k < 4; k++)
        {
            if (game.players[game.playerPlaying].horses[k].pos == (((game.playerPlaying + 1) * 100) + 6) ||
                game.players[game.playerPlaying].horses[k].pos == (((game.playerPlaying + 1) * 100) + 5)) {

                walkingDeadHorse(game, rand, k);
                return;

            }
        }

        for (int i = 0; i < game.nbPlayers; i++)
        {
            if (i != game.playerPlaying) {
                for (int j = 0; j < 4; j++)
                {
                    if (game.players[i].horses[j].pos == startPos[game.playerPlaying]) {
                        exitHorse(game);
                        return;
                    }
                }
            }
        }


        for (int j = 0; j < 4; j++)
        {
            if (game.players[game.playerPlaying].horses[j].pos == startPos[game.playerPlaying]) {
                walkingDeadHorse(game, rand, j);
                return;
            }
        }

        if (nbOut == 4) {
            WatsonWalk(game, rand);
            return;
        }

        int inRange = 0;
        for (int r = startPos[game.playerPlaying]; r < startPos[game.playerPlaying]+6; r++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (game.players[game.playerPlaying].horses[j].pos == r) {
                    inRange = 1;
                }
            }
        }

        if (!inRange && nbOut < 4) {
            exitHorse(game);
            return;
        }

        WatsonWalk(game, rand);

    }
    else {

        WatsonWalk(game, rand);

    }

}

void WatsonWalk(Game game, int r) {
    int nbOut = 0;
    int horses[4] = {0};

    if (!game.players[game.playerPlaying].horses[0].inStand) {
        horses[nbOut] = 0;
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[1].inStand) {
        horses[nbOut] = 1;
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[2].inStand) {
        horses[nbOut] = 2;
        nbOut++;
    }
    if (!game.players[game.playerPlaying].horses[3].inStand) {
        horses[nbOut] = 3;
        nbOut++;
    }

    if (nbOut == 0) {
        printf("\n\t%s%s\033[0m n'a pas de cheval de sorti... Il ne peut donc pas encore jouer.", game.players[game.playerPlaying].color, game.players[game.playerPlaying].name);
        return;
    }

    if (nbOut == 1) {
        walkingDeadHorse(game, r, horses[0]);
        return;
    }

    for (int k = 0; k < 4; k++)
    {

        if (game.players[game.playerPlaying].horses[k].inStand) {
            continue;
        }

        if (game.players[game.playerPlaying].horses[k].pos == endPos[game.playerPlaying] && r == 1) {
            walkingDeadHorse(game, r, k);
            return;
        }

        if (game.players[game.playerPlaying].horses[k].pos > 100) {

            if (((game.players[game.playerPlaying].horses[k].pos - ((game.playerPlaying + 1) * 100))) + 1 == r) {
                walkingDeadHorse(game, r, k);
                return;
            }

        }

    }

    for (int k = 0; k < 4; k++) // Second loop to prioritise stairs
    {

        if (game.players[game.playerPlaying].horses[k].inStand) {
            continue;
        }

        int posi = game.players[game.playerPlaying].horses[k].pos+r;
        if (posi > 56) {
            posi = (posi - 56);
        }

        if (posi == endPos[game.playerPlaying]) {
            walkingDeadHorse(game, r, k);
            return;
        }

        for (int i = 0; i < game.nbPlayers; i++)
        {
            if (i != game.playerPlaying) {
                for (int j = 0; j < 4; j++)
                {
                    if (posi == game.players[i].horses[j].pos) {
                        walkingDeadHorse(game, r, k);
                        return;
                    }
                }
            }
            
        }

    }

    int moveableHorses[4] = { 0 };
    int tju = 0;
    if (!game.players[game.playerPlaying].horses[0].inStand && game.players[game.playerPlaying].horses[0].pos < 100) {
        moveableHorses[tju] = 0;
        tju++;
    }
    if (!game.players[game.playerPlaying].horses[1].inStand && game.players[game.playerPlaying].horses[1].pos < 100) {
        moveableHorses[tju] = 1;
        tju++;
    }
    if (!game.players[game.playerPlaying].horses[2].inStand && game.players[game.playerPlaying].horses[2].pos < 100) {
        moveableHorses[tju] = 2;
        tju++;
    }
    if (!game.players[game.playerPlaying].horses[3].inStand && game.players[game.playerPlaying].horses[3].pos < 100) {
        moveableHorses[tju] = 3;
        tju++;
    }

    if (tju == 0) {
        walkingDeadHorse(game, r, horses[rand() % nbOut]);
        return;
    }

    walkingDeadHorse(game, r, moveableHorses[rand() % tju]);
    return;

}

/*
                  1  2  3
                  56    4
                  55    5
                  54    6
                  53    7
                  52    8
45 46 47 48 49 50 51    9  10 11 12 13 14 15
44                                        16
43 42 41 40 39 38 37    23 22 21 20 19 18 17
                  36    24
                  35    25
                  34    26
                  33    27
                  32    28
                  31 30 29
*/

/*
                  X  X  X
                  X     X
                  X     X
                  X     X
                  X     X
                  X     X
X  X  X  X  X  X  X     X  X  X  X  X  X  X
X                                         X
X  X  X  X  X  X  X     X  X  X  X  X  X  X
                  X     X
                  X     X
                  X     X
                  X     X
                  X     X
                  X  X  X
*/