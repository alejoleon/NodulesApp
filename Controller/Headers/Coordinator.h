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
#include "BinaryFilters.h"
#include "BoumaMethods.h"
#include "Configuracion.h"
#include "DICOMIOManage.h"
#include "DICOMProperties.h"
#include "ImageFilters.h"
#include "ImageProcessingUtils.h"

using namespace std;

class Coordinator
{
public:

    void funcionPrueba();





    /**
     * @brief Coordinator Constructor por defecto.
     */
    Coordinator();
    /**
     * @brief Destructor por defecto.
     */
    ~Coordinator();

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
     * @brief setImageIn Establece la imagen de entrada a partir de la ruta o directorio donde se encuentra la imagen inicial.
     * @param dirImgIn Ruta o directorio donde se encuentra la imagen DICOM.
     */
    void setImageIn(string dirImgIn);
    /**
     * @brief doMedian Hace el filtrado de mediana mediante el coordinador.
     * @param radius Radio con el que se va a hacer la mediana.
     */
    void doMedian(int radius);
    /**
     * @brief doLungsMask Genera la máscara de los pulmones a partir de dos semillas, una para cada pulmón.
     * @param seeds Arreglo con las coordenadas de las dos semillas de cada pulmón.
     */
    void doLungsMask(float seeds[]);


    void doMediastinumMask (int first = 0, int last =0);


    /**
     * @brief getImageIn Obtiene la imagen de entrada.
     * @return Imagen de entrada en formato VTK.
     */
    vtkImageData* getImageIn();
    /**
     * @brief getImageMedian Obtiene la imagen despues de haber pasado por el filtro mediana.
     * @return Imagen despues de filtro mediana en formato VTK.
     */
    vtkImageData* getImageMedian();
    /**
     * @brief getImageLungsMask Obtiene la imagen con la máscara de los pulmones.
     * @return Imagen con máscara de los pulmones en formato VTK.
     */
    vtkImageData* getImageLungsMask();
    /**
     * @brief getImageMediastinumMask Obtiene la imagen con la máscara del mediastino.
     * @return Imagen con máscara del mediastino.
     */
    vtkImageData* getImageMediastinumMask();
    /**
     * @brief getImageInterestRegion Obtiene la imagen con la máscara del área de interés.
     * @return Imagen con mascara del área de interés.
     */
    vtkImageData* getImageInterestRegion();

protected:

    /**
     * @brief currentImage Imagen ITK que se esta usando actualmente y con la cual se van a hacer los calculos o metodos necesarios.
     */
    ImageType::Pointer currentImage;
    /**
     * @brief dicomIOmanage Apuntador a objeto de la clase DICOMIOManage, que contiene los datos necesarios para escribir y leer imágenes dicom, además tiene el diccionario de metadatos de la imagen de entrada.
     */
    DICOMIOManage* dicomIOmanage;
    /**
     * @brief dicomProperties Apuntador a objeto de la clase DICOMProperties, que contiene la información del encabezado de la imagen dicom de entrada.
     */
    DICOMProperties* dicomProperties;


    //Algoritmo.
    /**
     * @brief dirImgIn Ruta o directorio donde se encuentra la imagen de entrada.
     */
    string dirImgIn;

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
    /**
     * @brief imageMediastinumMask Apuntador a la imagen de la máscara del mediastino.
     */
    ImageBinaryType::Pointer imageMediastinumMask;
    /**
     * @brief imageInterestRegion Apuntador a la imagen de la máscara de la region de interés
     */
    ImageBinaryType::Pointer imageInterestRegion;
};

#endif // COORDINATOR_H
