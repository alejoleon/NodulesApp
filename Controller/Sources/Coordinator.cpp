#include "coordinator.h"

Coordinator::Coordinator()
{
    this->dicomProperties = new DICOMProperties();
    this->dicomIOmanage = new DICOMIOManage();

    this->currentImage =   ImageType::New();
    this->imageIn = ImageType::New();
    this->imageMedian = ImageType::New();
    this->imageLungsMask = ImageBinaryType::New();
    this->imageMediastinumMask = ImageBinaryType::New();
    this->imageInterestRegion = ImageBinaryType::New();
}

Coordinator::~Coordinator()
{
}

vector<int> Coordinator::getHistogramData (int & lower , int & upper){

    ImageProcessingUtils * utils = new ImageProcessingUtils();
    typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;

    lower = utils->getMinimumValue(this->currentImage);
    upper = utils->getMaximumValue(this->currentImage);

    ImageToHistogramFilterType::Pointer hist = utils->histogram(this->currentImage);

    vector<int> data = utils->histogramFilterToVector(hist);

    return data;
}

void Coordinator::setCurrentImage (int image){

    typedef itk::CastImageFilter< ImageBinaryType, ImageType > CastFilterType;
    CastFilterType::Pointer castFilter = CastFilterType::New();

    switch (image){
    case 1:
        currentImage = imageIn;
        break;
    case 2:
        currentImage = imageMedian;
        break;
    case 3:
        castFilter->SetInput(imageLungsMask);
        castFilter->Update();
        currentImage = castFilter->GetOutput();
        break;
    case 4:
        castFilter->SetInput(imageMediastinumMask);
        castFilter->Update();
        currentImage = castFilter->GetOutput();
    default:
        currentImage = imageIn;
        break;
    }
}

void Coordinator::setImageIn(string dirImgIn){
    this->dirImgIn = dirImgIn;
    imageIn = this->dicomIOmanage->readInputImage(this->dirImgIn)->GetOutput();
    currentImage = imageIn;

    //Se obtienen las propiedades de la imagen de entrada mediante el diccionario de metadatos o encabezado de la imagen DICOM.
    this->dicomProperties->loadData(dirImgIn);
}

vtkImageData* Coordinator::getImageIn () {
    vtkImageData* image;
    this->dicomIOmanage->castImage(this->imageIn, image);
    return image;
}

vtkImageData* Coordinator::getImageMedian(){
    vtkImageData* image;
    this->dicomIOmanage->castImage(this->imageMedian, image);
    return image;
}

vtkImageData* Coordinator::getImageLungsMask(){
    vtkImageData* image;
    this->dicomIOmanage->castImage(this->imageLungsMask, image);
    return image;
}

vtkImageData* Coordinator::getImageMediastinumMask(){
    vtkImageData* image;
    this->dicomIOmanage->castImage(this->imageMediastinumMask, image);
    return image;
}

vtkImageData* Coordinator::getImageInterestRegion(){
    vtkImageData* image;
    this->dicomIOmanage->castImage(this->imageInterestRegion , image);
    return image;
}

void Coordinator::doMedian (int radius){
    ImageFilters * imageFilters = new ImageFilters();
    imageFilters->filtroMediana(imageIn, radius , imageMedian );
}

void Coordinator::doLungsMask(float seeds[]){

    BoumaMethods * boumaMethods = new BoumaMethods();
    BinaryFilters * binaryFilters = new BinaryFilters();
    //Se hace el crecimiento de regiones en cada pulmón.
    boumaMethods->createLungsRegion(imageMedian , imageLungsMask , seeds , -2000 , -500);

    //Se hace el closing
    double radius = (double)2.4/this->dicomProperties->getPixelValue();
    binaryFilters->closingFilter(imageLungsMask,imageLungsMask,radius);
}

void Coordinator::doMediastinumMask (int first, int last){
    BoumaMethods * boumaMethods = new BoumaMethods();
    BinaryFilters * binaryFilters = new BinaryFilters();
    ImageFilters * imageFilters = new ImageFilters();
    boumaMethods->rowWiseMethod(imageMedian,imageLungsMask,imageInterestRegion,imageMediastinumMask,150);


    //Filtro opening con 6.0 mm de radio
    double radiusOp = (double)6.0/this->dicomProperties->getPixelValue();
    binaryFilters->openingFilter(imageMediastinumMask, imageMediastinumMask, radiusOp);
    //Se quitan las regiones pequeñas sueltas
    boumaMethods->clearSmallRegions(imageMediastinumMask);

    if (first != last) {
        imageFilters->clipBinaryVolume(imageMediastinumMask, imageMediastinumMask, first, last, 3);
    }


}














//Carga todas las imagenes.
void Coordinator::funcionPrueba(){
    ReaderType::Pointer readerImIn = dicomIOmanage->readInputImage("/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/PCsub1-20090909/W0001.1/1.2.826.0.1.3680043.2.656.1.136/S02A01");
    imageIn = readerImIn->GetOutput();

    ReaderType::Pointer readerImMedian = dicomIOmanage->readInputImage("/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0.0_Mediana7");
    imageMedian = readerImMedian->GetOutput();

    ReaderBinaryType::Pointer readerRegGrow = dicomIOmanage->readInputImageBin("/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/1.1_Closing2.4mm");
    imageLungsMask = readerRegGrow->GetOutput();

    ReaderBinaryType::Pointer readerMediastinum = dicomIOmanage->readInputImageBin("/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/2.4.MediastinoSinDiafragma");
    imageMediastinumMask = readerMediastinum->GetOutput();

    currentImage = imageIn;

    //Se obtienen las propiedades de la imagen de entrada mediante el diccionario de metadatos o encabezado de la imagen DICOM.
    this->dicomProperties->loadData("/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/PCsub1-20090909/W0001.1/1.2.826.0.1.3680043.2.656.1.136/S02A01");

}
