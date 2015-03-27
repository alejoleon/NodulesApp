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


/**
 * @brief The FourPanelForm class Encargada de realizar todo lo relacionado con mostrar al usuario las 3 vistas de las imágenes médicas (sagital, coronal, transversal) en la interfáz gráfica.
 */
class FourPanelForm : public QWidget
{
    Q_OBJECT

public:

    /**
     * @brief FourPanelForm Constructor por defecto.
     * @param parent Clase padre que crea esta ventana.
     */
    explicit FourPanelForm(QWidget *parent = 0);

    /**
     * @brief FourPanelForm::~FourPanelForm Destructor por defecto.
     */
    ~FourPanelForm();

    /**
     * @brief setUpImages Muestra en la interfáz gráfica (4 páneles QVTK) la imágen que llega por parámetro en 3 vistas (Sagital, Coronal, Transversal) y en una vista de volumen.
     * De igual forma se ajustan los controles de acuerdo a la imagen de entrada.
     * @param image Imagen VTK que va a ser mostrada en la interfáz gráfica.
     */
    void setUpImages(vtkImageData * image);

    /**
     * @brief setCoordinator Asigna el coordinador al objeto de esta clase.
     * @param coordinator Coordinador.
     */
    void setCoordinator (Coordinator * &coordinator);

    /**
     * @brief modifyViews Modifica la ventana para ver una sola vista.
     * @param view Vista que se va a ver en la ventana. 1 para la vista transversal, 2 para la vista coronal, 3 para la vista sagital, 4 para la vista de volumen, 5 para las 3 vistas y la vista del volumen.
     */
    void modifyViews(int view);

protected:

    /**
     * @brief changeViews Realiza los cambios en las 3 vistas y la vista del volumen de acuerdo a los parametros del valor, y el elemento que requiere ser cambiado.
     * @param value Valor que debe tomar el slice. 0 si es la vista de volumen.
     * @param element Elemento que requiere ser cambiado. O para la vista 1, 1 para la vista 2, 2 para la vista 3 y 3 para la vista 4
     */
    void changeViews(int value, int element);

    /**
     * @brief riw Encargada de procesar las imágenes para mostrarlas en las 3 vistas.
     */
    vtkSmartPointer< vtkResliceImageViewer > riw[3];
    /**
     * @brief planeWidget Encargada de hacer una imagen de volumen combinando 3 planos.
     */
    vtkSmartPointer< vtkImagePlaneWidget > planeWidget[3];

private slots:
    /**
     * @brief on_scroll_view1_valueChanged Cada vez que exista un cambio en el ScrollView, se cambia el slice de la vista 1 al valor dado por el ScrollView.
     * @param value Valor en que se encuentra el ScrollView.
     */
    void on_scroll_view1_valueChanged(int value);

    /**
     * @brief on_scroll_view2_valueChanged Cada vez que exista un cambio en el ScrollView, se cambia el slice de la vista 2 al valor dado por el ScrollView.
     * @param value Valor en que se encuentra el ScrollView.
     */
    void on_scroll_view2_valueChanged(int value);

    /**
     * @brief on_scroll_view3_valueChanged Cada vez que exista un cambio en el ScrollView, se cambia el slice de la vista 3 al valor dado por el ScrollView.
     * @param value Valor en que se encuentra el ScrollView.
     */
    void on_scroll_view3_valueChanged(int value);

    /**
     * @brief on_pb_resetViews_2_clicked Cuando es presionado el boton de "Reset", todas las vistas vuelven a su estado inicial, es decir al slice central.
     */
    void on_pb_resetViews_2_clicked();

    /**
     * @brief on_spinBoxView1_valueChanged Cambia el valor del ScrollView 1 siempre que el SpinBoxView 1 sea cambiado.
     * @param value Valor que tiene el SpinBoxView 1 luego de un cambio de valor.
     */
    void on_spinBoxView1_valueChanged(int value);

    /**
     * @brief on_spinBoxView2_valueChanged Cambia el valor del ScrollView 2 siempre que el SpinBoxView 2 sea cambiado.
     * @param value Valor que tiene el SpinBoxView 2 luego de un cambio de valor.
     */
    void on_spinBoxView2_valueChanged(int value);

    /**
     * @brief FourPanelForm::on_spinBoxView3_valueChanged Cambia el valor del ScrollView 3 siempre que el SpinBoxView 3 sea cambiado.
     * @param value Valor que tiene el SpinBoxView luego de un cambio de valor.
     */
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
