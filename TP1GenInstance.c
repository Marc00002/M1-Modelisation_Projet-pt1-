#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int randint(int lo, int hi) 
{ 
    return lo + rand() % (hi - lo + 1); 
}

int main(int argc, char **argv) {
    const char *out = (argc >= 2) ? argv[1] : "instance_gen.csv";

    int n = (argc >= 3) ? atoi(argv[2]) : 10;    
    int b = (argc >= 4) ? atoi(argv[3]) : 100;   

    if (n < 1 || n > 100) { fprintf(stderr, "n doit être entre 1 et 100\n"); return 1; }
    if (b < 1 || b > 1000) { fprintf(stderr, "b doit être entre 1 et 1000\n"); return 1; }

    srand((unsigned)time(NULL));

    FILE *f = fopen(out, "w");
    if (!f) { perror("fopen"); return 1; }

    fprintf(f, "%d,%d\n", n, b);

    for (int i = 0; i < n; i++) {
        int a = randint(1, b);      
        int c = randint(1, 1000);   
        fprintf(f, "%d,%d\n", c, a);
    }

    fclose(f);
    printf("Fichier créé: %s\n", out);
    return 0;
}