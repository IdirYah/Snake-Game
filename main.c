#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//---------------------------------
#define SIZE_X 24
#define SIZE_Y 12
#define NB_POMMES 8
#define NB_OBSCTACLES 4
#define TAILLEMAX 3
#define NORTH 1
#define SOUTH 2
#define EAST 3
#define WEST 4
//---------------------------------
typedef struct{
    int ligne,colonne;
}position;
//---------------------------------
typedef struct{
    position tabPosition[SIZE_X*SIZE_Y];
    int direction;
    int longueur;
}serpent;
//---------------------------------
typedef struct{
    position tabObstacle[TAILLEMAX];
    int direction;
    int longueur;
}obstacle;
//---------------------------------
void initialiser(position tab[],int taille){
    for(int i=0;i<taille;i++){
        tab[i].ligne = -1;
        tab[i].colonne = -1;
    }
}
//---------------------------------
void ajouter(position tab[],int taille,position pos){
    int i=0;
    while(i<taille && tab[i].colonne != -1 && tab[i].ligne != -1){
        i++;
    }
    if(i<taille){
        tab[i].colonne = pos.colonne;
        tab[i].ligne = pos.ligne;
    }
}
//---------------------------------
position retirer(position tab[],int taille){
    int i=0;
    position inter;
    inter.ligne = tab[0].ligne;
    inter.colonne = tab[0].colonne;
    while(i<taille-1 && tab[i].ligne != -1 && tab[i].colonne != -1){
        tab[i].ligne = tab[i+1].ligne;
        tab[i].colonne = tab[i+1].colonne;
        i++;
    }
    if(i<taille){
        tab[i].ligne = -1;
        tab[i].colonne = -1;
    }
    return inter;
}
//---------------------------------
int appartient(position tab[],int taille,position pos){
    int trouv = -1;
    int i = 0;
    while(i<taille && trouv == -1){
        if(tab[i].ligne == pos.ligne && tab[i].colonne == pos.colonne){
            trouv = 1;
        }else{
            i++;
        }
    }
    return trouv;
}
//---------------------------------
serpent creer_serpent(position tete,int direction,int longueur){
    serpent snake;
    initialiser(snake.tabPosition,longueur);
    snake.direction = direction;
    snake.longueur = longueur;
    position head;
    head.ligne = tete.ligne;
    head.colonne = tete.colonne;
    for(int i=0;i<longueur;i++){
        ajouter(snake.tabPosition,longueur,head);
        if(direction == NORTH){
            head.ligne++;
            if(head.ligne>=SIZE_X){
                head.ligne = 0;
            }
        }else if(direction == SOUTH){
            head.ligne--;
            if(head.ligne<0){
                head.ligne = SIZE_X;
            }
        }else if(direction == EAST){
            head.colonne--;
            if(head.colonne<0){
                head.colonne = SIZE_Y;
            }
        }else if(direction == WEST){
            head.colonne++;
            if(head.colonne>=SIZE_Y){
                head.colonne = 0;
            }
        }
    }
    return snake;
}
//---------------------------------
int avancer(serpent *snake){
    int sorti = 0;
    position head;
    head.ligne = snake->tabPosition[0].ligne;
    head.colonne = snake->tabPosition[0].colonne;
    int i=snake->longueur-2;
    while(i>=0){
        snake->tabPosition[i+1].ligne = snake->tabPosition[i].ligne;
        snake->tabPosition[i+1].colonne = snake->tabPosition[i].colonne;
        i--;
    }
    if(snake->direction == NORTH){
        head.ligne--;
        if(head.ligne<0){
            sorti = 1;
        }else{
            snake->tabPosition[0].ligne = head.ligne;
        }
    }else if(snake->direction == SOUTH){
        head.ligne++;
        if(head.ligne>=SIZE_X){
            sorti = 1;
        }else{
            snake->tabPosition[0].ligne = head.ligne;
        }
    }else if(snake->direction == EAST){
        head.colonne++;
        if(head.colonne>=SIZE_Y){
            sorti = 1;
        }else{
            snake->tabPosition[0].colonne = head.colonne;
        }
    }else if(snake->direction == WEST){
        head.colonne--;
        if(head.colonne<0){
            sorti = 1;
        }else{
            snake->tabPosition[0].colonne = head.colonne;
        }
    }
    return sorti;
}
//---------------------------------
void changer_direction(serpent *snake,int direction){
    snake->direction = direction;
}
//---------------------------------
void creer_pommes(position pommes[],serpent snake){
    int i=0;
    initialiser(pommes,NB_POMMES);
    while(i<NB_POMMES){
        position point;
        point.ligne = rand()%SIZE_X;
        point.colonne = rand()%SIZE_Y;
        if(appartient(pommes,NB_POMMES,point) == -1 && appartient(snake.tabPosition,snake.longueur,point) == -1){
            ajouter(pommes,NB_POMMES,point);
            i++;
        }
    }
}
//---------------------------------
int manger(position pomme[],serpent snake){
    int eaten = 0;
    if(appartient(pomme,NB_POMMES,snake.tabPosition[0]) == 1){
        eaten = 1;
        int i=0;
        int find = -1;
        while(i<NB_POMMES && find != -1){
            if(pomme[i].ligne == snake.tabPosition[0].ligne && pomme[i].colonne == snake.tabPosition[0].colonne){
                find = 1;
            }else{
                i++;
            }
        }
        int trouv = 0;
        while(trouv == 0){
            position point;
            point.ligne = rand()%SIZE_X;
            point.colonne = rand()%SIZE_Y;
            if(appartient(pomme,NB_POMMES,point) == -1){
                pomme[i].ligne = point.ligne;
                pomme[i].colonne = point.colonne;
                trouv = 1;
            }
        }
    }
    return eaten;
}
//---------------------------------
void redimensionner_serpent(serpent *snake){
    position point;
    point.ligne = snake->tabPosition[snake->longueur-1].ligne;
    point.colonne = snake->tabPosition[snake->longueur-1].colonne;
    snake->longueur++;
    int dx = snake->tabPosition[1].ligne - snake->tabPosition[0].ligne;
    int dy = snake->tabPosition[1].colonne - snake->tabPosition[0].colonne;
    point.ligne = point.ligne + dx;
    point.colonne = point.colonne + dy;
    if(point.ligne>=0 && point.ligne<SIZE_X){
        snake->tabPosition[snake->longueur-1].ligne = point.ligne;
    }else if(point.ligne<0){
        snake->tabPosition[snake->longueur-1].ligne = SIZE_X;
    }else{
        snake->tabPosition[snake->longueur-1].ligne = 0;
    }
    if(point.colonne>=0 && point.colonne<SIZE_Y){
        snake->tabPosition[snake->longueur-1].colonne = point.colonne;
    }else if(point.ligne<0){
        snake->tabPosition[snake->longueur-1].colonne = SIZE_Y;
    }else{
        snake->tabPosition[snake->longueur-1].colonne = 0;
    }
}
//---------------------------------
obstacle creer_obstacle(position tete,int direction,int longueur){
    obstacle obs;
    obs.direction = direction;
    obs.longueur = longueur;
    position head;
    head.ligne = tete.ligne;
    head.colonne = tete.colonne;
    int i=0;
    while(i<longueur){
        obs.tabObstacle[i].ligne = head.ligne;
        obs.tabObstacle[i].colonne = head.colonne;
        i++;
        if(direction == NORTH){
            head.ligne++;
            if(head.ligne>=SIZE_X){
                head.ligne = 0;
            }
        }else if(direction == SOUTH){
            head.ligne--;
            if(head.ligne<0){
                head.ligne = SIZE_X;
            }
        }else if(direction == EAST){
            head.colonne--;
            if(head.colonne<0){
                head.colonne = SIZE_Y;
            }
        }else if(direction == WEST){
            head.colonne++;
            if(head.colonne>=SIZE_Y){
                head.colonne = 0;
            }
        }
    }
    return obs;
}
//---------------------------------
void creer_obstacles(obstacle obs[],serpent snake){
    int i=0;
    while(i<NB_OBSCTACLES){
        position point;
        obstacle obst;
        point.ligne = rand()%SIZE_X;
        point.colonne = rand()%SIZE_Y;
        int longueur = rand()%TAILLEMAX+1;
        int direction = rand()%4+1;
        obst = creer_obstacle(point,direction,longueur);
        int j=0;
        int trouv = 0;
        while(j<longueur && trouv == 0){
            if(appartient(snake.tabPosition,snake.longueur,obst.tabObstacle[j]) == 1){
                trouv = 1;
            }else{
                j++;
            }
        }
        if(trouv == 0){
            obs[i].direction = obst.direction;
            obs[i].longueur = obst.longueur;
            for(int k=0;k<obst.longueur;k++){
                obs[i].tabObstacle[k].ligne = obst.tabObstacle[k].ligne;
                obs[i].tabObstacle[k].colonne = obst.tabObstacle[k].colonne;
            }
            i++;
        }
    }
}
//---------------------------------
int percuter(obstacle obs[],serpent snake){
    int percute = 0;
    int i=0;
    while(i<NB_OBSCTACLES && percute == 0){
        if(appartient(obs[i].tabObstacle,obs[i].longueur,snake.tabPosition[0]) == 1){
            percute = 1;
        }
        else{
            i++;
        }
    }
    return percute;
}
//---------------------------------
void initialiser_grille(char grille[SIZE_X][SIZE_Y]){
    for(int i=0;i<SIZE_X;i++){
        for(int j=0;j<SIZE_Y;j++){
            grille[i][j] = ' ';
        }
    }
}
//---------------------------------
void remplir_grille(char grille[SIZE_X][SIZE_Y],serpent snake,position pommes[],obstacle obs[]){
    position point;
    for(int i=0;i<NB_OBSCTACLES;i++){
        for(int j=0;j<obs[i].longueur;j++){
            point = obs[i].tabObstacle[j];
            grille[point.ligne][point.colonne] = 'O';
        }
    }
    for(int i=0;i<NB_POMMES;i++){
        point = pommes[i];
        grille[point.ligne][point.colonne] = 'P';
    }
    for(int i=0;i<snake.longueur;i++){
        point = snake.tabPosition[i];
        if(i == 0){
            grille[point.ligne][point.colonne] = 'T';
        }else{
            grille[point.ligne][point.colonne] = 'S';
        }
    }
}
//---------------------------------
void afficher_grille(char grille[SIZE_X][SIZE_Y]){
    for(int i=0;i<SIZE_X;i++){
        for(int j=0;j<SIZE_Y;j++){
            printf(" %c |",grille[i][j]);
        }
        printf("\n");
    }
}
//---------------------------------
void fonction_principale(){
    char grille[SIZE_X][SIZE_Y];
    initialiser_grille(grille);
    int mange;
    int choix;
    position tete = {SIZE_X/2,SIZE_Y/2};
    int percute = 0;
    int sorti = 0;
    serpent snake;
    position pomme[NB_POMMES];
    obstacle obs[NB_OBSCTACLES];
    snake = creer_serpent(tete,1,3);
    creer_obstacles(obs,snake);
    creer_pommes(pomme,snake);
    remplir_grille(grille,snake,pomme,obs);
    afficher_grille(grille);
    while(percute == 0 && snake.longueur<SIZE_X*SIZE_Y && sorti == 0){
        printf("------------------------\n");
        printf("Voici les choix possible : \n");
        printf("  1: NORD\n");
        printf("  2: SUD\n");
        printf("  3: EST\n");
        printf("  4: OUEST\n");
        printf("-------------------------\n");
        printf("--> Veuillez entrer votre choix : ");
        scanf("%d",&choix);
        if(choix == 1 && snake.direction != SOUTH){
            changer_direction(&snake,NORTH);
            sorti = avancer(&snake);
        }else if(choix == 2 && snake.direction != NORTH){
            changer_direction(&snake,SOUTH);
            sorti = avancer(&snake);
        }else if(choix == 3 && snake.direction != WEST){
            changer_direction(&snake,EAST);
            sorti = avancer(&snake);
        }else if(choix == 4 && snake.direction != EAST){
            changer_direction(&snake,WEST);
            sorti = avancer(&snake);
        }
        mange = manger(pomme,snake);
        if(mange == 1){
            redimensionner_serpent(&snake);
            printf("%d",snake.longueur);
        }else{
            percute = percuter(obs,snake);
        }
        if(sorti == 0 && percute == 0 && snake.longueur<SIZE_X*SIZE_Y){
            system("cls");
            initialiser_grille(grille);
            remplir_grille(grille,snake,pomme,obs);
            afficher_grille(grille);
        }
    }
    if(sorti == 1 || percute == 1){
        printf("You lose\n");
    }else{
        printf("You win !!!\n");
    }
}
//---------------------------------
int main(){
    srand(time(NULL));
    fonction_principale();
    return 0;
}
