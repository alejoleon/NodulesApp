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

    this->existImageIn = false;
    this->existImageMedian = false;
    this->existImageLungsMask = false;
    this->existImageMediastinumMask = false;
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

vector<int> Coordinator::getHistogramData (int & lower , int & upper , int mask){

    QStringList maskList = getMaskList();
    vector<int> data;

    ImageProcessingUtils * utils = new ImageProcessingUtils();
    typedef itk::Statistics::MaskedImageToHistogramFilter< ImageType, ImageBinaryType > MaskedImageToHistogramFilterType;

    if (maskList.at(mask).toStdString() == "Lungs") {

        lower = utils->getMinimumValue(this->currentImage , this->imageLungsMask);
        upper = utils->getMaximumValue(this->currentImage, this->imageLungsMask);

        MaskedImageToHistogramFilterType::Pointer hist = utils->histogram(this->currentImage, this->imageLungsMask);

        data = utils->histogramFilterToVector(hist);
    }
    else if (maskList.at(mask).toStdString() == "Mediastinum"){

        lower = utils->getMinimumValue(this->currentImage , this->imageMediastinumMask);
        upper = utils->getMaximumValue(this->currentImage, this->imageMediastinumMask);

        MaskedImageToHistogramFilterType::Pointer hist = utils->histogram(this->currentImage, this->imageMediastinumMask);

        data = utils->histogramFilterToVector(hist);

    }

    return data;
}


QStringList Coordinator::getMaskList (){
    QStringList maskList;

    maskList<<"None";

    if (existImageLungsMask) {
        maskList<<"Lungs";
    }
    if (existImageMediastinumMask) {
        maskList<<"Mediastinum";
    }

    return maskList;
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

    this->existImageIn = true;
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

    this->existImageMedian = true;
}

void Coordinator::doLungsMask(float seeds[]){

    BoumaMethods * boumaMethods = new BoumaMethods();
    BinaryFilters * binaryFilters = new BinaryFilters();
    //Se hace el crecimiento de regiones en cada pulmón.
    boumaMethods->createLungsRegion(imageMedian , imageLungsMask , seeds , -2000 , -500);

    //Se hace el closing
    double radius = (double)2.4/this->dicomProperties->getPixelValue();
    binaryFilters->closingFilter(imageLungsMask,imageLungsMask,radius);

    this->existImageLungsMask = true;
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

    this->existImageMediastinumMask = true;
    modifyLungsMask();
}


void Coordinator::modifyLungsMask(){

    BoumaMethods * boumaMethods = new BoumaMethods();
    ImageFilters * imageFilters = new ImageFilters();

    //Creación de mapa de distancias.
    typedef itk::Image<float, 3> ImageFloatType;
    ImageFloatType::Pointer imageDistanceMap = ImageFloatType::New();

    imageFilters->distanceMap(imageMediastinumMask, imageDistanceMap);

    //Creación de región Geodésica
    ImageBinaryType::Pointer imageRegionGeodesic = ImageBinaryType::New();

     double distanceGeo = (double)11.0/this->dicomProperties->getPixelValue();
     boumaMethods->createExclusionRegionLungs(imageIn, imageDistanceMap, imageRegionGeodesic, -300 , distanceGeo);

     //Se modifica la máscara de los pulmones, restando la región geodésica.
     imageFilters->subtractImage(imageLungsMask, imageRegionGeodesic, imageLungsMask);
}





















//Carga todas las imagenes.
void Coordinator::funcionPrueba(){

    /*
    string rutaimageIn = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/PCsub1-20090909/W0001.1/1.2.826.0.1.3680043.2.656.1.136/S02A01";
    string rutaimageMedian = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0.0_Mediana7";
    string rutaimageLungs = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/1.1_Closing2.4mm";
    string rutaimageMediastinum = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/2.4.MediastinoSinDiafragma";
    */


    string rutaimageIn = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/0.In";
    string rutaimageMedian = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/1.0.Med";
    string rutaimageLungs = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/3.0.FinalLungsMask";
    string rutaimageMediastinum = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/3.0.MediastinoOpen";


    ReaderType::Pointer readerImIn = dicomIOmanage->readInputImage(rutaimageIn);
    imageIn = readerImIn->GetOutput();
    this->existImageIn = true;

    ReaderType::Pointer readerImMedian = dicomIOmanage->readInputImage(rutaimageMedian);
    imageMedian = readerImMedian->GetOutput();
    this->existImageMedian = true;

    ReaderBinaryType::Pointer readerRegGrow = dicomIOmanage->readInputImageBin(rutaimageLungs);
    imageLungsMask = readerRegGrow->GetOutput();
    this->existImageLungsMask = true;

    ReaderBinaryType::Pointer readerMediastinum = dicomIOmanage->readInputImageBin(rutaimageMediastinum);
    imageMediastinumMask = readerMediastinum->GetOutput();
    this->existImageMediastinumMask = true;

    currentImage = imageIn;

    //Se obtienen las propiedades de la imagen de entrada mediante el diccionario de metadatos o encabezado de la imagen DICOM.
    this->dicomProperties->loadData(rutaimageIn);

}
