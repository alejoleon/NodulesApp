#ifndef COORDINATOR_H
#define COORDINATOR_H

//Estandar de C++
#include <iostream>
#include <vector>

//Lectura de imágenes DICOM en VTK
#include "vtkDICOMImageReader.h"
#include "vtkSmartPointer.h"

//Lectura imagenes ITK
#include "itkImageSeriesReader.h"
#include "itkImage.h"

//Histograma ITK
#include "itkImageToHistogramFilter.h"

//Casteo de imagenes ITK
#include "itkCastImageFilter.h"

//Clases del modelo
#include "DICOMIOManage.h"
#include "ImageProcessingUtils.h"
#include "ImageFilters.h"
#include "Configuracion.h"
#include "BoumaMethods.h"

using namespace std;

class Coordinator
{
public:
    /**
     * @brief Coordinator Constructor por defecto.
     */
    Coordinator();
    /**
     * @brief Destructor por defecto.
     */
    ~Coordinator();

    /**
     * @brief getVtkImageReader Se obtiene un apuntador a una imagen de tipo VTK reader, a partir de la ruta del directorio donde se encuentra la imagen.
     * @param path Ruta donde está el directorio que contiene las imágenes DICOM.
     * @return Apuntador a imágen de tipo VTK reader, correspondiente a la imágen que se va a mostrar.
     */
    vtkSmartPointer< vtkDICOMImageReader > getVtkImageReader(string path);

    /**
     * @brief getHistogramData Retorna el histograma de la imagen actual en un vector, ademas retorna el valor correspondientes al mínimo y máximo valor de gris del histograma en los parámetros pasados por referencia.
     * @param lower Correspondiente al mínimo valor de gris en el histograma. Variable pasada por referencia.
     * @param upper Correspondiente al máximo valor de gris en el histograma. Variable pasada por referencia.
     * @return vector del histograma de la imagen.
     */
    vector<int> getHistogramData (int & lower , int & upper);

    /**
     * @brief setCurrentImage Establece la imagen actual con la que se está trabajando de acuerdo al ID que le llega por parámetro.
     * @param image ID que indica cual imagen es la actual.
     * 1.Para la imagen de entrada.
     * 2.Para la imagen de mediana.
     * 3.Para la imagen de mascara de pulmones.
     */
    void setCurrentImage (int image);

    //Algoritmo
    /**
     * @brief doMedian Realiza la mediana de la imagen de entrada (imageIn), usando el radio que ingresa por parámetro.
     * @param radius Radio con el que se va a hacer la mediana.
     * @return Ruta o directorio donde se encuentra la imagen resultante.
     */
    const string doMedian(int radius);

    /**
     * @brief doLungsMask Hace una máscara de los pulmones, usando las dos semillas pasadas por parámetro.
     * @param seeds Arreglo que contiene las coordenadas espaciales de las semillas de cada pulmón.
     * @return Ruta o directorio donde se encuentra la imagen resultante.
     */
    const string doLungsMask(float seeds[]);

    /**
     * @brief getImageIn Retorna la ruta donde se encuentra la imagen de entrada.
     * @return Ruta o directorio donde se encuentra la imagen de entrada.
     */
    const string getImageIn();
    /**
     * @brief setImageIn Establece la ruta donde se encuentra la imagen de entrada.
     * @param dirImgIn Ruta o directorio donde se encuentra la imagen de entrada.
     */
    void setImageIn(string dirImgIn);

    /**
     * @brief getImageMedian Retorna la ruta donde se encuentra la imagen despues de pasada por el filtro mediana.
     * @return Ruta o directorio donde se encuentra la imagen luego de haber pasado por el filtro mediana.
     */
    const string getImageMedian();

    /**
     * @brief getImageLungsMask Retorna la ruta donde se encuentra la imagen de máscara de pulmones.
     * @return Ruta o directorio donde se encuentra la segmentación de los pulmones o máscara.
     */
    const string getImageLungsMask();


protected:

    /**
     * @brief currentImage Imagen ITK que se esta usando actualmente y con la cual se van a hacer los calculos o metodos necesarios.
     */
    ImageType::Pointer currentImage;
    /**
     * @brief dicomIOmanage Apuntador a objeto de la clase DICOMIOManage.
     */
    DICOMIOManage* dicomIOmanage;
    /**
     * @brief utils Apuntador a objeto de la clase ImageProcessingUtils.
     */
    ImageProcessingUtils* utils;
    /**
     * @brief imageFilters Apuntador a objeto de la clase ImageFilters.
     */
    ImageFilters* imageFilters;
    /**
     * @brief config Apuntador a objeto de la clase Configuration.
     */
    Configuracion* config;
    /**
     * @brief boumaMethods Apuntador a objeto de la clase BoumaMethods.
     */
    BoumaMethods* boumaMethods;


    //Algoritmo.
    /**
     * @brief readerImageIn Lector de la imagen de entrada.
     */
    ReaderType::Pointer readerImageIn;

    /**
     * @brief dirImgIn Ruta o directorio donde se encuentra la imagen de entrada.
     */
    string dirImgIn;
    /**
     * @brief dirImgMedian Ruta o directorio donde se encuentra la imagen luego de haber pasado por el filtro mediana.
     */
    string dirImgMedian;
    /**
     * @brief dirImgLungsMask Ruta o directorio donde se encuentra la imagen de máscara de los pulmones.
     */
    string dirImgLungsMask;


    /**
     * @brief imageIn Apuntador a la imagen de entrada.
     */
    ImageType::Pointer imageIn;
    /**
     * @brief imageMedian Apuntador a la imagen despues de pasar por el filtro mediana.
     */
    ImageType::Pointer imageMedian;
    /**
     * @brief imageLungsMask Apuntador a la imagen de la máscara de los pulmones.
     */
    ImageBinaryType::Pointer imageLungsMask;
};

#endif // COORDINATOR_H
