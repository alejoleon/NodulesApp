#include "mainwindow.h"
#include "ui_mainwindow.h"


/**
 * @brief MainWindow::MainWindow Constructor por defecto.
 * @param parent Padre que crea esta ventana.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //A todos los botones de las vistas se les da el atributo de checkeable
    this->ui->actionCoronal_view->setCheckable(true);
    this->ui->action_Sagital_view->setCheckable(true);
    this->ui->action_All_views->setCheckable(true);
    this->ui->action_Volume_View->setCheckable(true);
    this->ui->action_Transversal_view->setCheckable(true);
    this->ui->action_All_views->setChecked(true);

}

/**
 * @brief MainWindow::~MainWindow Destructor.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::setCoordinator Asigna el coordinador al objeto de esta clase.
 * @param coordinator Coordinador.
 */
void MainWindow::setCoordinator (Coordinator * &coordinator){
    this->coordinator = coordinator;
}

/**
 * @brief MainWindow::on_actionSave_as_triggered Acción para el evento de "guardar como".
 * @return
 */
bool MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
                this,
                "NodulesApp - Save as",
                "./",
                "Text Files (*.txt)::All Files (*.*)");

    if(!filename.isEmpty()){
        curFile = filename;
        return saveFile();
    }
    return false;
}

/**
 * @brief MainWindow::saveFile Acción para el evento "guardar"
 * @return
 */
bool MainWindow::saveFile()
{
    QFile file(curFile);
    if(file.open(QFile::WriteOnly)){
        cout<<"Se guarda el archivo"<<endl;
        return true;
    }else{
        QMessageBox::warning(
                    this,
                    "NodulesApp",
                    tr("Cannot write file %1.\nError: %2")
                    .arg(curFile)
                    .arg(file.errorString()));
        return false;
    }
}

bool MainWindow::maybeSave()
{
    cout<<"Metodo maybeSave();"<<endl;
    QMessageBox::StandardButton ret =
            QMessageBox::warning(
                this,
                "NodulesApp",
                tr("The document has been Modified"
                   "Do you want to save your changes?"),
                QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);
    if (ret == QMessageBox::Yes){
        return on_action_Save_triggered();
    }else if (ret == QMessageBox::Cancel){
        return false;
    }
    return true;
}

bool MainWindow::on_action_Save_triggered()
{
    if(curFile.isEmpty()){
        return on_actionSave_as_triggered();
    }
    else {
        return saveFile();
    }
}


/**
 * @brief MainWindow::on_action_Open_triggered Acción para el evento de abrir.
 */
void MainWindow::on_action_Open_triggered()
{
    if(maybeSave()){
        QString fileName = QFileDialog::getExistingDirectory(
                    this,
                    "NodulesApp - Open file",
                    "../../",
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
        if (!fileName.isEmpty()){
            QDir file (fileName);
            if (file.exists()){
                cout<<"Se hace la operacion para abir el archivo"<<endl;
                cout<<"Ruta que se abre: "<<fileName.toStdString()<<endl;
                this->pathImage = fileName.toStdString();
                this->setUpImages(this->pathImage);
            }else {
                QMessageBox::warning(
                            this,
                            "NodulesApp",
                            tr("Cannot read File %1.")
                            .arg(fileName));
            }

        }
    }
}

/**
 * @brief MainWindow::setUpImages Evento para visualizar las imágenes DICOM a partir de la ruta obtenida al abrir un archivo.
 * @param dirDICOMImg Ruta donde se encuentra el directorio de imágenes DICOM.
 */
void MainWindow::setUpImages(string dirDICOMImg){
    this->ui->fourPanelWidget->setUpImages(dirDICOMImg);
}

/**
 * @brief MainWindow::on_action_Histogram_triggered Crea una nueva ventana de histograma de la imagen actual.
 */
void MainWindow::on_action_Histogram_triggered()
{
    int lower,upper;
    vector<int> data = this->coordinator->getHistogramData(lower,upper);
    HistogramForm *histWindow = new HistogramForm(this, lower, upper);
    histWindow->setCoordinator(this->coordinator);
    histWindow->setModal(true);
    histWindow->setHistogram(data);
    histWindow->show();
}


void MainWindow::on_prueba_clicked()
{
    this->setUpImages("/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001/2.0.Mediastino");
}

/**
 * @brief MainWindow::setCoordinator Asigna el coordinador al objeto de esta clase.
 * @param coordinator Coordinador.
 */
void MainWindow::setCoordinatorFourPanel(Coordinator * &coordinator){
    this->ui->fourPanelWidget->setCoordinator(coordinator);
}

/**
 * @brief MainWindow::on_action_Transversal_view_triggered Cuando se presiona el botón de Vista Transversal se muestra únicamente dicha vista, mientras que las otras 3 se ocultan.
 */
void MainWindow::on_action_Transversal_view_triggered()
{
    this->ui->actionCoronal_view->setChecked(false);
    this->ui->action_Sagital_view->setChecked(false);
    this->ui->action_Transversal_view->setChecked(true);
    this->ui->action_Volume_View->setChecked(false);
    this->ui->action_All_views->setChecked(false);

    this->ui->fourPanelWidget->modifyViews(1);
}

/**
 * @brief MainWindow::on_actionCoronal_view_triggered Cuando se presiona el botón de Vista Coronal se muestra únicamente dicha vista, mientras que las otras 3 se ocultan.
 */
void MainWindow::on_actionCoronal_view_triggered()
{
    this->ui->actionCoronal_view->setChecked(true);
    this->ui->action_Sagital_view->setChecked(false);
    this->ui->action_Transversal_view->setChecked(false);
    this->ui->action_Volume_View->setChecked(false);
    this->ui->action_All_views->setChecked(false);

    this->ui->fourPanelWidget->modifyViews(2);
}

/**
 * @brief MainWindow::on_action_Sagital_view_triggered Cuando se presiona el botón de Vista Sagital se muestra únicamente dicha vista, mientras que las otras 3 se ocultan.
 */
void MainWindow::on_action_Sagital_view_triggered()
{
    this->ui->actionCoronal_view->setChecked(false);
    this->ui->action_Sagital_view->setChecked(true);
    this->ui->action_Transversal_view->setChecked(false);
    this->ui->action_Volume_View->setChecked(false);
    this->ui->action_All_views->setChecked(false);

    this->ui->fourPanelWidget->modifyViews(3);
}

/**
 * @brief MainWindow::on_action_Volume_View_triggered Cuando se presiona el botón de Vista de Volumen se muestra únicamente dicha vista, mientras que las otras 3 se ocultan.
 */
void MainWindow::on_action_Volume_View_triggered()
{
    this->ui->actionCoronal_view->setChecked(false);
    this->ui->action_Sagital_view->setChecked(false);
    this->ui->action_Transversal_view->setChecked(false);
    this->ui->action_Volume_View->setChecked(true);
    this->ui->action_All_views->setChecked(false);

    this->ui->fourPanelWidget->modifyViews(4);
}

/**
 * @brief MainWindow::on_action_All_views_triggered Cuando se presiona el botón de Todas las Vistas, se muestran las 4 vistas.
 */
void MainWindow::on_action_All_views_triggered()
{
    this->ui->actionCoronal_view->setChecked(false);
    this->ui->action_Sagital_view->setChecked(false);
    this->ui->action_Transversal_view->setChecked(false);
    this->ui->action_Volume_View->setChecked(false);
    this->ui->action_All_views->setChecked(true);

    this->ui->fourPanelWidget->modifyViews(5);
}
