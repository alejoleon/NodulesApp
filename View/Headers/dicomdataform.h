#ifndef DICOMDATAFORM_H
#define DICOMDATAFORM_H

#include <QWidget>

using namespace std;
namespace Ui {
class DicomDataForm;
}

class DicomDataForm : public QWidget
{
    Q_OBJECT

public:
    explicit DicomDataForm(QWidget *parent = 0);
    ~DicomDataForm();
    void setNombre(string);

private:
    /**
     * @brief ui Apuntador al Ui de esta clase.
     */
    Ui::DicomDataForm *ui;
};

#endif // DICOMDATAFORM_H
