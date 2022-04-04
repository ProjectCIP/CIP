#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include<time.h>

#include "LibOutils.h"
#include "libCIPSI1.h"
#include "Librairie.h"

BNDBOX* ReadBndBox(char* Img, int* NbBndBox)
{
	char ligne[255];
	FILE* F;
	int cpt = 0;
	int m[4] = { 0 };
	int j = 4;
	char* Border = "    <bndbox>\n";
	char ligneCopie[100];
	char* token = ' ';
	const char s[6] = "<xmin>";
	F = fopen(Img, "r");
	BNDBOX* res = NULL;
	res = (BNDBOX*)malloc(sizeof(BNDBOX));
	int comp;
	while (fgets(ligne, 255, F) != NULL)
	{


		/*   token = strtok(ligne, s);
			if (token != ' ')
			{
				res = (BNDBOX*)realloc(res, (cpt + 1) * sizeof(BNDBOX));
				cpt += 1;

			}*/

		comp = strcmp(Border, ligne);
		if (comp < 0)
		{
			res = (BNDBOX*)realloc(res, (cpt + 1) * sizeof(BNDBOX));
			cpt += 1;
			j = 0;
			while (fgets(ligne, 255, F) != "    </bndbox>\n")
			{
				/*for (int i = 0; i < 2; i++)
				{
					copieLigne = strtok(ligne, "<");
					copieLigne = strtok(NULL, "<");
				}

			if (copieLigne[0] == "x" && copieLigne[1] == "m" && copieLigne[2] == "i")
			{
				l += 1;
				liste = (REC*)realloc(l, sizeof(REC));
				copieLigne = strtok(copieLigne, ">");
				copieLigne = strtok(NULL, "<");
			}
			}*/
			}

			token = ' ';
		}
		*NbBndBox = cpt;
		return res;
	}



/*
TraceBnbBox(BNDBOX* res, IMAGERGB Img)
{
    BNDBOX* res = NULL;

    res->xmax = 1000;
    res->xmin = 1800;
    res->ymax = 300;
    res->ymin = 500;

    IMAGERGB Img = { NULL, NULL,NULL };

    Img = lectureImageRGB("BloodImage_00002.ppm", "P6");


}
*/
