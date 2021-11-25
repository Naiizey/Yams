#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define LONGDE 5
#define MAXX 3
#define MAXY 18
#define MAXMOT 22
#define NBSUM 6

const int MAXDE = 6; // Utilisé dans la randomisation du chiffre des dés
const int NBMAXP = 2; // Constante permettant de modifier le nombre de joueurs max

typedef char t_fmarq[MAXY][MAXX][MAXMOT];
typedef int t_de[LONGDE];

// Cette procédure permet l'affichage de la feuille de marque.
void aff_f_marq(t_fmarq feuille_marq) {
    for (int i = 0; i < MAXY; i++) {
        for (int j = 0; j < MAXX; j++) {
            printf("%s", feuille_marq[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Cette procédure demande au joueur un par un quel est leur nom.
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

// Cette procédure permet de déterminer quel est le joueur qui doit jouer
void current_player(char player1[20], char player2[20], char curr_player[20], int tour) {
    if ((tour % 2) == 0) {
        strcpy(curr_player, player1);
    }
    else {
        strcpy(curr_player, player2);
    }
}

// Cette procédure permet l'affichage des dés.
void aff_des(t_de des) {
    printf("| ");
    for (int i = 0; i < LONGDE; i++) {
        printf("%d", des[i]);
        printf(" | ");
    }
    printf("\n");
}

// Permet de trier les dés pour la procédure des combinaisons spéciales
void trie(t_de des) {
    int tmp;
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

// Cette procédure permet de lancer aléatoirement les dés puis de les afficher. Elle est utilsée en début de tour.
int lancer_de(t_de des) {
    srand(time(NULL));
    for (int i = 0; i < LONGDE; i++) {
        des[i] = 1 + rand() % MAXDE;
    }
    aff_des(des)
}

/* Cette procédure demande dans un premier temps si le joueur souhaite ou non relancer des dés.Puis selon sa réponse lui demande ou non combien de dés et enfin quels dés(de 1 à 5)
Si la réponse du joueur au moment de la demande de relance n'est pas admise, le programme reposera la question. */
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
            printf("Vos dés sont donc : ");
            aff_des(des);
        }
    }
    aff_des(des);
}

// Cette procédure permet de remettre les dés à zéro et sera exploitée à la fin de chaque tour pour assurer l'aléatoire
void reset(t_de des) {
    for (int indice = 0; indice < LONGDE; indice++)
        des[indice] = 0;
}


void combinaison(t_de des, int* somme_high, int* somme_mid, int* somme_low, int* compt_high, int* compt_mid, int* compt_low, bool somme6, bool somme5, bool somme4, bool somme3, bool somme2, bool somme1) {
    *somme_low = 0; *somme_mid = 0; *somme_high = 0;
    *compt_low = 0; *compt_mid = 0; *compt_high = 0;

    // Recherche de 6 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur
    for (int indice6 = 0; indice6 < LONGDE ^ somme6 == false; indice6++) {
        if (des[indice6] == 6) {
            *compt_high = *compt_high + 1;
            *somme_high = *somme_high + des[indice6];
        }
    }

    // Recherche de 5 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur
    if (*compt_high == 0) {
        for (int indice5 = 0; indice5 < LONGDE ^ somme5 == false; indice5++) {
            if (des[indice5] == 5) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice5];
            }
        }
    }
    else {
        for (int indice5 = 0; indice5 < LONGDE ^ somme5 == false; indice5++) {
            if (des[indice5] == 5) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice5];
            }
        }
    }

    // Recherche de 4 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur
    if (*compt_high == 0) {
        for (int indice4 = 0; indice4 < LONGDE ^ somme4 == false; indice4++) {
            if (des[indice4] == 4) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice4];
            }
        }
    }
    else if (*compt_mid == 0) {
        for (int indice4 = 0; indice4 < LONGDE ^ somme4 == false; indice4++) {
            if (des[indice4] == 4) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice4];
            }
        }
    }
    else {
        for (int indice4 = 0; indice4 < LONGDE ^ somme4 == false; indice4++) {
            if (des[indice4] == 4) {
                *compt_low = *compt_low + 1;
                *somme_low = *somme_low + des[indice4];
            }
        }
    }

    // Recherche de 3 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur
    if (*compt_high == 0) {
        for (int indice3 = 0; indice3 < LONGDE ^ somme3 == false; indice3++) {
            if (des[indice3] == 3) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice3];
            }
        }
    }
    else if (*compt_mid == 0) {
        for (int indice3 = 0; indice3 < LONGDE ^ somme3 == false; indice3++) {
            if (des[indice3] == 3) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice3];
            }
        }
    }
    else if (*compt_low == 0) {
        for (int indice3 = 0; indice3 < LONGDE ^ somme3 == false; indice3++) {
            if (des[indice3] == 3) {
                *compt_low = *compt_low + 1;
                *somme_low = *somme_low + des[indice3];
            }
        }
    }

    // Recherche de 2 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur
    if (*compt_high == 0) {
        for (int indice2 = 0; indice2 < LONGDE ^ somme2 == false; indice2++) {
            if (des[indice2] == 2) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice2];
            }
        }
    }
    else if (*compt_mid == 0) {
        for (int indice2 = 0; indice2 < LONGDE ^ somme2 == false; indice2++) {
            if (des[indice2] == 2) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice2];
            }
        }
    }
    else if (*compt_low == 0) {
        for (int indice2 = 0; indice2 < LONGDE ^ somme2 == false; indice2++) {
            if (des[indice2] == 2) {
                *compt_low = *compt_low + 1;
                *somme_low = *somme_low + des[indice2];
            }
        }
    }

    // Recherche de 1 dans le lancer de dés puis calcul si présence et qu'il n'a pas déjà été exploité par le joueur
    if (*compt_high == 0) {
        for (int indice1 = 0; indice1 < LONGDE ^ somme1 == false; indice1++) {
            if (des[indice1] == 1) {
                *compt_high = *compt_high + 1;
                *somme_high = *somme_high + des[indice1];
            }
        }
    }
    else if (*compt_mid == 0) {
        for (int indice1 = 0; indice1 < LONGDE ^ somme1 == false; indice1++) {
            if (des[indice1] == 1) {
                *compt_mid = *compt_mid + 1;
                *somme_mid = *somme_mid + des[indice1];
            }
        }
    }
    else if (*compt_low == 0) {
        for (int indice1 = 0; indice1 < LONGDE ^ somme1 == false; indice1++) {
            if (des[indice1] == 1) {
                *compt_low = *compt_low + 1;
                *somme_low = *somme_low + des[indice1];
            }
        }
    }
}

void combinaison_spe(t_de des, int *somme_high_spe, int *somme_mid_spe, int *somme_low_spe, int somme_high, int somme_mid, int somme_low, int compt_high, int compt_mid, int compt_low, bool brelan, bool carre, bool fullhouse, bool ptsuite, bool gdsuite, bool yams, bool chance) {
    trie(des);

    // Recherche de la présence de YAMS dans la suite de dés selon si il a déjà été choisi ou non par le joueur
    if (compt_high == 5 && yams == true) {
        *somme_high_spe == 50;
    }

    // Recherche de la présence de GRANDE SUITE dans la suite de dés selon si il a déjà été choisi ou non par le joueur
    if (*somme_high_spe == 0 && gdsuite == true) {
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_high_spe = 40;
        }
    }
    else if(gdsuite == true){
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_mid_spe =  40;
        }
    }

    // Recherche de la présence de PETITE SUITE dans la suite de dés selon si il a déjà été choisi ou non par le joueur
    if (*somme_high_spe == 0 && ptsuite == true) {
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5) ^ (des[0] == 3 && des[1] == 4 && des[2] == 5 && des[3] == 6)) {
            *somme_high_spe = 30;
        }
        else if ((des[1] == 1 && des[2] == 2 && des[3] == 3 && des[4] == 4) ^ (des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_high_spe = 30;
        }
    }
    else if (*somme_mid_spe == 0 && ptsuite == true) {
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5) ^ (des[0] == 3 && des[1] == 4 && des[2] == 5 && des[3] == 6)) {
            *somme_mid_spe = 30;
        }
        else if ((des[1] == 1 && des[2] == 2 && des[3] == 3 && des[4] == 4) ^ (des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_mid_spe = 30;
        }
    }
    else if (ptsuite == true) {
        if ((des[0] == 1 && des[1] == 2 && des[2] == 3 && des[3] == 4) ^ (des[0] == 2 && des[1] == 3 && des[2] == 4 && des[3] == 5) ^ (des[0] == 3 && des[1] == 4 && des[2] == 5 && des[3] == 6)) {
            *somme_low_spe = 30;
        }
        else if ((des[1] == 1 && des[2] == 2 && des[3] == 3 && des[4] == 4) ^ (des[1] == 2 && des[2] == 3 && des[3] == 4 && des[4] == 5) ^ (des[1] == 3 && des[2] == 4 && des[3] == 5 && des[4] == 6)) {
            *somme_low_spe = 30;
        }
    }

    // Recherche de la présence de FULLHOUSE dans la suite de dés selon si il a déjà été choisi ou non par le joueur
    if (*somme_high_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_high_spe = 25;
        }
    }
    else if (*somme_mid_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_mid_spe = 25;
        }
    }
    else if (*somme_low_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_high_spe = 25;
        }
    }
    if (*somme_high_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_high_spe = 25;
        }
    }
    else if (*somme_mid_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_mid_spe = 25;
        }
    }
    else if (*somme_low_spe == 0 && fullhouse == true) {
        if ((compt_high == 3 && compt_mid == 2) ^ (compt_high == 2 && compt_mid == 3)) {
            *somme_high_spe = 25;
        }
    }

    // Recherche de la présence de CARRÉ dans la suite de dés selon si celui-ci a déjà été choisi par le joueur ou non
    if (*somme_high_spe == 0 && carre == true) {
        if (compt_high == 4) {
            *somme_high_spe = somme_high;
        }
        else if (compt_mid == 4) {
            *somme_high_spe = somme_mid;
        }
    }
    else if (*somme_mid_spe == 0 && carre == true) {
        if (compt_high == 4) {
            *somme_mid_spe = somme_high;
        }
        else if (compt_mid == 4) {
            *somme_mid_spe = somme_mid;
        }
    }
    else if (*somme_low_spe == 0 && carre == true) {
        if (compt_high == 4) {
            *somme_low_spe = somme_high;
        }
        else if (compt_mid == 4) {
            *somme_low_spe = somme_mid;
        }
    }

    // Recherche de la présence de BRELAN dans la suite de dés selon si il a déjà été choisi ou non par le joueur
    if (*somme_high_spe == 0 && brelan == true) {
        if (compt_high == 3) {
            *somme_high_spe = somme_high;
        }
        else if (compt_mid == 3) {
            *somme_high_spe = somme_mid;
        }
        else if (compt_low == 3) {
            *somme_high_spe = somme_low;
        }
    }
    else if (*somme_mid_spe == 0 && brelan == true) {
        if (compt_high == 3) {
            *somme_mid_spe = somme_high;
        }
        else if (compt_mid == 3) {
            *somme_mid_spe = somme_mid;
        }
        else if (compt_low == 3) {
            *somme_mid_spe = somme_low;
        }
    }
    else if (*somme_low_spe == 0 && brelan == true) {
        if (compt_high == 3) {
            *somme_low_spe = somme_high;
        }
        else if (compt_mid == 3) {
            *somme_low_spe = somme_mid;
        }
        else if (compt_low == 3) {
            *somme_low_spe = somme_low;
        }
    }
}

//Cette procédure demande au joueur quelle combinaison il veut jouer
void combinaison_tour(int somme_high, int somme_mid, int somme_low, int somme_high_spe, int somme_mid_spe, int somme_low_spe, 
                      int compt_high, int compt_mid, int compt_low, int *choice) {

    if(somme_high_spe == 0){
        if(somme_mid == 0){
            printf("Seule combinaison possible : %d\n", somme_high);
        }
        else if(somme_low == 0){
            printf("Combinaison possible numéro 1 : %d\n", somme_high);
            printf("Combinaison possible numéro 2 : %d\n", somme_mid);
        }
        else{
            printf("Combinaison possible numéro 1 : %d\n", somme_high);
            printf("Combinaison possible numéro 2 : %d\n", somme_mid);
            printf("Combinaison possible numéro 3 : %d\n", somme_low);
        }
    }
    else if(somme_mid_spe == 0){
        if(somme_mid == 0){
            printf("Combinaison possible numéro 1 : %d\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_high);
        }
        else if(somme_low == 0){
            printf("Combinaison possible numéro 1 : %d\n\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_high);
            printf("Combinaison possible numéro 3 : %d\n", somme_mid);
        }
        else{
            printf("Combinaison possible numéro 1 : %d\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_high);
            printf("Combinaison possible numéro 3 : %d\n", somme_mid);
            printf("Combinaison possible numéro 4 : %d\n", somme_low);
        }
    }
    else if(somme_low_spe == 0){
        if(somme_mid == 0){
            printf("Combinaison possible numéro 1 : %d\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_mid_spe);
            printf("Combinaison possible numéro 3 : %d\n", somme_high);
        }
        else if(somme_low == 0){
            printf("Combinaison possible numéro 1 : %d\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_mid_spe);
            printf("Combinaison possible numéro 3 : %d\n", somme_high);
            printf("Combinaison possible numéro 4 : %d\n", somme_mid);
        }
        else{
            printf("Combinaison possible numéro 1 : %d\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_mid_spe);
            printf("Combinaison possible numéro 3 : %d\n", somme_high);
            printf("Combinaison possible numéro 4 : %d\n", somme_mid);
            printf("Combinaison possible numéro 5 : %d\n", somme_low);
        }
    }
    else{
        if(somme_mid == 0){
            printf("Combinaison possible numéro 1 : %d\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_mid_spe);
            printf("Combinaison possible numéro 3 : %d\n", somme_low_spe);
            printf("Combinaison possible numéro 4 : %d\n", somme_high);
        }
        else if(somme_low == 0){
            printf("Combinaison possible numéro 1 : %d\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_mid_spe);
            printf("Combinaison possible numéro 3 : %d\n", somme_low_spe);
            printf("Combinaison possible numéro 4 : %d\n", somme_high);
            printf("Combinaison possible numéro 5 : %d\n", somme_mid);
        }
        else{
            printf("Combinaison possible numéro 1 : %d\n", somme_high_spe);
            printf("Combinaison possible numéro 2 : %d\n", somme_mid_spe);
            printf("Combinaison possible numéro 3 : %d\n", somme_low_spe);
            printf("Combinaison possible numéro 4 : %d\n", somme_high);
            printf("Combinaison possible numéro 5 : %d\n", somme_mid);
            printf("Combinaison possible numéro 6 : %d\n", somme_low);
        }
    }
        
    printf("Quelle combinaison souhaitez vous choisir : ");
    scanf("%d", &*choice);
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
        "|Brelan       [Total]","|\t\t","|\t|",
        "|Carré        [Total]","|\t\t","|\t|",
        "|FullHouse    [Total]","|\t\t","|\t|",
        "|Petite Suite [Total]","|\t\t","|\t|",
        "|Grande Suite [Total]","|\t\t","|\t|",
        "|YAMS         [Total]","|\t\t","|\t|",
        "|Chance       [Total]","|\t\t","|\t|",
        "|Total inférieur     ","|\t\t","|\t|",
        "|Total               ","|\t\t","|\t|",
        "└-------------------","-┴---------","-┴-------┘",
    };

    t_de des;
    int somme_high, somme_mid, somme_low;
    int compt_high, compt_mid, compt_low;

    int p_choice;

    bool bool6, bool5, bool4, bool3, bool2, bool1;

    bool6 = true; bool5 = true; bool4 = true; bool3 = true; bool2 = true; bool1 = true;

    brelan = true; carre = true; fullhouse = true; ptsuite = true; gdsuite = true; yams = true; chance = true;

    lancer_de(des);
}