/* outils liés aux images */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "libImages.h"
#include "libOutils.h"

IMAGE allocationImage(int Nblig, int Nbcol)
{
	IMAGE mat = { 0,0,NULL,NULL };
	int i;

	mat.Nblig = Nblig;
	mat.Nbcol = Nbcol;
	mat.data = (unsigned char*)malloc(Nblig*Nbcol*sizeof(unsigned char));
	if (mat.data == NULL)
		return(mat);
	mat.pixel = (unsigned char**)malloc(Nblig*sizeof(unsigned char*));
	if (mat.pixel == NULL) {
		free(mat.data);
		mat.data = NULL;
		return(mat);
	}
	for (i = 0; i<Nblig; i++)
		mat.pixel[i] = &mat.data[i*Nbcol];

	return(mat);
}

void initialisationAleatoireImage(IMAGE img, int ngMin, int ngMax)
{
	int i, j;
	printf("Dans initialisation : img -> %p img.data -> %p img.pixel -> %p\n", &img, img.data, img.pixel);

	for (i = 0; i<img.Nblig; i++)
		for (j = 0; j<img.Nbcol; j++)
			img.pixel[i][j] = (unsigned char)(ngMin + rand() % (ngMax + 1 - ngMin));

	/* solution avec un seul indice
	for (i=0;i<img.Nblig*img.Nbcol;i++)
		img.data[i] = (unsigned char)(ngMin + rand() % (ngMax + 1 - ngMin));
	*/
}

void liberationImage(IMAGE *img)
{
	if (img->data != NULL) {
		free(img->data);
		img->data = NULL;
	}
	if (img->pixel != NULL) {
		free(img->pixel);
		img->pixel = NULL;
	}
}

IMAGE lectureImage(const char *in)
{
	FILE *F = NULL;
	IMAGE img = { 0,0,NULL };
	int dynamique = 0;

	if ((F = fopen(in, "r")) == NULL)
	{
		printf("Pb image inexistante");
	}
	else
	{
		char type[3];

		fgets(type, 3, F);
		fclose(F);
		/* selon le type ouverture binaire ou texte */
		if (strcmp(type, "P2") == 0)  /* cas ASCII niveaux de gris */
		{
			char buf;

			F = fopen(in, "r");

			/* lecture caractère après caractère compte-tenu de la diversité des entêtes possibles */
			fscanf(F, "%c", &type[0]);
			fscanf(F, "%c", &type[1]);
			fscanf(F, "%c", &buf); /* caractère espacement */

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilité d'utiliser également isdigit de la librairie standard <ctype.h> */
				img.Nbcol = img.Nbcol * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				img.Nblig = img.Nblig * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				dynamique = dynamique * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			/* début des data */

			printf("Lecture image NG type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

			/* taille connue, allocation dynamique possible */
			img = allocationImage(img.Nblig, img.Nbcol);

			/* lecture pixel par pixel */
			{
				int i, j;
				int tmp;

				for (i = 0; i<img.Nblig; i++)
					for (j = 0; j<img.Nbcol; j++)
					{
						fscanf(F, "%d", &tmp);
						img.pixel[i][j] = (unsigned char)tmp;
					}
			}
		}
		else
			if (strcmp(type, "P5") == 0)  /* cas brut niveaux de gris */
			{
				char buf;

				F = fopen(in, "rb");

				/* lecture caractère après caractère compte-tenu de la diversité des entêtes possibles */
				type[0] = fgetc(F);
				type[1] = fgetc(F);
				buf = fgetc(F); /* caractère espacement */

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'à trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilité d'utiliser également isdigit de la librairie standard <ctype.h> */
					img.Nbcol = img.Nbcol * 10 + (buf - '0');
					buf = fgetc(F);
				}

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'à trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
					img.Nblig = img.Nblig * 10 + (buf - '0');
					buf = fgetc(F);
				}

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'à trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
					dynamique = dynamique * 10 + (buf - '0');
					buf = fgetc(F);
				}

				/* début des data */

				printf("Lecture image NG type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

				/* taille connue, allocation dynamique possible */
				img = allocationImage(img.Nblig, img.Nbcol);

				/* lecture d'un bloc */
				fread(img.data, sizeof(unsigned char), img.Nbcol*img.Nblig, F);
			}
			else
				printf("Format non supporte pour l'instant...\n");
		fclose(F);
	}
	return img;
}

void sauvegardeImage(IMAGE img, const char *type, const char *out)
{
	FILE *F = NULL;

	if (img.data) /* image non vide */
	{
		printf("Sauvegarde image au type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);
		/* selon le type ouverture binaire ou texte */
		if (strcmp(type, "P2") == 0)  /* cas ASCII niveaux de gris */
		{
			F = fopen(out, "w");

			fprintf(F, "%s\n", type);
			fprintf(F, "# Created by TSE FISA IPSI\n");
			fprintf(F, "%d %d %d\n", img.Nbcol, img.Nblig, 255);
			/* sauvegarde pixel par pixel */
			{
				int i, j;

				for (i = 0; i<img.Nblig; i++)
					for (j = 0; j<img.Nbcol; j++)
						fprintf(F, "%d ", img.pixel[i][j]);
			}
			fclose(F);
		}
		else
			if (strcmp(type, "P5") == 0)  /* cas brut niveaux de gris */
			{
				char tmp[255];

				F = fopen(out, "wb");

				fputs("P5\n", F);
				fputs("# Created by TSE FISA IPSI\n", F);
				sprintf(tmp, "%d %d %d\n", img.Nbcol, img.Nblig, 255);
				fputs(tmp, F);
				/* sauvegarde par bloc */
				fwrite(img.data, sizeof(unsigned char), img.Nbcol*img.Nblig, F);
				fclose(F);
			}
	}
	else
		printf("Image vide\n");
}

IMAGERGB allocationImageRGB(int Nblig, int Nbcol)
{
	IMAGERGB mat = { 0,0,NULL,NULL };
	int i;

	mat.Nblig = Nblig;
	mat.Nbcol = Nbcol;
	mat.data = (RGB*)malloc(Nblig*Nbcol*sizeof(RGB));
	if (mat.data == NULL)
		return(mat);
	mat.pixel = (RGB**)malloc(Nblig*sizeof(RGB*));
	if (mat.pixel == NULL) {
		free(mat.data);
		mat.data = NULL;
		return(mat);
	}
	for (i = 0; i<Nblig; i++)
		mat.pixel[i] = &(mat.data[i*Nbcol]);

	return(mat);
}

void liberationImageRGB(IMAGERGB *m)
{
	if (m->data != NULL) {
		free(m->data);
		m->data = NULL;
	}
	if (m->pixel != NULL) {
		free(m->pixel);
		m->pixel = NULL;
	}
}

IMAGERGB lectureImageRGB(const char *in)
{
	FILE *F = NULL;
	IMAGERGB img = { 0,0,NULL };
	int dynamique = 0;

	if ((F = fopen(in, "r")) == NULL)
	{
		printf("Pb image inexistante");
	}
	else
	{
		char type[3];

		fgets(type, 3, F);
		fclose(F);
		/* selon le type ouverture binaire ou texte */
		if (strcmp(type, "P3") == 0)  /* cas ASCII couleur */
		{
			char tmp[255] = "";
			char buf;

			F = fopen(in, "r");

			/* lecture caractère après caractère compte-tenu de la diversité des entêtes possibles */
			fscanf(F, "%c", &type[0]);
			fscanf(F, "%c", &type[1]);
			fscanf(F, "%c", &buf); /* caractère espacement */

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilité d'utiliser également isdigit de la librairie standard <ctype.h> */
				img.Nbcol = img.Nbcol * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				img.Nblig = img.Nblig * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			fscanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					fscanf(F, "%c", &buf);
				fscanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				dynamique = dynamique * 10 + (buf - '0');
				fscanf(F, "%c", &buf);
			}

			/* début des data */

			printf("Lecture image RGB type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

			/* taille connue, allocation dynamique possible */
			img = allocationImageRGB(img.Nblig, img.Nbcol);

			/* lecture pixel par pixel */
			{
				int i, j;
				int tmp;

				for (i = 0; i<img.Nblig; i++)
					for (j = 0; j<img.Nbcol; j++)
					{
						fscanf(F, "%d", &tmp);
						img.pixel[i][j].R = (unsigned char)tmp;
						fscanf(F, "%d", &tmp);
						img.pixel[i][j].G = (unsigned char)tmp;
						fscanf(F, "%d", &tmp);
						img.pixel[i][j].B = (unsigned char)tmp;
					}
			}
		}
		else
			if (strcmp(type, "P6") == 0)  /* cas brut couleur */
			{
				char buf;

				F = fopen(in, "rb");

				/* lecture caractère après caractère compte-tenu de la diversité des entêtes possibles */
				type[0] = fgetc(F);
				type[1] = fgetc(F);
				buf = fgetc(F); /* caractère espacement */

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'à trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilité d'utiliser également isdigit de la librairie standard <ctype.h> */
					img.Nbcol = img.Nbcol * 10 + (buf - '0');
					buf = fgetc(F);
				}

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'à trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
					img.Nblig = img.Nblig * 10 + (buf - '0');
					buf = fgetc(F);
				}

				buf = fgetc(F);
				if (buf == '#') {
					/* on ignore tout jusqu'à trouver '\n' */
					while (buf != '\n')
						buf = fgetc(F);
					buf = fgetc(F);
				}
				while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
					dynamique = dynamique * 10 + (buf - '0');
					buf = fgetc(F);
				}

				/* début des data */

				printf("Lecture image RGB type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

				/* taille connue, allocation dynamique possible */
				img = allocationImageRGB(img.Nblig, img.Nbcol);

				/* lecture d'un bloc */
				fread(img.data, sizeof(RGB), img.Nbcol*img.Nblig, F);
			}
			else
				printf("Format non supporte pour l'instant...\n");
		fclose(F);
	}
	return img;
}

void sauvegardeImageRGB(IMAGERGB img, const char *type, const char *out)
{
	FILE *F = NULL;

	if (img.data) /* image non vide */
	{
		printf("Sauvegarde image au type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);
		/* selon le type ouverture binaire ou texte */
		if (strcmp(type, "P3") == 0)  /* cas ASCII couleur */
		{
			F = fopen(out, "w");

			fprintf(F, "%s\n", type);
			fprintf(F, "# Created by TSE FISA IPSI\n");
			fprintf(F, "%d %d %d\n", img.Nbcol, img.Nblig, 255);
			/* sauvegarde pixel par pixel */
			{
				int i, j;

				for (i = 0; i<img.Nblig; i++)
					for (j = 0; j<img.Nbcol; j++) {
						fprintf(F, "%d ", img.pixel[i][j].R);
						fprintf(F, "%d ", img.pixel[i][j].G);
						fprintf(F, "%d ", img.pixel[i][j].B);
					}
				fclose(F);
			}
		}
		else
			if (strcmp(type, "P6") == 0)  /* cas brut couleur */
			{
				char tmp[255];

				F = fopen(out, "wb");

				fputs("P6\n", F);
				fputs("# Created by TSE FISA IPSI\n", F);
				sprintf(tmp, "%d %d %d\n", img.Nbcol, img.Nblig, 255);
				fputs(tmp, F);

				/* sauvegarde par bloc */
				fwrite(img.data, sizeof(RGB), img.Nbcol*img.Nblig, F);
				fclose(F);
			}
	}
	else
		printf("Image vide\n");
}

IMAGE inverseImage(IMAGE img)
{
	IMAGE out = { 0,0,NULL,NULL };
	int* lut = NULL;
	int i;

	lut = (int*)calloc(256, sizeof(int)); /* LUT pour optimiser */
	for (i = 0; i<256; i++)
		lut[i] = 255 - i;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);

	return out;
}

IMAGE seuillageImage(IMAGE img, int sInf, int sSup)
{
	IMAGE out = { 0,0,NULL,NULL };
	int* lut = NULL;
	int i;

	lut = (int*)calloc(256, sizeof(int));
	for (i = sInf; i <= sSup; i++)
		lut[i] = 255; /* 1 -> bleu */

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);

	return out;
}


IMAGE planImage(IMAGERGB img, int plan)
{
	IMAGE out = { 0,0,NULL,NULL };
	int i;

	out = allocationImage(img.Nblig, img.Nbcol);
	switch (plan)
	{
	case 0: /* plan rouge */
		for (i = 0; i<out.Nbcol*out.Nblig; i++)
			out.data[i] = img.data[i].R;
		break;
	case 1: /* plan vert */
		for (i = 0; i<out.Nbcol*out.Nblig; i++)
			out.data[i] = img.data[i].G;
		break;
	case 2: /* plan bleu */
		for (i = 0; i<out.Nbcol*out.Nblig; i++)
			out.data[i] = img.data[i].B;
		break;
	}
	return(out);
}

IMAGE luminanceImage(IMAGERGB img, float r, float g, float b)
{
	IMAGE out = { 0,0,NULL,NULL };
	int i;

	out = allocationImage(img.Nblig, img.Nbcol);

	for (i = 0; i<out.Nbcol*out.Nblig; i++)
		out.data[i] = (unsigned char)((float)img.data[i].R*r + (float)img.data[i].G*g + (float)img.data[i].B*b);
	return(out);
}

TABLEAU_INT histogrammeImage(IMAGE img, int choix)
{
	TABLEAU_INT h = { 0,NULL };

	if (img.data)
	{
		int i;

		h = allocationTableau(256);

		for (i = 0; i < img.Nbcol*img.Nblig; i++)
			h.data[(int)img.data[i]] += 1;

		if (choix)
		{
			/* sauvegarde dans histo.csv */
			FILE *F;

			F = fopen("..\\Res\\histo.csv", "w");
			int i;
			for (i = 0; i < h.size; i++)
				fprintf(F, "%d\n", h.data[i]);
			fclose(F);
		}
	}

	return h;
}

IMAGE expansionImage(IMAGE img, int outMin, int outMax)
{
	IMAGE out = { 0,0,NULL,NULL };
	int* lut = NULL;
	int i;
	float a, b;
	int mini = 255, maxi = 0;

	for (i = 0; i<img.Nblig*img.Nbcol; i++)
	{
		mini = (img.data[i] < mini) ? img.data[i] : mini;
		maxi = (img.data[i] > maxi) ? img.data[i] : maxi;
	}

	a = (float)(outMax - outMin) / ((float)(maxi - mini));
	b = (float)outMin - a * mini;

	lut = (int*)calloc(256, sizeof(int));
	for (i = mini; i <= maxi; i++)
		lut[i] = (int)(a*i + b);

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);

	return out;
}

IMAGE egalisationImage(IMAGE img)
{
	IMAGE out = { 0,0,NULL,NULL };
	TABLEAU_INT h = histogrammeImage(img, 0);
	TABLEAU_INT hC = { 0,NULL };
	int* lut = NULL;
	int i;

	hC = allocationTableau(256);

	hC.data[0] = h.data[0];
	for (i = 1; i<256; i++)
		hC.data[i] = hC.data[i - 1] + h.data[i];
	liberationTableau(&h);

	lut = (int*)calloc(256, sizeof(int));
	for (i = 0; i<256; i++)
		lut[i] = (int)((255 / (float)(img.Nbcol*img.Nblig)) * hC.data[i]);
	liberationTableau(&hC);

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];
	free(lut);

	return(out);
}

SIGNATURES signaturesImage(IMAGE img)
{
	SIGNATURES sig;
	int i;
	double som = 0, som2 = 0;
	int somme, moitPop = (img.Nbcol*img.Nblig) / 2;
	TABLEAU_INT h = histogrammeImage(img, 0);

	for (i = 0; i<img.Nbcol*img.Nblig; i++) {
		som += (double)img.data[i];
		som2 += (double)img.data[i] * (double)img.data[i];
	}

	som /= (double)(img.Nblig*img.Nbcol);
	som2 = sqrt(som2 / (double)(img.Nblig*img.Nbcol) - som*som);

	sig.moyenne = som;
	sig.ecartType = som2;

	i = 0;
	while (h.data[i] == 0)
		i += 1;
	sig.min = i;

	i = 255;
	while (h.data[i] == 0)
		i -= 1;
	sig.max = i;

	i = 0;
	somme = h.data[i];
	while (somme < moitPop) {
		i += 1;
		if (i < 256)
			somme += h.data[i];
	}
	sig.mediane = i;

	liberationTableau(&h);

	return sig;
}

IMAGERGB colorisationImage(IMAGE img, char *table)
{
	FILE *lut;
	IMAGERGB out = { 0,0,NULL,NULL };
	char ligne[255];

	int i;
	RGB *LUT = NULL;

	lut = fopen(table, "r");

	LUT = (RGB*)calloc(256, sizeof(RGB));

	while (fgets(ligne, 255, lut) != NULL)
	{
		int a, b, c, d;

		sscanf(ligne, "%d %d %d %d", &a, &b, &c, &d);
		LUT[a].R = b;
		LUT[a].G = c;
		LUT[a].B = d;
	}

	fclose(lut);

	out = allocationImageRGB(img.Nblig, img.Nbcol);

	for (i = 0; i < img.Nblig*img.Nbcol; i++)
	{
		out.data[i].R = LUT[img.data[i]].R;
		out.data[i].G = LUT[img.data[i]].G;
		out.data[i].B = LUT[img.data[i]].B;
	}
	free(LUT);

	return out;
}

IMAGE seuillageOtsu(IMAGE img)
{
	IMAGE out = { 0, 0, NULL, NULL };
	TABLEAU_INT h = histogrammeImage(img, 0);
	TABLEAU_INT hC = { 0,NULL };

	double *tab = NULL;
	int i, seuil;
	double M1, M2;
	double w1;
	int* lut = NULL;
	int min, max;

	/* recherche min et max */
	i = 0;
	while (h.data[i] == 0)
		i++;
	min = i;
	i = 255;
	while (h.data[i] == 0)
		i--;
	max = i;

	hC = allocationTableau(256);

	hC.data[0] = h.data[0];
	for (i = 1; i < 256; i++)
		hC.data[i] = hC.data[i - 1] + h.data[i];

	tab = (double*)calloc(256, sizeof(double));

	/* parcours entre min et max pour éviter divisions par 0 */
	/* initialisation */
	M1 = min;
	seuil = min;
	w1 = (double)hC.data[min] / (double)(img.Nbcol*img.Nblig);
	M2 = 0;
	for (int i = min + 1; i <= max; i++)
		M2 += (double)h.data[i] * i;
	M2 /= (double)(hC.data[max] - h.data[min]);
	tab[min] = w1*(1 - w1)*(M1 - M2)*(M1 - M2);

	// parcours
	for (int i = min + 1; i < max; i++) {
		M1 = ((double)hC.data[i - 1] * M1 + (double)h.data[i] * i) / hC.data[i];
		M2 = ((double)(hC.data[255] - hC.data[i - 1])*M2 - (double)(h.data[i] * i)) / (double)(hC.data[255] - hC.data[i]);
		w1 = (double)hC.data[i] / (double)(img.Nbcol*img.Nblig);
		tab[i] = w1*(1 - w1)*(M1 - M2)*(M1 - M2);
		if (tab[i] > tab[seuil])
			seuil = i;
	}

	liberationTableau(&h);
	liberationTableau(&hC);
	free(tab);

	lut = (int*)calloc(256, sizeof(int));
	for (i = seuil; i <= max; i++)
		lut[i] = 255; /* 1 -> bleu */

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i<img.Nbcol*img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);
	return out;
}

double distanceHistogrammeImage(TABLEAU_INT h1, TABLEAU_INT h2)
{
	double dist = 0;
	int i;

	/* histos supposés de même taille */
	for (i = 0; i < h1.size; i++) {
		double ecart = (double)h1.data[i] - (double)h2.data[i];
		dist += ecart*ecart;
	}
	/* normalisation */
	dist = sqrt(dist/(double)h1.size);

	return(dist);
}

IMAGERGB bruitAleatoireImage(IMAGERGB img, int amplitude)
{
	IMAGERGB out = { 0,0,NULL,NULL };

	out = allocationImageRGB(img.Nblig, img.Nbcol);

	for (int i = 0; i < img.Nblig*img.Nbcol; i++)
	{
		int bruit = (-amplitude + rand() % (amplitude + 1 + amplitude));

		if (bruit > 0)
		{
			out.data[i].R = (unsigned char)min((int)(img.data[i].R+bruit),255);
			out.data[i].G = (unsigned char)min((int)(img.data[i].G+bruit),255);
			out.data[i].B = (unsigned char)min((int)(img.data[i].B+bruit),255);
		}
		else
		{
			out.data[i].R = (unsigned char)max(0,(int)(img.data[i].R+bruit));
			out.data[i].G = (unsigned char)max(0,(int)(img.data[i].G+bruit));
			out.data[i].B = (unsigned char)max(0,(int)(img.data[i].B+bruit));
		}
	}

	return out;
}
/*
************************************************************************************************
*			Fonction de gestion des bords des images. Duplication des bords					   *
************************************************************************************************
*/
IMAGE fct_replicateImage(IMAGE ImgInput, int nb_duplicate)
{
	IMAGE ImgOut = { 0,0,NULL,NULL };
	int ind1 = 0, ind2 = 0;
	ImgOut = allocationImage(ImgInput.Nblig + (nb_duplicate * 2), ImgInput.Nbcol + (nb_duplicate * 2));
	for (int i = 0; i < ImgInput.Nblig; i++)
	{
		for (int j = 0; j <ImgInput.Nbcol; j++)
		{
			ImgOut.pixel[i + nb_duplicate][j + nb_duplicate] = ImgInput.pixel[i][j];
		}
	}
	for (int i = 0; i < nb_duplicate; i++)
	{
		ind1 = (ImgOut.Nblig - 1) - i;
		ind2 = ImgInput.Nblig - 1;
		for (int j = 0; j <ImgInput.Nbcol; j++)
		{
			ImgOut.pixel[i][j + nb_duplicate] = 0;
			//ImgInput.pixel[0][j];
			ImgOut.pixel[ind1][j + nb_duplicate] = 0;
			//ImgInput.pixel[ind2][j];
		}
	}
	for (int i = 0; i <nb_duplicate; i++)
	{
		ind2 = (ImgOut.Nbcol - 1);
		ind1 = (ImgOut.Nbcol - 1 - nb_duplicate);
		for (int j = 0; j <ImgOut.Nblig; j++)
		{
			ImgOut.pixel[j][nb_duplicate - 1 - i] = 0;
			//ImgOut.pixel[j][nb_duplicate];
			ImgOut.pixel[j][ind2 - i] = 0;
			//ImgOut.pixel[j][ind1];
		}
	}
	return(ImgOut);
}
IMAGE fct_EtiquettageImage(IMAGE ImgInput)
{
	//***************** Déclaration et initialisation des variables*******************//
	int table_Equivalence[180];
	int predecNonNul[10], nb_EPS = 0;
	int nb_replicate = 1, nb_taillEqui = 5, nbLinK_K = 1, indi = 0, indj = 0, indic = 0;
	int  precPixelMIN = 0, precPixelMAX = 0, predec1 = 0, predec2 = 0, predec3 = 0, predec4 = 0, nbPredecNonNuls = 0;
	int nb_compconnexeMAX = 0;
	IMAGE ImgOutLab = { 0,0,NULL,NULL }, ImgreplicateBord = { 0,0,NULL,NULL };
	ImgreplicateBord = fct_replicateImage(ImgInput, nb_replicate);
	ImgOutLab = allocationImage(ImgInput.Nblig, ImgInput.Nbcol);
	nb_compconnexeMAX = 180;
	//*******************************************************************************//
	for (int kk = 0; kk < nb_compconnexeMAX; kk++)
	{
		table_Equivalence[kk] = kk;
	}
	for (int i = 0; i < ImgInput.Nblig; i++)
	{
		indi = nb_replicate + i;
		for (int j = 0; j < ImgInput.Nbcol; j++)
		{
			indj = nb_replicate + j;
			
			if(ImgreplicateBord.pixel[indi][indj] != 0)
			{
				predec1 = ImgreplicateBord.pixel[indi][indj - 1];
				predec2 = ImgreplicateBord.pixel[indi - 1][indj - 1];
				predec3 = ImgreplicateBord.pixel[indi - 1][indj];
				predec4 = ImgreplicateBord.pixel[indi - 1][indj + 1];
				nbPredecNonNuls = 0;
				if (predec1) predecNonNul[nbPredecNonNuls++] = predec1;
				if (predec2) predecNonNul[nbPredecNonNuls++] = predec2;
				if (predec3) predecNonNul[nbPredecNonNuls++] = predec3;
				if (predec4) predecNonNul[nbPredecNonNuls++] = predec4;
				int indIce = 0;
				if (nbPredecNonNuls == 0)
				{
					ImgreplicateBord.pixel[indi][indj] = nbLinK_K;
					nbLinK_K++;
				}
				else
				{
					indic = predecNonNul[0];
					nb_EPS = 0;
					precPixelMIN = table_Equivalence[indic];
					precPixelMAX = table_Equivalence[indic];
					while (nb_EPS < nbPredecNonNuls)
					{
						if (table_Equivalence[predecNonNul[nb_EPS]] < precPixelMIN) { precPixelMIN = table_Equivalence[predecNonNul[nb_EPS]]; }
						if (table_Equivalence[predecNonNul[nb_EPS]] > precPixelMAX) { precPixelMAX = table_Equivalence[predecNonNul[nb_EPS]]; }
						nb_EPS++;
					}
					if (precPixelMIN == precPixelMAX)
					{
						ImgreplicateBord.pixel[indi][indj] = precPixelMIN;
					}
					else
					{
						ImgreplicateBord.pixel[indi][indj] = precPixelMIN;
						int beta = 0, m = 0;
						for (nb_EPS = 0; nb_EPS < nbPredecNonNuls; nb_EPS++)
						{
							beta = predecNonNul[nb_EPS];
							while (table_Equivalence[beta] != precPixelMIN)
							{
								m = table_Equivalence[beta];
								table_Equivalence[beta] = precPixelMIN;
								beta = m;
							}
						}
					}
				}
			}
		}
	}
	for (int kk = 1; kk < nbLinK_K; kk++)
	{
		int m = kk;
		while (table_Equivalence[m] != m)
		{
			m = table_Equivalence[m];
		}
		table_Equivalence[kk] = m;
	}
	int* etiquettes = allocationTableauInt(nbLinK_K + 1);
	for (int kk = 0; kk < nbLinK_K; kk++)
		etiquettes[kk] = 0;
	//************************************************************************************
	for (int kk = 1; kk < nbLinK_K; kk++)
		etiquettes[table_Equivalence[kk]]++;
	//************************************************************************************
	etiquettes[0] = 0;  int compt = 1;
	for (int kk = 1; kk < nbLinK_K; kk++)
	{
		if (etiquettes[kk])
			etiquettes[kk] = compt++;
	}
	int indI = 0, indJ = 0, indIce = 0;
	for (int i = 0; i < ImgOutLab.Nblig; i++)
	{
		indI = i + nb_replicate;
		for (int j = 0; j < ImgOutLab.Nbcol; j++)
		{
			indJ = j + nb_replicate;
			indIce = ImgreplicateBord.pixel[indI][indJ];
			ImgreplicateBord.pixel[indI][indJ] = etiquettes[table_Equivalence[indIce]];
		}
	}
	int IndI = 0, IndJ = 0;
	for (int i = 0; i < ImgOutLab.Nblig; i++)
	{
		IndI = i + nb_replicate;
		for (int j = 0; j < ImgOutLab.Nbcol; j++)
		{
			IndJ = j + nb_replicate;
			ImgOutLab.pixel[i][j] = ImgreplicateBord.pixel[IndI][IndJ];
		}
	}
		free(etiquettes);
		liberationImage(&ImgreplicateBord);
		return (ImgOutLab);
}

IMAGE Division_Image(IMAGE ImgInput,int Nord) //Nord = 1 donc return(ImgOutNord) Nord =0 donc return(ImgOutSud)
{
	IMAGE ImgOutNord = { 0,0,NULL,NULL };
	IMAGE ImgOutSud = { 0,0,NULL,NULL };
	int NbligImgOutput = ImgInput.Nblig * ImgInput.Nbcol / 2;
	int k = 0;
	if (Nord == 1)
	{
		ImgOutNord = allocationImage(ImgInput.Nblig/2, ImgInput.Nbcol);
		for (int i = 0; i < NbligImgOutput; i++)
		{
			ImgOutNord.data[i] = ImgInput.data[i];

		}
		return(ImgOutNord);
	}
	if (Nord == 0)
	{	
		ImgOutSud = allocationImage(ImgInput.Nblig/2, ImgInput.Nbcol);

		for (int j = NbligImgOutput; j < ImgInput.Nblig * ImgInput.Nbcol; j++, k++)
		{

		ImgOutSud.data[k] = ImgInput.data[j];

		}
		return(ImgOutSud);
	}
}

POINT Center_of_Gravity(IMAGE ImgInput)// x : ligne y: colonne determine le centre de gravite d'une composante connexe
{
	POINT p = { 0,0 };
	int i, j, nbpoint = 0;
	for (i = 0; i < ImgInput.Nblig; i++)
	{
		for (j = 0; j < ImgInput.Nbcol; j++)
		{

			if (ImgInput.pixel[i][j] == 255)
			{
				p.abscisse = p.abscisse + (double)i;
				p.ordonnee = p.ordonnee + (double)j;
				nbpoint = nbpoint + 1;
			}
		}
	}
	 p.abscisse = (int) p.abscisse / (nbpoint);
	 p.ordonnee = (int) p.ordonnee / (nbpoint);
	return p;
}

double Distance_pixel(POINT p1,POINT p2) // distance euclidienne entre deux pixels
{
	double ecart = (double)(p1.abscisse - p2.abscisse) * (p1.abscisse - p2.abscisse);
	double d = ecart;
	ecart = (double)(p1.ordonnee - p2.ordonnee) * (p1.ordonnee - p2.ordonnee);
	d += ecart;
	d = sqrt(d);
	return d;
}


double Cercle_Inscrit(IMAGE ImgInput)// l'objet est plein ( par exemple un disque blanc ou carré blanc) ,recherche la distance minimal entre un pixel noir et le centre de gravité 

{
	double R = 0;
	int taille = ImgInput.Nblig * ImgInput.Nbcol;
	int k = 0;
	POINT p = { 0,0 };
	POINT Centre = Center_of_Gravity(ImgInput, p);
	POINT px = { 0,0 };
	TABLEAU_DOUBLE TabDist = { 0,NULL };
	TabDist = allocationTableauDouble(taille);
	for (int i = 0; i < ImgInput.Nblig; i++)
	{
		for (int j = 0; j < ImgInput.Nbcol; j++)
		{
			if (ImgInput.pixel[i][j] == 255)
			{
				
				px.abscisse = (double)i;
				px.ordonnee = (double)j;
				TabDist.data[k] = Distance_pixel(Centre, px);
				k++;
			}
		}
	}
	R = TabDist.data[0];
	for (int i = 1; i < ImgInput.Nblig * ImgInput.Nbcol; i++)
	{
		if (TabDist.data[i] < R && TabDist.data[i]!=0)
		{
			R = TabDist.data[i];
		}
	}

	liberationTableauDouble(&TabDist);
	return R;

}

double Cercle_Circonscrit(IMAGE ImgInput)// l'objet est creux ( par exemple un disque blanc ou carré blanc) ,recherche la distance maximale entre un pixel blanc et le centre de gravité 
{
	double R = 0;
	int taille = ImgInput.Nblig * ImgInput.Nbcol;
	int k = 0;
	POINT p = { 0,0 };
	POINT Centre = Center_of_Gravity(ImgInput, p);
	POINT px = { 0,0 };
	TABLEAU_DOUBLE TabDist = { 0,NULL };
	TabDist = allocationTableauDouble(taille);

	for (int i = 0; i < ImgInput.Nblig; i++)
	{
		for (int j = 0; j < ImgInput.Nbcol; j++)
		{
			if (ImgInput.pixel[i][j] == 255)
			{
			
				px.abscisse = (double)i;
				px.ordonnee = (double)j;
				TabDist.data[k] = Distance_pixel(Centre, px);
				k++;
			}
		}
	}
	R = TabDist.data[0];
	for (int i = 1; i < ImgInput.Nblig * ImgInput.Nbcol; i++)
	{
		if (TabDist.data[i] > R && TabDist.data[i] != 0)
		{
			R = TabDist.data[i];
		}
	}
	liberationTableauDouble(&TabDist);
	return R;
}
STRCT_ELEMENT allocationStructElement(int Nblig, int Nbcol)
{
	STRCT_ELEMENT mat = { 0,0,NULL,NULL };
	int i;
	mat.NbLig = Nblig;
	mat.NbCol = Nbcol;
	mat.data = (int*)malloc(Nblig * Nbcol * sizeof(int));
	if (mat.data == NULL)
		return(mat);
	mat.pixel = (int**)malloc(Nblig * sizeof(int*));
	if (mat.pixel == NULL) {
		free(mat.data);
		mat.data = NULL;
		return(mat);
	}
	for (i = 0; i < Nblig; i++)
		mat.pixel[i] = &mat.data[i * Nbcol];
	return(mat);
}

STRCT_ELEMENT fct_generationElementStructurant(int type_Elem, int tailleES)
{
	STRCT_ELEMENT ElemStruct = { 0,0,NULL,NULL };
	if (type_Elem == 1)
	{
		double R,D;
		int Nblig = (tailleES * 2) + 1;
		int Nbcol = (tailleES * 2) + 1;
		int ci =  tailleES;
		int cj = tailleES;
		int ray_carre = tailleES*tailleES;
		ElemStruct = allocationStructElement(Nblig, Nbcol);
		for (int i = 0; i < Nblig; i++)
		{
			for (int j = 0; j < Nbcol; j++)
			{
				D = (double)((i - ci) * (i - ci) + (j - cj) * (j - cj));
				R = sqrt(D) - 0.5;
				if (R <= (double)tailleES)
				{
					ElemStruct.pixel[i][j] = 1;
				}
				else
				{
					ElemStruct.pixel[i][j] = 0;
				}
			}
		}
	}
	else if (type_Elem == 2)
	{
		int tailleLigne = tailleES * 2 + 1;
		ElemStruct = allocationStructElement(1, tailleLigne);
		for (int i = 0; i < tailleLigne; i++)
		{
			ElemStruct.pixel[0][i] = 1;

		}
	}
	return ElemStruct;
}
//Fonction bouchage des trous d'une image binaire
IMAGE fct_bouchageTrou(IMAGE Imgin)
{
		IMAGE filledIm = { 0,0,NULL,NULL }, Imcomp = { 0,0,NULL,NULL }, ImgLab = { 0,0,NULL,NULL};
		TABLEAU_INT LUT_SuppBordHBGD = { 0,NULL };
		LUT_SuppBordHBGD = allocationTableau(256);
		int ind = 0;
		Imcomp = allocationImage(Imgin.Nblig, Imgin.Nbcol);
		filledIm = allocationImage(Imgin.Nblig, Imgin.Nbcol);
		ImgLab = allocationImage(Imgin.Nblig, Imgin.Nbcol);
		Imcomp = inverseImage(Imgin);
		ImgLab = fct_EtiquettageImage(Imcomp);
		for (int i = 0; i < 1, i<ImgLab.Nbcol-1; i++)
		{
			LUT_SuppBordHBGD.data[ImgLab.pixel[0][i]] += 1;
		}
		for (int i = 0; i < ImgLab.Nbcol; i++)
		{
			LUT_SuppBordHBGD.data[ImgLab.pixel[ImgLab.Nbcol-1][i]] += 1; // Comptage des pixels du bord bas
		}
		for (int i = ind; i < ImgLab.Nblig; i++)
		{
			LUT_SuppBordHBGD.data[ImgLab.pixel[i][0]] += 1; // Comptage des pixels du bord gauche
		}
		for (int i = ind; i < ImgLab.Nblig; i++)
		{
			LUT_SuppBordHBGD.data[ImgLab.pixel[i][ImgLab.Nbcol-1]] += 1; // Comptage des pixels du bord droit
		}
		// Inversion de la LUT
		LUT_SuppBordHBGD.data[0] = 0;
		for (int i = 1; i < LUT_SuppBordHBGD.size; i++)
		{
			if(LUT_SuppBordHBGD.data[i]==0)
			{
				LUT_SuppBordHBGD.data[i] = 255;
			}
			else
			{
				LUT_SuppBordHBGD.data[i] = 0;
			}
		}
		for (int i = 0; i < (ImgLab.Nblig*ImgLab.Nbcol); i++)
		{
			filledIm.data[i] = LUT_SuppBordHBGD.data[ImgLab.data[i]];
		}

		for (int i = 0; i < (ImgLab.Nblig*ImgLab.Nbcol); i++)
		{
			filledIm.data[i] += Imgin.data[i];
		}
		liberationImage(&Imcomp);
		liberationImage(&ImgLab);
		return(filledIm);
		
}

int fct_countobject(IMAGE ImgIn)
	{
		int nb_Object = 0;
		TABLEAU_INT Hist = { 0,NULL };
		Hist = allocationTableau(256);
		Hist = histogrammeImage(ImgIn, 0);
		for (int i = 1; i < Hist.size; i++)
		{
			if (Hist.data[i] != 0)
			{
				nb_Object++;
			}
		}
		liberationTableau(&Hist);
		return(nb_Object);
	}

int fct_EulerNumber(IMAGE ImgIn)
	{
		IMAGE filledIm = { 0,0,NULL,NULL }, Imcomp = { 0,0,NULL,NULL }, ImgLab = { 0,0,NULL,NULL }, ImgLab2 = { 0,0,NULL,NULL };
		TABLEAU_INT LUT_SuppBordHBGD = { 0,NULL };
		LUT_SuppBordHBGD = allocationTableau(256);
		int ind = 0, nb_trou = 0;
		Imcomp = allocationImage(ImgIn.Nblig, ImgIn.Nbcol);
		filledIm = allocationImage(ImgIn.Nblig, ImgIn.Nbcol);
		ImgLab = allocationImage(ImgIn.Nblig, ImgIn.Nbcol);
		Imcomp = inverseImage(ImgIn);
		ImgLab = fct_EtiquettageImage(Imcomp);
		for (int i = 0; i<ImgLab.Nbcol - 1; i++)
		{
			LUT_SuppBordHBGD.data[ImgLab.pixel[0][i]] += 1;
		}
		for (int i = 0; i < ImgLab.Nbcol; i++)
		{
			LUT_SuppBordHBGD.data[ImgLab.pixel[ImgLab.Nblig - 1][i]] += 1; // Comptage des pixels du bord bas
		}
		for (int i = ind; i < ImgLab.Nblig; i++)
		{
			LUT_SuppBordHBGD.data[ImgLab.pixel[i][0]] += 1; // Comptage des pixels du bord gauche
		}
		for (int i = ind; i < ImgLab.Nblig; i++)
		{
			LUT_SuppBordHBGD.data[ImgLab.pixel[i][ImgLab.Nbcol - 1]] += 1; // Comptage des pixels du bord droit
		}
		// Inversion de la LUT
		LUT_SuppBordHBGD.data[0] = 0;
		for (int i = 1; i < LUT_SuppBordHBGD.size; i++)
		{
			if (LUT_SuppBordHBGD.data[i] == 0)
			{
				LUT_SuppBordHBGD.data[i] = 255;
			}
			else
			{
				LUT_SuppBordHBGD.data[i] = 0;
			}
		}
		for (int i = 0; i < (ImgLab.Nblig*ImgLab.Nbcol); i++)
		{
			filledIm.data[i] = LUT_SuppBordHBGD.data[ImgLab.data[i]];
		}
		ImgLab2 = allocationImage(ImgIn.Nblig, ImgIn.Nbcol);
		ImgLab2 = fct_EtiquettageImage(filledIm);
		nb_trou = fct_countobject(ImgLab2);
		liberationImage(&Imcomp);
		liberationImage(&ImgLab);
		liberationImage(&ImgLab2);
		liberationImage(&filledIm);
		liberationTableau(&LUT_SuppBordHBGD);
		return(nb_trou);
	}
IMAGE fct_erosion(IMAGE ImgIn, STRCT_ELEMENT ElementStr)
{
	IMAGE ImgOut = { 0,0,NULL,NULL }, ImgBord = { 0,0,NULL,NULL }, ImInter = { 0,0,NULL,NULL };
	int nb_replicate = ElementStr.NbCol, Decalage_X = 0, Decalage_Y = 0, Is_Zero =0;
	int ind_kk = 0, ind_zz = 0;
	ImgOut = allocationImage(ImgIn.Nblig, ImgIn.Nbcol);
	ImgBord = fct_replicateImage(ImgIn, nb_replicate);
	ImInter = allocationImage(ImgBord.Nblig, ImgBord.Nbcol);
	for (int i = nb_replicate; i < (ImgIn.Nblig+nb_replicate); i++)
	{
		for (int j = nb_replicate; j < (ImgIn.Nbcol+nb_replicate); j++)
		{
			for (int k = Decalage_Y ;k<(ElementStr.NbLig + Decalage_Y); k++)
			{
				for (int z = Decalage_X; z < (ElementStr.NbCol - 1 + Decalage_X); z++)
				{
					if ((ElementStr.pixel[ind_kk][ind_zz] !=0) && ImgBord.pixel[k][z]==0) // test pour prendre en compte que les pixels situes dans une zone de l'element structurant egale à un
					{
						Is_Zero ++;
					}
					ind_zz++;
				}
				ind_zz = 0;
				ind_kk++;
			}
			ind_kk = 0;
			if (Is_Zero !=0)
			{
				ImInter.pixel[i][j] = 0;
			}
			else
			{
				ImInter.pixel[i][j] = 255;
			}
			Is_Zero = 0;
			Decalage_X++;
		}
		Decalage_X = 0;
		Decalage_Y++;
	}
	for (int i = 0; i < ImgIn.Nblig; i++)
	{
		for (int j = 0; j < ImgIn.Nbcol; j++)
		{
			ImgOut.pixel[i][j] = ImInter.pixel[i + nb_replicate][j + nb_replicate];
		}

	}
	liberationImage(&ImgBord);
	liberationImage(&ImInter);
	return(ImgOut);

}
void liberationStrElement(STRCT_ELEMENT *Str)
{
	if (Str->data != NULL) {
		free(Str->data);
		Str->data = NULL;
	}
	if (Str->pixel != NULL) {
		free(Str->pixel);
		Str->pixel = NULL;
	}
}

IMAGE fct_dilatation(IMAGE ImgIn, STRCT_ELEMENT ElementStr)
{
	IMAGE ImgOut = { 0,0,NULL,NULL }, ImgBord = { 0,0,NULL,NULL }, ImInter = { 0,0,NULL,NULL };;
	int nb_replicate = ElementStr.NbCol, Decalage_X = 0, Decalage_Y = 0, Is_One = 0;
	int ind_kk = 0, ind_zz = 0;
	ImgOut = allocationImage(ImgIn.Nblig, ImgIn.Nbcol);
	ImgBord = fct_replicateImage(ImgIn, nb_replicate);
	ImInter = allocationImage(ImgBord.Nblig, ImgBord.Nbcol);
	for (int i = 0; i < (ImgIn.Nblig*ImgIn.Nbcol); i++)
	{
		ImgOut.data[i] = ImgIn.data[i];
	}
	for (int i = nb_replicate; i < (ImgIn.Nblig+nb_replicate); i++)
	{
		for (int j = nb_replicate; j < (ImgIn.Nbcol+nb_replicate); j++)
		{
			for (int k = Decalage_Y; k< (ElementStr.NbLig + Decalage_Y); k++)
			{
				for (int z = Decalage_X; z < (ElementStr.NbCol + Decalage_X); z++)
				{
					if ((ElementStr.pixel[ind_kk][ind_zz] != 0) && ImgBord.pixel[k][z] != 0) // test pour prendre en compte que les pixels situes dans une zone de l'element structurant egale à un
					{
						Is_One ++;
					}
					ind_zz++;
				}
				ind_zz = 0;
				ind_kk++;
			}
			ind_kk = 0;
			if (Is_One != 0)
			{
				ImInter.pixel[i][j] = 255;
			}
			else
			{
				ImInter.pixel[i][j] = 0;
			}
			Is_One = 0;
			Decalage_X++;
		}
		Decalage_X = 0;
		Decalage_Y++;
	}
	for (int i = 0; i < ImgIn.Nblig; i++)
	{
		for (int j = 0; j < ImgIn.Nbcol; j++)
		{
			ImgOut.pixel[i][j] = ImInter.pixel[i + nb_replicate][j + nb_replicate];
		}

	}
	liberationImage(&ImgBord);
	liberationImage(&ImInter);
	return(ImgOut);


}

IMAGE fct_Ouverture(IMAGE Imgin, STRCT_ELEMENT ElementStr)
{
	IMAGE ImgOut = { 0,0,NULL,NULL };
	ImgOut = allocationImage(Imgin.Nblig, Imgin.Nbcol);
	ImgOut= fct_erosion(Imgin, ElementStr);
	ImgOut = fct_dilatation(ImgOut, ElementStr);
	return(ImgOut);
}

IMAGE fct_fermeture(IMAGE Imgin, STRCT_ELEMENT ElementStr)
{
	IMAGE ImgOut = { 0,0,NULL,NULL };
	ImgOut = allocationImage(Imgin.Nblig, Imgin.Nbcol);
	ImgOut = fct_dilatation(Imgin, ElementStr);
	ImgOut =  fct_erosion(ImgOut, ElementStr);
	return(ImgOut);
}
IMAGE fct_TopHat(IMAGE Imgin)
{
	IMAGE ImgOut = { 0,0,NULL,NULL }, Imboucl = { 0,0,NULL,NULL }, Imfill = { 0,0,NULL,NULL };
	STRCT_ELEMENT str = { 0,0,NULL,NULL }, str2 ={ 0,0,NULL,NULL };
	str = fct_generationElementStructurant(1, 1);
	str2 = fct_generationElementStructurant(1, 5);
	Imfill = fct_bouchageTrou(Imgin);
	Imboucl = fct_Ouverture(Imfill, str);
//	Imboucl = fct_dilatation(Imboucl, str2);
	ImgOut = allocationImage(Imgin.Nblig, Imgin.Nbcol);
	for (int i = 0; i < (Imgin.Nblig); i++)
	{
		for (int j = 0; j < Imgin.Nbcol; j++)
		{
			ImgOut.pixel[i][j] = (Imgin.pixel[i][j] - Imboucl.pixel[i][j]);
			if (ImgOut.pixel[i][j]<0)
			{
				ImgOut.pixel[i][j] = 0;
			}
		}
	}
	liberationImage(&Imboucl);
	liberationStrElement(&str);
	liberationStrElement(&str2);
	return(ImgOut);
}
int fct_Aire(IMAGE img)
{
	int Compteur = 0;
	int i, j;

	for (i = 0; i < img.Nblig*img.Nbcol; i++)
		if (img.data[i]!= 0)
			Compteur += 1;
	return(Compteur);
}

SIGNATURES_OCR fct_calcSignature(IMAGE ImgIn)
{
	SIGNATURES_OCR Sign_Number = { 0,0,0,0 };
	IMAGE ImBin = { 0,0,NULL,NULL }, Imcomp = { 0,0,NULL,NULL }, ImLab = { 0,0,NULL,NULL }, ImFill = { 0,0,NULL,NULL }, Imclose = { 0,0,NULL,NULL };
	IMAGE Imboucl = { 0,0,NULL,NULL }, ImNord = { 0,0,NULL,NULL }, ImSud = { 0,0,NULL,NULL }, ImboucleNS = { 0,0,NULL,NULL };
	IMAGE ImTopHat = { 0,0,NULL,NULL }, ImErodeB1 = { 0,0,NULL,NULL };
	double R_Inscr = 0, R_Circons = 0;
	STRCT_ELEMENT Str = { 0,0,NULL,NULL }, Str2 = { 0,0,NULL,NULL }, Str3 = { 0,0,NULL,NULL};
	Str = fct_generationElementStructurant(1, 1);
	Str2 = fct_generationElementStructurant(2, 10);
	Str3 = fct_generationElementStructurant(2, 5);
	Imcomp = inverseImage(ImgIn);
	ImBin = seuillageOtsu(Imcomp);
	Imclose = fct_dilatation(ImBin, Str);
	ImFill = fct_bouchageTrou(Imclose);
	Sign_Number.Nb_Euler = fct_EulerNumber(Imclose);
	switch (Sign_Number.Nb_Euler)
	{
	case 0:
		ImNord = Division_Image(ImBin, 1);
		ImSud = Division_Image(ImBin, 0);
		ImNord = fct_dilatation(ImNord,Str2);
		ImSud = fct_dilatation(ImSud, Str2);
		Sign_Number.nb_TrouN = fct_EulerNumber(ImNord);
		Sign_Number.nb_TrouS = fct_EulerNumber(ImSud);
		if (Sign_Number.nb_TrouN ==1)
		{
			Sign_Number.Verdict = 3;
		}
		else if (Sign_Number.nb_TrouS==1)
		{
			Sign_Number.Verdict = 9;
		}
		else
		{
			ImErodeB1 = fct_erosion(ImBin, Str2);
			ImLab = fct_EtiquettageImage(ImErodeB1);
			Sign_Number.NbObject = fct_countobject(ImLab);
			if (Sign_Number.NbObject>0)
			{
				Sign_Number.Verdict = 2;
			}
			else
			{
				Sign_Number.Verdict = 6;
			}
		}

		break;
	case 1 :
	// ******************************Calcule des signatures à boucle 1****************************************************************** //
	//Calcule du rapport Rayon_circonscrit/rayon_inscrit pour déterminer un objet de forme circulaire									//
	//Ensuite On mesure l'aire de la boucle pour distinguer une catégorie de chiffre. Faible Air : 1,2,3. Grand Air : 578			   //
	//Calcule de Tophat et comptage des objets ==> 0 objet-->5, 1 objet --> 7 et 2 objet --> 8										  //
	//Dicision de l'image: Nord et Sud. partie Nord ==> Une boucle --> 3. Partie sud mesure d'air, petit air -->2, grand air --> 1   //
	//******************************************************************************************************************************//
		R_Inscr = Cercle_Inscrit(Imclose);
		R_Circons = Cercle_Circonscrit(Imclose);
		Sign_Number.R_InscR_Circons = (R_Circons/R_Inscr);
		if (Sign_Number.R_InscR_Circons<=2.5)
		{
			Sign_Number.Verdict = 0;
		}
		else if (Sign_Number.R_InscR_Circons>2.5)
		{
			Imboucl = fct_Ouverture(ImFill, Str);
			Sign_Number.Aire_BoucleOne = fct_Aire(Imboucl);
			if (Sign_Number.Aire_BoucleOne<500)
			{
				ImSud = Division_Image(Imclose,0);
				ImNord = Division_Image(Imclose,1);
				Sign_Number.nb_TrouN = fct_EulerNumber(ImNord);
				Sign_Number.nb_TrouS = fct_EulerNumber(ImSud);
				if (Sign_Number.nb_TrouN !=0)
				{
					Sign_Number.Verdict = 3;
				}
				else if(Sign_Number.nb_TrouS !=0)
				{
					ImboucleNS = fct_dilatation(ImSud,Str);
					Sign_Number.Aire_BoucleOne = fct_Aire(ImboucleNS);
					if (Sign_Number.Aire_BoucleOne<300)
					{
						Sign_Number.Verdict = 2;
					}
					else
					{
						Sign_Number.Verdict = 1;
					}
				}
			}
			else
			{
				ImTopHat = fct_TopHat(ImFill);
				ImLab = fct_EtiquettageImage(ImTopHat);
				Sign_Number.NbObject = fct_countobject(ImLab);
				if (Sign_Number.NbObject ==0)
				{
					Sign_Number.Verdict = 5;
				}
				else if(Sign_Number.NbObject==1)
				{
					Sign_Number.Verdict = 7;
				}
				else
				{
					Sign_Number.Verdict = 8;
				}
			}
		}
		break;
	case 2 :
		ImFill = fct_bouchageTrou(ImBin);
		Imboucl = fct_Ouverture(ImFill, Str);
		Sign_Number.Aire = fct_Aire(Imboucl);
		if (Sign_Number.Aire>2000)
		{
			Sign_Number.Verdict = 4;
		}
		else
		{
			Sign_Number.Verdict = 2;
		}
		break;
	default:
		break;
	}
	liberationImage(&ImBin);
	liberationImage(&Imcomp);
	liberationImage(&ImLab);
	liberationImage(&ImFill);
	liberationImage(&Imclose);
	liberationImage(&Imboucl);
	liberationImage(&ImNord);
	liberationImage(&ImSud);
	liberationImage(&ImboucleNS);
	liberationImage(&ImTopHat);
	liberationImage(&ImErodeB1);
	liberationStrElement(&Str);
	liberationStrElement(&Str2);
	return(Sign_Number);
}
