#include <stdio.h>
#include <stdlib.h>   
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define LONGDE 5
#define MAXX 3
#define MAXY 18
#define MAXMOT 22

const int MAXDE = 6; // Utilisé dans la randomisation du chiffre des dés
const int NBMAXP = 2; // Constante permettant de modifier le nombre de joueurs max

typedef char t_fmarq[MAXY][MAXX][MAXMOT];
typedef int t_de[LONGDE];

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

void current_player(char player1[20], char player2[20], char curr_player[20], int tour) {
    if ((tour % 2) == 0) {
        strcpy(curr_player, player1);
    }
    else {
        strcpy(curr_player, player2);
    }
}

void aff_f_marq(t_fmarq feuille_marq) {
    for (int i = 0; i < MAXY; i++) {
        for (int j = 0; j < MAXX; j++) {
            printf("%s", feuille_marq[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void aff_des(t_de des) {
    printf("| ");
    for (int i = 0; i < LONGDE; i++) {
        printf("%d", des[i]);
        printf(" | ");
    }
    printf("\n");
}

int lancer_de(t_de des) {
    srand(time(NULL));
    printf("| ");
    for (int i = 0; i < LONGDE; i++) {
        des[i] = 1 + rand() % MAXDE;
        printf("%d", des[i]);
        printf(" | ");
    }
    printf("\n");
}

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

void reset(t_de des) {
    for (int indice = 0; indice < LONGDE; indice++)
        des[indice] = 0;
}


void somme(t_de des, int* somme_high, int* somme_mid, int* somme_low, int* compt_high, int* compt_mid, int* compt_low, bool somme6, bool somme5, bool somme4, bool somme3, bool somme2, bool somme1) {
    *somme_low = 0; *somme_mid = 0; *somme_high = 0;
    *compt_low = 0; *compt_mid = 0; *compt_high = 0;

    for (int indice6 = 0; indice6 < LONGDE ^ somme6 == false; indice6++) {
        if (des[indice6] == 6) {
            *compt_high = *compt_high + 1;
            *somme_high = *somme_high + des[indice6];
        }
    }
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

int main() {
    t_fmarq feuille_marq = {
        "YAMS                ", "J1   ", "   J2",
        "1 Total de 1        ", "     ", "     ",
        "2 Total de 2        ", "     ", "     ",
        "3 Total de 3        ", "     ", "     ",
        "4 Total de 4        ", "     ", "     ",
        "5 Total de 5        ", "     ", "     ",
        "6 Total de 6        ", "     ", "     ",
        "Bonus si > à 62 [35]", "     ", "     ",
        "Total supérieur     ", "     ", "     ",
        "Brelan       [Total]", "     ", "     ",
        "Carré        [Total]", "     ", "     ",
        "FullHouse    [Total]", "     ", "     ",
        "Petite Suite [Total]", "     ", "     ",
        "Grande Suite [Total]", "     ", "     ",
        "YAMS         [Total]", "     ", "     ",
        "Chance       [Total]", "     ", "     ",
        "Total inférieur     ", "     ", "     ",
        "Total               ", "     ", "     ",
    };

    t_de des;
    int somme_high, somme_mid, somme_low;
    int compt_high, compt_mid, compt_low;

    bool bool6, bool5, bool4, bool3, bool2, bool1;

    bool6 = false;
    bool5 = true;
    bool4 = false;
    bool3 = true;
    bool2 = true;
    bool1 = true;

    lancer_de(des);
    aff_des(des);

    somme(des, &somme_high, &somme_mid, &somme_low, &compt_high, &compt_mid, &compt_low, bool6, bool5, bool4, bool3, bool2, bool1);
    printf("%d, %d, %d\n%d, %d, %d", somme_high, somme_mid, somme_low, compt_high, compt_mid, compt_low);

    return EXIT_SUCCESS;
}
