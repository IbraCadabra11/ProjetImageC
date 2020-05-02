// HMR_Assamese.cpp : 

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "libImages.h"
#include "libOutils.h"
#include <stdlib.h>
#include <string.h>

void main(void)
{
	int Nord = 0;
	IMAGE ImgInput = { 0,0,NULL,NULL };
	ImgInput = lectureImage("..\\Database\\train\\001.pgm");
	IMAGE ImgOut = { 0,0,NULL,NULL };
	
	ImgOut = Division_Image(ImgInput,Nord);

	sauvegardeImage(ImgOut, "P5", "..\\Res\\Nord.pgm");


	liberationImage(&ImgInput);
	liberationImage(&ImgOut);
	system("pause");

}