#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "libOutils.h"

int fonction(int NN, int MM)
{
	int S = 0;

	while (NN != 1)
	{
		if (NN % 2 == 1)
			S = S + MM;
		NN = NN / 2;
		MM = MM * 2;
	}
	S = S + MM;

	return (S);
}

int factorielle(int n)
{
	int res = 1;
	int i;

	for (i = n; i>0; i--)
		res *= i;

	return(res);
}

int arrangements(int n, int k)
{
	return (factorielle(n) / factorielle(n - k));
}

int combinaisons(int n, int k)
{
	return(arrangements(n, k) / factorielle(k));
}

void affichageTableau(int t[], int n)
{
	int i = 0;
	int sautLigne = 0;

	printf("\naffichage tableau ---------\n");
	for (i = 0, sautLigne = 1; i < n; i++, sautLigne++)
	{
		printf("%d", t[i]);
		if (sautLigne % 10 != 0)
			printf("\t");
		else
			printf("\n");
	}
	printf("\nfin affichage tableau -----\n");
}

void initialisationAleatoireTableau(int t[], int n, int valMin, int valMax)
{
	int i = 0;

	for (i = 0; i<n; i++)
		t[i] = valMin + rand() % (valMax + 1 - valMin);
}

void affichage(COULEUR c)
{
	printf("%s -> (%d,%d,%d)\n", c.couleur, c.R, c.G, c.B);
}

double distanceEuclidienne(COULEUR c1, COULEUR c2)
{
	double ecart = (double)(c1.R - c2.R)*(c1.R - c2.R);
	double d = ecart;

	ecart = (double)(c1.G - c2.G)*(c1.G - c2.G);
	d += ecart;

	ecart = (double)(c1.B - c2.B)*(c1.B - c2.B);
	d += ecart;

	d = sqrt(d);
	return d;
}

void affichePoint(POINT p)
{
	printf("(%.2lf,%.2lf)\n",p.abscisse,p.ordonnee);
}

POINT milieuPoints(POINT p1, POINT p2)
{
	POINT p;

	p.abscisse = (p1.abscisse + p2.abscisse) / 2;
	p.ordonnee = (p1.ordonnee + p2.ordonnee) / 2;

	return(p);
}

void milieuPointsBis(POINT p1, POINT p2, POINT *ptm)
{
	ptm->abscisse = (p1.abscisse + p2.abscisse) / 2;
	ptm->ordonnee = (p1.ordonnee + p2.ordonnee) / 2;
}

void echangeInt(int *a, int *b) /* passage par valeurs adresse */
{
	int inter;

	inter = *b;
	*b = *a;
	*a = inter;
}

int* allocationTableauInt(int n)
{
	int *p = NULL;

	//p = (int*)malloc(n*sizeof(int));
	p = (int*)calloc(n,sizeof(int)); 

	return(p);
}

TABLEAU_INT allocationTableau(int n)
{
	TABLEAU_INT t = { 0,NULL };

	t.size = n;
	t.data = (int*)calloc(n, sizeof(int));

	return(t);
}

TABLEAU_DOUBLE allocationTableauDouble(int n)

{
	TABLEAU_DOUBLE t = { 0,NULL };

	t.size = n;
	t.data = (double*)calloc(n, sizeof(double));

	return(t);




}

void liberationTableau(TABLEAU_INT *t)
{
	free(t->data);
	t->data = NULL;
	t->size = 0;
}

void liberationTableauDouble(TABLEAU_DOUBLE* t) 
{
	free(t->data);
	t->data = NULL;
	t->size = 0;
}

void minMaxTableau(TABLEAU_INT t, int *Min, int *Max) 
{
	*Min = t.data[0];
	*Max = t.data[0]; 

	for (int i = 1; i < t.size; i++)
	{
		if (t.data[i] < *Min)
			*Min = t.data[i];
		if (t.data[i] > *Max)
			*Max = t.data[i];
	}
}

MOMENTS momentsTableau(TABLEAU_INT t)
{
	MOMENTS sig;
	int i;
	double som = 0, som2 = 0;
	int somme, moitPop = t.size / 2;

	TABLEAU_INT h = allocationTableau(256); /* valeurs entre 0 et 255 uniquement */

	for (i = 0; i < t.size; i++) /* génération histogramme parcours tableau entrée une fois pour toutes, tous les moments en découlent */
		h.data[t.data[i]] += 1;

	for (i = 0; i < h.size; i++) {
		som += (double)h.data[i]*i;
		som2 += ((double)h.data[i]*i) * ((double)h.data[i]*i);
	}

	som /= (double)t.size;
	som2 = sqrt(som2 / (double)t.size - som*som);

	sig.moyenne = som;
	sig.sigma = som2;

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

	free(h.data);
	h.data = NULL;

	return sig;
}

char tirageLettre(void)
{
	return('a' + rand() % ('z' + 1 - 'a'));
}

char* tirageMot(int taille)
{
	char *mot = NULL;
	int i;

	mot = (char*)malloc((taille + 1)*sizeof(char)); /* intégrer le caractère fin de chaîne */
	for (i = 0; i < taille; i++)
		mot[i] = tirageLettre();
	mot[i] = '\0';

	return(mot);
}

void transformMot(char*  mot)
{
	mot[0] = mot[0] - 'a' + 'A';
}

void multiplicationVoyellesMot(char* mot1, int taille1, int coef, char** mot2, int* taille2)
{
	int i, j;
	int nbVoy = 0;

	/* compter le nombre de voyelles pour allouer nouveau mot*/
	for (i = 0; i < taille1; i++)
	{
		switch (mot1[i]) {
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		case 'y':
			nbVoy++;
		}
	}

	*taille2 = (taille1 - nbVoy) + nbVoy*coef;
	*mot2 = (char*)malloc((*taille2 + 1)*sizeof(char)); /* intégrer le caractère fin de chaîne */

	int indice = 0;
	for (i = 0; i < taille1; i++)
	{
		switch (mot1[i]) {
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		case 'y':
			for (j = 0; j < coef; j++)
				(*mot2)[indice++] = mot1[i];
			break;
		default: (*mot2)[indice++] = mot1[i];
		}
	}
	(*mot2)[indice] = '\0'; /* caractère fin de chaîne */
}

char* copieMot(char* mot, int taille)
{
	char *mot2 = NULL;
	int i;

	mot2 = (char*)malloc((taille + 1)*sizeof(char)); /* intégrer le caractère fin de chaîne */
	for (i = 0; i < taille; i++)
		mot2[i] = mot[i];
	mot2[i] = '\0';

	return(mot2);
}


TABLEAU_INT ligneTriangle(int numero)
{
	TABLEAU_INT t = { 0,NULL };
	int i;

	t = allocationTableau(numero + 1);
	for (i = 0; i <= numero; i++)
		t.data[i] = combinaisons(numero, i);

	return(t);
}

void afficheLigneTriangle(TABLEAU_INT ligne)
{
	int i;

	for (i = 0; i < ligne.size; i++)
		printf("%d\t", ligne.data[i]);
}

TABLEAU_INT* constructionTriangle(int taille)
{
	TABLEAU_INT *t = NULL;
	int i;

	t = (TABLEAU_INT*)malloc(taille*sizeof(TABLEAU_INT));
	for (i = 0; i < taille; i++)
		t[i] = ligneTriangle(i);

	return(t);
}

void afficheTriangle(TABLEAU_INT* triangle, int taille)
{
	int i;

	for (i = 0; i < taille; i++)
	{
		afficheLigneTriangle(triangle[i]);
		printf("\n");
	}
}


void affichageMatrice(MATRICE_INT matrice)
{
	int i, j;

	printf("\naffichage matrice ---------\n");
	for (i = 0; i<matrice.height; i++)
	{
		for (j = 0; j < matrice.width; j++)
			printf("%d\t", matrice.data[i][j]);
		printf("\n");
	}
	printf("\nfin affichage matrice -----\n");
}

MATRICE_INT initialisationMatrice(int h, int l, int vMin, int vMax) {
	MATRICE_INT matrice = { 0,0,NULL };
	int i, j;

	matrice.height = h;
	matrice.width = l;	
	
	matrice.data = (int**)calloc(matrice.height, sizeof(int*));
	if (matrice.data == NULL)
		printf("Pb allocation matrice.\n");
	else
	{
		int i;
		for (i = 0; i < matrice.height; i++)
			matrice.data[i] = (int*)calloc(matrice.width, sizeof(int*));
		if (matrice.data[i] == NULL)
			printf("Pb allocation ligne %d.", i);
	}

	/*for (i = 0; i < matrice.height; i++)
		for (j = 0; j < matrice.width; j++)
			matrice.data[i][j] = vMin + rand() % (vMax + 1 - vMin);*/
	return(matrice);
}

void liberationMatrice(MATRICE_INT *matrice) {
	int i;

	for (i = 0; i < matrice->height; i++)
		free(matrice->data[i]);
	free(matrice->data);
	matrice->data = NULL;
}

int traceMatrice(MATRICE_INT matrice)
{
	int trace = 0;
	int i;

	if (matrice.height == matrice.width)
	{
		for (i = 0; i < matrice.height; i++)
			trace += matrice.data[i][i];
	}
	else
	{
		printf("Matrice non carree\n");
	}
	return(trace);
}
