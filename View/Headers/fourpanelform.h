#ifndef FOURPANELFORM_H
#define FOURPANELFORM_H

#include <QWidget>

//Standar de C++
#include <iostream>

//Usado para imagenes en QVTKWidgets
#include "vtkResliceCursor.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkResliceCursorThickLineRepresentation.h"
#include "vtkResliceImageViewerMeasurements.h"
#include "vtkResliceImageViewer.h"
#include "vtkCellPicker.h"
#include "vtkProperty.h"
#include "vtkImagePlaneWidget.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkPlane.h"
#include "vtkPlaneSource.h"
#include <vtkAlgorithmOutput.h>

#include <vtkImageDataGeometryFilter.h>

//VTK
#include "vtkImageViewer2.h"
#include "vtkResliceImageViewer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkSmartPointer.h"

//ITK images
#include "itkImage.h"
#include "itkImageSeriesReader.h"

//VTK images
#include "vtkImageData.h"


//Coordinador
#include "coordinator.h"


//Definición de parámetros de imagen DICOM.
typedef signed short    PixelType;
const unsigned int      DimensionFP = 3;
typedef itk::Image< PixelType, DimensionFP >      ImageType;

//Definición de lector de imagen de entrada.
typedef itk::ImageSeriesReader< ImageType >     ReaderType;


using namespace std;
namespace Ui {
class FourPanelForm;
}

class FourPanelForm : public QWidget
{
    Q_OBJECT

public:

    explicit FourPanelForm(QWidget *parent = 0);
    ~FourPanelForm();

    void setUpImages(vtkImageData * image);
    void setCoordinator (Coordinator * &coordinator);
    void modifyViews(int view);

protected:

    void changeViews(int value, int element);
    vtkSmartPointer< vtkResliceImageViewer > riw[3];
    vtkSmartPointer< vtkImagePlaneWidget > planeWidget[3];

private slots:

    void on_scroll_view1_valueChanged(int value);
    void on_scroll_view2_valueChanged(int value);
    void on_scroll_view3_valueChanged(int value);
    void on_pb_resetViews_2_clicked();
    void on_spinBoxView1_valueChanged(int value);
    void on_spinBoxView2_valueChanged(int value);
    void on_spinBoxView3_valueChanged(int arg1);

private:
    /**
    * @brief coordinator Coordinador encargado de conectar las vistas con el modelo.
    */
    Coordinator * coordinator;
    /**
     * @brief ui Apuntador al Ui de esta clase.
     */
    Ui::FourPanelForm *ui;
};

#endif // FOURPANELFORM_H
