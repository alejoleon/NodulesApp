#ifndef DICOMPROPERTIES_H
#define DICOMPROPERTIES_H

#include <string>
#include <vector>
#include <map>

// Para leer imagenes dicom
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"

#include "itkImage.h"
#include "itkMetaDataObject.h"

using namespace std;

class DICOMProperties
{
private:
    /**
     * @brief pixelValue Valor en mm correspondiente a 1 píxel.
     */
    double pixelValue;
    /**
     * @brief path Ruta donde esta el directorio con las imágenes DICOM.
     */
    string path;
    /**
     * @brief patientName Nombre del paciente.
     */
    string patientName;
    /**
     * @brief patientID Identificador del paciente (Identificación del hospital).
     */
    string patientID;
    /**
     * @brief patientBirth Fecha de nacimiento del paciente.
     */
    string patientBirth;
    /**
     * @brief patientSex Sexo del paciente.
     */
    string patientSex;
    /**
     * @brief dicomTags Mapa que contiene todos los TAGs|Valores obtenidos de la imagen DICOM.
     */
    map <string, string> dicomTags;

public:
    DICOMProperties(string direccion);
    ~DICOMProperties();

    void loadValues();
    void printMetadata();
    void printActualValues();
    const string getValue(string key) ;

    const string& getPath() const;
    void setPath(const string& path);

    double getPixelValue() const;
    void setPixelValue(double pixelValue);

    const string& getPatientName() const;
    void setPatientName(const string& patientName);

    const string& getPatientID() const;
    void setPatientID(const string& patientID);

    const string& getPatientBirth() const;
    void setPatientBirth(const string& patientBirth);

    const string& getPatientSex() const;
    void setPatientSex(const string& patientSex);

};

#endif // DICOMPROPERTIES_H
