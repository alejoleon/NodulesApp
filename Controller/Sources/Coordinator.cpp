#include "coordinator.h"

Coordinator::Coordinator()
{
    this->dicomIOmanage = new DICOMIOManage();
    this->imageFilters = new ImageFilters();
    this->config = new Configuracion();
    this->currentImage =   ImageType::New();

    this->readerImageIn = ReaderType::New();
    this->imageIn = ImageType::New();
    this->imageMedian = ImageType::New();
    this->imageLungsMask = ImageBinaryType::New();
    //Para borrar luego:
    this->dirImgMedian = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/SalidasQT/0.1_median";
    this->dirImgLungsMask = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/SalidasQT/1.0_LungsMask";
}

Coordinator::~Coordinator()
{
}

vtkSmartPointer< vtkDICOMImageReader > Coordinator:: getVtkImageReader(string path){
    return this->dicomIOmanage->getVtkImageReader(path);
}

vector<int> Coordinator::getHistogramData (int & lower , int & upper){

    typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;

    lower = this->utils->getMinimumValue(this->currentImage);
    upper = this->utils->getMaximumValue(this->currentImage);

    ImageToHistogramFilterType::Pointer hist = this->utils->histogram(this->currentImage);

    vector<int> data = this->utils->histogramFilterToVector(hist);

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
    default:
        currentImage = imageIn;
        break;
    }
}

const string Coordinator::getImageIn(){
    return this->dirImgIn;
}

void Coordinator::setImageIn(string dirImgIn){
    this->dirImgIn = dirImgIn;

    readerImageIn = this->dicomIOmanage->readInputImage(this->dirImgIn);
    imageIn = readerImageIn->GetOutput();

    currentImage = imageIn;
}


const string Coordinator::getImageMedian(){
    return this->dirImgMedian;
}

const string Coordinator::getImageLungsMask(){
    return this->dirImgLungsMask;
}

const string Coordinator::doMedian (int radius){

    this->imageFilters->filtroMediana(imageIn, radius , imageMedian );
    //Se crea la imagen de mediana en el disco.
    this->dicomIOmanage->SetNameOutputFiles("Median");
    this->dicomIOmanage->writeDicomFile(imageMedian , this->dirImgMedian);

    return this->dirImgMedian;
}

const string Coordinator::doLungsMask(float seeds[]){

    this->boumaMethods->createLungsRegion(imageMedian , imageLungsMask , seeds , -2000 , -500);
     //Se crea la imagen de máscara de pulmones en el disco.
    this->dicomIOmanage->SetNameOutputFiles("LungsMask");
    this->dicomIOmanage->writeDicomFile(imageLungsMask , this->dirImgLungsMask);

    return this->dirImgLungsMask;
}
