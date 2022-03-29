// libCIPSI1.c 
// Hubert Konik - filière IPSI

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "LibCIPSI1.h"

/* static -> non extern */

typedef struct
{
	int Nblig;
	int Nbcol;
	int* data;
	int** pixel;
} intIMAGE;

static intIMAGE allocationIntImage(int Nblig, int Nbcol)
{
	intIMAGE mat = { 0,0,NULL,NULL };
	int i = 0;

	mat.Nblig = Nblig;
	mat.Nbcol = Nbcol;
	mat.data = (int*)calloc(Nblig * Nbcol, sizeof(int)); /* mise à 0 par défaut, gestion des bords lors de l'agrandissement opéré dans l'étiquetage */
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

static void liberationIntImage(intIMAGE* img)
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

/* fonctionnalités -> exportables */

IMAGE allocationImage(int Nblig, int Nbcol)
{
	IMAGE mat = { 0,0,NULL,NULL };
	int i = 0;

	mat.Nblig = Nblig;
	mat.Nbcol = Nbcol;
	mat.data = (unsigned char*)malloc(Nblig * Nbcol * sizeof(unsigned char));
	if (mat.data == NULL)
		return(mat);
	mat.pixel = (unsigned char**)malloc(Nblig * sizeof(unsigned char*));
	if (mat.pixel == NULL) {
		free(mat.data);
		mat.data = NULL;
		return(mat);
	}
	for (i = 0; i < Nblig; i++)
		mat.pixel[i] = &mat.data[i * Nbcol];

	return(mat);
}

void initialisationAleatoireImage(IMAGE img, int ngMin, int ngMax)
{
	int i, j;
	printf("Dans initialisation : img -> %p img.data -> %p img.pixel -> %p\n", &img, img.data, img.pixel);

	for (i = 0; i < img.Nblig; i++)
		for (j = 0; j < img.Nbcol; j++)
			img.pixel[i][j] = (unsigned char)(ngMin + rand() % (ngMax + 1 - ngMin));

	/* solution avec un seul indice
	for (i=0;i<img.Nblig*img.Nbcol;i++)
	img.data[i] = (unsigned char)(ngMin + rand() % (ngMax + 1 - ngMin));
	*/
}

void liberationImage(IMAGE* img)
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

IMAGE lectureImage(const char* in)
{
	FILE* F = NULL;
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

				for (i = 0; i < img.Nblig; i++)
					for (j = 0; j < img.Nbcol; j++)
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
				fread(img.data, sizeof(unsigned char), img.Nbcol * img.Nblig, F);
			}
			else
				printf("Format non supporte pour l'instant...\n");
		fclose(F);
	}
	return img;
}

void sauvegardeImage(IMAGE img, const char* type, const char* out)
{
	FILE* F = NULL;

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

				for (i = 0; i < img.Nblig; i++)
					for (j = 0; j < img.Nbcol; j++)
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
				fwrite(img.data, sizeof(unsigned char), img.Nbcol * img.Nblig, F);
				fclose(F);
			}
	}
	else
		printf("Image vide\n");
}

IMAGERGB allocationImageRGB(int Nblig, int Nbcol)
{
	IMAGERGB mat = { 0,0,NULL,NULL };
	int i = 0;

	mat.Nblig = Nblig;
	mat.Nbcol = Nbcol;
	mat.data = (RGB*)malloc(Nblig * Nbcol * sizeof(RGB));
	if (mat.data == NULL)
		return(mat);
	mat.pixel = (RGB**)malloc(Nblig * sizeof(RGB*));
	if (mat.pixel == NULL) {
		free(mat.data);
		mat.data = NULL;
		return(mat);
	}
	for (i = 0; i < Nblig; i++)
		mat.pixel[i] = &(mat.data[i * Nbcol]);

	return(mat);
}

void liberationImageRGB(IMAGERGB* m)
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

IMAGERGB lectureImageRGB(const char* in)
{
	FILE* F = NULL;
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
			scanf(F, "%c", &type[0]);
			scanf(F, "%c", &type[1]);
			scanf(F, "%c", &buf); /* caractère espacement */

			scanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					scanf(F, "%c", &buf);
				scanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) { /* possibilité d'utiliser également isdigit de la librairie standard <ctype.h> */
				img.Nbcol = img.Nbcol * 10 + (buf - '0');
				scanf(F, "%c", &buf);
			}

			scanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					scanf(F, "%c", &buf);
				scanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				img.Nblig = img.Nblig * 10 + (buf - '0');
				scanf(F, "%c", &buf);
			}

			scanf(F, "%c", &buf);
			if (buf == '#') {
				/* on ignore tout jusqu'à trouver '\n' */
				while (buf != '\n')
					scanf(F, "%c", &buf);
				scanf(F, "%c", &buf);
			}
			while (((buf - '0') >= 0) && ((buf - '0') <= 9)) {
				dynamique = dynamique * 10 + (buf - '0');
				scanf(F, "%c", &buf);
			}

			/* début des data */

			printf("Lecture image RGB type %s avec %d lignes et %d colonnes...\n", type, img.Nblig, img.Nbcol);

			/* taille connue, allocation dynamique possible */
			img = allocationImageRGB(img.Nblig, img.Nbcol);

			/* lecture pixel par pixel */
			{
				int i, j;
				int tmp;

				for (i = 0; i < img.Nblig; i++)
					for (j = 0; j < img.Nbcol; j++)
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
				fread(img.data, sizeof(RGB), img.Nbcol * img.Nblig, F);
			}
			else
				printf("Format non supporte pour l'instant...\n");
		fclose(F);
	}
	return img;
}

void sauvegardeImageRGB(IMAGERGB img, const char* type, const char* out)
{
	FILE* F = NULL;

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

				for (i = 0; i < img.Nblig; i++)
					for (j = 0; j < img.Nbcol; j++) {
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
				fwrite(img.data, sizeof(RGB), img.Nbcol * img.Nblig, F);
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
	for (i = 0; i < 256; i++)
		lut[i] = 255 - i;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i < img.Nbcol * img.Nblig; i++)
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
		lut[i] = 255;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i < img.Nbcol * img.Nblig; i++)
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
		for (i = 0; i < out.Nbcol * out.Nblig; i++)
			out.data[i] = img.data[i].R;
		break;
	case 1: /* plan vert */
		for (i = 0; i < out.Nbcol * out.Nblig; i++)
			out.data[i] = img.data[i].G;
		break;
	case 2: /* plan bleu */
		for (i = 0; i < out.Nbcol * out.Nblig; i++)
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

	for (i = 0; i < out.Nbcol * out.Nblig; i++)
		out.data[i] = (unsigned char)((float)img.data[i].R * r + (float)img.data[i].G * g + (float)img.data[i].B * b);
	return(out);
}

/*int* histogrammeImage(IMAGE img, int choix)
{
	int* h = NULL;

	if (img.data)
	{
		int i;

		h = (int*)calloc(256, sizeof(int));

		for (i = 0; i<img.Nbcol*img.Nblig; i++)
			h[(int)img.data[i]] += 1;

		if (choix)
		{
			/* sauvegarde dans histo.csv
			FILE *F;

			F = fopen("res/histo.csv", "w");
			int i;
			for (i = 0; i < 256; i++)
				fprintf(F, "%d ; %d\n", i, h[i]);
			fclose(F);
		}
	}

	return h;
}
						Calcul histo fait que sur 256 couleurs alors qu'avec celle en dessous on peux choisir

*/
int* histogrammeImage(IMAGE img, int choix, int n)
{
	int* h = NULL;

	if (img.data)
	{
		int i;
		int bins = 256 >> (8 - n);

		h = (int*)calloc(bins, sizeof(int));

		for (i = 0; i < img.Nbcol * img.Nblig; i++)
			h[((int)img.data[i]) >> (8 - n)] += 1;

		if (choix)
		{
			/* sauvegarde dans histo.csv */
			FILE* F;

			F = fopen("res/histo.csv", "w");
			int i;
			for (i = 0; i < bins; i++)
				fprintf(F, "%d ; %d\n", i, h[i]);
			fclose(F);
		}
	}

	return h;
}

int* histogrammeImageRGB(IMAGERGB img, int choix, int n)
{
	int* h = NULL; /* RGB pas possible car UC, donc 3xint avec R G B successivement pour chaque niveau */

	if (img.data)
	{
		int i;
		int bins = 256 >> (8 - n);

		h = (int*)calloc(3 * bins, sizeof(int));

		for (i = 0; i < img.Nbcol * img.Nblig; i++)
		{
			h[3 * (((int)img.data[i].R) >> (8 - n))] += 1; /* R */
			h[3 * (((int)img.data[i].G) >> (8 - n)) + 1] += 1; /* G */
			h[3 * (((int)img.data[i].B) >> (8 - n)) + 2] += 1; /* B */
		}

		if (choix)
		{
			/* sauvegarde dans histo.csv */
			FILE* F;

			F = fopen("res/histo_rgb.csv", "w");
			int i;
			for (i = 0; i < bins; i++)
				fprintf(F, "%d ; %d; %d; %d\n", i, h[3 * i], h[3 * i + 1], h[3 * i + 2]);
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

	for (i = 0; i < img.Nblig * img.Nbcol; i++)
	{
		mini = (img.data[i] < mini) ? img.data[i] : mini;
		maxi = (img.data[i] > maxi) ? img.data[i] : maxi;
	}

	a = (float)(outMax - outMin) / ((float)(maxi - mini));
	b = (float)outMin - a * mini;

	lut = (int*)calloc(256, sizeof(int));
	for (i = mini; i <= maxi; i++)
		lut[i] = (int)(a * i + b);

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i < img.Nbcol * img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);

	return out;
}

IMAGE egalisationImage(IMAGE img)
{
	IMAGE out = { 0,0,NULL,NULL };
	int* h = histogrammeImage(img, 0, 8);/* A modifier suivant nb de couleurs voulu */
	int* hC = NULL;
	int* lut = NULL;
	int i;

	hC = (int*)calloc(256, sizeof(int));
	hC[0] = h[0];
	for (i = 1; i < 256; i++)
		hC[i] = hC[i - 1] + h[i];
	free(h);
	h = NULL;

	lut = (int*)calloc(256, sizeof(int));
	for (i = 0; i < 256; i++)
		lut[i] = (int)((255 / (float)(img.Nbcol * img.Nblig)) * hC[i]);
	free(hC);
	hC = NULL;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i < img.Nbcol * img.Nblig; i++)
		out.data[i] = lut[img.data[i]];
	free(lut);

	return(out);
}

SIGNATURES statistiquesImage(IMAGE img)
{
	SIGNATURES sig;
	int i;
	double som = 0, som2 = 0;
	int somme, moitPop = (img.Nbcol * img.Nblig) / 2;
	int* h = histogrammeImage(img, 0, 8); /* A changer pour nb de couleur voulu*/

	for (i = 0; i < img.Nbcol * img.Nblig; i++) {
		som += (double)img.data[i];
		som2 += (double)img.data[i] * (double)img.data[i];
	}

	som /= (double)(img.Nblig * img.Nbcol);
	som2 = sqrt(som2 / (double)(img.Nblig * img.Nbcol) - som * som);

	sig.moyenne = som;
	sig.ecartType = som2;

	i = 0;
	while (h[i] == 0)
		i += 1;
	sig.min = i;

	i = 255;
	while (h[i] == 0)
		i -= 1;
	sig.max = i;

	i = 0;
	somme = h[i];
	while (somme < moitPop) {
		i += 1;
		if (i < 256)
			somme += h[i];
	}
	sig.mediane = i;

	free(h);
	h = NULL;

	return sig;
}

IMAGE seuillageOtsu(IMAGE img)
{
	IMAGE out = { 0, 0, NULL, NULL };
	int* h = histogrammeImage(img, 0, 8); /* A changer pour nb de couleur voulu */
	int* hC = NULL;

	double* tab = NULL;
	int i, seuil;
	double M1, M2;
	double w1;
	int* lut = NULL;
	int min, max;

	/* recherche min et max */
	i = 0;
	while (h[i] == 0)
		i++;
	min = i;
	i = 255;
	while (h[i] == 0)
		i--;
	max = i;

	hC = (int*)calloc(256, sizeof(int));
	hC[0] = h[0];
	for (i = 1; i < 256; i++)
		hC[i] = hC[i - 1] + h[i];

	tab = (double*)calloc(256, sizeof(double));

	/* parcours entre min et max pour éviter divisions par 0 */
	/* initialisation */
	M1 = min;
	seuil = min;
	w1 = (double)hC[min] / (double)(img.Nbcol * img.Nblig);
	M2 = 0;
	for (int i = min + 1; i <= max; i++)
		M2 += (double)h[i] * i;
	M2 /= (double)(hC[max] - h[min]);
	tab[min] = w1 * (1 - w1) * (M1 - M2) * (M1 - M2);

	// parcours
	for (int i = min + 1; i < max; i++) {
		M1 = ((double)hC[i - 1] * M1 + (double)h[i] * i) / hC[i];
		M2 = ((double)(hC[255] - hC[i - 1]) * M2 - (double)(h[i] * i)) / (double)(hC[255] - hC[i]);
		w1 = (double)hC[i] / (double)(img.Nbcol * img.Nblig);
		tab[i] = w1 * (1 - w1) * (M1 - M2) * (M1 - M2);
		if (tab[i] > tab[seuil])
			seuil = i;
	}

	free(h); h = NULL;
	free(hC); hC = NULL;

	free(tab);

	lut = (int*)calloc(256, sizeof(int));
	for (i = seuil; i <= max; i++)
		lut[i] = 255;

	out = allocationImage(img.Nblig, img.Nbcol);
	for (i = 0; i < img.Nbcol * img.Nblig; i++)
		out.data[i] = lut[img.data[i]];

	free(lut);
	return out;
}

/* étiquetage V8 */
IMAGE labelImage(IMAGE img, int* nbComp)
{
	IMAGE out = { 0,0,NULL,NULL };

	int i, ii, j, jj;

	int k = 1, kk;
	int eps;
	int predNonNul[5];
	int nbPredNonNuls;

	int nbMaxEtiquettes = img.Nblig * img.Nbcol;
	intIMAGE withBords = { 0,0,NULL,NULL };

	withBords = allocationIntImage(img.Nblig + 2, img.Nbcol + 2);

	for (i = 0, ii = 1; i < img.Nblig; i++, ii++) // pure copie 
		for (j = 0, jj = 1; j < img.Nbcol; j++, jj++)
			withBords.pixel[ii][jj] = (int)img.pixel[i][j];

	int* tableEtiquette = NULL;
	tableEtiquette = (int*)calloc(nbMaxEtiquettes, sizeof(int));

	for (kk = 0; kk < nbMaxEtiquettes; kk++) {
		tableEtiquette[kk] = kk;
	}

	for (i = 1; i < withBords.Nblig - 1; i++)
		for (j = 1; j < withBords.Nbcol - 1; j++) {
			if (withBords.pixel[i][j] != 0) {
				// voisinage V8 
				int pred1 = withBords.pixel[i][j - 1];
				int pred2 = withBords.pixel[i - 1][j - 1];
				int pred3 = withBords.pixel[i - 1][j];
				int pred4 = withBords.pixel[i - 1][j + 1];

				nbPredNonNuls = 0;
				if (pred1) predNonNul[nbPredNonNuls++] = pred1;
				if (pred2) predNonNul[nbPredNonNuls++] = pred2;
				if (pred3) predNonNul[nbPredNonNuls++] = pred3;
				if (pred4) predNonNul[nbPredNonNuls++] = pred4;

				if (nbPredNonNuls == 0) {
					withBords.pixel[i][j] = k;
					k++;
				}
				else {
					eps = 0;
					int minPred = tableEtiquette[predNonNul[0]];
					int maxPred = tableEtiquette[predNonNul[0]];
					while (eps < nbPredNonNuls) {
						if (tableEtiquette[predNonNul[eps]] < minPred) minPred = tableEtiquette[predNonNul[eps]];
						if (tableEtiquette[predNonNul[eps]] > maxPred) maxPred = tableEtiquette[predNonNul[eps]];
						eps++;
					}
					if (minPred == maxPred)
						withBords.pixel[i][j] = minPred;
					else {
						withBords.pixel[i][j] = minPred;
						for (eps = 0; eps < nbPredNonNuls; eps++) {
							int beta = predNonNul[eps];
							while (tableEtiquette[beta] != minPred) {
								int m = tableEtiquette[beta];
								tableEtiquette[beta] = minPred;
								beta = m;
							}
						}
					}
				}
			}
		}


	// actualisation de la table d'équivalence
	for (int kk = 1; kk < k; kk++) {
		int m = kk;
		while (tableEtiquette[m] != m)
			m = tableEtiquette[m];
		tableEtiquette[kk] = m;
	}

	int* etiquettes = (int*)calloc(k + 1, sizeof(int));

	// histo pour repérer les trous
	for (int kk = 1; kk < k; kk++)
		etiquettes[tableEtiquette[kk]]++;

	// on remet à jour les index (etiquetage définitif hors trou)
	etiquettes[0] = 0;
	int compt = 1;
	for (int kk = 1; kk < k; kk++) {
		if (etiquettes[kk])
			etiquettes[kk] = compt++;
	}

	for (i = 0; i < withBords.Nblig; i++)
		for (j = 0; j < withBords.Nbcol; j++) {
			withBords.pixel[i][j] = etiquettes[tableEtiquette[withBords.pixel[i][j]]];
		}

	free(etiquettes);
	free(tableEtiquette);

	out = allocationImage(img.Nblig, img.Nbcol);

	*nbComp = compt - 1;
	for (i = 0, ii = 1; i < img.Nblig; i++, ii++)
		for (j = 0, jj = 1; j < img.Nbcol; j++, jj++)
			out.pixel[i][j] = (unsigned char)withBords.pixel[ii][jj];

	liberationIntImage(&withBords);

	return out;
}

