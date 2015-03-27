#include "BoumaMethods.h"

/**
 * @brief Constructor por defecto.
 * @return Objeto de la clase BoumaMethods.
 */
BoumaMethods::BoumaMethods()
{
}

/**
 * @brief Destructor por defecto. 
 * @return 
 */
BoumaMethods::~BoumaMethods()
{
}

/**
 * @brief Se hace la funcion de procesamiento por filas para obtener las imagenes del mediastino y de la region de interes que se guardan en las variables pasadas por parametro.
 * @param lungs : Imagen de la segmentación de los pulmones.
 * @param interestRegion : Variable donde se guardará la segmentación de la región de interés.
 * @param mediastinum : Variable donde se guardará la segmentación del mediastino.
 * Ejemplo de Duplicar imagen: http://www.itk.org/Wiki/ITK/Examples/SimpleOperations/ImageDuplicator
 */
void BoumaMethods:: rowWiseMethod (ImageType::Pointer imageIn, ImageBinaryType::Pointer lungs, ImageBinaryType::Pointer &interestRegion, ImageBinaryType::Pointer &mediastinum, int upperThreshold) {
	
	//Copia de la imagen que sale del filtro de closing
	typedef itk::ImageDuplicator< ImageBinaryType > DuplicatorType;
	DuplicatorType::Pointer duplicator = DuplicatorType::New();
	DuplicatorType::Pointer duplicatorInteres = DuplicatorType::New();
	duplicator->SetInputImage(lungs);
	duplicatorInteres->SetInputImage(lungs);
	duplicator->Update();
	duplicatorInteres->Update();
	mediastinum = duplicator->GetOutput();
    //interestRegion=duplicatorInteres->GetOutput();
	
	//Para dejar todos los vóxeles de ambas copias con valor de 0
	ImageBinaryType::SizeType size = mediastinum->GetLargestPossibleRegion().GetSize();
	ImageBinaryType::IndexType index = mediastinum->GetLargestPossibleRegion().GetIndex();

	for (int i=0 ; i< size[0] ; i++)
	{
		for (int j=0; j< size[1]; j++)
		{
			for (int k=0; k<size [2]; k++)
			{
				ImageBinaryType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k;
				mediastinum->SetPixel(currentIndex,(char)(0));
                //interestRegion->SetPixel(currentIndex,(char)(0));
			}	
		}
	}
		
	// Recorrer la imagen binaria de pulmones en forma de fila.
	// Encontrar el primer pixel que aparezca en cada fila recorriendo de izquierda a derecha y guardar esa coordenada
	// Encontrar el primer pixel que aparezca en cada fila recorriendo de derecha a izquierda y guardar esa coordenada
	// Poner en 1 toda la fila entre esas dos coordenadas

	ImageBinaryType::SizeType sizeBinLung = lungs->GetLargestPossibleRegion().GetSize();
    //Maximos y minimas coordenadas en cada fila
	int xmin=0;
	int xmax=0;
	bool minFound=false;
	bool maxFound=false;
	
	for (int i=0 ; i< sizeBinLung[2] ; i++)
	{
		for (int j=0; j< sizeBinLung[1]; j++)
		{
			for (int k=0; k<sizeBinLung [0]  && !minFound ; k++)
			{
				ImageBinaryType::IndexType currentIndexLeft;
				currentIndexLeft[0] = k;
				currentIndexLeft[1] = j;
				currentIndexLeft[2] = i;
				
				ImageBinaryType::PixelType currentValue = lungs->GetPixel(currentIndexLeft);
				
				if((int)currentValue==255){
					xmin=k;
					minFound=true;
				}
			}
			
			
			if(minFound){
				for (int m=sizeBinLung[0];m>=0 && !maxFound;m--){
										
					ImageBinaryType::IndexType currentIndexRight;
					currentIndexRight[0] = m;
					currentIndexRight[1] = j;
					currentIndexRight[2] = i;
					
					ImageBinaryType::PixelType currentValue = lungs->GetPixel(currentIndexRight);			
					
					if((int)currentValue==255){
						xmax=m;
						maxFound=true;
					}
				}
			}
				
			if(minFound && maxFound) {
				for (int e=xmin; e<xmax ; e++){
					ImageBinaryType::IndexType currentIndex;
					currentIndex[0] = e;
					currentIndex[1] = j;
					currentIndex[2] = i;
					ImageBinaryType::PixelType currentValueP = lungs->GetPixel(currentIndex);
					
					if((int)currentValueP!=255) {
						mediastinum->SetPixel(currentIndex,(char)(255));
                        //interestRegion->SetPixel(currentIndex,(char)(255));
					}
					else{
                        //interestRegion->SetPixel(currentIndex,(char)(255));
					}
					
					
				}
			}
			minFound=false;
			maxFound=false;
			xmin=0;
			xmax=0;
		}
	}

    //Para quitar la region de huesos (columna vertebral)
    this->createMediastinumRegion(imageIn,mediastinum,mediastinum,upperThreshold);

    //Para quitar regiones pequeñas que quizas no corresponden al mediastino.
    clearSmallRegions(mediastinum);

	
    //Se suman las dos imagenes (pulmones y mediastini) para obtener la mascara del area de interes.
	//Operacion or
    //BinaryFilters* orFunc=new BinaryFilters();
    //orFunc->orFunction(lungs,mediastinum,interestRegion);

    mediastinum->Update();
    //interestRegion->Update();
}

/**
 * @brief BoumaMethods::clearSmallRegions Se hace un procesamiento usando etiquetado de regiones con el fin de eliminar pequeñas regiones que pueden no pertenecer al mediastino.
 * @param mediastinum Imagen de segmentación del mediastino.
 * Tomado de: http://www.itk.org/Wiki/ITK/Examples/ImageProcessing/LabelStatisticsImageFilter
 */
void BoumaMethods:: clearSmallRegions (ImageBinaryType::Pointer &mediastinum) {

	//Etiquetado para la imagen del mediastino con el fin de quitar las regiones muy pequeñas que quedaron en la imagen y que pueden no corresponder al mediastino.
	typedef itk::BinaryImageToLabelMapFilter<ImageBinaryType> BinaryImageToLabelMapFilterType;
	BinaryImageToLabelMapFilterType::Pointer binaryImageToLabelMapFilter = BinaryImageToLabelMapFilterType::New();
	binaryImageToLabelMapFilter->SetInput(mediastinum);
	binaryImageToLabelMapFilter->Update();
	
	typedef itk::LabelMapToLabelImageFilter<BinaryImageToLabelMapFilterType::OutputImageType, ImageBinaryType> LabelMapToLabelImageFilterType;
	LabelMapToLabelImageFilterType::Pointer labelMapToLabelImageFilter = LabelMapToLabelImageFilterType::New();
	labelMapToLabelImageFilter->SetInput(binaryImageToLabelMapFilter->GetOutput());
	labelMapToLabelImageFilter->Update();
	
	typedef itk::LabelStatisticsImageFilter< ImageBinaryType, ImageBinaryType > LabelStatisticsImageFilterType;
	LabelStatisticsImageFilterType::Pointer labelStatisticsImageFilter = LabelStatisticsImageFilterType::New();
	labelStatisticsImageFilter->SetLabelInput( labelMapToLabelImageFilter->GetOutput() );
	labelStatisticsImageFilter->SetInput(mediastinum);
	labelStatisticsImageFilter->Update();
 	
	unsigned int maximum=0;
	unsigned int maxValue=0;
	unsigned int pos=0;
	
	typedef LabelStatisticsImageFilterType::ValidLabelValuesContainerType ValidLabelValuesType;
	typedef LabelStatisticsImageFilterType::LabelPixelType                LabelPixelType; 
	 
	 //Se calcula la region mas grande que corresponde al mediastino
	for(ValidLabelValuesType::const_iterator vIt=labelStatisticsImageFilter->GetValidLabelValues().begin();
      vIt != labelStatisticsImageFilter->GetValidLabelValues().end();
      ++vIt)
    {
		if ( labelStatisticsImageFilter->HasLabel(*vIt) )
		{
			LabelPixelType labelValue = *vIt;
						
			//std::cout << "sum: " << labelStatisticsImageFilter->GetSum( labelValue ) << std::endl;
			//std::cout << "region: " << labelStatisticsImageFilter->GetRegion( labelValue ) << std::endl;
			
			if(labelStatisticsImageFilter->GetSum( labelValue )>maxValue){
				maxValue=labelStatisticsImageFilter->GetSum( labelValue );
				maximum=pos;
			}
		}
		pos++;
	}
	
	pos=0;
	
	//Para borrar regiones pequeñas y dejar solo el mediastino
	for(ValidLabelValuesType::const_iterator vIt=labelStatisticsImageFilter->GetValidLabelValues().begin();
      vIt != labelStatisticsImageFilter->GetValidLabelValues().end();
      ++vIt)
    {
		if ( labelStatisticsImageFilter->HasLabel(*vIt) )
		{
			LabelPixelType labelValue = *vIt;
						
			if(pos!=maximum && pos!=0 ){
							//std::cout << "sum: " << labelStatisticsImageFilter->GetSum( labelValue ) << std::endl;
							//std::cout << "region: " << labelStatisticsImageFilter->GetRegion( labelValue ) << std::endl;
						unsigned int ii,j,k;
						unsigned int sizex=labelStatisticsImageFilter->GetRegion( labelValue ).GetSize()[0];
						unsigned int sizey=labelStatisticsImageFilter->GetRegion( labelValue ).GetSize()[1];
						unsigned int sizez=labelStatisticsImageFilter->GetRegion( labelValue ).GetSize()[2];
						
							for (ii=labelStatisticsImageFilter->GetRegion( labelValue ).GetIndex()[0]; ii< (labelStatisticsImageFilter->GetRegion( labelValue ).GetIndex()[0])+sizex;ii++)
							{
								for (j=labelStatisticsImageFilter->GetRegion( labelValue ).GetIndex()[1]; j< (labelStatisticsImageFilter->GetRegion( labelValue ).GetIndex()[1])+sizey;j++)
								{
									for (k=labelStatisticsImageFilter->GetRegion( labelValue ).GetIndex()[2];k<labelStatisticsImageFilter->GetRegion( labelValue ).GetIndex()[2]+sizez; k++)
									{
										
										ImageBinaryType::IndexType currentIndex;
										currentIndex[0] = ii;
										currentIndex[1] = j;
										currentIndex[2] = k;
										ImageBinaryType::PixelType currentValue = mediastinum->GetPixel(currentIndex);
										mediastinum->SetPixel(currentIndex,char(0));
									}
								}
							}
			}
		}
		pos++;
    }
    mediastinum->Update();
}


void BoumaMethods:: funcionVasos (ImageType::Pointer image ,ImageBinaryType::Pointer mask,ImageBinaryType::Pointer &imageOut) {
	
	
	//freq0 indica la frecuencia o cantidad de vóxeles en 0 HU.
	//majorHU indica la HU que mas se repite despues de 150 HU.
	//majorFreq indica la mayor frecuencia que hay despues de 150 HU. 
	//thresholdSelected es el valor de umbral seleccionado para este conjunto de imagenes, que es el promedio de 0 HU con la intensidad mas frecuente despues de 150 HU.
	unsigned int freq0=0;
	unsigned int majorHU=150;
	unsigned int majorFreq=0;
	signed short thresholdSelected=0;
	
	ImageProcessingUtils* utils=new ImageProcessingUtils();
	//Valores maximos y minimos de la imagen usando la mascara, con el fin de construir el histograma.
	short max=utils->getMaximumValue(image,mask);
	short min=utils->getMinimumValue(image,mask);
	min=0;
	MaskedImageToHistogramFilterType::Pointer imageToHistogramFilter=utils->histogram(image,mask,0,max);
	ImageToHistogramFilterType::HistogramType* histogram = imageToHistogramFilter->GetOutput();
	
	freq0=histogram->GetFrequency(0); 
	
	//Se recorre todo el histograma desde 150HU para calcular el valor mayor de frecuencia
	for (int i=majorHU;i<histogram->GetSize()[0];i++){
		if(histogram->GetFrequency(i)>majorFreq){
			majorFreq=histogram->GetFrequency(i);
			majorHU=i;
		}
	}
	
	thresholdSelected=majorHU/2;

	std::cout << "Frequency = [ ";
	for(int i = 0; i < histogram->GetSize()[0]; ++i)
    {
		std::cout << i +min<< "="<<histogram->GetFrequency(i) ;
		if( i != histogram->GetSize()[0] - 1 )
		{
			std::cout << std::endl ;
		}
    }
  std::cout << " ]" << std::endl;

	std::cout<<"FRECUENCIA DE CERO : "<<freq0<<std::endl;
	std::cout<<"Mayor frecuencia  : "<<majorHU<<" , Con valor de : "<<majorFreq<<std::endl;
	std::cout<<"Umbral : "<<thresholdSelected<<std::endl;
	
    /*
	ImageFilters* filters=new ImageFilters();
	
	ImageBinaryType::Pointer vesels = ImageBinaryType::New();
	//filters->regionGrowing(image,vesels,189.244,196.769,4.999,thresholdSelected-500,thresholdSelected+500);
	//filters->regionGrowing(image,vesels,310.733,178.492,0,thresholdSelected-10,thresholdSelected+10);
	filters->binaryThreshold(image,vesels,thresholdSelected-10,thresholdSelected+10);
	imageOut=vesels;
    */
}

void BoumaMethods::peripheralVessel (ImageType::Pointer image ,ImageBinaryType::Pointer mask,ImageBinaryType::Pointer &imageOut) {
	
	/*
	//ImageBinaryType::Pointer imageMasked = ImageType::New();
	ImageType::Pointer imageMasked = ImageType::New();
	BinaryFilters* filter = new BinaryFilters();
	ImageFilters* filterImg = new ImageFilters();
	ImageProcessingUtils* utils= new ImageProcessingUtils();
	
	
	filter->maskImage(image,mask,imageMasked);
	
	short maxValue = utils->getMaximumValue(imageMasked);
	
	
	filterImg->binaryThreshold(imageMasked,imageOut,-150,maxValue);
	
	
	
	//imageOut=imageMasked;
	*/
	/*
	
	//Para mostrar en VTK
	VTKVisualization* vtkVis= new VTKVisualization();
	vtkVis->quickView(imageMasked);
	*/
	
}


void BoumaMethods::createMediastinumRegion(ImageType::Pointer image, ImageBinaryType::Pointer regionMask , ImageBinaryType::Pointer &mediastinumRegion, int upperThreshold){

    VTKVisualization *vtkvis=new VTKVisualization();
    ImageType::Pointer imageMasked = ImageType::New();
	ImageFilters* filters = new ImageFilters();
	ImageProcessingUtils* utils=new ImageProcessingUtils();

    filters->maskImage(image,regionMask,imageMasked);
    filters->binaryThreshold(imageMasked,regionMask,mediastinumRegion,utils->getMinimumValue(imageMasked),upperThreshold);
    mediastinumRegion->Update();
}

/**
 * @brief BoumaMethods::createLungsRegion Realiza un doble crecimiento de regiones (en ambos pulmones) para hacer una imagen binaria o máscara de los pulmones.
 * @param imageIn Imagen de entrada a la cual se le hace el crecimiento de regiones, esta imagen es en escala de grises (HU)
 * @param lungsRegion Imagen binaria de salida que contiene una mascara de los pulmones.
 * @param seeds Coordenadas X, Y, Z correspondientes a las dos semillas (una de cada pulmón).
 * @param min Valor de umbral mínimo desde el cual se hace el crecimiento de regiones.
 * @param max Valor de umbral máximo desde el cual se hace el crecimiento de regiones.
 */
void BoumaMethods::createLungsRegion (ImageType::Pointer imageIn, ImageBinaryType::Pointer &lungsRegion, float seeds[],int min, int max){

	ImageFilters* filters = new ImageFilters();
	ImageBinaryType::Pointer lung1 = ImageBinaryType::New();
	ImageBinaryType::Pointer lung2 = ImageBinaryType::New();
	
	float seedLung [3];
	
	//Crecimiento de regiones en un pulmon.
	seedLung[0] = seeds [0];
	seedLung[1] = seeds [1];
    seedLung[2] = seeds [2];
	filters->regionGrowing(imageIn,lung1,seedLung,min,max);
		
	//Crecimiento de regiones en el otro pulmon.
	seedLung[0] = seeds [3];
	seedLung[1] = seeds [4];
    seedLung[2] = seeds [5];
	filters->regionGrowing(imageIn,lung2,seedLung,min,max);

    //Se suman las dos imagenes para obtener la mascara de los dos pulmones.
    //Operacion or
	BinaryFilters* orFunc=new BinaryFilters();
    orFunc->orFunction(lung1,lung2,lungsRegion);
}


void BoumaMethods::createExclusionRegionLungs (ImageType::Pointer image, ImageFloatType::Pointer distanceMap , ImageBinaryType::Pointer &output, signed short threshold, double pixelLimit){

    //Copia de la imagen que sale del filtro de closing
    typedef itk::ImageDuplicator< ImageType > DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(image);
    duplicator->Update();

    typedef itk::CastImageFilter< ImageType, ImageBinaryType > CastingFilterType;
    CastingFilterType::Pointer caster = CastingFilterType::New();
    caster->SetInput( duplicator->GetOutput() );

    caster->Update();

    output=caster->GetOutput();

    //Para recorrer imagenes.
    ImageBinaryType::SizeType size = output->GetLargestPossibleRegion().GetSize();
    for (int i=0 ; i< size[0] ; i++)
    {
        for (int j=0; j< size[1]; j++)
        {
            for (int k=0; k<size [2]; k++)
            {
                ImageBinaryType::IndexType currentIndex;
                currentIndex[0] = i;
                currentIndex[1] = j;
                currentIndex[2] = k;

                ImageFloatType::IndexType currentIndexMap;
                currentIndexMap[0] = i;
                currentIndexMap[1] = j;
                currentIndexMap[2] = k;

                ImageType::IndexType currentIndexImage;
                currentIndexImage[0] = i;
                currentIndexImage[1] = j;
                currentIndexImage[2] = k;

                ImageFloatType::PixelType currentValueMap = distanceMap->GetPixel(currentIndexMap);
                ImageType::PixelType currentValueImage = image->GetPixel(currentIndexImage);

                if ( currentValueMap > 0 && currentValueMap < (float)pixelLimit && currentValueImage > threshold ) {

                    output->SetPixel(currentIndex,char(255));

                } else {
                    output->SetPixel(currentIndex,char(0));
                }
            }
        }
    }
    output->Update();
}
