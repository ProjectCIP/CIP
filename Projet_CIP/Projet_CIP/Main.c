#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include<time.h>

#include "LibOutils.h"
#include "libCIPSI1.h"
#include "Librairie.h"

void main(void) {
	int Nb = 0;
	BNDBOX* res = NULL;
	char* Img = "Annotations\\BloodImage_00003.xml";
	res = ReadBndBox(Img, &Nb);
	printf("%d", Nb);
};