#include "algorithmform.h"
#include "ui_algorithmform.h"

AlgorithmForm::AlgorithmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlgorithmForm)
{
    ui->setupUi(this);
}

AlgorithmForm::~AlgorithmForm()
{
    delete ui;
}

void AlgorithmForm::setCoordinator (Coordinator * &coordinator){
    this->coordinator = coordinator;
}

void AlgorithmForm::setMainWindow(MainWindow * &mainWindow){
    this->mainWindow = mainWindow;
}

void AlgorithmForm::on_pb_viewImgIn_clicked()
{
    this->mainWindow->setUpImages(this->coordinator->getImageIn());
    this->coordinator->setCurrentImage(1);
}

void AlgorithmForm::on_pb_viewMedian_clicked()
{
    this->mainWindow->setUpImages(this->coordinator->getImageMedian());
    this->coordinator->setCurrentImage(2);
}

void AlgorithmForm::on_pb_viewLungsMask_clicked()
{
    this->mainWindow->setUpImages(this->coordinator->getImageLungsMask());
    this->coordinator->setCurrentImage(3);
}


void AlgorithmForm::on_pb_Mediana_clicked()
{
    //Se hace la mediana, paso 0.
    int radius = (this->ui->tx_medianRadius->text()).toInt();
    if (radius <= 0){
        cout<<"PONER MENSAJE DE NO HAY TEXTO"<<endl;
    }
    else {
        string medianImage = this->coordinator->doMedian(radius);
        this->mainWindow->setUpImages(medianImage);
        this->coordinator->setCurrentImage(2);
    }
}

void AlgorithmForm::on_pb_LungsMask_clicked()
{

    //Se hace el crecimiento de regiones de pulmones, paso 1.
    float seeds[6];
    seeds[0] = this->ui->tx_seedX1->text().toFloat();
    seeds[1] = this->ui->tx_seedY1->text().toFloat();
    seeds[2] = this->ui->tx_seedZ1->text().toFloat();
    seeds[3] = this->ui->tx_seedX2->text().toFloat();
    seeds[4] = this->ui->tx_seedY2->text().toFloat();
    seeds[5] = this->ui->tx_seedZ2->text().toFloat();

    if (seeds[0] <=0 || seeds[1] <=0 ||seeds[2] <=0 ||
        seeds[3] <=0 || seeds[4] <=0 ||seeds[5] <=0)
    {
        cout<<"NOTIFICACION DE ALLGO MAL"<<endl;
    }

    else {
        string lungsMaskImage = this->coordinator->doLungsMask(seeds);
        this->mainWindow->setUpImages(lungsMaskImage);
        this->coordinator->setCurrentImage(3);

    }
}
