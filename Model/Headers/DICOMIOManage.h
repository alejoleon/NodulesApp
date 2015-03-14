#ifndef DICOMIOMANAGE_H
#define DICOMIOMANAGE_H

//Estandar de C++
#include <string>

// Lectura y escritura de imagenes en ITK (imagenes DICOM)
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkNumericSeriesFileNames.h"

//Lectura de imágenes DICOM en VTK
#include "vtkDICOMImageReader.h"
#include "vtkSmartPointer.h"

//Conversion itk-vtk
#include <itkImageToVTKImageFilter.h>
#include <vtkImageData.h>



typedef itk::NumericSeriesFileNames OutputNamesGeneratorType;


//Definicion de imagen de entrada
typedef signed short PixelType;
const unsigned int DimensionDM= 3;
typedef itk::Image<PixelType, DimensionDM> ImageType;
typedef itk::ImageSeriesReader<ImageType> ReaderType;

//Definicion de Imagenes DICOM
typedef itk::GDCMImageIO ImageIOType;
typedef itk::GDCMSeriesFileNames NamesGeneratorType;

//Definiciion de imagen Binaria
typedef unsigned char   PixelBinaryType;
typedef itk::Image<PixelBinaryType, DimensionDM> ImageBinaryType;
typedef itk::ImageSeriesReader< ImageBinaryType > ReaderBinaryType;


using namespace std;

class DICOMIOManage
{
private:
    string nameOutputFiles;
    int inputSize;

    //Para guardar los metadatos de la imagen de entrada y ponerlos en la imagen de salida
	ReaderType::Pointer reader;
    ReaderBinaryType::Pointer readerBinary;

public:
    DICOMIOManage();
    ~DICOMIOManage();

    ReaderType::Pointer readInputImage(string inputDirectory);
    ReaderBinaryType::Pointer readInputImageBin(string inputDirectory);

    ReaderType::Pointer getItkImage(string path);
    vtkSmartPointer< vtkDICOMImageReader > getVtkImageReader(string path);


    void castImageItkToVtk (ImageType::Pointer imageIn, vtkImageData* &imageOut);

    void writeDicomFile(ImageType::Pointer image, string outputDirectory);
	void writeDicomFile (ImageBinaryType::Pointer image,string outputDirectory);

    void SetNameOutputFiles(const string& nameOutputFiles);
    const string& GetNameOutputFiles() const;

    void SetInputSize(int inputSize);
    int GetInputSize() const;
};

#endif // DICOMIOMANAGE_H
