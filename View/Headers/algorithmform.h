#ifndef ALGORITHMFORM_H
#define ALGORITHMFORM_H

#include <QWidget>

//Standar de C++
#include <iostream>

//Clases del proyecto
#include "coordinator.h"
#include "mainwindow.h"

using namespace std;

namespace Ui {
class AlgorithmForm;
}

/**
 * @brief The AlgorithmForm class Encargada de realizar todo lo relacionado con la interfaz gráfica donde se presentará el algoritmo para encontrar nódulos pulmonares.
 */
class AlgorithmForm : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief AlgorithmForm Constructor.
     * @param parent Apuntador al objeto a partir del cual es creado.
     */
    explicit AlgorithmForm(QWidget *parent = 0);

    /**
     * @brief AlgorithmForm Destructor por defecto.
     */
    ~AlgorithmForm();

    /**
     * @brief setCoordinator Asigna el coordinador al objeto de esta clase.
     * @param coordinator Coordinador que controla los datos entre la vista y el modelo.
     */
    void setCoordinator (Coordinator * &coordinator);

    /**
     * @brief setMainWindow Asigna la ventana principal donde se muestran los datos.
     * @param mainWindow Ventana principal donde el usuario interactua con el sistema.
     */
    void setMainWindow (MainWindow * &mainWindow);

private slots:

    /**
     * @brief on_pb_Mediana_clicked Realiza un filtrado de mediana de la imagen de entrada, mediante el coordinador, usando el radio escrito en el cuadro de texto.
     */
    void on_pb_Mediana_clicked();
    /**
     * @brief on_pb_LungsMask_clicked Realiza una segmentación de los pulmones mediante crecimiento de regiones, por medio del coordinador, usando las semillas escritas en los cuadros de texto correspondiente.
     */
    void on_pb_LungsMask_clicked();
    /**
     * @brief on_pb_doMediastinum_clicked Realiza la segmentación del mediastino teniendo en cuenta los radioButton de selección de recorte del diafragma.
     * Si está el YES seleccionado, entonces se deben tener valores válidos en los campos de texto y se hace el recorte del diafragma o de la imagen, de acuerdo a los parámetros del recorte.
     * Si está en NO, o si los valores en el cuadro de texto son iguales, entonces solo se hace la segmentación del mediastino, dejando el diafragma.
     */
    void on_pb_doMediastinum_clicked();
    
    
    
    
    
    
    
    /**
     * @brief on_pb_viewImgIn_clicked Muestra la imagen original que fue abierta.
     */
    void on_pb_viewImgIn_clicked();
    /**
     * @brief on_pb_viewMedian_clicked Muestra la imagen luego de haber realizado la mediana.
     */
    void on_pb_viewMedian_clicked();
    /**
     * @brief on_pb_viewLungsMask_clicked Muestra la imagen de mascara de los pulmones luego de haber sido creada.
     */
    void on_pb_viewLungsMask_clicked();
    /**
     * @brief on_pb_viewMediastinum_clicked Muestra la imagen de la mascara del mediastino.
     */
    void on_pb_viewMediastinum_clicked();
    

    
    /**
     * @brief on_rb_ClipMedYes_clicked Acción que debe realizarse al momento de seleccionar la opcion YES en el cuadro de parámetros del mediastino.
     * Se deben habilitar los cuadros de texto de selección de slices mínimo y máximo.
     */
    void on_rb_ClipMedYes_clicked();
    /**
     * @brief on_rb_clipMedNo_clicked Acción que debe realizarse al momento de seleccionar la opcion NO en el cuadro de parámetros del mediastino.
     * Se deben deshabilitar los cuadros de texto de selección de slices mínimo y máximo.
     */
    void on_rb_clipMedNo_clicked();



private:

    /**
     * @brief coordinator Coordinador encargado de conectar las vistas con el modelo.
     */
    Coordinator * coordinator;
    /**
     * @brief mainWindow Ventana principal donde se van a mostrar los diferentes resultados del algoritmo.
     */
    MainWindow * mainWindow;
    /**
     * @brief ui Contiene los elementos de la interfaz gráfica.
     */
    Ui::AlgorithmForm *ui;
};

#endif // ALGORITHMFORM_H
