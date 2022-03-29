#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "LibOutils.h"

/* calcul1 : produit entre N et M basés sur * et / par 2 uniquement */
int calcul1(int N, int M)
{
	int tempN = N, tempM = M;
	int S2 = 0;

	printf("Entre dans calcul1\n");

	while (tempN != 1)
	{
		if (tempN % 2 == 1)
			S2 += tempM;
		tempN /= 2;
		tempM *= 2;
	}
	S2 = S2 + tempM;
	/* fin du calcul */

	printf("Sortie\n");

	return(S2);
}


int arrangements(int n, int k)
{
	return (factorielle(n) / factorielle(n - k));
}

int combinaisons(int n, int k)
{
	return(arrangements(n, k) / factorielle(k));
}

int factorielle(int n)
{
	int res = 1;

	for (int i = n; i > 0; i--)
		res *= i;

	return(res);
}

void affichageCombinatoire(int n, int k)
{
	printf("A(%d,%d) = %d\n", n, k, arrangements(n, k));
	printf("C(%d,%d) = %d\n", n, k, combinaisons(n, k));
}


void miseTableauAuCarre(int tIn[], int n, int tOut[])
{
	for (int i = 0; i < n; i++)
		tOut[i] = tIn[i] * tIn[i];
}


void affichageTableauInt(int t[], int n)
{
	printf("\naffichage tableau ---------\n");
	for (int i = 0; i < n; i++)
		printf("%d ", t[i]);
	printf("\nfin affichage tableau -----\n");
}

void initialisationAleatoireTableauInt(int t[], int n, int valMin, int valMax)
{
	/* srand((unsigned)time(NULL)); */
	for (int i = 0; i < n; i++)
		t[i] = valMin + rand() % (valMax + 1 - valMin);
}

int minTableauInt(int tab[], int n)
{
	int min = tab[0];
	for (int i = 1; i < n; i++)
		if (tab[i] < min)
			min = tab[i];

	return(min);
}

int maxTableauInt(int tab[], int n)
{
	int max = tab[0];
	for (int i = 1; i < n; i++)
		if (tab[i] > max)
			max = tab[i];

	return(max);
}

void copieTableauInt(int t1[], int t2[], int n)
{
	for (int i = 0; i < n; i++)
		t2[i] = t1[i];
}

void triEchangeTableauInt(int tab[], int n)
{
	for (int i = 0; i < n; i++)
	{
		int indMin = i;
		for (int j = i; j < n; j++)
			if (tab[j] < tab[indMin])
				indMin = j;
		if (i != indMin)
		{
			int temp = tab[indMin];
			tab[indMin] = tab[i];
			tab[i] = temp;
		}
	}
}

int seuilOtsu(int H[], int n)
{
	int min = 0, max = 0;
	int i = 0;
	while (H[i] == 0)
		i++;
	min = i;
	i = 255;
	while (H[i] == 0)
		i--;
	max = i;

	printf("Ng entre %d et %d\n", min, max);

	float V[256] = { 0 }; // pas nécessaire si mise à jour de la valeur max (et donc du seuil) pas à pas.
						  // histo cumulé
	int HC[256] = { 0 };

	HC[min] = H[min];
	for (int i = min + 1; i <= max; i++)
		HC[i] = HC[i - 1] + H[i];

	/* initialisation */
	float M1 = (float)min;
	float W1 = (float)H[min] / (float)(HC[max]);
	float M2 = (float)0;
	for (int i = min + 1; i <= max; i++)
		M2 += (float)H[i] * i;
	M2 /= (float)(HC[max] - H[min]);
	V[min] = W1 * (1 - W1) * (M1 - M2) * (M1 - M2);
	int otsu = min;

	// parcours
	for (int s = min + 1; s < max; s++) {
		M1 = ((float)HC[s - 1] * M1 + (float)H[s] * s) / HC[s];
		M2 = ((float)(HC[max] - HC[s - 1]) * M2 - (float)(H[s] * s)) / (float)(HC[max] - HC[s]);
		W1 = (float)HC[s] / (float)(HC[max]);
		V[s] = W1 * (1 - W1) * (M1 - M2) * (M1 - M2);
		if (V[s] > V[otsu])
			otsu = s;
	}

	return(otsu);
}

/* POINTS */
POINT lecturePoint()
{
	POINT pt;

	printf("Abscisse : ");
	scanf("%f", &pt.abscisse);
	printf("Ordonnee : ");
	scanf("%f", &pt.ordonnee);

	return(pt);
}

void affichePoint(POINT p)
{
	printf("(%.2f,%.2f)\n", p.abscisse, p.ordonnee);
}

float distancePoints(POINT p1, POINT p2)
{
	return ((float)sqrt((p1.abscisse - p2.abscisse) * (p1.abscisse - p2.abscisse) + (p1.ordonnee - p2.ordonnee) * (p1.ordonnee - p2.ordonnee)));
}

POINT milieuPoints(POINT p1, POINT p2)
{
	POINT milieu;

	milieu.abscisse = (p1.abscisse + p2.abscisse) / 2;
	milieu.ordonnee = (p1.ordonnee + p2.ordonnee) / 2;
	return(milieu);
}

void generationAleatoirePoints(POINT t[], int n, float min, float max)
{
	/* srand((unsigned)time(NULL)); */
	for (int i = 0; i < n; i++)
	{
		t[i].abscisse = ((float)rand() / (float)RAND_MAX) * (max - min) + min;
		t[i].ordonnee = ((float)rand() / (float)RAND_MAX) * (max - min) + min;
	}
}

POINT centreGravitePoints(POINT t[], int n) {
	POINT cG = { 0,0 };

	for (int i = 0; i < n; i++)
	{
		cG.abscisse += t[i].abscisse;
		cG.ordonnee += t[i].ordonnee;
	}
	cG.abscisse /= (float)n;
	cG.ordonnee /= (float)n;

	return(cG);
}


char tirageLettre(void)
{
	return('a' + rand() % ('z' + 1 - 'a'));
}

char* tirageMot(int taille)
{
	char* mot = NULL;
	int i;

	mot = (char*)malloc((taille + 1) * sizeof(char)); /* intégrer le caractère fin de chaîne */
	for (i = 0; i < taille; i++)
		mot[i] = tirageLettre();
	mot[i] = '\0';

	return(mot);
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

	*taille2 = (taille1 - nbVoy) + nbVoy * coef;
	*mot2 = (char*)malloc((*taille2 + 1) * sizeof(char)); /* intégrer le caractère fin de chaîne */

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
	char* mot2 = NULL;
	int i;

	mot2 = (char*)malloc((taille + 1) * sizeof(char)); /* intégrer le caractère fin de chaîne */
	for (i = 0; i < taille; i++)
		mot2[i] = mot[i];
	mot2[i] = '\0';

	return(mot2);
}

void transformMotMajuscule(char* mot)
{
	mot[0] = mot[0] - 'a' + 'A';
}

void transformMotMinuscule(char* mot)
{
	mot[0] = mot[0] - 'A' + 'a';
}

/* structures MOT et PHRASE */

MOT generationAleatoireMot(int taille)
{
	MOT mot = { 0,NULL };
	int i;

	mot.longueur = taille;
	mot.data = (char*)malloc((taille + 1) * sizeof(char)); /* intégrer le caractère fin de chaîne */
	for (i = 0; i < taille; i++)
		mot.data[i] = tirageLettre();
	mot.data[i] = '\0';

	return(mot);
}

void libereMot(MOT* pMot)
{
	free(pMot->data);
	pMot->data = NULL;
	pMot->longueur = 0;
}

PHRASE tiragePhrase(int nbMots)
{
	PHRASE phrase = NULL;

	phrase = (PHRASE)malloc(nbMots * sizeof(MOT));
	for (int i = 0; i < nbMots; i++)
	{
		int nbLettres = rand() % 11; /* entre 0 et 10 lettres */
		phrase[i] = generationAleatoireMot(nbLettres);
	}

	return(phrase);
}

void liberePhrase(PHRASE* ptPhrase, int nbMots)
{
	// sens inverse de l'allocation
	for (int i = 0; i < nbMots; i++)
		libereMot(&(*ptPhrase)[i]);
	free(*ptPhrase);

	*ptPhrase = NULL;
}

/* matrice d'entiers */
MATRICE initialisationMatrice(int h, int l, int vMin, int vMax) {
	MATRICE matrice = { 0,0,NULL };
	int i, j;

	matrice.hauteur = h;
	matrice.largeur = l;

	matrice.valeurs = (int**)malloc(matrice.hauteur * sizeof(int*));
	if (matrice.valeurs == NULL)
		printf("Pb allocation matrice.\n");
	else
	{
		int i;
		for (i = 0; i < matrice.hauteur; i++)
		{
			matrice.valeurs[i] = (int*)malloc(matrice.largeur * sizeof(int));
			if (matrice.valeurs[i] == NULL)
				printf("Pb allocation ligne %d.", i);
		}
	}

	for (i = 0; i < matrice.hauteur; i++)
		for (j = 0; j < matrice.largeur; j++)
			matrice.valeurs[i][j] = vMin + rand() % (vMax + 1 - vMin);

	return(matrice);
}

void affichageMatrice(MATRICE matrice)
{
	for (int i = 0; i < matrice.hauteur; i++)
	{
		for (int j = 0; j < matrice.largeur; j++)
			printf("%d\t", matrice.valeurs[i][j]);
		printf("\n");
	}
}

void liberationMatrice(MATRICE* matrice) {
	int i;

	for (i = 0; i < matrice->hauteur; i++)
		free(matrice->valeurs[i]);
	free(matrice->valeurs);
	matrice->valeurs = NULL;
}

int traceMatrice(MATRICE matrice)
{
	int trace = 0;
	int i;

	if (matrice.hauteur == matrice.largeur)
	{
		for (i = 0; i < matrice.hauteur; i++)
			trace += matrice.valeurs[i][i];
	}
	else
	{
		printf("Matrice non carree pour trace\n");
	}
	return(trace); /* vigilance sur exploitation de ce résultat dans focntion appelante */
}

MATRICE additionMatrice(MATRICE m1, MATRICE m2)
{
	MATRICE m = { 0,0,NULL };

	if ((m1.hauteur == m2.hauteur) && (m1.largeur == m2.largeur))
	{

		m = initialisationMatrice(m1.hauteur, m1.largeur, 0, 0); /* allocation mémoire et mise à 0 (bof) */
		for (int i = 0; i < m1.hauteur; i++)
			for (int j = 0; j < m1.largeur; j++)
				m.valeurs[i][j] = m1.valeurs[i][j] + m2.valeurs[i][j];
	}
	else
		printf("Tailles matrices incompatibles pour addition\n");

	return(m);
}

MATRICE multiplicationMatrice(MATRICE m1, MATRICE m2)
{
	MATRICE m = { 0,0,NULL };

	if (m1.largeur == m2.hauteur)
	{
		m = initialisationMatrice(m1.hauteur, m2.largeur, 0, 0); /* allocation mémoire et mise à 0 (bof) */
		int somme = 0;
		for (int i = 0; i < m1.hauteur; i++)
			for (int j = 0; j < m1.largeur; j++)
			{
				somme = 0;
				for (int k = 0; k < m1.largeur; k++)
					somme += m1.valeurs[i][k] * m2.valeurs[k][j];
				m.valeurs[i][j] = somme;
			}
	}
	else
		printf("Tailles matrices incompatibles pour multiplication\n");

	return(m);
}

iMATRICE allocationiMatrice(int hauteur, int largeur)
{
	iMATRICE mat = { 0,0,NULL,NULL };
	int i;

	mat.hauteur = hauteur;
	mat.largeur = largeur;
	mat.valeurs = (int*)calloc(largeur * hauteur, sizeof(int)); /* mise à 0 par défaut */
	if (mat.valeurs == NULL)
		return(mat);
	mat.ptLignes = (int**)malloc(hauteur * sizeof(int*));
	if (mat.ptLignes == NULL) {
		free(mat.valeurs);
		mat.valeurs = NULL;
		return(mat);
	}
	for (i = 0; i < hauteur; i++)
		mat.ptLignes[i] = &mat.valeurs[i * largeur];

	return(mat);
}

void remplissageAleatoireiMatrice(iMATRICE matrice, int valMin, int valMax)
{
	//// parcours en (i,j)
	//for (int i = 0; i < matrice.hauteur; i++)
	//	for (int j = 0; j < matrice.largeur; j++)
	//		matrice.ptLignes[i][j] = valMin + rand() % (valMax + 1 - valMin);
	// parcours en (ind)
	for (int ind = 0; ind < matrice.hauteur * matrice.largeur; ind++)
		matrice.valeurs[ind] = valMin + rand() % (valMax + 1 - valMin);
}

void affichageiMatrice(iMATRICE matrice)
{
	// intérêt parcours en (i,j)
	for (int i = 0; i < matrice.hauteur; i++)
	{
		for (int j = 0; j < matrice.largeur; j++)
			printf("%d\t", matrice.ptLignes[i][j]);
		printf("\n");
	}
}
void liberationiMatrice(iMATRICE* ptMatrice)
{
	if (ptMatrice->ptLignes != NULL) {
		free(ptMatrice->ptLignes);
		ptMatrice->ptLignes = NULL;
	}
	if (ptMatrice->valeurs != NULL) {
		free(ptMatrice->valeurs);
		ptMatrice->valeurs = NULL;
	}
}


iMATRICE additioniMatrice(iMATRICE m1, iMATRICE m2)
{
	iMATRICE m_addi = { 0,0,NULL, NULL };


	if ((m1.hauteur == m2.hauteur) && (m1.largeur == m2.largeur))
	{

		m_addi = allocationiMatrice(m1.hauteur, m1.largeur); /* allocation mémoire et mise à 0 (bof) */

		for (int i = 0; i < m1.hauteur; i++)
		{
			for (int j = 0; j < m1.largeur; j++)
			{
				m_addi.ptLignes[i][j] = m1.ptLignes[i][j] + m2.ptLignes[i][j];


			}

		}
	}
	else
		printf("Tailles imatrices incompatibles pour addition\n");

	return m_addi;
}


iMATRICE TransposeiMatrice(iMATRICE m1)
{
	iMATRICE mt = { 0, 0, NULL, NULL };

	mt = allocationiMatrice(m1.largeur, m1.hauteur);

	for (int i = 0; i < m1.hauteur; i++)
	{
		for (int j = 0; j < m1.largeur; j++)
		{
			mt.ptLignes[i][j] = m1.ptLignes[j][i];
		}

	}

	return mt;

}


