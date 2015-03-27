#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//Para el menu
#include <QFileDialog>
//Para mensajes de salida
#include <QMessageBox>
//Standar de C++
#include <iostream>
#include <vector>

//Coordinador, controlador
#include "coordinator.h"

//Ventanas
#include "histogramForm.h"
#include "fourpanelform.h"


//Imagen

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow Constructor por defecto.
     * @param parent Apuntador al Padre que crea esta ventana.
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief MainWindow::~MainWindow Destructor.
     */
    ~MainWindow();

    /**
     * @brief setCoordinator Asigna el coordinador al objeto de esta clase.
     * @param coordinator Coordinador que controla los datos entre la vista y el modelo.
     */
    void setCoordinator (Coordinator * &coordinator);

    /**
     * @brief setCoordinatorFourPanel Asigna el coordinador a la vista de la visualización de la imagen DICOM.
     * @param coordinator Coordinador que controla los datos entre la vista y el modelo.
     */
    void setCoordinatorFourPanel(Coordinator * &coordinator);

    /**
     * @brief setCoordinatorAlgorithmForm Asigna el coordinador a la vista del algoritmo.
     * @param coordinator Coordinador que controla los datos entre la vista y el modelo.
     */
    void setCoordinatorAlgorithmForm(Coordinator * &coordinator);

    /**
     * @brief setUpImages Por medio del coordinador y de la ruta donde está la imagen, se obtiene una imagen VTK para mostrar.
     */
    void setUpImages(vtkImageData*);



private slots:
    bool on_actionSave_as_triggered();
    bool on_action_Save_triggered();
    void on_action_Open_triggered();


    /**
     * @brief on_action_Histogram_triggered Crea una nueva ventana de histograma de la imagen actual.
     */
    void on_action_Histogram_triggered();

    /**
     * @brief on_action_Transversal_view_triggered Cuando se selecciona la opción de Vista Transversal se muestra únicamente dicha vista, mientras que las otras 3 se ocultan.
     */
    void on_action_Transversal_view_triggered();

    /**
     * @brief on_actionCoronal_view_triggered Cuando se selecciona la opción de Vista Coronal se muestra únicamente dicha vista, mientras que las otras 3 se ocultan.
     */
    void on_actionCoronal_view_triggered();

    /**
     * @brief MainWindow::on_action_Sagital_view_triggered Cuando se selecciona la opción de Vista Sagital se muestra únicamente dicha vista, mientras que las otras 3 se ocultan.
     */
    void on_action_Sagital_view_triggered();

    /**
     * @brief on_action_Volume_View_triggered Cuando se selecciona la opción de Vista de Volumen se muestra únicamente dicha vista, mientras que las otras 3 se ocultan.
     */
    void on_action_Volume_View_triggered();

    /**
     * @brief on_action_All_views_triggered Cuando se selecciona la opción de Todas las Vistas, se muestran las 4 vistas.
     */
    void on_action_All_views_triggered();

    /**
     * @brief on_action_Algorithm_triggered Cuando se selecciona la opcion de Algoritmo, se desaparece el panel con la información de la imagen y aparece el panel con la información del algoritmo.
     */
    void on_action_Algorithm_triggered();







    void on_prueba_clicked();



private:

    /**
      * @brief ui Contiene los elementos de la interfaz gráfica.
      */
     Ui::MainWindow *ui;

     /**
     * @brief coordinator Coordinador encargado del flujo de datos entre la vista y el modelo.
     */
    Coordinator * coordinator;

    /**
     * @brief mw Apuntador a este mismo objeto (this). Con el fin de poder establecer este objeto en otras ventanas instanciadas a partir de este.
     */
    MainWindow * mw;






    //Guarda el archivo actual.
    QString curFile;

    bool saveFile();
    bool maybeSave();

};

#endif // MAINWINDOW_H
