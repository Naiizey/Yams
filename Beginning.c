#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LONGDE 5
#define MAXX 3
#define MAXY 18
#define MAXMOT 22

const int MINDE = 0;
const int MAXDE = 6;

typedef char t_fmarq[MAXY][MAXX][MAXMOT];
typedef int t_de[LONGDE];

void aff_des(t_de des){
    for(int i = 0; i < LONGDE; i++){
        printf("%d", des[i]);
        printf(" | ");
    }
    printf("\n");
}

void aff_f_marq(t_fmarq feuille_marq){
    for(int i = 0; i < MAXY; i++){
        for(int j = 0; j < MAXX; j++){
            printf("%s", feuille_marq[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int lancer_de(t_de des){
    for(int i = 0; i < LONGDE; i++){
        des[i] = (rand() % (MAXDE - MINDE + 1)) + MINDE;
        printf("%d", des[i]);
        printf(" | ");
    }
}

int bloq_de(t_de des){
    int nb_de, de;
    char answer[4];

    printf("%d\n", lancer_de(des));
    do{
        printf("Souhaitez vous garder ce lancer ? ( Yes or No ) : ");
        scanf("%s", answer);

        if(strcmp(answer,"Yes") != 1){
            printf("Combien de dés souhaitez vous relancer : ");
            scanf("%d", &nb_de);
            for(int i = 0; i < nb_de; i++){
                printf("Sélectionnez un dé que vous voulez relancer : ");
                scanf("%d", &de);
                des[de-1] = (rand() % (MAXDE - MINDE + 1)) + MINDE;
                printf("Voici la nouvelle valeur du dé %d : %d\n", de, des[de-1]);
            }
        }
    }while(strcmp(answer,"Yes") == 1 || strcmp(answer,"No") == 1);

    aff_des(des);
}

int main(){
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

    aff_f_marq(feuille_marq);
    printf("%d", bloq_de(des));

    return EXIT_SUCCESS;
}