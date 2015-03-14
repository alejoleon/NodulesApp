#ifndef BINARYFILTERS_H
#define BINARYFILTERS_H

//CPP standar
#include <iostream>
#include <stdlib.h>


//Filtro OR
#include "itkOrImageFilter.h"
//Filtro Closing y opening
#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
//Filtro Mascara
#include "itkMaskImageFilter.h"
//Filtro NOT
#include "itkBinaryNotImageFilter.h"


//Definicion de imagen DICOM.
typedef signed short    PixelType;
const unsigned int      DimensionBF = 3;
typedef itk::Image< PixelType, DimensionBF > ImageType;

//Definiciion de imagen Binaria
typedef unsigned char   PixelBinaryType;
typedef itk::Image<PixelBinaryType, DimensionBF> ImageBinaryType;



//Clases del proyecto
#include "ImageProcessingUtils.h"


using namespace std;

class BinaryFilters
{
public:
	BinaryFilters();
	~BinaryFilters();

	void orFunction(ImageBinaryType::Pointer input1,ImageBinaryType::Pointer input2,ImageBinaryType::Pointer &output);
	void closingFilter(ImageBinaryType::Pointer imageIn,ImageBinaryType::Pointer &imageOut,double radius);
    void openingFilter(ImageBinaryType::Pointer imageIn,ImageBinaryType::Pointer &imageOut,double radius);
    void notImage(ImageBinaryType::Pointer imageIn,ImageBinaryType::Pointer &imageOut);
};

#endif // BINARYFILTERS_H
