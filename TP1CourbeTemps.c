#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "TP1Functions.h"

static double courbe_greedy(dataSet *d, int R) {
    double t0 = now_sec();
    for (int r = 0; r < R; r++) KP_greedy(d);
    double t1 = now_sec();
    return (t1 - t0) / (double)R;
}

static double courbe_lp(dataSet *d, int R) {
    double t0 = now_sec();
    for (int r = 0; r < R; r++) KP_LP(d);
    double t1 = now_sec();
    return (t1 - t0) / (double)R;
}

static double courbe_pd(dataSet *d, int R) {
    double t0 = now_sec();

    for (int r = 0; r < R; r++) {
        int *x = malloc(sizeof(int) * d->n);
        KP_PD(d, x);
        free(x);
    }

    double t1 = now_sec();
    return (t1 - t0) / (double)R;
}

int main(void) {
    srand((unsigned)time(NULL));

    const int R = 200;   
    const int INST = 30;  

            // (a) n fixé, b variable

    {
        int n = 100;
        int b_values[] = {50, 100, 200, 300, 500, 700, 1000};
        int nb = (int)(sizeof(b_values)/sizeof(b_values[0]));

        FILE *f = fopen("bench_nfixed.csv", "w");
        if (!f) { perror("fopen bench_nfixed.csv"); return 1; }
        fprintf(f, "b,greedy_sec,lp_sec,pd_sec\n");

        for (int i = 0; i < nb; i++) {
            int b = b_values[i];
            double tg = 0.0, tlp = 0.0 , tpd = 0.0;

            for (int k = 0; k < INST; k++) {
                dataSet d;
                gen_instance_mem(&d, n, b);

                tg  += courbe_greedy(&d, R);
                tlp += courbe_lp(&d, R);
                tpd += courbe_pd(&d, R);


                free_instance(&d);
            }

            tg  /= (double)INST;
            tlp /= (double)INST;
            tpd /= (double)INST;

            fprintf(f, "%d,%.9f,%.9f,%.9f\n", b, tg, tlp, tpd);
            printf("[n fixe] b=%d greedy=%.9g lp=%.9g pd=%.9g\n", b, tg, tlp, tpd);
        }

        fclose(f);
    }
            
        // (b) b fixé, n variable

    {
        int b = 500;
        int n_values[] = {10, 20, 30, 50, 70, 100, 150, 200, 300, 500, 800, 1000};
        int nn = (int)(sizeof(n_values)/sizeof(n_values[0]));

        FILE *f = fopen("bench_bfixed.csv", "w");
        if (!f) { perror("fopen bench_bfixed.csv"); return 1; }
        fprintf(f, "n,greedy_sec,lp_sec,pd_sec\n");
        for (int i = 0; i < nn; i++) {
            int n = n_values[i];
            double tg = 0.0, tlp = 0.0, tpd = 0.0;

            for (int k = 0; k < INST; k++) {
                dataSet d;
                gen_instance_mem(&d, n, b);

                tg  += courbe_greedy(&d, R);
                tlp += courbe_lp(&d, R);
                tpd += courbe_pd(&d, R);

                free_instance(&d);
            }

            tg  /= (double)INST;
            tlp /= (double)INST;
            tpd /= (double)INST;

            fprintf(f, "%d,%.9f,%.9f,%.9f\n", n, tg, tlp, tpd);
            printf("[b fixe] n=%d greedy=%.9g lp=%.9g pd=%.9g\n", n, tg, tlp, tpd);
        }

        fclose(f);
    }

    printf("Benchmark terminé : bench_nfixed.csv et bench_bfixed.csv créés.\n");
    return 0;
}