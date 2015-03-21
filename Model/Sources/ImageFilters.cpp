#include "ImageFilters.h"
/**
 * @brief Constructor por defecto
 * @return Objeto de la clase ImageFilters
 */
ImageFilters::ImageFilters()
{
}

/**
 * @brief Destructor por defecto.
 * @return 
 */
ImageFilters::~ImageFilters()
{
}

/**
 * @brief Hace un filtro mediada de la imagen (seriesReader) que es pasa por parametro. Se guarda la salida del filtro en la variable pasada por referencia.
 * @param reader :Apuntador a la imagen (seriesReader) que se va a filtrar.
 * @param radius :Radio con el cual se hará el filtro mediada.
 * Ejemplo tomado de: http://itk.org/Wiki/ITK/Examples/Smoothing/MedianImageFilter
 */
void ImageFilters:: filtroMediana(ReaderType::Pointer reader, int radio,ImageType::Pointer &filterOutput){
	
	typedef itk::MedianImageFilter<ImageType, ImageType > FilterType;
	FilterType::Pointer medianFilter = FilterType::New();
	FilterType::InputSizeType radiusInic;
	
	radiusInic.Fill(radio);
	
	medianFilter->SetRadius(radiusInic);
	medianFilter->SetInput( reader->GetOutput() );
	medianFilter->Update();
	filterOutput=medianFilter->GetOutput();
}

/**
 * @brief Hace un filtro mediada de la imagen que es pasa por parametro. Se guarda la salida del filtro en la variable pasada por referencia.
 * @param image :Apuntador a la imagen que se va a filtrar.
 * @param radius :Radio con el cual se hará el filtro mediada.
 * @param filterOutput: Apuntador pasado por referencia en el que se guarda la salida del filtro.
 * Ejemplo tomado de: http://itk.org/Wiki/ITK/Examples/Smoothing/MedianImageFilter
 */
void ImageFilters:: filtroMediana(ImageType::Pointer image, int radius,ImageType::Pointer &filterOutput){
	
	typedef itk::MedianImageFilter<ImageType, ImageType > FilterType;
	FilterType::Pointer medianFilter = FilterType::New();
	FilterType::InputSizeType radiusInic;
	
	radiusInic.Fill(radius);
	
	medianFilter->SetRadius(radiusInic);
	medianFilter->SetInput( image );
	medianFilter->Update();
	filterOutput=medianFilter->GetOutput();
}

/**
 * @brief Realiza una binarización usando crecimiento de regiones en una imagen a partir de dos semillas pasadas por parametro.
 * @param image: imagen a la que se le hará el crecimiento de regiones.
 * @param binLungs: variable pasada por parametro donde se guardará el resultado de la binarización con crecimiento de regiones.
 * @param seeds: Arreglo que contiene las coordenadas de las dos semillas (una de cada pulmón)	 
 * @param min: Umbral minimo
 * @param max: Umbral maximo
 * Ejemplo de binarización http://www.itk.org/Doxygen45/html/Segmentation_2ConnectedThresholdImageFilter_8cxx-example.html
 * Ejemplo de Casteo de imagenes: http://www.itk.org/Wiki/ITK/Examples/ImageProcessing/CastImageFilter
 */
void ImageFilters::regionGrowing(ImageType::Pointer image,ImageBinaryType::Pointer &binLungs, float seeds[], int min, int max) {

	typedef itk::CastImageFilter< ImageType, ImageBinaryType > CastingFilterType;	
	CastingFilterType::Pointer caster = CastingFilterType::New();
	
	typedef itk::CurvatureFlowImageFilter< ImageType, ImageGrowingType > CurvatureFlowImageFilterType;
	CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();
	
	typedef itk::ConnectedThresholdImageFilter< ImageGrowingType,ImageType > ConnectedFilterType;
	ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
	
	smoothing->SetInput( image );
	connectedThreshold->SetInput( smoothing->GetOutput());
	caster->SetInput( connectedThreshold->GetOutput() );
	
	smoothing->SetNumberOfIterations( 5 );
	//smoothing->SetTimeStep( 0.125 );
	smoothing->SetTimeStep( 0.025 );
	
	const PixelType lowerThreshold = min;
	const PixelType upperThreshold = max;
	
	connectedThreshold->SetLower(  lowerThreshold  );
	connectedThreshold->SetUpper(  upperThreshold  );
	
	//connectedThreshold->SetReplaceValue( 32767 );
	connectedThreshold->SetReplaceValue( 32767 );
	
	ImageType::IndexType leftLungSeed;
	leftLungSeed[0] = (signed short)seeds[0];
	leftLungSeed[1] = (signed short)seeds[1];
	leftLungSeed[2] = (signed short)seeds[2];
	
	connectedThreshold->SetSeed( leftLungSeed );
	caster->Update();

	binLungs=caster->GetOutput();
}


//http://itk.org/Wiki/ITK/Examples/ImageProcessing/BinaryThresholdImageFilter
void ImageFilters::binaryThreshold(ImageType::Pointer imageIn,ImageBinaryType::Pointer &imageOut,short lowerThreshold, short upperThreshold){
	
	typedef itk::BinaryThresholdImageFilter <ImageType, ImageBinaryType> BinaryThresholdImageFilterType;
	
	BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
	
	thresholdFilter->SetInput(imageIn);
	thresholdFilter->SetLowerThreshold(lowerThreshold);
	thresholdFilter->SetUpperThreshold(upperThreshold);
	thresholdFilter->SetInsideValue(255);
	thresholdFilter->SetOutsideValue(0);
	thresholdFilter->Update();
	imageOut=thresholdFilter->GetOutput();

}

//http://itk.org/Wiki/ITK/Examples/ImageProcessing/BinaryThresholdImageFilter
void ImageFilters::binaryThreshold(ImageType::Pointer imageIn,ImageBinaryType::Pointer mask,ImageBinaryType::Pointer &imageOut,short lowerThreshold, short upperThreshold){
	
	
	//Copia de la imagen de la mascara
	typedef itk::ImageDuplicator< ImageBinaryType > DuplicatorType;
	DuplicatorType::Pointer duplicator = DuplicatorType::New();
	duplicator->SetInputImage(mask);
	duplicator->Update();
	imageOut = duplicator->GetOutput();

	//Deja toda la imagen en 0
	ImageBinaryType::SizeType sizeImg = mask->GetLargestPossibleRegion().GetSize();
	
	for (int i=0 ; i< sizeImg[0] ; i++)
	{
		for (int j=0; j< sizeImg[1]; j++)
		{
			for (int k=0; k<sizeImg [2]; k++)
			{
				ImageBinaryType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k; 
				imageOut->SetPixel(currentIndex,(char)(0));
			}	
		}
	}
	
	for (int i=0 ; i< sizeImg[0] ; i++)
	{
		for (int j=0; j< sizeImg[1]; j++)
		{
			for (int k=0; k<sizeImg [2]; k++)
			{
				ImageType::IndexType currentIndexLung;
				currentIndexLung[0] = i;
				currentIndexLung[1] = j;
				currentIndexLung[2] = k; 
				
				ImageBinaryType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k; 
				
				ImageType::PixelType currentValueOut = imageIn->GetPixel(currentIndexLung);
				ImageBinaryType::PixelType currentValueMask = mask->GetPixel (currentIndexLung);
				if (currentValueOut>lowerThreshold && currentValueOut<upperThreshold && (int)currentValueMask==255){
		
					imageOut->SetPixel(currentIndex,char(255));
					
				}
			}	
		}
	}
	
	
}





//http://www.itk.org/Wiki/ITK/Examples/ImageProcessing/MaskImageFilter
void ImageFilters::maskImage(ImageType::Pointer input,ImageBinaryType::Pointer mask,ImageType::Pointer &output){

    typedef itk::MaskImageFilter< ImageType, ImageBinaryType > MaskFilterType;
	MaskFilterType::Pointer maskFilter = MaskFilterType::New();
	maskFilter->SetInput(input);
    maskFilter->SetMaskImage(mask);

    //ImageProcessingUtils *utils=new ImageProcessingUtils();
    //maskFilter->SetOutsideValue(utils->getMinimumValue(input));
    maskFilter->Update();
    output=maskFilter->GetOutput();
}

void ImageFilters::erodeFilter(ImageType::Pointer input,ImageType::Pointer &output, int radius){ 
  
	typedef itk::BinaryBallStructuringElement<ImageType::PixelType,ImageType::ImageDimension> StructuringElementType;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(radius);
	structuringElement.CreateStructuringElement();
 
	typedef itk::GrayscaleErodeImageFilter <ImageType, ImageType, StructuringElementType> GrayscaleErodeImageFilterType;
 
	GrayscaleErodeImageFilterType::Pointer erodeFilter = GrayscaleErodeImageFilterType::New();
	erodeFilter->SetInput(input);
	erodeFilter->SetKernel(structuringElement);
	erodeFilter->Update();
	
	output = erodeFilter->GetOutput();
}


void ImageFilters::dilateFilter(ImageType::Pointer input,ImageType::Pointer &output, int radius){ 
   
	typedef itk::BinaryBallStructuringElement< ImageType::PixelType,ImageType::ImageDimension> StructuringElementType;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(radius);
	structuringElement.CreateStructuringElement();
 
	typedef itk::GrayscaleDilateImageFilter <ImageType, ImageType, StructuringElementType> GrayscaleDilateImageFilterType;
 
	GrayscaleDilateImageFilterType::Pointer dilateFilter = GrayscaleDilateImageFilterType::New();
	dilateFilter->SetInput(input);
	dilateFilter->SetKernel(structuringElement);
	dilateFilter->Update();
	
	output = dilateFilter->GetOutput();
	
}


/**
 * @brief ImageFilters::clipBinaryVolume Recorta una imagen binaria, eliminando todos los slices del rango pasado por parametro, en la vista indicada.
 * @param inputIm Imagen de entrada que se va a recortar.
 * @param output Imagen donde se guarda la salida del método.
 * @param initialCoord Coordenada inicial desde la cual se empiezan a eliminar slices.
 * @param endCoord Coordenada final hasta la cual se eliminarán slices.
 * @param plane Vista o proyección que se va a recortar.1:Vista sagital ,2: Vista Coronal, 3: Vista Transversal.
 */
void ImageFilters::clipBinaryVolume(ImageBinaryType::Pointer inputIm , ImageBinaryType::Pointer &output, int initialCoord, int endCoord, int plane){

    typedef itk::ImageDuplicator< ImageBinaryType > DuplicatorType;
    DuplicatorType::Pointer duplicatorIm = DuplicatorType::New();
    duplicatorIm->SetInputImage(inputIm);
    duplicatorIm->Update();
    output = duplicatorIm->GetOutput();

    //Se obtiene el tamaño de cada uno de los planos.
    ImageBinaryType::SizeType size = output->GetLargestPossibleRegion().GetSize();

    int xin, yin, zin, xend, yend, zend;

    switch (plane) {
    case 1:
        xin = size[0]-endCoord;//initialCoord;
        xend = size[0]-initialCoord;//endCoord;
        yin = 0;
        yend = size[1];
        zin = 0;
        zend = size[2];
        break;
    case 2:
        xin = 0;
        xend = size[0];
        yin = initialCoord;//size[1]-endCoord;
        yend = endCoord;//size[1]-initialCoord;
        zin = 0;
        zend = size[2];
        break;
    case 3:
        xin = 0;
        xend = size[0];
        yin = 0;
        yend = size[1];
        zin =  initialCoord;//size[2]-endCoord;
        zend = endCoord;//size[2]-initialCoord;
        break;
    default:
        xin = 0;
        xend = 0;
        yin = 0;
        yend = 0;
        zin = 0;
        zend = 0;
        break;
    }

    for (int i = xin ; i < xend ; i++)
    {
        for (int j = yin ; j < yend; j++)
        {
            for (int k = zin ; k < zend; k++)
            {
                ImageBinaryType::IndexType currentIndex;
                currentIndex[0] = i;
                currentIndex[1] = j;
                currentIndex[2] = k;

                output->SetPixel(currentIndex, char (0));
            }
        }
    }
    output->Update();
}
