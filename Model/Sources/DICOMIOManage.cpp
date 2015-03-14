#include "DICOMIOManage.h"

/**
 * @brief DICOMIOManage::DICOMIOManage Constructor por defecto
 */
DICOMIOManage::DICOMIOManage()
{
	this->nameOutputFiles = "";
	this->inputSize = 0;
	this->reader = ReaderType::New();
    this->readerBinary = ReaderBinaryType::New();
}

/**
 * @brief DICOMIOManage::~DICOMIOManage Destructor por defecto.
 */
DICOMIOManage::~DICOMIOManage()
{
}

/**
 * @brief Lee una imagen DICOM Binaria y la guarda en una variable para trabajar con ITK
 * @param inputDirectory Directorio donde están las imágenes de entrada.
 * @return Imagen para trabajar con ITK
 * Ejemplo en : http://www.itk.org/Doxygen/html/Examples_2IO_2DicomSeriesReadSeriesWrite_8cxx-example.html
 */
ReaderBinaryType::Pointer DICOMIOManage:: readInputImageBin(string inputDirectory){
	
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
	
	namesGenerator->SetInputDirectory( inputDirectory );
	const ReaderBinaryType::FileNamesContainer & filenames = namesGenerator->GetInputFileNames();
	
	//Guarda el numero de archivos que hay en el directorio de entrada.
	this->inputSize=(int)filenames.size();
	
	//Para imprimir el nombre de los archivos de DICOM
	/*
	const unsigned int numberOfFileNames =  filenames.size();
	cout << numberOfFileNames << endl;
	
	for(unsigned int fni = 0; fni < numberOfFileNames; ++fni)
    {
		cout << "filename # " << fni << " = ";
		cout << filenames[fni] << std::endl;
    }
	*/
	
    readerBinary->SetImageIO( gdcmIO );
    readerBinary->SetFileNames( filenames );
	
	try
    {
        readerBinary->Update();
    }
	catch (itk::ExceptionObject &excp)
    {
		std::cerr << "Exception thrown while writing the image" << std::endl;
		std::cerr << excp << std::endl;
		return NULL;
    }
    return readerBinary;
}


/**
 * @brief Lee una imagen DICOM y la guarda en una variable para trabajar con ITK
 * @param inputDirectory Directorio donde están las imágenes de entrada.
 * @return Imagen para trabajar con ITK
 * Ejemplo en : http://www.itk.org/Doxygen/html/Examples_2IO_2DicomSeriesReadSeriesWrite_8cxx-example.html
 */
ReaderType::Pointer DICOMIOManage:: readInputImage(string inputDirectory){
	
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
	NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
	
	namesGenerator->SetInputDirectory( inputDirectory );
	const ReaderType::FileNamesContainer & filenames = namesGenerator->GetInputFileNames();
	
	//Guarda el numero de archivos que hay en el directorio de entrada.
	this->inputSize=(int)filenames.size();
	
	//Para imprimir el nombre de los archivos de DICOM
	/*
	const unsigned int numberOfFileNames =  filenames.size();
	cout << numberOfFileNames << endl;
	
	for(unsigned int fni = 0; fni < numberOfFileNames; ++fni)
    {
		cout << "filename # " << fni << " = ";
		cout << filenames[fni] << std::endl;
    }
	*/
	
	reader->SetImageIO( gdcmIO );
	reader->SetFileNames( filenames );
	
	try
    {
		reader->Update();
    }
	catch (itk::ExceptionObject &excp)
    {
		std::cerr << "Exception thrown while writing the image" << std::endl;
		std::cerr << excp << std::endl;
		return NULL;
    }
	return reader;
}


/**
 * @brief Escribe las imagenes de salida
 * @param image : imagen que se guardara en formato DICOM
 * @param outputPath : Directorio donde se guardará la salida.
 * Ejemplo en : http://itk.org/Wiki/ITK/Examples/DICOM/ResampleDICOM
 */
void DICOMIOManage:: writeDicomFile (ImageType::Pointer image,string outputPath){

	string name="";
	
	if ((this->nameOutputFiles=="")) {
		name="Img";
	}
	else {
		name=this->nameOutputFiles;
	}
	
	typedef signed short    OutputPixelType;
	const unsigned int      OutputDimension = 2;
	
	typedef itk::Image< OutputPixelType, OutputDimension >    Image2DType;
	typedef itk::ImageSeriesWriter<ImageType, Image2DType >  SeriesWriterType;

	
	const char * outputDirectory = outputPath.c_str();	
	itksys::SystemTools::MakeDirectory( outputDirectory);
	
	// Generate the file names
	OutputNamesGeneratorType::Pointer outputNames = OutputNamesGeneratorType::New();
	string seriesFormat(outputDirectory);
	seriesFormat = seriesFormat + "/" + name+"%d.dcm";
	outputNames->SetSeriesFormat (seriesFormat.c_str());
	outputNames->SetStartIndex (1);
	outputNames->SetEndIndex (this->GetInputSize());
 
	SeriesWriterType::Pointer seriesWriter = SeriesWriterType::New();
	//seriesWriter->SetInput( image->GetOutput() );
	seriesWriter->SetInput(image);
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
    seriesWriter->SetImageIO( gdcmIO );
    seriesWriter->SetFileNames( outputNames->GetFileNames() );
    seriesWriter->SetMetaDataDictionaryArray( reader->GetMetaDataDictionaryArray() );
  try
    {
		seriesWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
		std::cerr << "Exception thrown while writing the series " << std::endl;
		std::cerr << excp << std::endl;
		return;
    }
}

/**
 * @brief Escribe las imagenes de salida
 * @param image : imagen que se guardara en formato DICOM
 * @param outputPath : Directorio donde se guardará la salida.
 * Ejemplo en : http://itk.org/Wiki/ITK/Examples/DICOM/ResampleDICOM
 */ 
void DICOMIOManage:: writeDicomFile (ImageBinaryType::Pointer image,string outputPath){

	string name="";
	
	if ((this->nameOutputFiles=="")) {
		name="Img";
	}
	else {
		name=this->nameOutputFiles;
	}
	
	typedef signed short    OutputPixelType;
	const unsigned int      OutputDimension = 2;
	
	typedef itk::Image< OutputPixelType, OutputDimension >    Image2DType;
	typedef itk::ImageSeriesWriter<ImageBinaryType, Image2DType >  SeriesWriterType;

	const char * outputDirectory = outputPath.c_str();	
	itksys::SystemTools::MakeDirectory( outputDirectory);
 
	// Generate the file names
    OutputNamesGeneratorType::Pointer outputNames = OutputNamesGeneratorType::New();
	string seriesFormat(outputDirectory);
	seriesFormat = seriesFormat + "/" + name+"%d.dcm";
	outputNames->SetSeriesFormat (seriesFormat.c_str());
	outputNames->SetStartIndex (1);
	outputNames->SetEndIndex (this->GetInputSize());
 
	SeriesWriterType::Pointer seriesWriter = SeriesWriterType::New();
	//seriesWriter->SetInput( image->GetOutput() );
	seriesWriter->SetInput(image);
	ImageIOType::Pointer gdcmIO = ImageIOType::New();
    seriesWriter->SetImageIO( gdcmIO );
    seriesWriter->SetFileNames( outputNames->GetFileNames() );
    seriesWriter->SetMetaDataDictionaryArray( reader->GetMetaDataDictionaryArray() );
  try
    {		
		seriesWriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
		std::cerr << "Exception thrown while writing the series " << std::endl;
		std::cerr << excp << std::endl;
		return;
    }
}

/**
 * @brief DICOMIOManage::getVtkImageReader Se obtiene una imagen reader de tipo vtk.
 * @param path Ruta donde se encuentra el directorio con las imágenes DICOM.
 * @return Imagen reader VTK.
 */
vtkSmartPointer< vtkDICOMImageReader > DICOMIOManage:: getVtkImageReader(string path){
    vtkSmartPointer< vtkDICOMImageReader > reader = vtkSmartPointer< vtkDICOMImageReader >::New();
    reader->SetDirectoryName(path.c_str());
    reader->Update();
    return reader;
}

/**
 * @brief DICOMIOManage::getItkImage
 * @param path Ruta donde está el directorio con las imágenes de tipo DICOM.
 * @return Apuntador a reader de imagen DICOM.
 */
ReaderType::Pointer DICOMIOManage::getItkImage(string path){

    ImageIOType::Pointer gdcmIO = ImageIOType::New();
    NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();

    namesGenerator->SetInputDirectory(path.c_str());
    const ReaderType::FileNamesContainer & filenames = namesGenerator->GetInputFileNames();

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetImageIO( gdcmIO );
    reader->SetFileNames( filenames );

    try
    {
        reader->Update();
        return reader;
    }
    catch (itk::ExceptionObject &excp)
    {
        std::cerr << "Exception thrown while writing the image" << std::endl;
        std::cerr << excp << std::endl;
        return 0;
    }

    return 0;
}

/**
 * @brief DICOMIOManage::castImageItkToVtk Convierte una imagen ITK en una imagen VTK
 * @param imageIn Apuntador a la imagen de tipo ITK, es la imagen de entrada.
 * @param imageOut Apuntador a la imagen de tipo VTK, es la imagen de salida.
 * Ejemplo: http://itk.org/Wiki/ITK/Examples/IO/ImageToVTKImageFilter
 */
void DICOMIOManage::castImageItkToVtk (ImageType::Pointer imageIn, vtkImageData* &imageOut){
    typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
    ConnectorType::Pointer connector= ConnectorType::New();
    connector->SetInput( imageIn );

    try
    {
        connector->Update();
    }
    catch (itk::ExceptionObject & e)
    {
        std::cerr << "exception in file reader " << std::endl;
        std::cerr << e << std::endl;
        return;
    }

    imageOut->DeepCopy(connector->GetOutput());
    //vtkImageData * image = vtkImageData::New();
    //image->DeepCopy(connector->GetOutput());
}


/**
 * @brief Define el nombre que tendran los archivos de salida. 
 * @param nameOutputFiles : Nombre que se le dará a los archivos de salida.
 */
void DICOMIOManage:: SetNameOutputFiles(const string& nameOutputFiles)
{
	this->nameOutputFiles = nameOutputFiles;
}
 
/**
 * @brief Retorna el nombre que tienen los archivos de salida
 * @return Nombre de los archivos de salida
 */   
const string& DICOMIOManage::GetNameOutputFiles() const
{
	return nameOutputFiles;
}
/**
 * @brief Define la cantidad de imagenes que habrá a la salida.
 * @param inputSize : Cantidad de imagenes
 */
 void DICOMIOManage::SetInputSize(int inputSize)
{
	this->inputSize = inputSize;
}
/**
 * @brief Retorna la cantidad de imágenes que debe haber a la salida.
 * @return Cantidad de imágenes a la salida.
 */
int DICOMIOManage::GetInputSize() const
{
	return inputSize;
}
