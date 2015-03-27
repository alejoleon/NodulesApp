#include "mainwindow.h"
#include "coordinator.h"
#include "fourpanelform.h"


#include <QApplication>
#include <vtkAutoInit.h>


/**
 * @brief main Clase principal del proyecto desde la cual se crea el coordinador y la ventana principal.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Coordinator* coordinator = new Coordinator();

    MainWindow* mainWin = new MainWindow();

    //Se asigna el coordinador a las vistas.
    mainWin->setCoordinator(coordinator);
    //Se asigna el coordinador a la vista de cuatro paneles.
    mainWin->setCoordinatorFourPanel(coordinator);
    //Se asigna el coordinador a la vista del algoritmo.
    mainWin->setCoordinatorAlgorithmForm(coordinator);


    mainWin->show();

    return a.exec();
}
