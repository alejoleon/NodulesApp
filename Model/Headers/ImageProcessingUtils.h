#ifndef IMAGEPROCESSINGUTILS_H
#define IMAGEPROCESSINGUTILS_H
#include "ImageProcessingUtils.h"


//Standar de C++
#include <vector>


//Calculo de maximos y minimos
#include "itkMinimumMaximumImageCalculator.h"
//Calculo de histograma
#include "itkImageToHistogramFilter.h"
#include "itkMaskedImageToHistogramFilter.h"

using namespace std;

//Definicion del tipo de imagen.
typedef signed short    PixelType;
const unsigned int      DimensionIP = 3;
typedef itk::Image< PixelType, DimensionIP > ImageType;

//Definicion de imagen binaria
typedef unsigned char   PixelBinaryType;
typedef itk::Image<PixelBinaryType, DimensionIP> ImageBinaryType;

//Definicion de variables de Histograma
typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;
typedef itk::Statistics::MaskedImageToHistogramFilter< ImageType, ImageBinaryType > MaskedImageToHistogramFilterType;

class ImageProcessingUtils
{
public:
	ImageProcessingUtils();
	~ImageProcessingUtils();
	ImageType::PixelType getMaximumValue (ImageType::Pointer image);
	ImageType::PixelType getMinimumValue (ImageType::Pointer image);
	ImageType::PixelType getMaximumValue (ImageType::Pointer image,ImageBinaryType::Pointer mask);
	ImageType::PixelType getMinimumValue (ImageType::Pointer image,ImageBinaryType::Pointer mask);
	
	ImageToHistogramFilterType::Pointer histogram (ImageType::Pointer image ,int lower, int upper);
	ImageToHistogramFilterType::Pointer histogram (ImageType::Pointer image);
	ImageToHistogramFilterType::Pointer histogramFilter (ImageType::Pointer image , int bins ,int lower, int upper);
	ImageToHistogramFilterType::Pointer histogramFilter (ImageType::Pointer image);

	MaskedImageToHistogramFilterType::Pointer histogram (ImageType::Pointer image,ImageBinaryType::Pointer mask ,int lower, int upper);
	MaskedImageToHistogramFilterType::Pointer histogram (ImageType::Pointer image,ImageBinaryType::Pointer mask);
	MaskedImageToHistogramFilterType::Pointer histogramFilter (ImageType::Pointer image ,ImageBinaryType::Pointer mask, int bins ,int lower, int upper);
	MaskedImageToHistogramFilterType::Pointer histogramFilter (ImageType::Pointer image ,ImageBinaryType::Pointer mask);

    vector<int> histogramFilterToVector(ImageToHistogramFilterType::Pointer);
    vector<int> histogramFilterToVector(MaskedImageToHistogramFilterType::Pointer);

};
	
#endif // IMAGEPROCESSINGUTILS_H
