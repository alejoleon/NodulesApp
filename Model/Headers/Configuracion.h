// Bibliotecas de lectura de archivos
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Configuracion
{
private:
    // Ruta donde se encuentra la imagen a leer
    string ruta_imagenIn;
    string ruta_MedianFilter;
    string ruta_regionGrowing;
    string ruta_regGrowClosing;
    string ruta_mediastinum;
    string ruta_mediastinumOpen;
    string ruta_mediastinumGeodesic;
    string ruta_interestRegion;
    string ruta_majorVessels;
    string ruta_PeripheralVesselsInt;
    string ruta_PeripheralVesselsDist;
	string ruta_MediastinoDiafragma;


public:
    Configuracion();
    ~Configuracion();
    const string& GetRutaImagenIn() const;
    const string& GetRutaMedianFilter() const;
    const string& GetRutaRegionGrowing() const;
    const string& GetRutaRegGrowClosing() const;
    const string& GetRutaMediastinum() const;
    const string& GetRutaMediastinumOpen() const;
    const string& GetRutaMediastinumGeodesic() const;
    const string& GetRutaInterestRegion() const;
    const string& GetRutaMajorVessels() const;
    const string& GetRutaPeripheralVesselsInt() const;
    const string& GetRutaPeripheralVesselsDist() const;
	const string& GetRutaMediastinoDiafragma() const;

};
