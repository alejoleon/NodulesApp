#include "DICOMProperties.h"


/**
 * @brief Contructor con el parametro de la dirección o path donde se encuentra la imagen DICOM de entrada.
 * @param path dirección o path donde se encuentra la imagen DICOM de entrada.
 * @return Objeto de la clase DICOMProperties
 */

DICOMProperties::DICOMProperties(string path)
{
	this->path=path;
    this->loadValues();

    this->pixelValue = atof(this->getValue("0028|0030").c_str());
    this->patientName = this->getValue("0010|0010");
    this->patientID = this->getValue("0010|0020");
    this->patientBirth = this->getValue("0010|0030");
    this->patientSex = this->getValue("0010|0040");

}


/**
 * @brief DICOMProperties::~DICOMProperties Destructor de la case PropiedadesDICOM
 */
DICOMProperties::~DICOMProperties()
{
}

/**
 * @brief DICOMProperties::loadValues carga todos los valores que tiene el diccionario de metadatos en un mapa (dicomTags).
 */
void DICOMProperties::loadValues(){
    string direccion=this->path;

    // Se declara el tipo de imagen seleccionando un tipo de pixel y de dimension
    typedef signed short PixelType;
    const unsigned int   Dimension = 3;
    typedef itk::Image< PixelType, Dimension > ImageType;
    typedef itk::ImageSeriesReader<ImageType> ReaderType;

    ReaderType::Pointer reader = ReaderType::New();

    typedef itk::GDCMImageIO       ImageIOType;
    ImageIOType::Pointer dicomIO = ImageIOType::New();

    reader->SetImageIO(dicomIO);

    typedef itk::GDCMSeriesFileNames     NamesGeneratorType;
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
    nameGenerator->SetInputDirectory(direccion);

    typedef std::vector<std::string>    FileNamesContainer;
    FileNamesContainer fileNames = nameGenerator->GetInputFileNames();

    reader->SetFileNames(fileNames);

    try
    {
        reader->Update();
    }
    catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
        return;
    }

    typedef itk::MetaDataDictionary   DictionaryType;
    const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
    typedef itk::MetaDataObject< std::string > MetaDataStringType;

    DictionaryType::ConstIterator itr = dictionary.Begin();
    DictionaryType::ConstIterator end = dictionary.End();


    cout << endl << endl << "---VALORES DEL METADATA---" << endl;
    while (itr != end)
    {
        itk::MetaDataObjectBase::Pointer  entry = itr->second;
        MetaDataStringType::Pointer entryvalue =
            dynamic_cast<MetaDataStringType *>(entry.GetPointer());
        if (entryvalue)
        {
            std::string tagkey = itr->first;
            std::string tagvalue = entryvalue->GetMetaDataObjectValue();
            //std::cout << tagkey << " = " << tagvalue << std::endl;
            dicomTags.insert(std::pair<string,string>(tagkey,tagvalue));
        }
        ++itr;
    }
}


/**
 * @brief DICOMProperties::printMetadata Se imprimen por consola todas las propiedades que existan en el encabezado de las imagenes.
 * Ejemplo en : https://github.com/InsightSoftwareConsortium/ITK/blob/master/Examples/IO/DicomSeriesReadPrintTags.cxx
 */
void DICOMProperties::printMetadata()
{
	string direccion=this->path;
	
	// Se declara el tipo de imagen seleccionando un tipo de pixel y de dimension
	typedef signed short PixelType;
	const unsigned int   Dimension = 3;
	typedef itk::Image< PixelType, Dimension > ImageType;
	typedef itk::ImageSeriesReader<ImageType> ReaderType;
	
	ReaderType::Pointer reader = ReaderType::New();

	typedef itk::GDCMImageIO       ImageIOType;
	ImageIOType::Pointer dicomIO = ImageIOType::New();

	reader->SetImageIO(dicomIO);

	typedef itk::GDCMSeriesFileNames     NamesGeneratorType;
	NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
	nameGenerator->SetInputDirectory(direccion);

	typedef std::vector<std::string>    FileNamesContainer;
	FileNamesContainer fileNames = nameGenerator->GetInputFileNames();

	reader->SetFileNames(fileNames);

	try
	{
		reader->Update();
	}
	catch (itk::ExceptionObject &ex)
	{
		std::cout << ex << std::endl;
		return;
	}

	typedef itk::MetaDataDictionary   DictionaryType;
	const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
	typedef itk::MetaDataObject< std::string > MetaDataStringType;

	DictionaryType::ConstIterator itr = dictionary.Begin();
	DictionaryType::ConstIterator end = dictionary.End();


	cout << endl << endl << "---VALORES DEL METADATA---" << endl;
	while (itr != end)
	{
		itk::MetaDataObjectBase::Pointer  entry = itr->second;
		MetaDataStringType::Pointer entryvalue =
			dynamic_cast<MetaDataStringType *>(entry.GetPointer());
		if (entryvalue)
		{
			std::string tagkey = itr->first;
			std::string tagvalue = entryvalue->GetMetaDataObjectValue();
			std::cout << tagkey << " = " << tagvalue << std::endl;
		}
		++itr;
	}
}

/**
 * @brief DICOMProperties::printActualValues se imprimen por consola todos los elementos que tiene el mapa de dicomTags.
 */
void DICOMProperties::printActualValues()
{
    for (map<string,string>::iterator it=dicomTags.begin(); it!=dicomTags.end(); ++it){
        cout<<it->first<< ":" <<it->second <<endl;
    }
}

/**
 * @brief DICOMProperties::getValue A partir de una llave que contiene un TAG se devuelve su valor correspondiente.
 * @param key TAG de DICOM de la forma (XXXX|XXXX) .
 * @return valor correspondiente al TAG.
 */
const string DICOMProperties::getValue(string key) {

    map<string,string>::iterator it;
    it = dicomTags.find(key);

    if(it!=dicomTags.end())
        return dicomTags.find(key)->second;
    else
        return "NA";
}

/**
 * @brief DICOMProperties::setPath Agrega el valor de la ruta donde esta la imagen DICOM que pasa por parametro.
 * @param path Ruta donde se encuentra la imagen.
 */
void DICOMProperties::setPath(const string& path)
{
	this->path = path;
}

/**
 * @brief DICOMProperties::getPath Retorna el path donde se encuentra el directorio con las imagenes DICOM .
 * @return Ruta donde esta la imagen DICOM.
 */
const string& DICOMProperties::getPath() const
{
	return this->path;
}

/**
 * @brief DICOMProperties::setPixelValue Establece el valor en mm al que corresponde un pixel.
 * @param pixelValue Valor en mm de lo que debe corresponder un pixel.
 */
void DICOMProperties:: setPixelValue(double pixelValue)
{
	this->pixelValue = pixelValue;
}

/**
 * @brief DICOMProperties::getPixelValue Retorna el valor en mm correspondiente a un pixel.
 * @return Valor en mm correspondiente a un pixel.
 */
double DICOMProperties:: getPixelValue() const
{
	return pixelValue;
}

/**
 * @brief DICOMProperties::setPatientName Establece el nombre del paciente.
 * @param patientName Nombre de paciente.
 */
void DICOMProperties::setPatientName(const string& patientName)
{
	this->patientName = patientName;
}

/**
 * @brief DICOMProperties::getPatientName Retorna el nombre del paciente.
 * @return Nombre del paciente.
 */
const string& DICOMProperties::getPatientName() const
{
	return patientName;
}

/**
 * @brief DICOMProperties::setPatientID Establece el ID del paciente.
 * @param patientID ID del paciente.
 */
void DICOMProperties::setPatientID(const string& patientID)
{
	this->patientID = patientID;
}

/**
 * @brief DICOMProperties::getPatientID Retorna el ID del paciente.
 * @return ID del paciente.
 */
const string& DICOMProperties::getPatientID() const
{
	return patientID;
}
    
/**
 * @brief DICOMProperties::setPatientBirth Establece la fecha de nacimiento del paciente.
 * @param patientBirth Fecha de nacimiento del paciente.
 */
void DICOMProperties::setPatientBirth(const string& patientBirth)
{
	this->patientBirth = patientBirth;
}
    
/**
 * @brief DICOMProperties::getPatientBirth Retorna la fecha de nacimiento del paciente.
 * @return Fecha de nacimiento del paciente.
 */
const string& DICOMProperties::getPatientBirth() const
{
	return patientBirth;
}
	
/**
 * @brief DICOMProperties::setPatientSex Establece el sexo del paciente.
 * @param patientSex Sexo del paciente.
 */
void DICOMProperties::setPatientSex(const string& patientSex)
{
	this->patientSex = patientSex;
}
    
/**
 * @brief DICOMProperties::getPatientSex Retorna el sexo del paciente.
 * @return Sexo del paciente.
 */
const string& DICOMProperties::getPatientSex() const
{
	return patientSex;
}




