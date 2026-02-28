#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <assert.h>




typedef struct dataSet 
{
	//Attributes of the instance
	//Nombre d'objets
	int n;
	//Capacite b
	int b;
	//Tableau d'entiers de taille n contenant la valeur de chacun des objets
	int*c;
	//Tableau d'entiers de taille n contenant le poids de chacun des objets
	int*a;

} dataSet;

typedef struct utilite 
{
	double util;
	int index;

} utilite;

int read_TP1_instance(FILE*fin,dataSet* dsptr);
int KP_greedy(dataSet* dsptr);
double KP_LP(dataSet* dsptr );
int KP_PD(dataSet* dsptr, int *x_etoile);
int KP_VP(dataSet* dsptr);

void init (dataSet* dsptr , double x_bar[], double *b_bar);
void sort (dataSet* dsptr , utilite utility[]);
void util_calc(dataSet* dsptr , utilite utility[]);

double now_sec(void);
extern double g_t_dp_orig;

void gen_instance_mem(dataSet *d, int n, int b);
void free_instance(dataSet *d);