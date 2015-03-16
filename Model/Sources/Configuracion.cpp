#include "Configuracion.h"

/**
 * @brief Lee el archivo de configuración y agrega las propiedades al objeto de la clase Configuracion
 * @return Objeto de la clase Configuracion
 */
Configuracion::Configuracion()
{
	ifstream entrada;
	stringstream linea;

	//entrada.open("C:\\Users\\Alejandro\\Dropbox\\Universidad\\TrabajoGrado2015\\9_CodigoFuenteProyecto\\AlgoritmoTB\\FuentesBouma\\ArchivoConfig.txt", ios::in);
	entrada.open("/Users/AlejoMac/Dropbox/Universidad/TrabajoGrado2015/9_CodigoFuenteProyecto/NodulesApp/Model/ArchivoConfig.txt", ios::in);//Se lee la ruta relativa del archivo de configuracion
	if (!entrada)
	{
		cout << "No se pudo cargar archivo"<<endl;
	}
	else
	{
		//cout << "Si se leyo" << endl;

		char cad[200];
		string cad1;
		stringstream parametro;
		
		entrada.getline(cad, 200, '\n');//se lee el titulo
		do
		{
			entrada.getline(cad, 200, '\n');//se lee la propiedad
			cad1 = cad;
			
			parametro.clear(); //Se hace para borrar lo que pueda haber en el flujo
			parametro << cad1; //Se convierte a SS para poder usar los tokens

			parametro.getline(cad, 200, ' '); //Se lee subtitulo
			
			if ((string)cad=="DirImgIn:")
			{
				parametro.getline(cad, 200, '\n');//Se lee el valor del parametro
				cad1 = (string)cad;
				//cad1.erase(cad1.size()-1);
				this->ruta_imagenIn = cad1;
				
			}else if ((string)cad=="DirImgMedianOut:") {
	
                parametro.getline(cad, 200, '\n');//Se lee el valor del parametro
                cad1 = (string)cad;
                this->ruta_MedianFilter=cad;
				
			}else if ((string)cad=="DirImgRegGrowOut:") {
				
				parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
				this->ruta_regionGrowing=cad;
				
			}else if ((string)cad=="DirImgRegGrowClosingOut:") {
				
				parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
				this->ruta_regGrowClosing=cad;
				
			}else if ((string)cad=="DirImgMediastinumOut:") {
				
				parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
				this->ruta_mediastinum=cad;
				
			}else if ((string)cad=="DirImgInterestRegOut:") {
				
				parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
				this->ruta_interestRegion=cad;
				
			}else if ((string)cad=="DirImgMajorVesselsOut:") {
				
				parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
				this->ruta_majorVessels=cad;
				
			}else if ((string)cad=="DirImgPerVesselsIntOut:") {
				
				parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
				this->ruta_PeripheralVesselsInt=cad;
				
			}else if ((string)cad=="DirImgPerVesselsDistOut:") {
				
				parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
				this->ruta_PeripheralVesselsDist=cad;
				
            }else if ((string)cad=="DirImgMediastinumOpenOut:") {

                parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
                this->ruta_mediastinumOpen=cad;

            }else if ((string)cad=="DirImgMediastinumGeodesicOut:") {

                parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
                this->ruta_mediastinumGeodesic=cad;
				
            }else if ((string)cad=="DirImgMediastinumDiafragmaOut:") {

                parametro.getline(cad, 200, ' ');//Se lee el valor del parametro
                this->ruta_MediastinoDiafragma=cad;
            }
			
			
			
			
		} while (!entrada.eof());
	}
}

/**
 * @brief Destructor de la clase Configuracion
 * @return 
 */
Configuracion::~Configuracion()
{
	
}

/**
 * @brief Retorna la dirección o path donde se encuentra la imagen de entrada.
 * @return Ruta donde esta la imagen de entrada.
 */
const string& Configuracion::GetRutaImagenIn() const
{
	return ruta_imagenIn;
}

/**
 * @brief Retorna la dirección o path donde se encuentra la imagen con filtro de mediana (salida).
 * @return Ruta donde esta la imagen de salida.
 */
const string& Configuracion::GetRutaMedianFilter() const
{
	return ruta_MedianFilter;
}

/**
 * @brief Retorna la direccion o path donde se encuentra la imagen que ya fue pasada por el crecimiento de regiones.
 * @return Ruta donde esta la imagen de regionGrowing.
 */
const string& Configuracion::GetRutaRegionGrowing() const
{
	return ruta_regionGrowing;
}

/**
 * @brief Retorna la direccion o path donde se encuentra la imagen que ya fue pasada por el crecimiento de regiones y un closing.
 * @return Ruta donde esta la imagen de regionGrowing luego de haberse hecho un Closing.
 */
const string& Configuracion::GetRutaRegGrowClosing() const
{
	return ruta_regGrowClosing;
}


/**
 * @brief Retorna la direccion o path donde se encuentra la imagen correspondiente al mediastino.
 * @return Ruta donde esta la imagen del mediastino.
 */
const string& Configuracion::GetRutaMediastinum() const
{
	return ruta_mediastinum;
}


/**
 * @brief Retorna la direccion o path donde se encuentra la imagen correspondiente a la region de interes.
 * @return Ruta donde esta la imagen de la region de interes.
 */
const string& Configuracion::GetRutaInterestRegion() const
{
	return ruta_interestRegion;
}
/**
 * @brief Retorna la direccion o path donde se encuentra la imagen correspondiente a las venas grandes.
 * @return Ruta donde esta la imagen de las venas grandes.
 */
const string& Configuracion::GetRutaMajorVessels() const
{
	return ruta_majorVessels;
}


/**
 * @brief Retorna la direccion o path donde se encuentra la imagen correspondiente a las venas periféricas con criterio de intensidad.
 * @return Ruta donde esta la imagen de las venas periféricas con criterio de intensidad.
 */
const string& Configuracion::GetRutaPeripheralVesselsInt() const
{
	return ruta_PeripheralVesselsInt;
}

/**
 * @brief Retorna la direccion o path donde se encuentra la imagen correspondiente a las venas periféricas con criterio de distancia a mediastino.
 * @return Ruta donde esta la imagen de las venas periféricas con criterio de distancia al mediastino.
 */
const string& Configuracion::GetRutaPeripheralVesselsDist() const
{
	return ruta_PeripheralVesselsDist;
}

const string& Configuracion::GetRutaMediastinumOpen() const
{
    return ruta_mediastinumOpen;
}
const string& Configuracion::GetRutaMediastinumGeodesic() const
{
    return ruta_mediastinumGeodesic;
}

const string& Configuracion::GetRutaMediastinoDiafragma() const {
	return ruta_MediastinoDiafragma;
}

