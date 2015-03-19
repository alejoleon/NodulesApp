#ifndef IMAGEFILTERS_H
#define IMAGEFILTERS_H

//Para manejo de imagenes.
#include "itkImage.h"
#include "itkImageSeriesReader.h"

//Para filtrado de imagenes con filtro mediana
#include "itkMedianImageFilter.h"

//Para binarizacion con crecimiento de regiones.
#include "itkConnectedThresholdImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"

//Para hacer umbralización
#include "itkBinaryThresholdImageFilter.h"

//Duplicar imagen
#include "itkImageDuplicator.h"

//Erosion dilatacion
#include <itkGrayscaleErodeImageFilter.h>
#include <itkGrayscaleDilateImageFilter.h>


//Clases del proyecto
#include "BinaryFilters.h"
#include "VTKVisualization.h"


//Definicion del tipo de imagen.
typedef signed short    PixelType;
const unsigned int      DimensionIF = 3;
typedef itk::Image< PixelType, DimensionIF > ImageType;
typedef itk::ImageSeriesReader< ImageType > ReaderType;

//Definiciion de imagen Binaria
typedef unsigned char   PixelBinaryType;
typedef itk::Image<PixelBinaryType, DimensionIF> ImageBinaryType;

//Definicion de imagen para Crecimiento de regiones (WARNING del compilador)
typedef float PixelGrowingType;
typedef itk::Image<PixelGrowingType, DimensionIF> ImageGrowingType;

using namespace std;

class ImageFilters
{
private:
	
public:
	ImageFilters();
	~ImageFilters();

	void filtroMediana(ReaderType::Pointer reader, int radius,ImageType::Pointer &filterOutput);
	void filtroMediana(ImageType::Pointer image, int radius,ImageType::Pointer &filterOutput);
	void regionGrowing(ImageType::Pointer image,ImageBinaryType::Pointer &binLungs,float seeds[], int min, int max) ;
	void binaryThreshold(ImageType::Pointer imageIn,ImageBinaryType::Pointer &imageOut,short lowerThreshold, short upperThreshold);
	void binaryThreshold(ImageType::Pointer imageIn,ImageBinaryType::Pointer mask,ImageBinaryType::Pointer &imageOut,short lowerThreshold, short upperThreshold);
	void maskImage(ImageType::Pointer input,ImageBinaryType::Pointer mask,ImageType::Pointer &output);
	void erodeFilter(ImageType::Pointer input,ImageType::Pointer &output, int radius);
	void dilateFilter(ImageType::Pointer input,ImageType::Pointer &output, int radius);
    void clipBinaryVolume(ImageBinaryType::Pointer input , ImageBinaryType::Pointer &output, int initialCoord, int endCoord, int plane);
  
};

#endif // IMAGEFILTERS_H
