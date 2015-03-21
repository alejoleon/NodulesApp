#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    this->ui->action_Algorithm->setCheckable(true);

    this->ui->widgetAlgorithm->setVisible(false);
    this->mw = this;
    this->ui->widgetAlgorithm->setMainWindow(mw);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCoordinator (Coordinator * &coordinator){
    this->coordinator = coordinator;
}

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

void MainWindow::on_action_Open_triggered()
{
    //if(maybeSave()){
        QString fileName = QFileDialog::getExistingDirectory(
                    this,
                    "NodulesApp - Open file",
                    "/Users/AlejoMac/Documents/AlgoritmosTG/ImagenesTG/Outputs/W0001",
                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
        if (!fileName.isEmpty()){
            QDir file (fileName);
            if (file.exists()){
                this->coordinator->setImageIn(fileName.toStdString());
                this->setUpImages(this->coordinator->getImageIn());

            }else {
                QMessageBox::warning(
                            this,
                            "NodulesApp",
                            tr("Cannot read File %1.")
                            .arg(fileName));
            }

        }
    //}
}

void MainWindow::setUpImages(vtkImageData* image){
    this->ui->fourPanelWidget->setUpImages(image);
}

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
    this->coordinator->funcionPrueba();
}

void MainWindow::setCoordinatorFourPanel(Coordinator * &coordinator){
    this->ui->fourPanelWidget->setCoordinator(coordinator);
}

void MainWindow::setCoordinatorAlgorithmForm(Coordinator * &coordinator){
    this->ui->widgetAlgorithm->setCoordinator(coordinator);
}

void MainWindow::on_action_Transversal_view_triggered()
{
    this->ui->actionCoronal_view->setChecked(false);
    this->ui->action_Sagital_view->setChecked(false);
    this->ui->action_Transversal_view->setChecked(true);
    this->ui->action_Volume_View->setChecked(false);
    this->ui->action_All_views->setChecked(false);

    this->ui->fourPanelWidget->modifyViews(1);
}

void MainWindow::on_actionCoronal_view_triggered()
{
    this->ui->actionCoronal_view->setChecked(true);
    this->ui->action_Sagital_view->setChecked(false);
    this->ui->action_Transversal_view->setChecked(false);
    this->ui->action_Volume_View->setChecked(false);
    this->ui->action_All_views->setChecked(false);

    this->ui->fourPanelWidget->modifyViews(2);
}

void MainWindow::on_action_Sagital_view_triggered()
{
    this->ui->actionCoronal_view->setChecked(false);
    this->ui->action_Sagital_view->setChecked(true);
    this->ui->action_Transversal_view->setChecked(false);
    this->ui->action_Volume_View->setChecked(false);
    this->ui->action_All_views->setChecked(false);

    this->ui->fourPanelWidget->modifyViews(3);
}

void MainWindow::on_action_Volume_View_triggered()
{
    this->ui->actionCoronal_view->setChecked(false);
    this->ui->action_Sagital_view->setChecked(false);
    this->ui->action_Transversal_view->setChecked(false);
    this->ui->action_Volume_View->setChecked(true);
    this->ui->action_All_views->setChecked(false);

    this->ui->fourPanelWidget->modifyViews(4);
}

void MainWindow::on_action_All_views_triggered()
{
    this->ui->actionCoronal_view->setChecked(false);
    this->ui->action_Sagital_view->setChecked(false);
    this->ui->action_Transversal_view->setChecked(false);
    this->ui->action_Volume_View->setChecked(false);
    this->ui->action_All_views->setChecked(true);

    this->ui->fourPanelWidget->modifyViews(5);
}

void MainWindow::on_action_Algorithm_triggered()
{
    if (this->ui->action_Algorithm->isChecked()) {
        this->ui->widgetDataInfo->setVisible(false);
        this->ui->widgetAlgorithm->setVisible(true);
    }
    else {
        this->ui->widgetDataInfo->setVisible(true);
        this->ui->widgetAlgorithm->setVisible(false);
    }
}
