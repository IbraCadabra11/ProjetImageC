

#pragma once
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	char couleur[20];
	int R;
	int G;
	int B;
} COULEUR;

typedef struct {
	double abscisse;
	double ordonnee;
} POINT;

typedef struct {
	int size;
	int *data;
} TABLEAU_INT;

typedef struct {
	int size;
	double* data;
} TABLEAU_DOUBLE;

typedef struct {
	int height;
	int width;
	int **data;
} MATRICE_INT;

typedef struct {
	int min;
	int max;
	int mediane;
	double moyenne;
	double sigma;
} MOMENTS;

int fonction(int NN, int MM);

int factorielle(int n);
int arrangements(int n, int k);
int combinaisons(int n, int k);

void affichageTableau(int t[], int n);
void initialisationAleatoireTableau(int t[], int n, int valMin, int valMax);

void affichage(COULEUR c);
double distanceEuclidienne(COULEUR c1, COULEUR c2);

void affichePoint(POINT p);
POINT milieuPoints(POINT p1, POINT p2);

void milieuPointsBis(POINT p1, POINT p2, POINT *ptm);

void echangeInt(int *a, int *b);
int* allocationTableauInt(int n);

TABLEAU_INT allocationTableau(int n);
void minMaxTableau(TABLEAU_INT t, int *Min, int *Max);
MOMENTS momentsTableau(TABLEAU_INT t);
void liberationTableau(TABLEAU_INT *t);
void liberationTableauDouble(TABLEAU_DOUBLE* t);
TABLEAU_DOUBLE allocationTableauDouble(int n);

char tirageLettre(void);
char* tirageMot(int taille);
void transformMot(char*  mot);
void multiplicationVoyellesMot(char* mot1, int taille1, int coef, char** mot2, int* taille2); 
char* copieMot(char* mot, int taille);

TABLEAU_INT ligneTriangle(int numero);
void afficheLigneTriangle(TABLEAU_INT ligne);
TABLEAU_INT* constructionTriangle(int taille);
void afficheTriangle(TABLEAU_INT* triangle, int taille);

MATRICE_INT initialisationMatrice(int hauteur, int largeur, int valMin, int valMax);
void affichageMatrice(MATRICE_INT matrice);
void liberationMatrice(MATRICE_INT *matrice);
int traceMatrice(MATRICE_INT matrice);







