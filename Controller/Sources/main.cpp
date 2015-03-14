#include "mainwindow.h"
#include "Coordinator.h"
#include "fourpanelform.h"

#include <QApplication>
#include <vtkAutoInit.h>


//ITK
#include "itkImage.h"
#include "itkImageSeriesReader.h"

//Definicion de imagen de entrada
typedef signed short PixelType;
const unsigned int Dimension= 3;
typedef itk::Image<PixelType, Dimension> ImageType;
typedef itk::ImageSeriesReader<ImageType> ReaderType;

//Definiciion de imagen Binaria
typedef unsigned char   PixelBinaryType;
typedef itk::Image<PixelBinaryType, Dimension> ImageBinaryType;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Coordinator* coordinator = new Coordinator();
    MainWindow* mainWin = new MainWindow();

    //Se asigna el coordinador a las vistas.
    mainWin->setCoordinator(coordinator);
    //Se asigna el coordinador a la vista de cuatro paneles.
    mainWin->setCoordinatorFourPanel(coordinator);

    mainWin->show();

    return a.exec();
}
