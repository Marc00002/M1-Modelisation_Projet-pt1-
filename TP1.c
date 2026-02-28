#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "TP1Functions.h"


int main(int argc, char **argv)
{
	int rval =0;

	int total = 0;
	int reduced = 0;
	//File instance name
	//-F option
        char instance_file[1024];
        snprintf(       instance_file,
                        1024,
                        "%s",
                        "instance3.csv");
	
	int single_instance = 0; 
	
	char c;
        while ((c=getopt (argc, argv,"F:h")) != EOF)
	{
		switch(c)
		{
                        case 'F':
				snprintf(       instance_file,
						1024,
						"%s",
						optarg);
				single_instance = 1;
				break;

			case 'h':
				fprintf(stderr,"Usage: ./TP1 [options]\nOptions:\n\n");
				fprintf(stderr,"******** INSTANCE DATA ********\n");
				fprintf(stderr,"\t-F Instance file name to load............................(default %s).\n",instance_file);
				break;
			default:
				exit(0);
		}
	}

	if (single_instance)
    {
		dataSet data;

		//Open the instance file
		FILE* fin = fopen(instance_file,"r");
		read_TP1_instance(fin,&data);
		fclose(fin);

		KP_greedy(&data);
		KP_LP(&data);

		double t0 = now_sec();
		int *x_dp = malloc(sizeof(int)*data.n);
		KP_PD(&data, x_dp);
		double t1 = now_sec();
		double t_dp_orig = t1 - t0;

		g_t_dp_orig = t_dp_orig;
		total++;

		if (KP_VP(&data))
			reduced++;

        free(x_dp);
        free(data.c);
        free(data.a);
    }

	else
    {
        int NB_INSTANCES = 10;

        for (int k = 1; k <= NB_INSTANCES; k++)
        {
            snprintf(instance_file, 1024, "instance%d.csv", k);

            printf("\n===== %s =====\n", instance_file);

            dataSet data;

            FILE *fin = fopen(instance_file, "r");
            if (!fin) continue;

            read_TP1_instance(fin, &data);
            fclose(fin);

            KP_greedy(&data);
            KP_LP(&data);

            double t0 = now_sec();
            int *x_dp = malloc(sizeof(int)*data.n);
            KP_PD(&data, x_dp);
            double t1 = now_sec();
            g_t_dp_orig = t1 - t0;

			total++;

			if (KP_VP(&data))
				reduced++;

            free(x_dp);
            free(data.c);
            free(data.a);
        }
    }

	
	printf("\n===== ANALYSE GLOBALE =====\n");

	if (total > 0)
	{
		double prop = 100.0 * reduced / total;
		printf("Proportion de problemes reduits = %d/%d (%.2f%%)\n",
			reduced, total, prop);
	}

	return rval;
}

