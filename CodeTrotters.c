#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char **argv){
	unsigned int N = 0;
	int H[256], P[256], Q[256], order[256];
	char temp[256];
	char *token;

	char const* const file_name = argv[1];
	FILE *fp = fopen(file_name, "r");
	if (!fp)
		return -1;


	/*read N, H, P and Q from file*/
	fgets(temp, 256, fp);
    sscanf(temp,"%d", &N);
	int i=0;
	fgets(temp, 256, fp);
	token = strtok(temp, ",");
    while(token!=NULL)
    {
		sscanf(token, "%d", &H[i]);
		token = strtok(NULL, ",");
		i++;
    }
    i=0;
	fgets(temp, 256, fp);
	token = strtok(temp, ",");
    while(token!=NULL)
    {
		sscanf(token, "%d", &P[i]);
		token = strtok(NULL, ",");
		i++;
    }
	i=0;
	fgets(temp, 256, fp);
	token = strtok(temp, ",");
    while(token!=NULL)
    {
		sscanf(token, "%d", &Q[i]);
		token = strtok(NULL, ",");
		i++;
    }
	/*YOUR CODE HERE*/

	int	*Time = (int *)malloc(sizeof(*Time) * 256);
	int	j;

	for (i = 0; i < N; i++)
	{
		j = 0;
		while (H[i] > 0)
		{
			H[i] -= P[i];
			if (H[i] > 0)
				H[i] += Q[i];
			j++;
		}
		Time[i] = j;
	}

	int	min, index;

	for (i = 0; i < N; i++)
	{
		min = 99999;
		index = 0;
		for (j = 0; j < N; j++)
		{
			if (min > Time[j])
			{
				min = Time[j];
				index = j;
			}
		}
		order[i] = index + 1;
		Time[index] = 99999;
	}

	/*print the output*/
	for (i = 0; i < N; i++)
    {
       printf("%d", order[i]);
	   if ((i + 1) != N)
		   printf(",");
	}
	printf("\n");

	fclose(fp);
	return N;
}
