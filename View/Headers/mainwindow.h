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

//Imagenes ITK
#include "itkImageSeriesReader.h"
#include "itkImage.h"

//Imagenes VTK
#include "vtkDICOMImageReader.h"

//Coordinador, controlador
#include "Coordinator.h"

//Conversion de imagen ITK a VTK
#include "itkImageToVTKImageFilter.h"
#include <vtkImageData.h>

//Ventanas
#include "histogramForm.h"
#include "fourpanelform.h"


using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setCoordinator (Coordinator * &coordinator);
    void setCoordinatorFourPanel(Coordinator * &coordinator);

private slots:
    bool on_actionSave_as_triggered();
    bool on_action_Save_triggered();
    void on_action_Open_triggered();
    void on_action_Histogram_triggered();
    void on_prueba_clicked();
    void on_action_Transversal_view_triggered();
    void on_actionCoronal_view_triggered();
    void on_action_Sagital_view_triggered();
    void on_action_Volume_View_triggered();
    void on_action_All_views_triggered();


private:

    /**
     * @brief coordinator Coordinador encargado de conectar las vistas con el modelo.
     */
    Coordinator * coordinator;
    /**
     * @brief ui Apuntador al Ui de esta clase.
     */
    Ui::MainWindow *ui;
    //Guarda el archivo actual.
    QString curFile;
    string pathImage;
    bool saveFile();
    bool maybeSave();

    void setUpImages(string dirDICOMImg);


};

#endif // MAINWINDOW_H
