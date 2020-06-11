#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libImages.h"
#include "libOutils.h"
#include <string.h>
#include<time.h>
typedef char NOM[100]; // nom des chemin+images born� � 100 caract�res

void main(void)
{
	/*********************************************************
	/		Declaration et initilisation des variables		*
	/******************************************************/
	FILE *F = NULL;
	FILE *SaveFile = NULL;
	NOM *liste = NULL; // liste des images
	NOM ligne;
	IMAGE Imin = { 0,0,NULL,NULL }, ImBin = { 0,0,NULL,NULL };
	SIGNATURES_OCR Sign_OCR = { 0,0,0,0,0,0,0,0 };
	MATRICE_INT Mat_Confusion = { 0,0,NULL};
	Mat_Confusion = initialisationMatrice(10, 10, 0,0);
	int nb = 0, *RealNumber = NULL, Nb_ImageParDossier=0, RealNumberMono = 0, DossierLect = 0;
	char *nom = NULL;
	NOM *ListeCop = NULL, *ListCopVerif = NULL;
	char Folder[10], NomFichier[8], Tempo[11];
	const char * separators = " ,-!:\\";
	char Dossier[50];
	int ind = 0, indiC_nbToken = 0, choixDeLecture = 0, ImAleaToir = 0, Nb_ImageALire = 0;
	double Vraidetect = 0;
	srand((unsigned)time(NULL));
	//mise en m�moire images pgm/ppm/
	//system("dir test /S /B *.p*m > listeDesImagestest.txt");
	//system("dir train /S /B  *.p*m > listeDesImagestrain.txt");

	/*********************************************************
	/					Lecture de fichier liste			*
	/******************************************************/
	printf("Choisissez le dossier a lire (0--> train /1--> test) :\n");
	scanf("\n%d", &DossierLect);
	while (DossierLect != 0 && DossierLect != 1)
	{
		printf("Choisissez le dossier a lire (0--> train /1--> test) :\n");
		scanf("\n%d", &DossierLect);
	}
	if (DossierLect==0)
	{
		F = fopen("listeDesImagestrain.txt", "r");
	}
	else
	{
		F = fopen("listeDesImagestest.txt", "r");
	}
	SaveFile = fopen("..\\Res\\Result.csv", "w");	// Création ou ouverture du fichier de sauvegarde des donnees
	while (fscanf(F, "%s",ligne) > 0)
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

	/*********************************************************************
	/		Mise en forme de la liste pour une lecture plus souple		*
	/*******************************************************************/
	// Recupération de la taille de la ligne
	int i = 0;
	while (liste[0][i] != NULL)
	{
		i++;
	}
	ListeCop = (NOM*)malloc(i*sizeof(NOM));
	strcpy(ListeCop, liste[0]);	// Copy de la premiere ligne car le Strtok detruit la chaine de caractere initiale
	char * strToken = NULL;
	strToken = strtok(ListeCop, separators);
	// Recuperation du nombre de Token
	while (strToken != NULL) {
		// On demande le token suivant.
		strToken = strtok(NULL, separators);
		indiC_nbToken++;
	}
	strToken = NULL;
	RealNumber = (int*)malloc(nb * sizeof(int*));
	//********************************************************************************************
	//	Mise en forme de la liste sous la forme : Sous-dossier//nom image ==> ex : 00//001.pgm	*
	//******************************************************************************************
	for (int i = 0; i < nb; i++)
	{
		strToken = strtok(liste[i], separators);
		while (strToken != NULL) {
			ind++;
			if (ind == indiC_nbToken-1)
			{
				strcpy(Folder, strToken);
				RealNumber[i] = atoi(Folder);
			}
			else if (ind ==indiC_nbToken)
			{
				strcat(Folder, "\\");
				strcpy(Tempo, Folder);
				strcat(Tempo, strToken);
				strcpy(liste[i], Tempo);
			}
			// On demande le token suivant.
			strToken = strtok(NULL, separators);
		}
		ind = 0;
	}
	/***********************************************
	/				Lecture des images			  *
	/*********************************************/
	printf("Choisissez le mode de lecture (0--> Lire tous le dossier/1--> lire une image) :\n");
	scanf("\n%d", &choixDeLecture);
	while (choixDeLecture!=0 && choixDeLecture!=1)
	{
		printf("Choisissez le mode de lecture (0--> Lire tous le dossier/1--> lire une image) :\n");
		scanf("\n%d", &choixDeLecture);
	}
	switch (choixDeLecture)
	{
	case 0 :
		for (int i = 0; i<nb; i++)
		{
			if (DossierLect == 0)
			{
				strcpy(Dossier, "train\\");
			}
			else
			{
				strcpy(Dossier, "test\\");
			}
			strcat(Dossier, liste[i]);
			Imin = lectureImage(Dossier);
			Sign_OCR = fct_calcSignature(Imin);
			Mat_Confusion.data[RealNumber[i]][Sign_OCR.Verdict] += 1;
			// Test et prise de decision pour attribuer les chiffres
			liberationImage(&Imin);
		}
		double tempRes = 0;
		Nb_ImageParDossier = (nb / 10) + 1;
		for (int i = 0; i < Mat_Confusion.height; i++)
		{
			for (int j = 0; j < Mat_Confusion.width; j++)
			{
				tempRes = (double)(((double)Mat_Confusion.data[i][j] / Nb_ImageParDossier) * 100);
				fprintf(SaveFile, "%.2lf;", tempRes);
			}
			fprintf(SaveFile, "\n");
		}
		break;
	case 1 : 
		printf("Combien d'image souhaitez-vous lire ? 10 maximum\n");
		ListCopVerif = (NOM*)malloc(sizeof(NOM));
		scanf("%d", &Nb_ImageALire);
		strcpy(ListCopVerif, liste[ImAleaToir]);
		strToken = strtok(ListCopVerif, separators);
		// Recuperation du nombre de Token
		while (strToken != NULL) {
			// On demande le token suivant.
			strToken = strtok(NULL, separators);
			indiC_nbToken++;
		}
		for (int i = 0;i < Nb_ImageALire;i++)
		{
			ImAleaToir = 0 + rand() % (nb + 1 - 0);
			indiC_nbToken = 0;
			if (DossierLect == 0)
			{
				strcpy(Dossier, "train\\");
			}
			else
			{
				strcpy(Dossier, "test\\");
			}
			strcat(Dossier, liste[ImAleaToir]);
			Imin = lectureImage(Dossier);
			Sign_OCR = fct_calcSignature(Imin);

			/***********************************************
			/		Recherche du vrai chiffre			  *
			/*********************************************/

			strcpy(ListCopVerif, liste[ImAleaToir]);
			strToken = strtok(ListCopVerif, separators);
			// Recuperation du nombre de Token
			while (strToken != NULL) {
				// On demande le token suivant.
				strToken = strtok(NULL, separators);
				indiC_nbToken++;
			}
			strToken = strtok(liste[ImAleaToir], separators);
			while (strToken != NULL) {
				ind++;
				if (ind == indiC_nbToken - 1)
				{
					strcpy(Folder, strToken);
					RealNumberMono = atoi(Folder);
				}
				// On demande le token suivant.
				strToken = strtok(NULL, separators);

			}
			ind = 0;
			if (Sign_OCR.Verdict != RealNumberMono)
			{
				printf("Vrai chiffre : %d\tChiffre trouve : %d\tMauvaise detection\n", RealNumberMono, Sign_OCR.Verdict);
			}
			else
			{
				Vraidetect++;
			}
		}
		double percentage = (Vraidetect / (double)Nb_ImageALire)*100;
		printf("%.2lf %% de vrai detection ", percentage);
		break;
	default:
		break;
	}
	liberationMatrice(&Mat_Confusion);
	free(RealNumber);
	free(ListeCop);
	free(ListCopVerif);
	free(liste);
	system("pause");
}