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

//Conversion vtk-itk
#include <itkVTKImageToImageFilter.h>





typedef itk::NumericSeriesFileNames OutputNamesGeneratorType;


//Definición de imagen de entrada.
typedef signed short PixelType;
const unsigned int DimensionDM= 3;
typedef itk::Image<PixelType, DimensionDM> ImageType;
typedef itk::ImageSeriesReader<ImageType> ReaderType;

//Definición de Imagenes DICOM.
typedef itk::GDCMImageIO ImageIOType;
typedef itk::GDCMSeriesFileNames NamesGeneratorType;

//Definición de imagen Binaria.
typedef unsigned char   PixelBinaryType;
typedef itk::Image<PixelBinaryType, DimensionDM> ImageBinaryType;
typedef itk::ImageSeriesReader< ImageBinaryType > ReaderBinaryType;

//Definición del diccionario de metadatos.
typedef itk::MetaDataDictionary *           DictionaryRawPointer;
typedef std::vector< DictionaryRawPointer > DictionaryArrayType;
typedef const DictionaryArrayType *         DictionaryArrayRawPointer;

//Definición de mapas de distancias
typedef itk::Image<float, 3>          ImageFloatType;



using namespace std;

class DICOMIOManage
{
private:
    string nameOutputFiles;
    int inputSize;

    /**
     * @brief dictionary Diccionario de metadatos que se obtienen a partir de la imagen que se abre y que se utiliza al momento de guardar o escribir una imagen DICOM.
     */
     DictionaryArrayRawPointer dictionary;

public:

    DICOMIOManage();
    ~DICOMIOManage();

    ReaderType::Pointer readInputImage(string inputDirectory);
    ReaderBinaryType::Pointer readInputImageBin(string inputDirectory);

    ReaderType::Pointer getItkImage(string path);
    vtkSmartPointer< vtkDICOMImageReader > getVtkImageReader(string path);

    void writeDicomFile(ImageType::Pointer image, string outputDirectory);
	void writeDicomFile (ImageBinaryType::Pointer image,string outputDirectory);
    void writeDicomFile (ImageFloatType::Pointer image,string outputDirectory);

    void SetNameOutputFiles(const string& nameOutputFiles);
    const string& GetNameOutputFiles() const;

    void SetInputSize(int inputSize);
    int GetInputSize() const;
};

#endif // DICOMIOMANAGE_H
