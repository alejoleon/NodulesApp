#include "ImageProcessingUtils.h"
/**
 * @brief Constructor por defecto.
 * @return Objeto de la clase ImageProcessUtils
 */
ImageProcessingUtils::ImageProcessingUtils()
{
}
/**
 * @brief Destructor por defecto.
 * @return 
 */
ImageProcessingUtils::~ImageProcessingUtils()
{
}

/**
 * @brief Calcula el valor máximo de una imagen.
 * @param image : imagen a la cual se le desea obtener el valor máximo.
 * @return valor máximo de la imagen.
 * tomado de : http://www.itk.org/Wiki/ITK/Examples/ImageProcessing/MinimumMaximumImageCalculator
 */
ImageType::PixelType ImageProcessingUtils:: getMaximumValue (ImageType::Pointer image){
	
	typedef itk::MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter = ImageCalculatorFilterType::New ();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	
	ImageType::IndexType maximumLocation = imageCalculatorFilter->GetIndexOfMaximum();
	ImageType::PixelType maxValue = image->GetPixel(maximumLocation);
	return maxValue;
}

/**
 * @brief Calcula el valor mínimo de una imagen.
 * @param image : imagen a la cual se le desea obtener el valor mínimo.
 * @return valor mínimo de la imagen.
 * tomado de : http://www.itk.org/Wiki/ITK/Examples/ImageProcessing/MinimumMaximumImageCalculator
 */
ImageType::PixelType ImageProcessingUtils:: getMinimumValue (ImageType::Pointer image){
	
	typedef itk::MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter = ImageCalculatorFilterType::New ();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	
	ImageType::IndexType minimumLocation = imageCalculatorFilter->GetIndexOfMinimum();
	ImageType::PixelType minValue = image->GetPixel(minimumLocation);
	return minValue;
}

/**
 * @brief Calcula el valor máximo de una imagen con una mascara.
 * @param image : imagen a la cual se le desea obtener el valor máximo.
 * @param mask : Mascara que se va a usar para verificar la region de interés.
 * @return valor máximo de la imagen.
 */
ImageType::PixelType ImageProcessingUtils:: getMaximumValue (ImageType::Pointer image,ImageBinaryType::Pointer mask) {
		
	ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();	
	signed short max=0;
	for (int i=0 ; i< size[0] ; i++)
	{
		for (int j=0; j< size[1]; j++)
		{
			for (int k=0; k<size[2]; k++)
			{
				ImageType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k; 
				ImageType::PixelType currentValueOut = image->GetPixel(currentIndex);
				
				ImageBinaryType::IndexType currentIndexMask;
				currentIndexMask[0] = i;
				currentIndexMask[1] = j;
				currentIndexMask[2] = k;
				ImageBinaryType::PixelType currentValueMask = mask->GetPixel(currentIndexMask);
				
				if((int)currentValueMask==255){
					if (currentValueOut>max){
					max=currentValueOut;
					}
				}
			}	
		}
	}
	return max;
}

/**
 * @brief Calcula el valor mínimo de una imagen con una mascara.
 * @param image : imagen a la cual se le desea obtener el valor mínimo.
 * @param mask : Mascara que se va a usar para verificar la region de interés.
 * @return valor mínimo de la imagen.
 */
ImageType::PixelType ImageProcessingUtils:: getMinimumValue (ImageType::Pointer image,ImageBinaryType::Pointer mask) {
		
	ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();	
	signed short min=10000;
	for (int i=0 ; i< size[0] ; i++)
	{
		for (int j=0; j< size[1]; j++)
		{
			for (int k=0; k<size[2]; k++)
			{
				ImageType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k; 
				ImageType::PixelType currentValueOut = image->GetPixel(currentIndex);
				
				ImageBinaryType::IndexType currentIndexMask;
				currentIndexMask[0] = i;
				currentIndexMask[1] = j;
				currentIndexMask[2] = k;
				ImageBinaryType::PixelType currentValueMask = mask->GetPixel(currentIndexMask);
				
				if((int)currentValueMask==255){
					if (currentValueOut<min){
					min=currentValueOut;
					}	
				}
			}	
		}
	}
	return min;
}

/**
 * @brief Calcula el histograma de la imagen pasada por parámetro. El número de elementos del histograma corresponde a la cantidad de frecuencias que hay en el intervalo determinado por la intensidad mayor y menor de la imagen.
 * @param image : Imagen a la que se le va a calcular el histograma.
 * @param lower : Menor valor de intensidad encontrado en la imagen que será el mínimo valor en el histograma.
 * @param upper : Mayor valor de intensidad encontrado en la imagen que será el máximo valor en el histograma.
 * @return Objeto que contiene el histograma calculado.
 */
ImageToHistogramFilterType::Pointer ImageProcessingUtils::histogram (ImageType::Pointer image ,int lower, int upper) {
	
	upper++;
	int hist[upper-lower];
	
	for (int i=0; i< upper-lower;i++) {
		hist [i]=0;
	}
	
	ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();	
	for (int i=0 ; i< size[0] ; i++)
	{
		for (int j=0; j< size[1]; j++)
		{
			for (int k=0; k<size[2]; k++)
			{
				ImageType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k; 
				ImageType::PixelType currentValueOut = image->GetPixel(currentIndex);
				
				hist[currentValueOut-lower]++;
			}	
		}
	}
	
	int bins=upper-lower;
	
	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = static_cast< unsigned int >(bins);
	typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;
	ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
	lowerBound.Fill(lower);
	ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
	upperBound.Fill(upper) ;
	ImageToHistogramFilterType::HistogramType::SizeType sizeH(MeasurementVectorSize);
	sizeH.Fill(binsPerDimension);
	
	ImageToHistogramFilterType::Pointer imageToHistogramFilter = ImageToHistogramFilterType::New();
	imageToHistogramFilter->SetInput( image );
	imageToHistogramFilter->SetHistogramBinMinimum( lowerBound );
	imageToHistogramFilter->SetHistogramBinMaximum( upperBound );
	imageToHistogramFilter->SetHistogramSize( sizeH );
	
	try{
		imageToHistogramFilter->Update();
    }
	catch( itk::ExceptionObject & error ){
		std::cerr << "Error: " << error << std::endl;
		return 0;
    }
	
	for(int i = 0; i < imageToHistogramFilter->GetOutput()->GetSize()[0]; ++i)
    {
		imageToHistogramFilter->GetOutput()->SetFrequency(i,hist[i]);
    }
	
	/*
	int cont=0;
	for (int i=0; i<upper-lower;i++) {
		std::cout<<i+lower<< " : "<<hist[i]<<std::endl;
		cont=cont+ hist[i];
	}
	
	std::cout<<"TOTAL : "<<cont<<std::endl;
	*/
	return imageToHistogramFilter;
}
/**
 * @brief Calcula el histograma de la imagen pasada por parámetro. El número de elementos del histograma corresponde a la cantidad de frecuencias que hay en el intervalo determinado por la intensidad mayor y menor de la imagen.
 * @param image : Imagen a la que se le va a calcular el histograma.
 * @return Objeto que contiene el histograma calculado.
 */
ImageToHistogramFilterType::Pointer ImageProcessingUtils::histogram (ImageType::Pointer image) {
	
	ImageType::PixelType upper = getMaximumValue(image);
	ImageType::PixelType lower = getMinimumValue(image);
	upper=upper+1;
	int hist[upper-lower];
	
	for (int i=0; i< upper-lower;i++) {
		hist [i]=0;
	}
	
	ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();	
	for (int i=0 ; i< size[0] ; i++)
	{
		for (int j=0; j< size[1]; j++)
		{
			for (int k=0; k<size[2]; k++)
			{
				ImageType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k; 
				ImageType::PixelType currentValueOut = image->GetPixel(currentIndex);
				
				hist[currentValueOut-lower]++;
			}	
		}
	}
	
	int bins=upper-lower;
	
	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = static_cast< unsigned int >(bins);
	typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;
	ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
	lowerBound.Fill(lower);
	ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
	upperBound.Fill(upper) ;
	ImageToHistogramFilterType::HistogramType::SizeType sizeH(MeasurementVectorSize);
	sizeH.Fill(binsPerDimension);
	
	ImageToHistogramFilterType::Pointer imageToHistogramFilter = ImageToHistogramFilterType::New();
	imageToHistogramFilter->SetInput( image );
	imageToHistogramFilter->SetHistogramBinMinimum( lowerBound );
	imageToHistogramFilter->SetHistogramBinMaximum( upperBound );
	imageToHistogramFilter->SetHistogramSize( sizeH );
	
	try{
		imageToHistogramFilter->Update();
    }
	catch( itk::ExceptionObject & error ){
		std::cerr << "Error: " << error << std::endl;
		return 0;
    }
	
	for(int i = 0; i < imageToHistogramFilter->GetOutput()->GetSize()[0]; ++i)
    {
		imageToHistogramFilter->GetOutput()->SetFrequency(i,hist[i]);
    }
	
	/*
	int cont=0;
	for (int i=0; i<upper-lower;i++) {
		std::cout<<i<< " : "<<hist[i]<<std::endl;
		cont=cont+ hist[i];
	}
	
	std::cout<<"TOTAL : "<<cont<<std::endl;
	*/
	return imageToHistogramFilter;
}

/**
 * @brief Calcula el histograma de la imagen pasada por parámetro. El número de elementos del histograma corresponde a la cantidad de frecuencias que hay en el intervalo determinado por la intensidad mayor y menor de la imagen.
 * @param image : Imagen a la que se le va a calcular el histograma.
 * @param bins : Cantidad de elementos o de compartimientos que tendrá el histograma.
 * @param lower : Menor valor de intensidad encontrado en la imagen que será el mínimo valor en el histograma.
 * @param upper : Mayor valor de intensidad encontrado en la imagen que será el máximo valor en el histograma.
 * @return Objeto que contiene el histograma calculado.
 * Basado en : http://www.itk.org/Doxygen/html/WikiExamples_2Statistics_2HistogramToImageFilterGrayscale_8cxx-example.html#_a1
 */
ImageToHistogramFilterType::Pointer ImageProcessingUtils:: histogramFilter (ImageType::Pointer image , int bins ,int lower, int upper) {

	bins++;
	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = static_cast< unsigned int >(bins);
	typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;
	ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
	lowerBound.Fill(lower);
	ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
	upperBound.Fill(upper) ;
	ImageToHistogramFilterType::HistogramType::SizeType size(MeasurementVectorSize);
	size.Fill(binsPerDimension);
	
	ImageToHistogramFilterType::Pointer imageToHistogramFilter = ImageToHistogramFilterType::New();
	imageToHistogramFilter->SetInput( image );
	imageToHistogramFilter->SetHistogramBinMinimum( lowerBound );
	imageToHistogramFilter->SetHistogramBinMaximum( upperBound );
	imageToHistogramFilter->SetHistogramSize( size );
	
	try{
		imageToHistogramFilter->Update();
    }
	catch( itk::ExceptionObject & error ){
		std::cerr << "Error: " << error << std::endl;
		return 0;
    }
	
	/*
	ImageToHistogramFilterType::HistogramType* histogram = imageToHistogramFilter->GetOutput();
	std::cout << "Frequency = [ ";
	for(unsigned int i = 0; i < histogram->GetSize()[0]; ++i)
    {
		std::cout << i << "="<<histogram->GetFrequency(i) ;
		if( i != histogram->GetSize()[0] - 1 )
		{
			std::cout << std::endl ;
		}
    }
  std::cout << " ]" << std::endl;
  */
  return imageToHistogramFilter;
}
/**
 * @brief Calcula el histograma de la imagen pasada por parámetro. El número de elementos del histograma corresponde a la cantidad de frecuencias que hay en el intervalo determinado por la intensidad mayor y menor de la imagen.
 * @param image : Imagen a la que se le va a calcular el histograma.
 * @return Objeto que contiene el histograma calculado.
 * Basado en : http://www.itk.org/Doxygen/html/WikiExamples_2Statistics_2HistogramToImageFilterGrayscale_8cxx-example.html#_a1
 */
ImageToHistogramFilterType::Pointer ImageProcessingUtils:: histogramFilter (ImageType::Pointer image) {
	
	ImageType::PixelType upper = getMaximumValue(image);
	ImageType::PixelType lower = getMinimumValue(image);
	int bins = upper-lower+1;
	
	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = static_cast< unsigned int >(bins);
	typedef itk::Statistics::ImageToHistogramFilter< ImageType > ImageToHistogramFilterType;
	ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
	lowerBound.Fill(lower);
	ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
	upperBound.Fill(upper) ;
	ImageToHistogramFilterType::HistogramType::SizeType size(MeasurementVectorSize);
	size.Fill(binsPerDimension);
	
	ImageToHistogramFilterType::Pointer imageToHistogramFilter = ImageToHistogramFilterType::New();
	imageToHistogramFilter->SetInput( image );
	imageToHistogramFilter->SetHistogramBinMinimum( lowerBound );
	imageToHistogramFilter->SetHistogramBinMaximum( upperBound );
	imageToHistogramFilter->SetHistogramSize( size );
	
	try{
		imageToHistogramFilter->Update();
    }
	catch( itk::ExceptionObject & error ){
		std::cerr << "Error: " << error << std::endl;
		return 0;
    }
	
	/*
	ImageToHistogramFilterType::HistogramType* histogram = imageToHistogramFilter->GetOutput();
	std::cout << "Frequency = [ ";
	for(unsigned int i = 0; i < histogram->GetSize()[0]; ++i)
    {
		std::cout << i << "="<<histogram->GetFrequency(i) ;
		if( i != histogram->GetSize()[0] - 1 )
		{
			std::cout << std::endl ;
		}
    }
  std::cout << " ]" << std::endl;
  */
  return imageToHistogramFilter;
}

/**
 * @brief Calcula el histograma de la imagen pasada por parámetro. El número de elementos del histograma corresponde a la cantidad de frecuencias que hay en el intervalo determinado por la intensidad mayor y menor de la imagen.
 * @param image : Imagen a la que se le va a calcular el histograma.
 * @param mask : Imagen que contiene la máscara con la región de interés.
 * @param lower : Menor valor de intensidad encontrado en la imagen que será el mínimo valor en el histograma.
 * @param upper : Mayor valor de intensidad encontrado en la imagen que será el máximo valor en el histograma.
 * @return Objeto que contiene el histograma calculado.
 */
MaskedImageToHistogramFilterType::Pointer ImageProcessingUtils::histogram (ImageType::Pointer image,ImageBinaryType::Pointer mask ,int lower, int upper) {
		
	upper++;
	int hist[upper-lower];
	
	for (int i=0; i< upper-lower;i++) {
		hist [i]=0;
	}
	
	int pixels=0;
	
	ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();	
	for (int i=0 ; i< size[0] ; i++)
	{
		for (int j=0; j< size[1]; j++)
		{
			for (int k=0; k<size[2]; k++)
			{
				ImageType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k; 
				ImageType::PixelType currentValueOut = image->GetPixel(currentIndex);
								
				ImageBinaryType::IndexType currentIndexMask;
				currentIndexMask[0] = i;
				currentIndexMask[1] = j;
				currentIndexMask[2] = k;
				ImageBinaryType::PixelType currentValueMask = mask->GetPixel(currentIndexMask);
				
				if((int)currentValueMask==255){
						hist[currentValueOut-lower]++;
						pixels++;
				}
			}	
		}
	}
	
	int bins=upper-lower;
	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = static_cast< unsigned int >(bins);
	typedef itk::Statistics::MaskedImageToHistogramFilter< ImageType, ImageBinaryType > MaskedImageToHistogramFilterType;
	MaskedImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
	lowerBound.Fill(lower);
	MaskedImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
	upperBound.Fill(upper) ;
	MaskedImageToHistogramFilterType::HistogramType::SizeType sizeH(MeasurementVectorSize);
	sizeH.Fill(binsPerDimension);
	
	MaskedImageToHistogramFilterType::Pointer maskedImageToHistogramFilter = MaskedImageToHistogramFilterType::New();
	maskedImageToHistogramFilter->SetInput( image );
	maskedImageToHistogramFilter->SetHistogramBinMinimum( lowerBound );
	maskedImageToHistogramFilter->SetHistogramBinMaximum( upperBound );
	maskedImageToHistogramFilter->SetHistogramSize( sizeH );
	maskedImageToHistogramFilter->SetMaskImage(mask);
	
	try{
		maskedImageToHistogramFilter->Update();
    }
	catch( itk::ExceptionObject & error ){
		std::cerr << "Error: " << error << std::endl;
		return 0;
    }
	
	for(int i = 0; i < maskedImageToHistogramFilter->GetOutput()->GetSize()[0]; ++i)
    {
		maskedImageToHistogramFilter->GetOutput()->SetFrequency(i,hist[i]);
    }
	
	/*
	int cont=0;
	for (int i=0 ; i< upper-lower ; i++) {
		std::cout<<i+lower<<" * "<<hist[i]<<std::endl;
		cont=cont+ hist[i];
	}
	std::cout<<"Pixeles : "<<cont<<std::endl;
	std::cout<<"TOTAL : "<<cont<<std::endl;
	*/
  
  return maskedImageToHistogramFilter;
	
	
	

}
/**
 * @brief Calcula el histograma de la imagen pasada por parámetro. El número de elementos del histograma corresponde a la cantidad de frecuencias que hay en el intervalo determinado por la intensidad mayor y menor de la imagen.
 * @param image : Imagen a la que se le va a calcular el histograma.
 * @param mask : Imagen que contiene la máscara con la región de interés.
 * @return Objeto que contiene el histograma calculado.
 */
MaskedImageToHistogramFilterType::Pointer ImageProcessingUtils::histogram (ImageType::Pointer image,ImageBinaryType::Pointer mask) {
	ImageType::PixelType upper = getMaximumValue(image,mask);
	ImageType::PixelType lower = getMinimumValue(image,mask);
	upper=upper+1;
	
	int hist[upper-lower];
	
	for (int i=0; i< upper-lower;i++) {
		hist [i]=0;
	}
	
	ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();	
	for (int i=0 ; i< size[0] ; i++)
	{
		for (int j=0; j< size[1]; j++)
		{
			for (int k=0; k<size[2]; k++)
			{
				ImageType::IndexType currentIndex;
				currentIndex[0] = i;
				currentIndex[1] = j;
				currentIndex[2] = k; 
				ImageType::PixelType currentValueOut = image->GetPixel(currentIndex);
								
				ImageBinaryType::IndexType currentIndexMask;
				currentIndexMask[0] = i;
				currentIndexMask[1] = j;
				currentIndexMask[2] = k;
				ImageBinaryType::PixelType currentValueMask = mask->GetPixel(currentIndexMask);
				
				if((int)currentValueMask==255){
						hist[currentValueOut-lower]++;
				}
			}	
		}
	}
	
		int bins=upper-lower;
	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = static_cast< unsigned int >(bins);
	typedef itk::Statistics::MaskedImageToHistogramFilter< ImageType, ImageBinaryType > MaskedImageToHistogramFilterType;
	MaskedImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
	lowerBound.Fill(lower);
	MaskedImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
	upperBound.Fill(upper) ;
	MaskedImageToHistogramFilterType::HistogramType::SizeType sizeH(MeasurementVectorSize);
	sizeH.Fill(binsPerDimension);
	
	MaskedImageToHistogramFilterType::Pointer maskedImageToHistogramFilter = MaskedImageToHistogramFilterType::New();
	maskedImageToHistogramFilter->SetInput( image );
	maskedImageToHistogramFilter->SetHistogramBinMinimum( lowerBound );
	maskedImageToHistogramFilter->SetHistogramBinMaximum( upperBound );
	maskedImageToHistogramFilter->SetHistogramSize( sizeH );
	maskedImageToHistogramFilter->SetMaskImage(mask);
	
	try{
		maskedImageToHistogramFilter->Update();
    }
	catch( itk::ExceptionObject & error ){
		std::cerr << "Error: " << error << std::endl;
		return 0;
    }
	
	for(int i = 0; i < maskedImageToHistogramFilter->GetOutput()->GetSize()[0]; ++i)
    {
		maskedImageToHistogramFilter->GetOutput()->SetFrequency(i,hist[i]);
    }
	
	/*
	int cont=0;
	for (int i=0 ; i< upper-lower ; i++) {
		std::cout<<i<<" * "<<hist[i]<<std::endl;
		cont=cont+ hist[i];
	}
	std::cout<<"TOTAL : "<<cont<<std::endl;
	*/
	 return maskedImageToHistogramFilter;
}

/**
 * @brief Calcula el histograma de la imagen pasada por parámetro. El número de elementos del histograma corresponde a la cantidad de frecuencias que hay en el intervalo determinado por la intensidad mayor y menor de la imagen.
 * @param image : Imagen a la que se le va a calcular el histograma.
 * @param mask : Imagen que contiene la máscara con la región de interés.
 * @param bins : Cantidad de elementos o de compartimientos que tendrá el histograma.
 * @param lower : Menor valor de intensidad encontrado en la imagen que será el mínimo valor en el histograma.
 * @param upper : Mayor valor de intensidad encontrado en la imagen que será el máximo valor en el histograma.
 * @return Objeto que contiene el histograma calculado.
 * Basado en : http://itk.org/Wiki/ITK/Examples/WishList/Statistics/MaskedImageToHistogramFilter
 */
MaskedImageToHistogramFilterType::Pointer ImageProcessingUtils::histogramFilter (ImageType::Pointer image ,ImageBinaryType::Pointer mask, int bins ,int lower, int upper) {
	
	bins++;
	
	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = static_cast< unsigned int >(bins);
	typedef itk::Statistics::MaskedImageToHistogramFilter< ImageType, ImageBinaryType > MaskedImageToHistogramFilterType;
	MaskedImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
	lowerBound.Fill(lower);
	MaskedImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
	upperBound.Fill(upper) ;
	MaskedImageToHistogramFilterType::HistogramType::SizeType size(MeasurementVectorSize);
	size.Fill(binsPerDimension);
	
	MaskedImageToHistogramFilterType::Pointer maskedImageToHistogramFilter = MaskedImageToHistogramFilterType::New();
	maskedImageToHistogramFilter->SetInput( image );
	maskedImageToHistogramFilter->SetHistogramBinMinimum( lowerBound );
	maskedImageToHistogramFilter->SetHistogramBinMaximum( upperBound );
	maskedImageToHistogramFilter->SetHistogramSize( size );
	maskedImageToHistogramFilter->SetMaskImage(mask);
	
	try{
		maskedImageToHistogramFilter->Update();
    }
	catch( itk::ExceptionObject & error ){
		std::cerr << "Error: " << error << std::endl;
		return 0;
    }
	
	/*
	ImageToHistogramFilterType::HistogramType* histogram = maskedImageToHistogramFilter->GetOutput();
	std::cout << "Frequency = [ ";
	for(unsigned int i = 0; i < histogram->GetSize()[0]; ++i)
    {
		std::cout << i << "="<<histogram->GetFrequency(i) ;
		if( i != histogram->GetSize()[0] - 1 )
		{
			std::cout << std::endl ;
		}
    }
  std::cout << " ]" << std::endl;*/
  
  return maskedImageToHistogramFilter;
}


/**
 * @brief Calcula el histograma de la imagen pasada por parámetro. El número de elementos del histograma corresponde a la cantidad de frecuencias que hay en el intervalo determinado por la intensidad mayor y menor de la imagen.
 * @param image : Imagen a la que se le va a calcular el histograma.
 * @param mask : Imagen que contiene la máscara con la región de interés.
 * @return Objeto que contiene el histograma calculado.
 * Basado en : http://itk.org/Wiki/ITK/Examples/WishList/Statistics/MaskedImageToHistogramFilter
 */
MaskedImageToHistogramFilterType::Pointer ImageProcessingUtils::histogramFilter (ImageType::Pointer image ,ImageBinaryType::Pointer mask) {

	ImageType::PixelType upper = getMaximumValue(image,mask);
	ImageType::PixelType lower = getMinimumValue(image,mask);
	int bins = upper-lower+1;
	
	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = static_cast< unsigned int >(bins);
	
	typedef itk::Statistics::MaskedImageToHistogramFilter< ImageType, ImageBinaryType > MaskedImageToHistogramFilterType;
	MaskedImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
	lowerBound.Fill(lower);
	MaskedImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
	upperBound.Fill(upper) ;
	MaskedImageToHistogramFilterType::HistogramType::SizeType size(MeasurementVectorSize);
	size.Fill(binsPerDimension);
	
	MaskedImageToHistogramFilterType::Pointer maskedImageToHistogramFilter = MaskedImageToHistogramFilterType::New();
	maskedImageToHistogramFilter->SetInput( image );
	maskedImageToHistogramFilter->SetHistogramBinMinimum( lowerBound );
	maskedImageToHistogramFilter->SetHistogramBinMaximum( upperBound );
	maskedImageToHistogramFilter->SetHistogramSize( size );
	maskedImageToHistogramFilter->SetMaskImage(mask);
	
	try{
		maskedImageToHistogramFilter->Update();
    }
	catch( itk::ExceptionObject & error ){
		std::cerr << "Error: " << error << std::endl;
		return 0;
    }
	
	/*
	ImageToHistogramFilterType::HistogramType* histogram = maskedImageToHistogramFilter->GetOutput();
	std::cout << "Frequency = [ ";
	for(unsigned int i = 0; i < histogram->GetSize()[0]; ++i)
    {
		std::cout << i << "="<<histogram->GetFrequency(i) ;
		if( i != histogram->GetSize()[0] - 1 )
		{
			std::cout << std::endl ;
		}
    }
  std::cout << " ]" << std::endl;*/
  
  return maskedImageToHistogramFilter;
}

vector<int> ImageProcessingUtils::histogramFilterToVector(ImageToHistogramFilterType::Pointer image){
    vector<int> hist;

    ImageToHistogramFilterType::HistogramType* histogram = image->GetOutput();
    for(unsigned int i = 0; i < histogram->GetSize()[0]; ++i)
    {
        hist.push_back(histogram->GetFrequency(i));
    }
    return hist;
}

vector<int> ImageProcessingUtils::histogramFilterToVector(MaskedImageToHistogramFilterType::Pointer image){
    vector<int> hist;

    MaskedImageToHistogramFilterType::HistogramType* histogram = image->GetOutput();
    for(unsigned int i = 0; i < histogram->GetSize()[0]; ++i)
    {
        hist.push_back(histogram->GetFrequency(i));
    }
    return hist;
}

