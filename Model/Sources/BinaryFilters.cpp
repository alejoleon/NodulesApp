#include "BinaryFilters.h"
/**
 * @brief Constructor por defecto.
 * @return objeto de la clase BinaryFilters.
 */
BinaryFilters::BinaryFilters()
{
}
/**
 * @brief Destructor por defecto.
 * @return 
 */
BinaryFilters::~BinaryFilters()
{
}

/**
 * @brief Realiza la operacion Binaria OR de dos imágenes binarias
 * @param input1 Imagen Binaria
 * @param input2 Imagen Binaria
 * @param output : Resultado de la función OR de las dos imágenes Binarias (Pasado por parámetro)
 * Ejemplo de : http://itk.org/Wiki/ITK/Examples/ImageProcessing/OrImageFilter
 */
void BinaryFilters::orFunction(ImageBinaryType::Pointer input1,ImageBinaryType::Pointer input2,ImageBinaryType::Pointer &output){
	
	typedef itk::OrImageFilter <ImageBinaryType> OrImageFilterType;
	OrImageFilterType::Pointer orFilter = OrImageFilterType::New();
	
	orFilter->SetInput(0, input1);
	orFilter->SetInput(1, input2);
	orFilter->Update();
	
	output=orFilter->GetOutput();

}
/**
 * @brief Filtro que hace una operacion morfológica de closing en una imagen, con un elemento estructurador de bola (Ball).
 * @param imageIn Imagen de entrada a la que se le hará la operacion Closing
 * @param imageOut Variable donde se guardará la imagen de salida despues de haberle realizado un closing.
 * @param radius Tamaño del radio con el que se hará el elemento estructurador (Ball)
 * Tomado de : http://www.itk.org/Doxygen45/html/Segmentation_2ConnectedThresholdImageFilter_8cxx-example.html
 */
void BinaryFilters::closingFilter(ImageBinaryType::Pointer imageIn,ImageBinaryType::Pointer &imageOut,double radius){
	
	typedef itk::BinaryBallStructuringElement<ImageBinaryType::PixelType, ImageBinaryType::ImageDimension> StructuringElementType;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(radius);
	structuringElement.CreateStructuringElement();
  
	typedef itk::BinaryMorphologicalClosingImageFilter <ImageBinaryType, ImageBinaryType, StructuringElementType> BinaryMorphologicalClosingImageFilterType;
	BinaryMorphologicalClosingImageFilterType::Pointer closingFilter = BinaryMorphologicalClosingImageFilterType::New();
	closingFilter->SetInput(imageIn);
	closingFilter->SetKernel(structuringElement);
	closingFilter->Update();
	imageOut=closingFilter->GetOutput();
}

/**
 * @brief BinaryFilters::openingFilter Filtro que hace una operacion morfológica de opening en una imagen, con un elemento estructurador de bola (Ball).
 * @param imageIn Imagen de entrada a la que se le hará la operacion opening.
 * @param imageOut Variable donde se guardará la imagen de salida despues de haberle realizado un closing.
 * @param radius Tamaño del radio con el que se hará el elemento estructurador (Ball)
 * Tomado de : http://www.itk.org/Doxygen/html/WikiExamples_2Morphology_2BinaryMorphologicalOpeningImageFilter_8cxx-example.html#_a4
 */
void BinaryFilters::openingFilter(ImageBinaryType::Pointer imageIn,ImageBinaryType::Pointer &imageOut,double radius){

    typedef itk::BinaryBallStructuringElement<ImageBinaryType::PixelType, ImageBinaryType::ImageDimension> StructuringElementType;
    StructuringElementType structuringElement;
    structuringElement.SetRadius(radius);
    structuringElement.CreateStructuringElement();

    typedef itk::BinaryMorphologicalOpeningImageFilter <ImageBinaryType, ImageBinaryType, StructuringElementType> BinaryMorphologicalOpeningImageFilterType;
    BinaryMorphologicalOpeningImageFilterType::Pointer openingFilter = BinaryMorphologicalOpeningImageFilterType::New();
    openingFilter->SetInput(imageIn);
    openingFilter->SetKernel(structuringElement);
    openingFilter->Update();
    imageOut=openingFilter->GetOutput();
}


//http://www.itk.org/Wiki/ITK/Examples/ImageProcessing/BinaryNotImageFilter
void BinaryFilters:: notImage(ImageBinaryType::Pointer imageIn,ImageBinaryType::Pointer &imageOut){
	typedef itk::BinaryNotImageFilter <ImageBinaryType> BinaryNotImageFilterType;
 
  BinaryNotImageFilterType::Pointer binaryNotFilter = BinaryNotImageFilterType::New();
  binaryNotFilter->SetInput(imageIn);
  binaryNotFilter->Update();
  
  imageOut = binaryNotFilter->GetOutput();

}
