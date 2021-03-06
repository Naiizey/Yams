/**
 * @file Yams.c
 * @author GUILLOU Florian
 * @brief Projet Yams pour la SAE 1.01
 * @version 0.9
 * @date 28-11-2021
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define LONGDE 5 /** @def Permet de définir le nombre de dés */
#define MAXX 3 /** @def Permet de définir le maximum du tableau de la feuille de marque pour l'horizontale */
#define MAXY 20 /** @def Permet de définir le maximum du tableau de la feuille de marque pour la verticale */
#define MAXMOT 25 /** @def Permet de définir le maximum du tableau de la feuille de marque pour le nombre de caratères */
#define NBSUM 6 /** @def Permet de définir la valeur maximale d'un dé */

const int MAXDE = 6; // Utilisé dans la randomisation du chiffre des dés
const int NBMAXP = 2; // Constante permettant de modifier le nombre de joueurs max

typedef char t_fmarq[MAXY][MAXX][MAXMOT]; /** @typedef type permettant de définir la feuille de marque */
typedef int t_de[LONGDE]; /** @typedef  type permettant de définir les dés */

/** @fn Cette procédure permet l'affichage de la feuille de marque. */
void aff_f_marq(t_fmarq feuille_marq) {
    for (int i = 0; i < MAXY; i++) {
        for (int j = 0; j < MAXX; j++) {
            printf("%s", feuille_marq[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/** @fn Cette procédure demande au joueur un par un quel est leur nom. */
void player(char player1[20], char player2[20]) {
    for (int indice = 0; indice < NBMAXP; indice++) {
        printf("Entrez le nom du joueur %d : ", indice + 1);
        if (indice == 0) {
            scanf("%s", player1);
        }
        else {
            scanf("%s", player2);
        }
    }
}

/** @fn Cette procédure permet de déterminer quel est le joueur qui doit jouer en fonction du tour en cours */
void current_player(char player1[20], char player2[20], char curr_player[20], int tour) {
    if ((tour % 2) == 0) {
        strcpy(curr_player, player1);
    }
    else {
        strcpy(curr_player, player2);
    }
    printf("%s\n", curr_player);
}

/** @fn Cette procédure permet l'affichage des dés */
void aff_des(t_de des) {
    printf("| ");
    for (int i = 0; i < LONGDE; i++) {
        printf("%d", des[i]);
        printf(" | ");
    }
    printf("\n");
}

/** @fn Permet de trier les dés pour la procédure des combinaisons spéciales */
void trie(t_de des) {
    int tmp; /** @var variable temporaire pour le tri bulle **/ 
    for(int indice_i = 0; indice_i < LONGDE; indice_i ++) {
        for(int indice_j = 0; indice_j < LONGDE; indice_j ++) {
            if( des[indice_j] > des[indice_j + 1]) {
                tmp = des[indice_j];
                des[indice_j] = des[indice_j + 1];
                des[indice_j + 1] = tmp;
            }
        }
    }
}

/** @fn Cette procédure permet de lancer aléatoirement les dés puis de les afficher. Elle est utilsée en début de tour */
int lancer_de(t_de des) {
    srand(time(NULL));
    for (int i = 0; i < LONGDE; i++) {
        des[i] = 1 + rand() % MAXDE;
    }
    aff_des(des);
}


/**
 * @fn Cette procédure demande dans un premier temps si le joueur souhaite ou non relancer des dés. Puis selon sa réponse lui demande ou non combien de dés et enfin quels dés(de 1 à 5)
Si la réponse du joueur au moment de la demande de relance n'est pas admise, le programme reposera la question
 */
int bloq_de(t_de des) {
    int nb_de, de, try;
    char answer[4];

    srand(time(NULL));
    lancer_de(des);
    printf("\n");

    try = 0;

    while (try < 3 ^ (strcmp(answer, "yes") != 0) ^ (strcmp(answer, "y") != 0) ^ (strcmp(answer, "Yes") != 0)
        ^ (strcmp(answer, "Y") != 0)) {

        printf("Souhaitez vous garder ce lancer ? ( Yes or No ) : ");
        scanf("%s", answer);

        if ((strcmp(answer, "No") == 0) || (strcmp(answer, "N") == 0) || (strcmp(answer, "no") == 0) || (strcmp(answer, "n") == 0)) {
            printf("Il reste : %d essais\n", 3 - try);
            printf("Combien de dés souhaitez vous relancer : ");
            scanf("%d", &nb_de);
            try = try + 1;
            for (int i = 0; i < nb_de; i++) {
                printf("Sélectionnez un dé que vous voulez relancer : ");
                scanf("%d", &de);
                des[de - 1] = 1 + rand() % MAXDE;
                printf("Voici la nouvelle valeur du dé %d : %d\n", de, des[de - 1]);
            }
            printf("\n");
            printf("Vos dés sont donc : ");
            aff_des(des);
        }
    }
}

/** @fn Cette procédure permet de remettre les dés à zéro et sera exploitée à la fin de chaque tour pour assurer l'aléatoire */
void reset(t_de des) {
    for (int indice = 0; indice < LONGDE; indice++)
        des[indice] = 0;
}

/** @fn Cette procédure détermine les combinaisons de sommes possibles */
void combinaison(t_de des, int* somme_high, int* somme_mid, int* somme_low, int* compt_high, int* compt_mid, int* compt_low, bool somme6, bool somme5, bool somme4, 
                 bool somme3, bool somme2, bool somme1, int tab_combi[6][2]) {
    *somme_low = 0; *somme_mid = 0; *somme_high = 0;
    *compt_low = 0; *compt_mid = 0; *compt_high = 0;

    /** @brief Recherche de 6 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur */
    for (int indice6 = 0; indice6 < LONGDE ^ somme6 == false; indice6++) {
        if (des[indice6] == 6) {
            *compt_high = *compt_high + 1;
            *somme_high = *somme_high + des[indice6];
            tab_combi[0][0] = 6;
            tab_combi[0][1] = *somme_high;
        }
    }
    if(tab_combi[0][0] == 6){   
        printf("La combinaison SOMME DE 6 est disponible pour %d points\n", *somme_high);
        printf("Tapez 6 pour la choisir !\n");
    }

    /** @brief Recherche de 5 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur */
    if (*compt_high == 0) {
        for (int indice5 = 0; indice5 < LONGDE ^ somme5 == false; indice5++) {
            if (des[indice5] == 5) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice5];
                tab_combi[0][0] = 5;
                tab_combi[0][1] = *somme_high;
            }
        }
    }
    else {
        for (int indice5 = 0; indice5 < LONGDE ^ somme5 == false; indice5++) {
            if (des[indice5] == 5) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice5];
                tab_combi[1][0] = 5;
                tab_combi[1][1] = *somme_mid;
            }
        }
    }
    if(tab_combi[0][0] == 5){
        printf("La combinaison SOMME DE 5 est disponible pour %d points\n", *somme_high);
        printf("Tapez 5 pour la choisir !\n");
    }
    else if(tab_combi[1][0] == 5){
        printf("La combinaison SOMME DE 5 est disponible pour %d points\n", *somme_mid);
        printf("Tapez 5 pour la choisir !\n");
    }

    /** @brief Recherche de 4 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur */
    if (*compt_high == 0) {
        for (int indice4 = 0; indice4 < LONGDE ^ somme4 == false; indice4++) {
            if (des[indice4] == 4) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice4];
                tab_combi[0][0] = 4;
                tab_combi[0][1] = *somme_high;
            }
        }
    }
    else if (*compt_mid == 0) {
        for (int indice4 = 0; indice4 < LONGDE ^ somme4 == false; indice4++) {
            if (des[indice4] == 4) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice4];
                tab_combi[1][0] = 4;
                tab_combi[1][1] = *somme_mid;
            }
        }
    }
    else {
        for (int indice4 = 0; indice4 < LONGDE ^ somme4 == false; indice4++) {
            if (des[indice4] == 4) {
                *compt_low = *compt_low + 1;
                *somme_low = *somme_low + des[indice4];
                tab_combi[2][0] = 4;
                tab_combi[2][1] = *somme_low;
            }
        }
    }
    if(tab_combi[0][0] == 4){
        printf("La combinaison SOMME DE 4 est disponible pour %d points\n", *somme_high);
        printf("Tapez 4 pour la choisir !\n");
    }
    else if(tab_combi[1][0] == 4){
        printf("La combinaison SOMME DE 4 est disponible pour %d points\n", *somme_mid);
        printf("Tapez 4 pour la choisir !\n");
    }
    else if(tab_combi[2][0] == 4){
        printf("La combinaison SOMME DE 4 est disponible pour %d points\n", *somme_low);
        printf("Tapez 4 pour la choisir !\n");
    }


    /** @brief Recherche de 3 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur */
    if (*compt_high == 0) {
        for (int indice3 = 0; indice3 < LONGDE ^ somme3 == false; indice3++) {
            if (des[indice3] == 3) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice3];
                tab_combi[0][0] = 3;
                tab_combi[0][1] = *somme_high;
            }
        }
    }
    else if (*compt_mid == 0) {
        for (int indice3 = 0; indice3 < LONGDE ^ somme3 == false; indice3++) {
            if (des[indice3] == 3) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice3];
                tab_combi[1][0] = 3;
                tab_combi[1][1] = *somme_mid;
            }
        }
    }
    else if (*compt_low == 0) {
        for (int indice3 = 0; indice3 < LONGDE ^ somme3 == false; indice3++) {
            if (des[indice3] == 3) {
                *compt_low = *compt_low + 1;
                *somme_low = *somme_low + des[indice3];
                tab_combi[2][0] = 3;
                tab_combi[2][1] = *somme_low;
            }
        }
    }
    if(tab_combi[0][0] == 3){
        printf("La combinaison SOMME DE 3 est disponible pour %d points\n", *somme_high);
        printf("Tapez 3 pour la choisir !\n");
    }
    else if(tab_combi[1][0] == 3){
        printf("La combinaison SOMME DE 3 est disponible pour %d points\n", *somme_mid);
        printf("Tapez 3 pour la choisir !\n");
    }
    else if(tab_combi[2][0] == 3){
        printf("La combinaison SOMME DE 3 est disponible pour %d points\n", *somme_low);
        printf("Tapez 3 pour la choisir !\n");
    }

    /** @brief Recherche de 2 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur */
    if (*compt_high == 0) {
        for (int indice2 = 0; indice2 < LONGDE ^ somme2 == false; indice2++) {
            if (des[indice2] == 2) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice2];
                tab_combi[0][0] = 2;
                tab_combi[0][1] = *somme_high;
            }
        }
    }
    else if (*compt_mid == 0) {
        for (int indice2 = 0; indice2 < LONGDE ^ somme2 == false; indice2++) {
            if (des[indice2] == 2) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice2];
                tab_combi[1][0] = 2;
                tab_combi[1][1] = *somme_mid;
            }
        }
    }
    else if (*compt_low == 0) {
        for (int indice2 = 0; indice2 < LONGDE ^ somme2 == false; indice2++) {
            if (des[indice2] == 2) {
                *compt_low = *compt_low + 1;
                *somme_low = *somme_low + des[indice2];
                tab_combi[2][0] = 2;
                tab_combi[2][1] = *somme_low;
            }
        }
    }
    if(tab_combi[0][0] == 2){
        printf("La combinaison SOMME DE 2 est disponible pour %d points\n", *somme_high);
        printf("Tapez 2 pour la choisir !\n");
    }
    else if(tab_combi[1][0] == 2){
        printf("La combinaison SOMME DE 2 est disponible pour %d points\n", *somme_mid);
        printf("Tapez 2 pour la choisir !\n");
    }
    else if(tab_combi[2][0] == 2){
        printf("La combinaison SOMME DE 2 est disponible pour %d points\n", *somme_low);
        printf("Tapez 2 pour la choisir !\n");
    }

    /** @brief Recherche de 1 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur */
    if (*compt_high == 0) {
        for (int indice1 = 0; indice1 < LONGDE ^ somme1 == false; indice1++) {
            if (des[indice1] == 1) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice1];
                tab_combi[0][0] = 1;
                tab_combi[0][1] = *somme_high;
            }
        }
    }
    else if (*compt_mid == 0) {
        for (int indice1 = 0; indice1 < LONGDE ^ somme1 == false; indice1++) {
            if (des[indice1] == 1) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice1];
                tab_combi[1][0] = 1;
                tab_combi[1][1] = *somme_mid;
            }
        }
    }
    else if (*compt_low == 0) {
        for (int indice1 = 0; indice1 < LONGDE ^ somme1 == false; indice1++) {
            if (des[indice1] == 1) {
                *compt_low = *compt_low + 1;
                *somme_low = *somme_low + des[indice1];
                tab_combi[2][0] = 1;
                tab_combi[2][1] = *somme_low;
            }
        }
    }
    if(tab_combi[0][0] == 1){
        printf("La combinaison SOMME DE 1 est disponible pour %d points\n", *somme_high);
        printf("Tapez 1 pour la choisir !\n");
    }
    else if(tab_combi[1][0] == 1){
        printf("La combinaison SOMME DE 1 est disponible pour %d points\n", *somme_mid);
        printf("Tapez 1 pour la choisir !\n");
    }
    else if(tab_combi[2][0] == 1){
        printf("La combinaison SOMME DE 1 est disponible pour %d points\n", *somme_low);
        printf("Tapez 1 pour la choisir !\n");
    }
}

/** @fn Cette procédure détermine les combinaisons spéciales ( brelan, carre, etc...) possibles */
void combinaison_spe(t_de des, int *somme_high_spe, int *somme_mid_spe, int *somme_low_spe, int somme_high, int somme_mid, int somme_low, int compt_high, int compt_mid, 
                    int compt_low, bool brelan, bool carre, bool fullhouse, bool ptsuite, bool gdsuite, bool yams, bool chance, int tab_combi[6][2], int *somme_chance) {
    *somme_low_spe = 0; *somme_mid_spe = 0; *somme_high_spe = 0; *somme_chance = 0;
    trie(des);

    /** @brief Recherche de la présence de YAMS dans la suite de dés selon si il a déjà été choisi ou non par le joueur */
    if (compt_high == 5 && yams == true) {
        *somme_high_spe = 50;
        tab_combi[3][0] = 12;
        tab_combi[3][1] = *somme_high_spe;
    }
    if(tab_combi[3][0] == 12){
        printf("La combinaison YAMS est disponible pour %d points\n", *somme_high_spe);
        printf("Tapez 12 pour la choisir !\n");
    }

    /** @brief Recherche de la présence de GRANDE SUITE dans la suite de dés selon si il a déjà été choisi ou non par le joueur */
    if (*somme_high_spe == 0 && gdsuite == true) {
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_high_spe = 40;
            tab_combi[3][0] = 11;
            tab_combi[3][1] = *somme_high_spe;
        }
    }
    else if(gdsuite == true){
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_mid_spe =  40;
            tab_combi[4][0] = 11;
            tab_combi[4][1] = *somme_mid_spe;
        }
    }

    if(tab_combi[3][0] == 11){
        printf("La combinaison GRANDE SUITE est disponible pour %d points\n", *somme_high_spe);
        printf("Tapez 11 pour la choisir !\n");
    }
    else if(tab_combi[4][0] == 11){
        printf("La combinaison GRANDE SUITE est disponible pour %d points\n", *somme_mid_spe);
        printf("Tapez 11 pour la choisir !\n");
    }

    /** @brief Recherche de la présence de PETITE SUITE dans la suite de dés selon si il a déjà été choisi ou non par le joueur */
    if (*somme_high_spe == 0 && ptsuite == true) {
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5) ^ (des[0] == 3 && des[1] == 4 && des[2] == 5 && des[3] == 6)) {
            *somme_high_spe = 30;
            tab_combi[3][0] = 10;
            tab_combi[3][1] = *somme_high_spe;
        }
        else if ((des[1] == 1 && des[2] == 2 && des[3] == 3 && des[4] == 4) ^ (des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_high_spe = 30;
            tab_combi[3][0] = 10;
            tab_combi[3][1] = *somme_high_spe;
        }
    }
    else if (*somme_mid_spe == 0 && ptsuite == true) {
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5) ^ (des[0] == 3 && des[1] == 4 && des[2] == 5 && des[3] == 6)) {
            *somme_mid_spe = 30;
            tab_combi[4][0] = 10;
            tab_combi[4][1] = *somme_mid_spe;
        }
        else if ((des[1] == 1 && des[2] == 2 && des[3] == 3 && des[4] == 4) ^ (des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_mid_spe = 30;
            tab_combi[4][0] = 10;
            tab_combi[4][1] = *somme_mid_spe;

        }
    }
    else if (ptsuite == true) {
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5) ^ (des[0] == 3 && des[1] == 4 && des[2] == 5 && des[3] == 6)) {
            *somme_low_spe = 30;
            tab_combi[5][0] = 10;
            tab_combi[5][1] = *somme_low_spe;
        }
        else if ((des[1] == 1 && des[2] == 2 && des[3] == 3 && des[4] == 4) ^ (des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_low_spe = 30;
            tab_combi[5][0] = 10;
            tab_combi[5][1] = *somme_low_spe;
        }
    }

    if(tab_combi[3][0] == 10){
        printf("La combinaison PETITE SUITE est disponible pour %d points\n", *somme_high_spe);
        printf("Tapez 10 pour la choisir !\n");
    }
    else if(tab_combi[4][0] == 10){
        printf("La combinaison PETITE SUITE est disponible pour %d points\n", *somme_mid_spe);
        printf("Tapez 10 pour la choisir !\n");
    }
    else if(tab_combi[5][0] == 10){
        printf("La combinaison PETITE SUITE est disponible pour %d points\n", *somme_low_spe);
        printf("Tapez 10 pour la choisir !\n");
    }

    /** @brief Recherche de la présence de FULLHOUSE dans la suite de dés selon si il a déjà été choisi ou non par le joueur */
    if (*somme_high_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_high_spe = 25;
            tab_combi[3][0] = 9;
            tab_combi[3][1] = *somme_high_spe;
        }
    }
    else if (*somme_mid_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_mid_spe = 25;
            tab_combi[4][0] = 9;
            tab_combi[4][1] = *somme_mid_spe;
        }
    }
    else if (*somme_low_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_low_spe = 25;
            tab_combi[5][0] = 9;
            tab_combi[5][1] = *somme_low_spe;
        }
    }

    if(tab_combi[3][0] == 9){
        printf("La combinaison FULLHOUSE est disponible pour %d points\n", *somme_high_spe);
        printf("Tapez 9 pour la choisir !\n");
    }
    else if(tab_combi[4][0] == 9){
        printf("La combinaison FULLHOUSE est disponible pour %d points\n", *somme_mid_spe);
        printf("Tapez 9 pour la choisir !\n");
    }
    else if(tab_combi[5][0] == 9){
        printf("La combinaison FULLHOUSE est disponible pour %d points\n", *somme_low_spe);
        printf("Tapez 9 pour la choisir !\n");
    }

    /** @brief Recherche de la présence de CARRÉ dans la suite de dés selon si celui-ci a déjà été choisi par le joueur ou non */
    if (*somme_high_spe == 0 && carre == true) {
        if (compt_high == 4) {
            *somme_high_spe = somme_high;
            tab_combi[3][0] = 8;
            tab_combi[3][1] = *somme_high_spe;
        }
        else if (compt_mid == 4) {
            *somme_high_spe = somme_mid;
            tab_combi[3][0] = 8;
            tab_combi[3][1] = *somme_high_spe;
        }
    }
    else if (*somme_mid_spe == 0 && carre == true) {
        if (compt_high == 4) {
            *somme_mid_spe = somme_high;
            tab_combi[4][0] = 8;
            tab_combi[4][1] = *somme_mid_spe;
        }
        else if (compt_mid == 4) {
            *somme_mid_spe = somme_mid;
            tab_combi[4][0] = 8;
            tab_combi[4][1] = *somme_mid_spe;
        }
    }
    else if (*somme_low_spe == 0 && carre == true) {
        if (compt_high == 4) {
            *somme_low_spe = somme_high;

            tab_combi[5][0] = 8;
            tab_combi[5][1] = *somme_low_spe;
        }
        else if (compt_mid == 4) {
            *somme_low_spe = somme_mid;
            tab_combi[5][0] = 8;
            tab_combi[5][1] = *somme_low_spe;
        }
    }

    if(tab_combi[3][0] == 8){
        printf("La combinaison CARRE est disponible pour %d points\n", *somme_high_spe);
        printf("Tapez 8 pour la choisir !\n");
    }
    else if(tab_combi[4][0] == 8){
        printf("La combinaison CARRE est disponible pour %d points\n", *somme_mid_spe);
        printf("Tapez 8 pour la choisir !\n");
    }
    else if(tab_combi[5][0] == 8){
        printf("La combinaison CARRE est disponible pour %d points\n", *somme_low_spe);
        printf("Tapez 8 pour la choisir !\n");
    }

    /** @brief Recherche de la présence de BRELAN dans la suite de dés selon si il a déjà été choisi ou non par le joueur */
    if (*somme_high_spe == 0 && brelan == true) {
        if (compt_high == 3) {
            *somme_high_spe = somme_high;
            tab_combi[3][0] = 7;
            tab_combi[3][1] = *somme_high_spe;
        }
        else if (compt_mid == 3) {
            *somme_high_spe = somme_mid;
            tab_combi[3][0] = 7;
            tab_combi[3][1] = *somme_high_spe;
        }
        else if (compt_low == 3) {
            *somme_high_spe = somme_low;
            tab_combi[3][0] = 7;
            tab_combi[3][1] = *somme_high_spe;
        }
    }
    else if (*somme_mid_spe == 0 && brelan == true) {
        if (compt_high == 3) {
            *somme_mid_spe = somme_high;
            tab_combi[4][0] = 7;
            tab_combi[4][1] = *somme_mid_spe;
        }
        else if (compt_mid == 3) {
            *somme_mid_spe = somme_mid;
            tab_combi[4][0] = 7;
            tab_combi[4][1] = *somme_mid_spe;
        }
        else if (compt_low == 3) {
            *somme_mid_spe = somme_low;
            tab_combi[4][0] = 7;
            tab_combi[4][1] = *somme_mid_spe;
        }
    }
    else if (*somme_low_spe == 0 && brelan == true) {
        if (compt_high == 3) {
            *somme_low_spe = somme_high;
            tab_combi[5][0] = 7;
            tab_combi[5][1] = *somme_low_spe;
        }
        else if (compt_mid == 3) {
            *somme_low_spe = somme_mid;
            tab_combi[5][0] = 7;
            tab_combi[5][1] = *somme_low_spe;
        }
        else if (compt_low == 3) {
            *somme_low_spe = somme_low;
            tab_combi[5][0] = 7;
            tab_combi[5][1] = *somme_low_spe;
        }
    }

    if(tab_combi[3][0] == 7){
        printf("La combinaison BRELAN est disponible pour %d points\n", *somme_high_spe);
        printf("Tapez 7 pour la choisir !\n");
    }
    else if(tab_combi[4][0] == 7){
        printf("La combinaison BRELAN est disponible pour %d points\n", *somme_mid_spe);
        printf("Tapez 7 pour la choisir !\n");
    }
    else if(tab_combi[5][0] == 7){
        printf("La combinaison BRELAN est disponible pour %d points\n", *somme_low_spe);
        printf("Tapez 7 pour la choisir !\n");
    }

    /** @brief Calcule la somme des dés pour la combinaison CHANCE seulement si CHANCE n'a pas été choisi par le joueur auparavant */
    if(chance == true){
        for(int i = 0; i < LONGDE; i++){
            *somme_chance = *somme_chance + des[i];
        }
        printf("La combinaison CHANCE est disponible pour %d points\n", *somme_chance);
        printf("Tapez 13 pour la choisir !\n");
    }
}

/** @fn Cette procédure demande au joueur quelle combinaison il veut jouer */
void combinaison_tour(int *choice) {
    printf("Quelle combinaison souhaitez vous donc choisir : ");
    scanf("%d", &*choice);
}

/** @fn Cette procédure permet de faire passer les int en string afin de les ajouter à la feuille de marque */
void int_to_str(int tab_combi_int[6][2], char tab_combi_str[6][2][10], int indice_y, int indice_x){
    sprintf(tab_combi_str[indice_y][indice_x], "%d", tab_combi_int[indice_y][indice_x]);
}

/** @fn Cette procédure mets à jour la feuille de marque après le choix du joueur */
void update_feuille_marq(t_fmarq feuille_marq ,char tab_combi_str[6][2][10], int tab_combi_int[6][2], int *choice,
                         char curr_player[20], char player1[20], char player2[20], int somme_chance, char somme_chance_string[10]){
    if(strcmp(curr_player, player1) == 0) {
        switch(*choice){
            case 13:
                sprintf(somme_chance_string, "%d", somme_chance);
                strcpy(feuille_marq[2][16],somme_chance_string);
                break;
            case 12:
                strcpy(feuille_marq[2][15],tab_combi_str[3][1]);
                break;
            case 11:
                if(tab_combi_int[3][0] == 11){
                    strcpy(feuille_marq[2][14],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][1] == 11){
                    strcpy(feuille_marq[2][14],tab_combi_str[4][1]);
                    break;
                }
            case 10:
                if(tab_combi_int[3][0] == 10){
                    strcpy(feuille_marq[2][13],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][0] == 10){
                    strcpy(feuille_marq[2][13],tab_combi_str[4][1]);
                    break;
                }
                else if(tab_combi_int[5][0] == 10){
                    strcpy(feuille_marq[2][13],tab_combi_str[5][1]);
                    break;
                }
            case 9:
                if(tab_combi_int[3][0] == 9){
                    strcpy(feuille_marq[2][12],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][0] == 9){
                    strcpy(feuille_marq[2][12],tab_combi_str[4][1]);
                    break;
                }
                else if(tab_combi_int[5][0] == 9){
                    strcpy(feuille_marq[2][12],tab_combi_str[5][1]);
                    break;
                }
            case 8:
                if(tab_combi_int[3][0] == 8){
                    strcpy(feuille_marq[2][11],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][0] == 8){
                    strcpy(feuille_marq[2][11],tab_combi_str[4][1]);
                    break;
                }
                else if(tab_combi_int[5][0] == 8){
                    strcpy(feuille_marq[2][11],tab_combi_str[5][1]);
                    break;
                }
            case 7:
                if(tab_combi_int[3][0] == 7){
                    strcpy(feuille_marq[2][10],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][0] == 7){
                    strcpy(feuille_marq[2][10],tab_combi_str[4][1]);
                    break;
                }
                else if(tab_combi_int[5][0] == 7){
                    strcpy(feuille_marq[2][10],tab_combi_str[5][1]);
                    break;
                }
            case 6:
                if(tab_combi_int[0][0] == 6){
                    strcpy(feuille_marq[2][7],tab_combi_str[0][1]);
                    break;
                }
            case 5:
                if(tab_combi_int[0][0] == 5){
                    strcpy(feuille_marq[2][6],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 5){
                    strcpy(feuille_marq[2][6],tab_combi_str[1][1]);
                    break;
                }
            case 4:
                if(tab_combi_int[0][0] == 4){
                    strcpy(feuille_marq[2][5],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 4){
                    strcpy(feuille_marq[2][5],tab_combi_str[1][1]);
                    break;
                }
                else if(tab_combi_int[2][0] == 4){
                    strcpy(feuille_marq[2][5],tab_combi_str[2][1]);
                    break;
                }
            case 3:
                if(tab_combi_int[0][0] == 3){
                    strcpy(feuille_marq[2][4],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 3){
                    strcpy(feuille_marq[2][4],tab_combi_str[1][1]);
                    break;
                }
                else if(tab_combi_int[2][0] == 3){
                    strcpy(feuille_marq[2][4],tab_combi_str[2][1]);
                    break;
                }
            case 2:
                if(tab_combi_int[0][0] == 2){
                    strcpy(feuille_marq[2][3],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 2){
                    strcpy(feuille_marq[2][3],tab_combi_str[1][1]);
                    break;
                }
                else if(tab_combi_int[2][0] == 2){
                    strcpy(feuille_marq[2][3],tab_combi_str[2][1]);
                    break;
                }
            case 1:
                if(tab_combi_int[0][0] == 1){
                    strcpy(feuille_marq[2][2],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 1){
                    strcpy(feuille_marq[2][2],tab_combi_str[1][1]);
                    break;
                }
                else if(tab_combi_int[2][0] == 1){
                    strcpy(feuille_marq[2][2],tab_combi_str[2][1]);
                    break;
                }  
        }
    }
    else if(strcmp(curr_player, player2) == 0) {
        switch(*choice){
            case 13:
                sprintf(somme_chance_string, "%d", somme_chance);
                strcpy(feuille_marq[2][16],somme_chance_string);
                break;
            case 12:
                strcpy(feuille_marq[2][15],tab_combi_str[3][1]);
                break;
            case 11:
                if(tab_combi_int[3][0] == 11){
                    strcpy(feuille_marq[2][14],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][1] == 11){
                    strcpy(feuille_marq[2][14],tab_combi_str[4][1]);
                    break;
                }
            case 10:
                if(tab_combi_int[3][0] == 10){
                    strcpy(feuille_marq[2][13],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][0] == 10){
                    strcpy(feuille_marq[2][13],tab_combi_str[4][1]);
                    break;
                }
                else if(tab_combi_int[5][0] == 10){
                    strcpy(feuille_marq[2][13],tab_combi_str[5][1]);
                    break;
                }
            case 9:
                if(tab_combi_int[3][0] == 9){
                   strcpy(feuille_marq[2][12],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][0] == 9){
                    strcpy(feuille_marq[2][12],tab_combi_str[4][1]);
                    break;
                }
                else if(tab_combi_int[5][0] == 9){
                    strcpy(feuille_marq[2][12],tab_combi_str[5][1]);
                    break;
                }
            case 8:
                if(tab_combi_int[3][0] == 8){
                    strcpy(feuille_marq[2][11],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][0] == 8){
                    strcpy(feuille_marq[2][11],tab_combi_str[4][1]);
                    break;
                }
                else if(tab_combi_int[5][0] == 8){
                    strcpy(feuille_marq[2][11],tab_combi_str[5][1]);
                    break;
                }
            case 7:
                if(tab_combi_int[3][0] == 7){
                    strcpy(feuille_marq[2][10],tab_combi_str[3][1]);
                    break;
                }
                else if(tab_combi_int[4][0] == 7){
                    strcpy(feuille_marq[2][10],tab_combi_str[4][1]);
                    break;
                }
                else if(tab_combi_int[5][0] == 7){
                    strcpy(feuille_marq[2][10],tab_combi_str[5][1]);
                    break;
                }
            case 6:
                if(tab_combi_int[0][0] == 6){
                    strcpy(feuille_marq[2][7],tab_combi_str[0][1]);
                    break;
                }
            case 5:
                if(tab_combi_int[0][0] == 5){
                    strcpy(feuille_marq[2][6],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 5){
                    strcpy(feuille_marq[2][6],tab_combi_str[1][1]);
                    break;
                }
            case 4:
                if(tab_combi_int[0][0] == 4){
                    strcpy(feuille_marq[2][5],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 4){
                    strcpy(feuille_marq[2][5],tab_combi_str[1][1]);
                    break;
                }
                else if(tab_combi_int[2][0] == 4){
                    strcpy(feuille_marq[2][5],tab_combi_str[2][1]);
                    break;
                }
            case 3:
                if(tab_combi_int[0][0] == 3){
                    strcpy(feuille_marq[2][4],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 3){
                    strcpy(feuille_marq[2][4],tab_combi_str[1][1]);
                    break;
                }
                else if(tab_combi_int[2][0] == 3){
                    strcpy(feuille_marq[2][4],tab_combi_str[2][1]);
                    break;
                }
            case 2:
                if(tab_combi_int[0][0] == 2){
                    strcpy(feuille_marq[2][3],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 2){
                    strcpy(feuille_marq[2][3],tab_combi_str[1][1]);
                    break;
                }
                else if(tab_combi_int[2][0] == 2){
                    strcpy(feuille_marq[2][3],tab_combi_str[2][1]);
                    break;
                }
            case 1:
                if(tab_combi_int[0][0] == 1){
                    strcpy(feuille_marq[2][2],tab_combi_str[0][1]);
                    break;
                }
                else if(tab_combi_int[1][0] == 1){
                    strcpy(feuille_marq[2][2],tab_combi_str[1][1]);
                    break;
                }
                else if(tab_combi_int[2][0] == 1){
                    strcpy(feuille_marq[2][2],tab_combi_str[2][1]);
                    break;
                }  
        }
    }
}

/** @fn Cette procédure mets à jour les booléens à la fin de chaque tour */
void updat_bool(t_fmarq feuille_marq, char curr_player[20], char player1[20], char player2[20], bool *somme6, bool *somme5, bool *somme4, bool *somme3, bool *somme2, bool *somme1,
                bool *brelan, bool *carre, bool *fullhouse, bool *ptsuite, bool *gdsuite, bool *yams, bool *chance){
    if(strcmp(curr_player, player1) == 0){
        if(strcmp(feuille_marq[2][1],"\t\t") != 0){
            somme1 = false;
        }
        if(strcmp(feuille_marq[3][1],"\t\t") != 0){
            somme2 = false;
        }
        if(strcmp(feuille_marq[4][1],"\t\t") != 0){
            somme3 = false;
        }
        if(strcmp(feuille_marq[5][1],"\t\t") != 0){
            somme4 = false;
        }
        if(strcmp(feuille_marq[6][1],"\t\t") != 0){
            somme5 = false;
        }
        if(strcmp(feuille_marq[7][1],"\t\t") != 0){
            somme6 = false;
        }
        if(strcmp(feuille_marq[10][1],"\t") != 0){
            brelan = false;
        }
        if(strcmp(feuille_marq[11][1],"\t") != 0){
            carre = false;
        }
        if(strcmp(feuille_marq[12][1],"\t") != 0){
            fullhouse = false;
        }
        if(strcmp(feuille_marq[13][1],"\t") != 0){
            ptsuite = false;
        }
        if(strcmp(feuille_marq[14][1],"\t") != 0){
            gdsuite = false;
        }
        if(strcmp(feuille_marq[15][1],"\t") != 0){
            yams = false;
        }
        if(strcmp(feuille_marq[16][1],"\t") != 0){
            chance = false;
        }
    }
    else if(strcmp(curr_player, player1) == 0){
        if(strcmp(feuille_marq[2][2],"\t\t") != 0){
            somme1 = false;
        }
        if(strcmp(feuille_marq[3][2],"\t\t") != 0){
            somme2 = false;
        }
        if(strcmp(feuille_marq[4][2],"\t\t") != 0){
            somme3 = false;
        }
        if(strcmp(feuille_marq[5][2],"\t\t") != 0){
            somme4 = false;
        }
        if(strcmp(feuille_marq[6][2],"\t\t") != 0){
            somme5 = false;
        }
        if(strcmp(feuille_marq[7][2],"\t\t") != 0){
            somme6 = false;
        }
        if(strcmp(feuille_marq[10][2],"\t") != 0){
            brelan = false;
        }
        if(strcmp(feuille_marq[11][2],"\t") != 0){
            carre = false;
        }
        if(strcmp(feuille_marq[12][2],"\t") != 0){
            fullhouse = false;
        }
        if(strcmp(feuille_marq[13][2],"\t") != 0){
            ptsuite = false;
        }
        if(strcmp(feuille_marq[14][2],"\t") != 0){
            gdsuite = false;
        }
        if(strcmp(feuille_marq[15][2],"\t") != 0){
            yams = false;
        }
        if(strcmp(feuille_marq[16][2],"\t") != 0){
            chance = false;
        }
    }
}

/** @fn Cette procédure réinitialise le tableau contenant les combinaisons du tour */
void reset_tab_combi(int tab_combi_int[6][2]){
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 2; j++){
            tab_combi_int[i][j] = 0;
        }
    }
}

int main() {
    t_fmarq feuille_marq = {
        "┌-------------------","-┬---------","-┬-------┐",
        "|YAMS                ","|\t\t","|\t|",
        "|1 Total de 1        ","|\t\t","|\t|",
        "|2 Total de 2        ","|\t\t","|\t|",
        "|3 Total de 3        ","|\t\t","|\t|",
        "|4 Total de 4        ","|\t\t","|\t|",
        "|5 Total de 5        ","|\t\t","|\t|",
        "|6 Total de 6        ","|\t\t","|\t|",
        "|Bonus si > à 62 [35]","|\t\t","|\t|",
        "|Total supérieur     ","|\t\t","|\t|",
        "|7 Brelan            ","|\t\t","|\t|",
        "|8 Carré             ","|\t\t","|\t|",
        "|9 FullHouse         ","|\t\t","|\t|",
        "|10 Petite Suite     ","|\t\t","|\t|",
        "|11 Grande Suite     ","|\t\t","|\t|",
        "|12 YAMS             ","|\t\t","|\t|",
        "|13 Chance           ","|\t\t","|\t|",
        "|Total inférieur     ","|\t\t","|\t|",
        "|Total               ","|\t\t","|\t|",
        "└-------------------","-┴---------","-┴-------┘",
    };

    char player1[20], player2[20], curr_player[20];

    t_de des;

    int somme_high, somme_mid, somme_low;
    int compt_high, compt_mid, compt_low;

    int somme_high_spe, somme_mid_spe, somme_low_spe, somme_chance;

    int p_choice;

    bool somme6_p1, somme5_p1, somme4_p1, somme3_p1, somme2_p1, somme1_p1;
    bool somme6_p2, somme5_p2, somme4_p2, somme3_p2, somme2_p2, somme1_p2;

    bool brelan_p1, carre_p1, fullhouse_p1, ptsuite_p1, gdsuite_p1, yams_p1, chance_p1;
    bool brelan_p2, carre_p2, fullhouse_p2, ptsuite_p2, gdsuite_p2, yams_p2, chance_p2;

    somme6_p1 = true; somme5_p1 = true; somme4_p1 = true; somme3_p1 = true; somme2_p1 = true; somme1_p1 = true;
    somme6_p2 = true; somme5_p2 = true; somme4_p2 = true; somme3_p2 = true; somme2_p2 = true; somme1_p2 = true;
    
    brelan_p1 = true; carre_p1 = true; fullhouse_p1 = true; ptsuite_p1 = true; gdsuite_p1 = true; yams_p1 = true; chance_p1 = true;
    brelan_p2 = true; carre_p2 = true; fullhouse_p2 = true; ptsuite_p2 = true; gdsuite_p2 = true; yams_p2 = true; chance_p2 = true;

    int tab_combi_int[6][2];
    char tab_combi_str[6][2][10], somme_chance_string[10];

    player(player1, player2);
    aff_f_marq(feuille_marq);

    /** @brief Cette boucle permet de mettre en route le jeu, et de créer les tours */
    for(int i = 0; i < 26; i++){
        current_player(player1, player2, curr_player, i);
        if(strcmp(curr_player, player1) == 0){
            bloq_de(des);

            combinaison(des, &somme_high, &somme_mid, &somme_low, &compt_high, &compt_mid, &compt_low, somme6_p1, somme5_p1, somme4_p1, somme3_p1, somme2_p1, somme1_p1, 
                        tab_combi_int);
            combinaison_spe(des, &somme_high_spe, &somme_mid_spe, &somme_low_spe, somme_high, somme_mid, somme_low, compt_high, compt_mid, compt_low, brelan_p1, carre_p1, 
                            fullhouse_p1, ptsuite_p1, gdsuite_p1, yams_p1, chance_p1, tab_combi_int, &somme_chance);
            combinaison_tour(&p_choice);

            int_to_str(tab_combi_int, tab_combi_str, p_choice, 1);

            update_feuille_marq(feuille_marq, tab_combi_str, tab_combi_int, &p_choice, curr_player, player1, player2, somme_chance, somme_chance_string);

            aff_f_marq(feuille_marq);
            updat_bool(feuille_marq, curr_player, player1, player2, &somme6_p1, &somme5_p1, &somme4_p1, &somme3_p1, &somme2_p1, &somme1_p1, 
                       &brelan_p1, &carre_p1, &fullhouse_p1, &ptsuite_p1, &gdsuite_p1, &yams_p1, &chance_p1);

            reset_tab_combi(tab_combi_int);
            reset(des);
        }
        else if(strcmp(curr_player, player2) == 0){
            bloq_de(des);

            combinaison(des, &somme_high, &somme_mid, &somme_low, &compt_high, &compt_mid, &compt_low, somme6_p2, somme5_p2, somme4_p2, somme3_p2, somme2_p2, somme1_p2, 
                        tab_combi_int);
            combinaison_spe(des, &somme_high_spe, &somme_mid_spe, &somme_low_spe, somme_high, somme_mid, somme_low, compt_high, compt_mid, compt_low, brelan_p2, carre_p2, 
                            fullhouse_p2, ptsuite_p2, gdsuite_p2, yams_p2, chance_p2, tab_combi_int, &somme_chance);
            combinaison_tour(&p_choice);

            int_to_str(tab_combi_int, tab_combi_str, p_choice, 1);

            update_feuille_marq(feuille_marq, tab_combi_str, tab_combi_int, &p_choice, curr_player, player1, player2, somme_chance, somme_chance_string);

            aff_f_marq(feuille_marq);
            updat_bool(feuille_marq, curr_player, player1, player2, &somme6_p2, &somme5_p2, &somme4_p2, &somme3_p2, &somme2_p2, &somme1_p2, 
                       &brelan_p2, &carre_p2, &fullhouse_p2, &ptsuite_p2, &gdsuite_p2, &yams_p2, &chance_p2);

            reset_tab_combi(tab_combi_int);
            reset(des);
        }
    }
}