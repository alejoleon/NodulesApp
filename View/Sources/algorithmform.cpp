#include "algorithmform.h"
#include "ui_algorithmform.h"

AlgorithmForm::AlgorithmForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlgorithmForm)
{
    ui->setupUi(this);

    this->ui->tx_medianRadius->setText("1");
    this->ui->tx_firstSliceClip->setText("0");
    this->ui->tx_lastSliceClip->setText("0");
    this->ui->rb_clipMedNo->setChecked(true);
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

void AlgorithmForm::on_pb_viewMediastinum_clicked()
{
    this->mainWindow->setUpImages(this->coordinator->getImageMediastinumMask());
    this->coordinator->setCurrentImage(4);
}


void AlgorithmForm::on_rb_ClipMedYes_clicked()
{
    this->ui->tx_firstSliceClip->setEnabled(true);
    this->ui->tx_lastSliceClip->setEnabled(true);
}

void AlgorithmForm::on_rb_clipMedNo_clicked()
{
    this->ui->tx_firstSliceClip->setEnabled(false);
    this->ui->tx_lastSliceClip->setEnabled(false);
}



void AlgorithmForm::on_pb_Mediana_clicked()
{
    //Se hace la mediana, paso 0.
    int radius = (this->ui->tx_medianRadius->text()).toInt();
    if (radius <= 0){
        cout<<"PONER MENSAJE DE NO HAY TEXTO"<<endl;
    }
    else {
        this->coordinator->doMedian(radius);
        this->mainWindow->setUpImages(this->coordinator->getImageMedian());
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
        cout<<"NOTIFICACION DE ALGO MAL"<<endl;
    }

    else {
        this->coordinator->doLungsMask(seeds);
        this->mainWindow->setUpImages(this->coordinator->getImageLungsMask());
        this->coordinator->setCurrentImage(3);

    }
}

void AlgorithmForm::on_pb_doMediastinum_clicked()
{
    int first = this->ui->tx_firstSliceClip->text().toInt();
    int last = this->ui->tx_lastSliceClip->text().toInt();

    if (this->ui->rb_clipMedNo->isChecked() || (first == last)) {

        this->coordinator->doMediastinumMask();
        this->mainWindow->setUpImages(this->coordinator->getImageMediastinumMask());
        this->coordinator->setCurrentImage(4);

    }
    else {
        if (first <0 || last <0){
            cout<<"TEXTO "<<first<<"   "<<last<<endl;
            cout<<"Notificacion de algo mal, numeros negativos o letras"<<endl;
        }
        else if (last < first){
            cout<<"final es menor que inicial o inicial es mayor que final"<<endl;
        }
        else {

            this->coordinator->doMediastinumMask( first ,  last);
            this->mainWindow->setUpImages(this->coordinator->getImageMediastinumMask());
            this->coordinator->setCurrentImage(4);
        }
    }
}


