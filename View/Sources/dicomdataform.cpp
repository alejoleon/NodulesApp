#include "dicomdataform.h"
#include "ui_dicomdataform.h"

DicomDataForm::DicomDataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicomDataForm)
{
    ui->setupUi(this);
}

DicomDataForm::~DicomDataForm()
{
    delete ui;
}



void DicomDataForm::setNombre(string dd){
    this->ui->pushButton->setText(QString::fromStdString(dd));
}
