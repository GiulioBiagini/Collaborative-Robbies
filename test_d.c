#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "environment.h"


int main(){

    /*TEST GENERAZIONE AZIONI RANDOM */
    /* GET_RANDOM_ACTION vedi che non venga genrata robaccia */
    /* posizioni random */
    /* mappa generazione */

    int i;
    int test[ACTIONS_NUMBER];
    const int NUMBER_OF_TEST=5000;
    int NUMBER_OF_CAN;
    map_t *m;
    int map_test;

    printf("TESTING: GENERATE_RANDOM_ACTION\n");
    for(i=0;i<ACTIONS_NUMBER;i++){
        test[i]=0;
    }
    for(i=0;i<NUMBER_OF_TEST;i++){
        test[GENERATE_RANDOM_ACTION()]++;
    }
    for(i=0;i<ACTIONS_NUMBER;i++){
        printf("ACTION[%d]: %d\n",i,test[i]);
    }

    printf("TESTING: GET_RANDOM_ACTION\n");
    for(i=0;i<ACTIONS_NUMBER;i++){
        test[i]=0;
    }
    for(i=0;i<NUMBER_OF_TEST;i++){
        test[GET_RANDOM_ACTION()]++;
    }
    for(i=0;i<ACTIONS_NUMBER;i++){
        printf("ACTION[%d]: %d\n",i,test[i]);
    }


    printf("TESTING: init_random_position\n");
    m=allocate_map(10,10,5);
    init_random_map(m);


    for(i=0;i<NUMBER_OF_TEST;i++){
        int x;
        int y;
        init_random_position(&x,&y,m);
        if((0>x)&&(m->width<x))
            printf("ERRORE X: %i",x);
        if((0>y)&&(m->height<y))
            printf("ERRORE Y: %i",y);

    }
    printf("TESTING: init_random_map\n");
    map_test=0;
    free(m);
    for(i=0;i<NUMBER_OF_TEST;i++){
        int x,y;
        int count=0;
        int sx;
        int sy;
        sx=rand()%100+1;
        sy=rand()%100+1;
        NUMBER_OF_CAN=rand()%((sx*sy)+1);
        allocate_map(sx,sy,NUMBER_OF_CAN);
        init_random_map(m);
        for (x = 0; x < m->width; x++)
    		for (y = 0; y < m->height; y++)
                if(m->items[x][y]==CAN){
                    count++;
                }
        if(count==NUMBER_OF_CAN){
            map_test++;
        }
        free(m);
    }
    printf("MAPPE CORRETTE: %i/%i\n",map_test,NUMBER_OF_TEST);




    return 0;
}
