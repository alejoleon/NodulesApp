//Bibliotecas estandar de C++
#include <iostream>
#include <stdlib.h>

//Para manejo de imagenes.
#include "itkImage.h"

//#include "QuickView.h"

//Clases del proyecto:
//Para leer propiedades:
#include "Configuracion.h"
//Para leer datos del Header de las imagenes DICOM
#include "DICOMProperties.h"
//Para leer y escribir imagenes DICOM
#include "DICOMIOManage.h"
//Para hacer todos los filtros
#include "ImageFilters.h"
//Para visualizar con VTK
#include "VTKVisualization.h"
//Para realizar operaciones propias del algoritmo
#include "BoumaMethods.h"
//Algunas operaciones útiles de procesamiento de imágenes
#include "ImageProcessingUtils.h"



//Pruebas
#include "itkApproximateSignedDistanceMapImageFilter.h"
#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkConnectedComponentImageFilter.h"



//Definicion de variables de Histograma
typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;
typedef itk::Statistics::MaskedImageToHistogramFilter< ImageType, ImageBinaryType > MaskedImageToHistogramFilterType;





using namespace std;

//Algunos typedef:Asignar un nombre alternativo a tipos existentes, a menudo cuando su declaración normal es aparatosa, potencialmente confusa o probablemente variable de una implementación a otra.
//Para la imagen de entrada que es en escala de grises.
typedef signed short    PixelType;
const unsigned int      DimensionB= 3;
typedef itk::Image< PixelType, DimensionB> ImageType;
typedef itk::ImageSeriesReader< ImageType > ReaderType;

//Imagen de entrada binaria
typedef unsigned char   PixelBinaryType;
typedef itk::Image<PixelBinaryType, DimensionB> ImageBinaryType;
typedef itk::ImageSeriesReader< ImageBinaryType > ReaderBinaryType;

//Mapa de distancia
typedef float   PixelFloatType;
typedef itk::Image<PixelFloatType, DimensionB> ImageFloatType;
typedef itk::ImageSeriesReader< ImageFloatType > ReaderFloatType;



void imprimirImagen(ReaderType::Pointer reader);

int main()

{
	//Se lee el archivo de configuracion
	Configuracion config;
	
	//Se obtiene la ruta donde esta la imagen a la que se le va a hacer el procesamiento.

    /*
	const string rutaImgIn = config.GetRutaImagenIn();
	const string rutaImgMedian = config.GetRutaMedianFilter();
	string rutaImgRegGrow = config.GetRutaRegionGrowing();
	string rutaImgRegGrowClosing = config.GetRutaRegGrowClosing();
    string rutaImgMediastinum = config.GetRutaMediastinum();
    string rutaImgMediastinumOpen = config.GetRutaMediastinumOpen();
    string rutaImgMediastinumGeodesic = config.GetRutaMediastinumGeodesic();
    string rutaImgMediastinumDiafrag = config.GetRutaMediastinoDiafragma();
    string rutaImgInterestReg = config.GetRutaInterestRegion();
	string rutaImgMajorVessels = config.GetRutaMajorVessels();
	string rutaImgPeriphVesselsInt = config.GetRutaPeripheralVesselsInt();
	string rutaImgPeriphVesselsDist = config.GetRutaPeripheralVesselsDist();
    */
    const string rutaImgIn = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/0.In";
    const string rutaImgMedian = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/1.0.Med";
    string rutaImgRegGrow = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/2.0.Pulmones";
    string rutaImgRegGrowClosing = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/2.0.PulmonesClosing";
    string rutaImgMediastinum = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/3.0.Mediastino";
    string rutaImgMediastinumOpen = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/3.0.MediastinoOpen";
    string rutaImgRegionGeodesic = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/3.0.RegionGeodesic";
    string rutaImgDistanceMap = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/3.0.DistanceMap";
    string rutaImgFinalLungMask = "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/PocasImg/3.0.FinalLungsMask";
    string rutaImgMediastinumDiafrag = config.GetRutaMediastinoDiafragma();
    string rutaImgInterestReg = config.GetRutaInterestRegion();
    string rutaImgMajorVessels = config.GetRutaMajorVessels();
    string rutaImgPeriphVesselsInt = config.GetRutaPeripheralVesselsInt();
    string rutaImgPeriphVesselsDist = config.GetRutaPeripheralVesselsDist();



	//Se crea un objeto que obtiene las propiedades que se usaran del header de la imagen DICOM
	DICOMProperties* prop = new DICOMProperties(rutaImgIn);
	double tamPixel = prop->getPixelValue();
    //prop->printActualValues();

	//cout<<"valor Pixel :  "<<tamPixel<<endl;
	
	//-->Se leen las imagen de entrada (readers)
	DICOMIOManage* imagen=new DICOMIOManage();
	

    //Variables donde se guardaran las imagenes:
    ImageType::Pointer imageIn = ImageType::New();
    ImageType::Pointer imagenfiltradaMediana = ImageType::New();
    ImageBinaryType::Pointer imageLungs = ImageBinaryType::New();
    ImageBinaryType::Pointer imageLungsAfterClosing = ImageBinaryType::New();
    ImageBinaryType::Pointer imageMediastinum = ImageBinaryType::New();
    ImageBinaryType::Pointer imageMediastinumOpen = ImageBinaryType::New();
    ImageBinaryType::Pointer imageRegionGeodesic = ImageBinaryType::New();
    ImageFloatType::Pointer imageDistanceMap = ImageFloatType::New();
    ImageBinaryType::Pointer imageFinalLungsMask = ImageBinaryType::New();

    ImageBinaryType::Pointer imageMediastinumDiafrag = ImageBinaryType::New();
    ImageBinaryType::Pointer imageInterestRegion = ImageBinaryType::New();
    ImageBinaryType::Pointer imageMajorVessels = ImageBinaryType::New();
    ImageBinaryType::Pointer periphVesselsInt = ImageBinaryType::New();
    ImageBinaryType::Pointer periphVesselsDist = ImageBinaryType::New();




    ReaderType::Pointer readerImIn = imagen->readInputImage(rutaImgIn);
    imageIn = readerImIn->GetOutput();

    ReaderType::Pointer readerImMedian = imagen->readInputImage(rutaImgMedian);
    imagenfiltradaMediana = readerImMedian->GetOutput();

    ReaderBinaryType::Pointer readerRegGrow = imagen->readInputImageBin(rutaImgRegGrow);
    imageLungs = readerRegGrow->GetOutput();

    ReaderBinaryType::Pointer readerRegGrowClosing = imagen->readInputImageBin(rutaImgRegGrowClosing);
    imageLungsAfterClosing = readerRegGrowClosing->GetOutput();

    ReaderBinaryType::Pointer readerMediastinum = imagen->readInputImageBin(rutaImgMediastinum);
    imageMediastinum = readerMediastinum->GetOutput();

    ReaderBinaryType::Pointer readerMediastinumOpen = imagen->readInputImageBin(rutaImgMediastinumOpen);
    imageMediastinumOpen = readerMediastinumOpen->GetOutput();

    ReaderFloatType::Pointer readerDistanceMap = imagen->readInputImageFloat(rutaImgDistanceMap);
    imageDistanceMap = readerDistanceMap->GetOutput();

    ReaderBinaryType::Pointer readerRegionGeodesic = imagen->readInputImageBin( rutaImgRegionGeodesic);
    imageRegionGeodesic = readerRegionGeodesic->GetOutput();

    ReaderBinaryType::Pointer readerFinalLungsMask = imagen->readInputImageBin( rutaImgFinalLungMask);
    imageFinalLungsMask = readerFinalLungsMask->GetOutput();

    /*
    ReaderBinaryType::Pointer readerMediastinumDiafrag = imagen->readInputImageBin(rutaImgMediastinumDiafrag);
    imageMediastinumDiafrag = readerMediastinumDiafrag->GetOutput();

    */
    //ReaderBinaryType::Pointer readerMediastinumGeodesic = imagen->readInputImageBin(rutaImgMediastinumGeodesic);

    //ReaderBinaryType::Pointer readerInterestRegion = imagen->readInputImageBin(rutaImgInterestReg);
	//ReaderBinaryType::Pointer readerMajorVessels = imagen->readInputImageBin(rutaImgMajorVessels); 
	//ReaderBinaryType::Pointer readerPeriphVesselsInt = imagen->readInputImageBin(rutaImgPeriphVesselsInt);
	//ReaderBinaryType::Pointer readerPeriphVesselsDist = imagen->readInputImageBin(rutaImgPeriphVesselsDist);
	

    //Se le da valores a las imagenes







    //imageMediastinumGeodesic = readerMediastinumGeodesic->GetOutput();
    //imageMediastinumDiafrag = readerMediastinumDiafrag->GetOutput();
    //imageInterestRegion = readerInterestRegion->GetOutput();
    //majorVessels = readerMajorVessels->GetOutput();
    //periphVesselsInt = readerPeriphVesselsInt->GetOutput();
    //periphVesselsDist = readerPeriphVesselsDist->GetOutput();









	
	//Objetos necesarios.
	ImageFilters* filters = new ImageFilters();
	BinaryFilters* binFilters = new BinaryFilters();
	BoumaMethods* boumaMet = new BoumaMethods();
	ImageProcessingUtils* utils = new ImageProcessingUtils();
	VTKVisualization* vtkVis = new VTKVisualization();
	
    //Mediana
/*
	filters->filtroMediana(readerImIn->GetOutput(),5,imagenfiltradaMediana);
	//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("OutMedian5");
    imagen->writeDicomFile(imagenfiltradaMediana,rutaImgMedian);
    */


    /*
	//1. Binarizacion por crecimiento de regiones
	//Semillas del pulmon derecho e izquierdo respectivamente
	float seeds[6];
    seeds[0] = 361;//302;
    seeds[1] = 242;//167;
    seeds[2] = 0;//140;//3.749;
	seeds[3] = 152;//109.684;  152
	seeds[4] = 145;//224.723;   145
    seeds[5] = 0;//140;//2.49;   13
	
    boumaMet->createLungsRegion(imagenfiltradaMediana,imageLungs,seeds,-2000,-500);
	//-->Se crean las imagenes de salida
    imagen->SetNameOutputFiles("OutRegGrow");
    imagen->writeDicomFile(imageLungs,rutaImgRegGrow);

    */

    /*
	//1.1 Filtro Closing con 2.4 mm de radio
	double radius = (double)2.4/tamPixel;
    binFilters->closingFilter(imageLungs,imageLungsAfterClosing,radius);
	//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("OutRegGrowClosing");
    imagen->writeDicomFile(imageLungsAfterClosing,rutaImgRegGrowClosing);
*/


/*
    //2.Operacion de row-wise para obtener el mediastino.

    boumaMet->rowWiseMethod(imagenfiltradaMediana,imageLungsAfterClosing,imageInterestRegion, imageMediastinum, 150);
	//-->Se crean las imagenes de salida
    imagen->SetNameOutputFiles("OutMediastinum");
    imagen->writeDicomFile(imageMediastinum,rutaImgMediastinum);
*/


/*
    //2.1 Filtro opening con 6.0 mm de radio
    double radiusOp = (double)6.0/tamPixel;
    binFilters->openingFilter(imageMediastinum,imageMediastinumOpen,radiusOp);
    //Se quitan las regiones pequeñas sueltas
    boumaMet->clearSmallRegions(imageMediastinumOpen);
    //-->Se crean las imagenes de salida
    imagen->SetNameOutputFiles("OutMediastinumOpen");
    imagen->writeDicomFile(imageMediastinumOpen,rutaImgMediastinumOpen);
*/

/*
    //2.2 Mediastino: corte del diafragma
    filters->clipBinaryVolume(imageMediastinumOpen, imageMediastinumDiafrag, 0 , 80 , 3);
    //-->Se crean las imagenes de salida
    imagen->SetNameOutputFiles("OutMediastinumDiafrag");
    imagen->writeDicomFile(imageMediastinumDiafrag,rutaImgMediastinumDiafrag);
*/

/*
    //2.3 Mapa de distancia.
    filters->distanceMap(imageMediastinumOpen, imageDistanceMap);
    //-->Se crean las imagenes de salida
    imagen->SetNameOutputFiles("OutDistanceMap");
    imagen->writeDicomFile(imageDistanceMap,rutaImgDistanceMap);
*/

/*
    //2.3 Creacion de region geodésica

    double distanceGeo = (double)11.0/tamPixel;
    cout<<"Distancia Geo "<<distanceGeo<<endl;
    boumaMet->createExclusionRegionLungs(imageIn, imageDistanceMap , imageRegionGeodesic , -300 , distanceGeo );
    //-->Se crean las imagenes de salida
    imagen->SetNameOutputFiles("Exclusion");
    imagen->writeDicomFile(imageRegionGeodesic,rutaImgRegionGeodesic);
*/

    /*
    //2.4 Pulmones sin region geodesica.

    filters->subtractImage(imageLungsAfterClosing,imageRegionGeodesic,imageFinalLungsMask);
    //-->Se crean las imagenes de salida
    imagen->SetNameOutputFiles("Pulmones");
    imagen->writeDicomFile(imageFinalLungsMask,rutaImgFinalLungMask);
*/








/*
    //2.3 Region de interes
    //Se suman las dos imagenes (pulmones y mediastino) para obtener la máscara del área de interés.
    //Operacion or
    binFilters->orFunction(imageLungsAfterClosing,imageMediastinum,imageInterestRegion);
    imagen->SetNameOutputFiles("OutInterestRegion");
    imagen->writeDicomFile(imageInterestRegion,rutaImgInterestReg);
*/

/*
	//3.Major Pulmonary Vessels    
    boumaMet->funcionVasos(imageIn,imageInterestRegion,imageMajorVessels);
	//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("OutMajorVessels");
    imagen->writeDicomFile(imageMajorVessels,rutaImgMajorVessels);
*/

/*
	//4. Segmentacion de vasos perifericos.
	//4.1 : Segmentacion por intensidad.
	boumaMet->peripheralVessel(imagenfiltradaMediana,lungsAfterClosing,periphVesselsInt);
		//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("OutPeriphInt");
	imagen->writeDicomFile(periphVesselsInt,rutaImgPeriphVesselsInt);
*/	



    //Para mostrar en VTK
    //VTKVisualization* vtkVis= new VTKVisualization();
    //vtkVis->SetDir1(rutaImgIn);
    //vtkVis->SetDir1("/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/dddd");
    //vtkVis->SetDir2(rutaImgPeriphVesselsDist);
    //vtkVis->readImages(imageMediastinumOpen);

    cout<<"Se acabó"<<endl;
	 return 0 ;
}
