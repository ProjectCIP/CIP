#pragma once

/* calcul du produit entre N et M */
/* N et M en entrée */
int calcul1(int N, int M);

/* fonctions */
int factorielle(int n);
int arrangements(int n, int k);
int combinaisons(int n, int k);

/* procédures -> fonctions type void en retour */
void affichageCombinatoire(int n, int k);

/* tableaux statiques */
void miseTableauAuCarre(int tIn[], int n, int tOut[]);

void affichageTableauInt(int t[], int n);
void initialisationAleatoireTableauInt(int t[], int n, int valMin, int valMax);

int minTableauInt(int t[], int n);
int maxTableauInt(int t[], int n);
void copieTableauInt(int t1[], int t2[], int n);
void triEchangeTableauInt(int t[], int n);

/* seuillage Otsu */
int seuilOtsu(int H[], int n);

/* structures */
typedef struct point {
	float abscisse;
	float ordonnee;
} POINT;

POINT lecturePoint();
void affichePoint(POINT p);
float distancePoints(POINT p1, POINT p2);
POINT milieuPoints(POINT p1, POINT p2);
void generationAleatoirePoints(POINT t[], int n, float min, float max);
POINT centreGravitePoints(POINT t[], int n);

/* mots et phrases */

char tirageLettre(void);
char* tirageMot(int taille);
void multiplicationVoyellesMot(char* mot1, int taille1, int coef, char** mot2, int* taille2);
char* copieMot(char* mot, int taille);
void transformMotMajuscule(char* mot);
void transformMotMinuscule(char* mot);

/* structure MOT, avec lettres + taille */

typedef struct mot {
	int longueur;
	char* data;
} MOT;

typedef MOT* PHRASE;

MOT generationAleatoireMot(int taille);
void libereMot(MOT* pMot);
PHRASE tiragePhrase(int nbMots);
void liberePhrase(PHRASE* phrase, int nbMots);

/* structure MATRICE */

typedef struct matrice {
	int hauteur;
	int largeur;
	int** valeurs;
} MATRICE;

MATRICE initialisationMatrice(int hauteur, int largeur, int valMin, int valMax);
void affichageMatrice(MATRICE matrice);
void liberationMatrice(MATRICE* matrice);
int traceMatrice(MATRICE matrice);
MATRICE additionMatrice(MATRICE m1, MATRICE m2);
MATRICE multiplicationMatrice(MATRICE m1, MATRICE m2);

typedef struct iMatrice {
	int hauteur;
	int largeur;
	int* valeurs;
	int** ptLignes;
} iMATRICE;

iMATRICE allocationiMatrice(int hauteur, int largeur);
void remplissageAleatoireiMatrice(iMATRICE matrice, int valMin, int valMax);
void affichageiMatrice(iMATRICE matrice);
void liberationiMatrice(iMATRICE* ptMatrice);

iMATRICE additioniMatrice(iMATRICE m1, iMATRICE m2);

iMATRICE TransposeiMatrice(iMATRICE m1);


/* exe */

void chercherVal(int tab[], int n, int A, int* pos, int* nb_occ);