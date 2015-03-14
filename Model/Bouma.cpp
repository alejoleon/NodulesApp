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


//Para duplicar una imagen.
#include "itkImageDuplicator.h"

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

void imprimirImagen(ReaderType::Pointer reader);

int main()

{
	//Se lee el archivo de configuracion
	Configuracion config;
	
	//Se obtiene la ruta donde esta la imagen a la que se le va a hacer el procesamiento.
	const string rutaImgIn = config.GetRutaImagenIn();
	const string rutaImgMedian = config.GetRutaMedianFilter();
	string rutaImgRegGrow = config.GetRutaRegionGrowing();
	string rutaImgRegGrowClosing = config.GetRutaRegGrowClosing();
    string rutaImgMediastinum = config.GetRutaMediastinum();
    string rutaImgMediastinumOpen = config.GetRutaMediastinumOpen();
    string rutaImgMediastinumGeodesic = config.GetRutaMediastinumGeodesic();
	string rutaImgInterestReg = config.GetRutaInterestRegion();
	string rutaImgMajorVessels = config.GetRutaMajorVessels();
	string rutaImgPeriphVesselsInt = config.GetRutaPeripheralVesselsInt();
	string rutaImgPeriphVesselsDist = config.GetRutaPeripheralVesselsDist();
	
	
	//Se crea un objeto que obtiene las propiedades que se usaran del header de la imagen DICOM
	DICOMProperties* prop = new DICOMProperties(rutaImgIn);
	double tamPixel = prop->getPixelValue();
    prop->printActualValues();

	//cout<<"valor Pixel :  "<<tamPixel<<endl;
	
	//-->Se leen las imagen de entrada (readers)
	DICOMIOManage* imagen=new DICOMIOManage();
	

	ReaderType::Pointer readerImIn = imagen->readInputImage(rutaImgIn);
    ReaderType::Pointer readerImMedian = imagen->readInputImage(rutaImgMedian);
    ReaderBinaryType::Pointer readerRegGrow = imagen->readInputImageBin(rutaImgRegGrow);
    ReaderBinaryType::Pointer readerRegGrowClosing = imagen->readInputImageBin(rutaImgRegGrowClosing);
    ReaderBinaryType::Pointer readerMediastinum = imagen->readInputImageBin(rutaImgMediastinum);
    ReaderBinaryType::Pointer readerMediastinumOpen = imagen->readInputImageBin(rutaImgMediastinumOpen);
    //ReaderBinaryType::Pointer readerMediastinumGeodesic = imagen->readInputImageBin(rutaImgMediastinumGeodesic);
    ReaderBinaryType::Pointer readerInterestRegion = imagen->readInputImageBin(rutaImgInterestReg);
	//ReaderBinaryType::Pointer readerMajorVessels = imagen->readInputImageBin(rutaImgMajorVessels); 
	//ReaderBinaryType::Pointer readerPeriphVesselsInt = imagen->readInputImageBin(rutaImgPeriphVesselsInt);
	//ReaderBinaryType::Pointer readerPeriphVesselsDist = imagen->readInputImageBin(rutaImgPeriphVesselsDist);
	
	//Variables donde se guardaran las imagenes:
	ImageType::Pointer imageIn = ImageType::New();
	ImageType::Pointer imagenfiltradaMediana = ImageType::New();
    ImageBinaryType::Pointer imageLungs = ImageBinaryType::New();
    ImageBinaryType::Pointer imageLungsAfterClosing = ImageBinaryType::New();
    ImageBinaryType::Pointer imageMediastinum = ImageBinaryType::New();
    ImageBinaryType::Pointer imageMediastinumOpen = ImageBinaryType::New();
    ImageBinaryType::Pointer imageMediastinumGeodesic = ImageBinaryType::New();
    ImageBinaryType::Pointer imageInterestRegion = ImageBinaryType::New();
    ImageBinaryType::Pointer imageMajorVessels = ImageBinaryType::New();
	ImageBinaryType::Pointer periphVesselsInt = ImageBinaryType::New();
	ImageBinaryType::Pointer periphVesselsDist = ImageBinaryType::New();
	
	//Se le da valores a las imagenes
	imageIn = readerImIn->GetOutput();
    imagenfiltradaMediana = readerImMedian->GetOutput();
    imageLungs = readerRegGrow->GetOutput();
    imageLungsAfterClosing = readerRegGrowClosing->GetOutput();
    imageMediastinum = readerMediastinum->GetOutput();
    imageMediastinumOpen = readerMediastinumOpen->GetOutput();
    //imageMediastinumGeodesic = readerMediastinumGeodesic->GetOutput();
    imageInterestRegion = readerInterestRegion->GetOutput();
	//majorVessels = readerMajorVessels->GetOutput();
	//periphVesselsInt = readerPeriphVesselsInt->GetOutput();
	//periphVesselsDist = readerPeriphVesselsDist->GetOutput();
	
	//Objetos necesarios.
	ImageFilters* filters = new ImageFilters();
	BinaryFilters* binFilters = new BinaryFilters();
	BoumaMethods* boumaMet = new BoumaMethods();
	ImageProcessingUtils* utils = new ImageProcessingUtils();
	VTKVisualization* vtkVis = new VTKVisualization();
	
	
	//imprimirImagen(readerImIn);


/*
	ImageBinaryType::Pointer prueba = ImageBinaryType::New();
	
	boumaMet->createInterestRegion(imagenfiltradaMediana,interestRegion,prueba);
	
		//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("prueba");
	imagen->writeDicomFile(prueba,"/Users/AlejoMac/Escritorio/PP");
	*/


/*
	// 0. Filtro mediana
	// Create and setup a reader
	filters->filtroMediana(readerImIn->GetOutput(),1,imagenfiltradaMediana);
	//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("OutMedian1");
	imagen->writeDicomFile(imagenfiltradaMediana,rutaImgMedian);

	filters->filtroMediana(readerImIn->GetOutput(),3,imagenfiltradaMediana);
	//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("OutMedian3");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Mediana3");
	
	filters->filtroMediana(readerImIn->GetOutput(),5,imagenfiltradaMediana);
	//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("OutMedian5");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Mediana5");

    filters->filtroMediana(readerImIn->GetOutput(),7,imagenfiltradaMediana);
	//-->Se crean las imagenes de salida
	imagen->SetNameOutputFiles("OutMedian7");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Mediana7");
*/
	/*
	//Erosion
	filters->erodeFilter(imageIn,imagenfiltradaMediana,1);
	imagen->SetNameOutputFiles("OutErode1");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Erode1");
	
	filters->erodeFilter(imageIn,imagenfiltradaMediana,3);
	imagen->SetNameOutputFiles("OutErode3");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Erode3");
	
	filters->erodeFilter(imageIn,imagenfiltradaMediana,5);
	imagen->SetNameOutputFiles("OutErode5");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Erode5");
	
	filters->erodeFilter(imageIn,imagenfiltradaMediana,7);
	imagen->SetNameOutputFiles("OutErode7");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Erode7");
	*/
	
	/*
	//Dilatacion
	filters->dilateFilter(imageIn,imagenfiltradaMediana,1);
	imagen->SetNameOutputFiles("OutDilate1");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Dilate1");
	
	filters->dilateFilter(imageIn,imagenfiltradaMediana,3);
	imagen->SetNameOutputFiles("OutDilate3");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Dilate3");
	
	filters->dilateFilter(imageIn,imagenfiltradaMediana,5);
	imagen->SetNameOutputFiles("OutDilate5");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Dilate5");
	
	filters->dilateFilter(imageIn,imagenfiltradaMediana,7);
	imagen->SetNameOutputFiles("OutDilate7");
	imagen->writeDicomFile(imagenfiltradaMediana,"/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/0_Dilate7");
	*/
	

/*
	//1. Binarizacion por crecimiento de regiones
	//Semillas del pulmon derecho e izquierdo respectivamente
	float seeds[6];
    seeds[0] = 361;//302;
    seeds[1] = 242;//167;
    seeds[2] = 140;//140;//3.749;
	seeds[3] = 152;//109.684;  152
	seeds[4] = 145;//224.723;   145
    seeds[5] = 140;//140;//2.49;   13
	
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
	
/*
	//Para mostrar en VTK
    //VTKVisualization* vtkVis= new VTKVisualization();
    vtkVis->SetDir1(rutaImgMedian);
    //vtkVis->SetDir1("/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/dddd");
    vtkVis->SetDir2(rutaImgMajorVessels);
    vtkVis->readImages(imagenfiltradaMediana,imageMajorVessels);
    */
	 return 0 ;
}


void imprimirImagen(ReaderType::Pointer reader){
	ImageBinaryType::SizeType sizeLung = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
	
	signed short max=0;
	signed short min=10000;
	
	
	for (int i=0 ; i< sizeLung[0] ; i++)
	{
		for (int j=0; j< sizeLung[1]; j++)
		{
			for (int k=0; k<sizeLung [2]; k++)
			{
				ImageType::IndexType currentIndexLung;
				currentIndexLung[0] = i;
				currentIndexLung[1] = j;
				currentIndexLung[2] = k; 
				ImageType::PixelType currentValueOut = reader->GetOutput()->GetPixel(currentIndexLung);
				//cout<<(int)currentValueOut<<",";
				if (currentValueOut<min){
					min=currentValueOut;
				}
				if (currentValueOut>max){
					max=currentValueOut;
				}
			}	
		}
	}
	
	cout<<" MAXIM "<<max<<endl;
	cout<<" MINIM "<<min<<endl;
}
