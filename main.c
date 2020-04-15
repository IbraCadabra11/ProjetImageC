#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libImages.h"
#include "libOutils.h"
#include <stdlib.h>
#include <string.h>
typedef char NOM[100]; // nom des chemin+images born� � 100 caract�res

void main(void)
{
	FILE *F = NULL;
	NOM *liste = NULL; // liste des images
	NOM ligne;
	IMAGE Imin = { 0,0,NULL,NULL};
	int nb = 0;
	char *nom = NULL;
	// mise en m�moire images pgm/ppm 

	system("dir /S /B test *.p*m > listeDesImagestest.txt");
	system("dir /S /B train *.p*m > listeDesImagestrain.txt");
	F = fopen("listeDesImagestrain.txt", "r");
	while (fscanf(F, "%s", ligne) > 0)
	{
		if (!liste)
		{
			liste = (NOM*)malloc(sizeof(NOM));
			strcpy(liste[nb], ligne);
			nb++;
		}
		else
		{
			liste = (NOM*)realloc(liste, sizeof(NOM)*(++nb));
			strcpy(liste[nb - 1], ligne);
		}
	}
	fclose(F);
	Imin = lectureImage(liste[nb-1]);
	printf("Derniere entree : %s\n", liste[nb - 1]);
	system("pause");
}