#ifndef BOUMAMETHODS_H
#define BOUMAMETHODS_H

//CPP standar
#include <iostream>
#include <stdlib.h>


using namespace std;

//Para duplicar una imagen.
#include "itkImageDuplicator.h"

//Para hacer las funciones de etiquetado de regiones.
#include "itkImageRegionIterator.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"


//Definicion del tipo de imagen.
typedef signed short    PixelType;
const unsigned int      DimensionBM = 3;
typedef itk::Image< PixelType, DimensionBM > ImageType;

//Definicion de imagen binaria
typedef unsigned char   PixelBinaryType;
typedef itk::Image<PixelBinaryType, DimensionBM> ImageBinaryType;

//Definicion del mapa de distancias.
typedef itk::Image<float, DimensionBM> ImageFloatType;

//Clases del proyecto:
#include "ImageProcessingUtils.h"
#include "ImageFilters.h"
#include "BinaryFilters.h"
#include "VTKVisualization.h"



class BoumaMethods
{
public:
	BoumaMethods();
	~BoumaMethods();
    void rowWiseMethod (ImageType::Pointer imageIn, ImageBinaryType::Pointer lungs, ImageBinaryType::Pointer &interestRegion, ImageBinaryType::Pointer &mediastinum, int upperThreshold);
	void clearSmallRegions (ImageBinaryType::Pointer &mediastinum);
	void funcionVasos (ImageType::Pointer image ,ImageBinaryType::Pointer mask,ImageBinaryType::Pointer &imageOut);
	void peripheralVessel (ImageType::Pointer image ,ImageBinaryType::Pointer mask,ImageBinaryType::Pointer &imageOut);
	void createLungsRegion (ImageType::Pointer imageIn, ImageBinaryType::Pointer &lungsRegion, float seeds[], int min, int max);
    void createMediastinumRegion(ImageType::Pointer image, ImageBinaryType::Pointer regionMask , ImageBinaryType::Pointer &mediastinumRegion, int upperThreshold);
    void createExclusionRegionLungs (ImageType::Pointer image, ImageFloatType::Pointer distanceMap , ImageBinaryType::Pointer &output, signed short threshold, double pixelLimit);

};

#endif // BOUMAMETHODS_H
