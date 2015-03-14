#ifndef COORDINATOR_H
#define COORDINATOR_H

//Estandar de C++
#include <iostream>
#include <vector>

//Lectura de imágenes DICOM en VTK
#include "vtkDICOMImageReader.h"
#include "vtkSmartPointer.h"

//Conversion itk-vtk
#include <itkImageToVTKImageFilter.h>
#include <vtkImageData.h>

//Lectura imagenes ITK
#include "itkImageSeriesReader.h"
#include "itkImage.h"

//Histograma ITK
#include "itkImageToHistogramFilter.h"

//Clases del modelo
#include "DICOMIOManage.h"
#include "ImageProcessingUtils.h"

/*
//Definicion de imagen ITK
typedef signed short PixelType;
const unsigned int DimensionC= 3;
typedef itk::Image<PixelType, DimensionC> ImageType;
typedef itk::ImageSeriesReader<ImageType> ReaderType;
*/

using namespace std;

class Coordinator
{
public:
	Coordinator();
	~Coordinator();

    vtkSmartPointer< vtkDICOMImageReader > getVtkImageReader(string path);
    ReaderType::Pointer getItkImage(string path);
    void castImageItkToVtk (ImageType::Pointer actualImage, vtkImageData* &imageOut);
    vector<int> getHistogramData (int & lower , int & upper);

protected:

    /**
     * @brief imageIn ReaderType::Pointer, imagen ITK que se esta usando actualmente.
     */
    ReaderType::Pointer actualImage;
    /**
     * @brief dicomIOmanage Apuntador a objeto de la clase DICOMIOManage.
     */
    DICOMIOManage * dicomIOmanage;
    /**
     * @brief utils Apuntador a objeto de la clase ImageProcessingUtils.
     */
    ImageProcessingUtils * utils;



};

#endif // COORDINATOR_H
