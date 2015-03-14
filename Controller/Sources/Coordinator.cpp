#include "Coordinator.h"

/**
 * @brief Coordinator::Coordinator Constructor por defecto.
 */
Coordinator::Coordinator()
{
    this->dicomIOmanage = new DICOMIOManage();
    actualImage =   ReaderType::New();
}

/**
 * @brief Coordinator::~Coordinator Destructor
 */
Coordinator::~Coordinator()
{
}

/**
 * @brief Coordinator::getVtkImageReader Se obtiene un apuntador a una imagen de tipo VTK reader.
 * @param path String, ruta donde está el directorio que contiene las imágenes DICOM.
 * @return vtkSmartPointer< vtkDICOMImageReader >, apuntador a imágen de tipo VTK reader.
 */
vtkSmartPointer< vtkDICOMImageReader > Coordinator:: getVtkImageReader(string path){
    return this->dicomIOmanage->getVtkImageReader(path);
}

/**
 * @brief Coordinator::getItkImage Se obtiene un apuntador a una imagen de tipo ITK reader.
 * @param path Ruta donde está el directorio que contiene las imágenes DICOM.
 * @return Apuntador a imágen de tipo ITK reader.
 */
ReaderType::Pointer Coordinator::getItkImage(string path){

    this->actualImage = this->dicomIOmanage->getItkImage(path);
    return this->dicomIOmanage->getItkImage(path);
}

/**
 * @brief Coordinator::castImageItkToVtk Convierte una imagen de tipo ITK a una imagen de tipo VTK
 * @param imageIn ImageType::Pointer, apuntador a imagen de tipo ITK.
 * @param imageOut vtkImageData*, apuntador a imagen de tipo VTK.
 */
void  Coordinator::castImageItkToVtk (ImageType::Pointer imageIn, vtkImageData* &imageOut){
    this->dicomIOmanage->castImageItkToVtk(imageIn,imageOut);
}

/**
 * @brief Coordinator::getHistogramData Retorna el histograma de la imagen actual en un vector, ademas retorna el valor correspondientes al mínimo y máximo valor de gris del histograma.
 * @param lower int, correspondiente al mínimo valor de gris en el histograma.
 * @param upper int, correspondiente al mínimo valor de gris en el histograma.
 * @return vector<int>, histograma de la imagen actua.
 */
vector<int> Coordinator::getHistogramData (int & lower , int & upper){

    typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;

    lower = this->utils->getMinimumValue(this->actualImage->GetOutput());
    upper = this->utils->getMaximumValue(this->actualImage->GetOutput());

    ImageToHistogramFilterType::Pointer hist = this->utils->histogram(this->actualImage->GetOutput());

    vector<int> data = this->utils->histogramFilterToVector(hist);

    return data;
}
