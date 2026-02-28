#include "TP1Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include<stdio.h>

double g_t_dp_orig = 0.0;

int read_TP1_instance(FILE*fin,dataSet* dsptr)
{
	int rval = 0;

	//capacite b
	int b;
	//Nombre d'objets
	int n;
	rval = fscanf(fin,"%d,%d\n",&n,&b);
	dsptr->b = b;
	dsptr->n = n;
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%d,%d\n",&(dsptr->c[i]),&(dsptr->a[i]));

	fprintf(stderr,"\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",
			b,n);
	for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d,%d\n",dsptr->c[i],dsptr->a[i]);
	fprintf(stderr,"\n");


	return rval;
}

void init (dataSet* dsptr , double x_bar[], double *b_bar)
{
	int i;
	for( i = 0 ; i < dsptr->n ; i++) 
		x_bar[i] = 0 ;
	*b_bar = dsptr->b ;
}

int comp(const void *a, const void *b) {
	utilite *utilA = (utilite *)a;
	utilite *utilB = (utilite *)b;
	
	if (utilA->util < utilB->util) 
		return 1;
	else if (utilA->util > utilB->util) 
		return -1;
	else 
		return 0;	
}

void sort (dataSet* dsptr , utilite utility[])
{
  	int n = dsptr->n;
  
  	qsort(utility, n, sizeof(utilite), comp);
  
}

void util_calc(dataSet* dsptr , utilite utility[])
{
	int i;
	for( i = 0 ; i < dsptr->n ; i++)
	{ 
		utility[i].util= (double)dsptr->c[i]/(double)dsptr->a[i];
		utility[i].index=i;
	}
}

double min (double x, double y)
{
	if (x < y)  return x;
	else return y ;
}

int max (int x,int y)
{
	if (x > y) return x;
	else return y;
}

int KP_find_p_pos(dataSet* dsptr, utilite utility[])
{
    double b_bar = dsptr->b;   

    for (int pos = 0; pos < dsptr->n; pos++)
    {
        int j = utility[pos].index;  

        if (b_bar < dsptr->a[j])
            return pos;             

        b_bar -= dsptr->a[j];
    }

    return -1; 
}

double now_sec(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + 1e-6 * (double)tv.tv_usec;
}

int KP_greedy(dataSet* dsptr)
{

	utilite utility[dsptr->n];
	util_calc(dsptr,utility);
	sort(dsptr,utility);

	double x_bar[dsptr->n];
	double b_bar;
	init(dsptr,x_bar,&b_bar);

	int z = 0;

	int i,j;
	for ( i = 0 ; i < dsptr->n ; i++) 
	{
		if (b_bar == 0)
			break;

		j = utility[i].index;
		if (b_bar >= dsptr->a[j])
		{
			x_bar[j] = 1;
			b_bar = b_bar - dsptr->a[j];
			z= z + dsptr->c[j];
		}
	}
	printf("Valeur totale du sac a dos utilisant la methode gloutonne : %d\n",z);

	return z;
}

double KP_LP(dataSet* dsptr)
{

	utilite utility[dsptr->n];
	util_calc(dsptr,utility);
	sort(dsptr,utility);

	double x_bar[dsptr->n];
	double b_bar;
	init(dsptr,x_bar,&b_bar);

	double z=0;

	int i,j;
	for ( i = 0 ; i < dsptr->n ; i++) 
	{
		if (b_bar == 0)
			break;

		j = utility[i].index;
		x_bar[j] = min (b_bar/dsptr->a[j],1);

		b_bar = b_bar - x_bar[j]*dsptr->a[j];
		z = z + x_bar[j] * dsptr->c[j];
	}

	printf("Valeur optimale du sac a dos utilisant LP : %.2f\n",z);

	return z;
}

static int randint(int lo, int hi) {
	 return lo + rand() % (hi - lo + 1); 
	}

void gen_instance_mem(dataSet *d, int n, int b) {
    d->n = n;
    d->b = b;
    d->c = (int*)malloc(sizeof(int)*n);
    d->a = (int*)malloc(sizeof(int)*n);

    for (int i = 0; i < n; i++) {
        d->a[i] = randint(1, b);
        d->c[i] = randint(1, 1000);
    }
}

void free_instance(dataSet *d) {
    free(d->c);
    free(d->a);
    d->c = NULL;
    d->a = NULL;
}

int KP_PD(dataSet* dsptr, int *x_etoile)
{
	int b = dsptr->b;
	int n = dsptr->n;

	utilite utility[n];
	util_calc(dsptr,utility);
	sort(dsptr,utility);

	int *z = (int *)malloc(sizeof(int)*(b+1));
	int *z_bar = (int *)malloc(sizeof(int)*(b+1));
	int *d = (int *)malloc(sizeof(int)*(b+1));

	int y;
	for ( y = 0 ; y <= b ; y++) 
	{
		z[y] = 0;
		d[y] = -1; 
	}

	int k;
	for ( k = 0 ; k <= n-1 ; k++) 
	{
		int j = utility[k].index;
		for ( y = 0 ; y <= b ; y++) 
		{
			z_bar[y] = z[y];
		}
		
		for ( y = dsptr->a[j] ; y <= b ; y++) 
		{
			if((z_bar[y - dsptr->a[j]] + dsptr->c[j]) > z_bar [y])
			{
				d[y] = j;
				z[y] = max(z_bar[y],dsptr->c[j] + z_bar[y-dsptr->a[j]]);
			}
		}
	}

	int j;
	for ( j = 0 ; j < n ; j++)  
	{
		x_etoile[j] = 0; 
	}
	y = b;

	while(z[y]>0)
	{
		while(y > 0 && z[y]==z[y-1])
		{
			y=y-1;
		}

		if (d[y] < 0)   
        	break;

		x_etoile[d[y]] = 1;
		y=y-dsptr->a[d[y]];
	}

	printf("Optimal value with DP: %d\n", z[b]);
	printf("Chosen items (x*): ");
	for (int i = 0; i < n; i++) printf("%d ", x_etoile[i]);
	printf("\n");

	return z[b];

}

int KP_VP(dataSet* dsptr)
{
	double tvp0 = now_sec();

	int n = dsptr->n;
	int bR = dsptr->b;

	double *c_bar = (double *)malloc(sizeof(double)*(n));
	int *x_bar = (int *)malloc(sizeof(int)*(n));

	for (int j = 0; j < n; j++)
    	x_bar[j] = -1; 

	utilite utility[dsptr->n];
	util_calc(dsptr,utility);
	sort(dsptr,utility);

	int z_x = KP_greedy (dsptr); 
	double z_bar = KP_LP (dsptr); 
	
	int p_pos = KP_find_p_pos(dsptr, utility);

	if (p_pos == -1) {
		printf("[VP] LP integral (no fractional item). Nothing to fix.\n");
		return 0;
	}

	int p_idx = utility[p_pos].index; 
	
	for ( int pos = 0 ; pos < n ; pos++)
	{
		int j = utility[pos].index; 
		c_bar[j] = fabs( dsptr->c[j] - ((double)dsptr->c[p_idx]/(double)dsptr->a[p_idx])*(double)dsptr->a[j]);
		if (c_bar[j] >= z_bar - z_x)  
		{
			if (pos <= p_pos-1)
			{
				x_bar[j] = 1;
				bR = bR - dsptr->a[j];
			}

			else
			{
				x_bar[j] = 0;
			}
		}
	}

	int m = 0;
	for (int j=0; j<n; j++)
		if (x_bar[j] == -1) 
			m++;

	dataSet red;
	red.n = m;
	red.b = bR;
	red.c = malloc(sizeof(int)*m);
	red.a = malloc(sizeof(int)*m);

	int *map = malloc(sizeof(int)*m);

	int t = 0;
	for (int j=0; j<n; j++) {
		if (x_bar[j] == -1) {
			red.c[t] = dsptr->c[j];
			red.a[t] = dsptr->a[j];
			map[t] = j;      
			t++;
		}
	}

	int *x_red = malloc(sizeof(int)*m);
	KP_PD(&red, x_red);   


	int *x_final = malloc(sizeof(int)*n);

	for (int j=0; j<n; j++) {
		if (x_bar[j] == -1)
			x_final[j] = 0;     
		else
			x_final[j] = x_bar[j];
	}

	for (int t=0; t<m; t++) {
		if (x_red[t] == 1) {
			int j_original = map[t];
			x_final[j_original] = 1;
		}
	}

	printf("[VP] x*: ");
	for (int j=0; j<n; j++) printf("%d ", x_final[j]);
	printf("\n");

	int is_reduced = (m < n);

	int fixed0 = 0, fixed1 = 0, freev = 0;

	for (int j = 0; j < n; j++) {
		if (x_bar[j] == -1) freev++;
		else if (x_bar[j] == 0) fixed0++;
		else if (x_bar[j] == 1) fixed1++;
	}

	double pct_fixed  = 100.0 * (fixed0 + fixed1) / (double)n;
	double pct_fixed0 = 100.0 * fixed0 / (double)n;
	double pct_fixed1 = 100.0 * fixed1 / (double)n;

	printf("\n===== ANALYSE Locale =====\n");

	printf("fixed=%d/%d (%.2f%%), fixed0=%d (%.2f%%), fixed1=%d (%.2f%%), free=%d\n",
		fixed0 + fixed1, n, pct_fixed,
		fixed0, pct_fixed0,
		fixed1, pct_fixed1,
		freev);
	
	printf("Temps DP original = %.6f secondes\n", g_t_dp_orig);

	double tvp1 = now_sec();
	double t_VP = tvp1 - tvp0;

	printf("Temps total VP = %.6f secondes\n", t_VP);

	if (g_t_dp_orig > 0.0) {
		double gain_pct = 100.0 * (g_t_dp_orig - t_VP) / g_t_dp_orig;
		double speedup  = g_t_dp_orig / t_VP;
		printf("Gain = %.2f%% | Speedup = %.2fx\n", gain_pct, speedup);
	}

	return is_reduced;
} 
