#include <stdio.h>

unsigned long long X[1000] = {0};

unsigned long long Y[1000] = {0};

int main(int argc, char ** argv){

    int N, M, ires = -1, jres = -1, i, j;

    unsigned long long S, tmax, sum;

    if (scanf ("%d%d", &N, &M) !=2 || N == 0 || M == 0)

        return 0;

    for (i = 0; i < N; ++i){

        if (!scanf ("%llo", &X[i]))

            return 0;

    }      

    for (j = 0; j < M; ++j){

        if (!scanf ("%llo", &Y[j]))

            return 0;

    }

    if (!scanf ("%llo", &S))

        return 0;

    for (i=0; i < N; ++i)

        for(j = 0; j< M; j++){

            tmax = X[i]<Y[j] ? Y[j] : X[i];

            sum = X[i] + Y[j];

            if (sum >= tmax && sum == S){

                ires = i;

                jres = j;

            }   

        }

    printf ("%d %d\n", ires, jres);

    return 0;

}