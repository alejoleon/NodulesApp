#ifndef HISTOGRAMFORM_H
#define HISTOGRAMFORM_H

#include <QDialog>

//Standar de C++
#include <iostream>
#include <vector>

// Para dibujar graficos estadisticos
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkIntArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>

//Coordinador
#include "coordinator.h"

//VTK
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>


using namespace std;

namespace Ui {
class HistogramForm;
}

class HistogramForm : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramForm(QWidget *parent = 0 , int lowerValue = 0 , int upperValue = 0);
    ~HistogramForm();

    void setHistogram(vector<int> hist);
    void setLowerValue(int lowerValue);
    void setUpperValue(int upperValue);
    void setCoordinator (Coordinator * &coordinator);

private:
    void changeHistogramBarView(int minValue , int maxValue );

private slots:

    void on_minBoxHUValues_valueChanged(int arg1);
    void on_maxBoxHUValues_valueChanged(int arg1);
    void on_rb_line_clicked();
    void on_rb_bar_clicked();
    void on_rb_points_clicked();
    void on_rb_stacked_clicked();
    void on_pb_exit_clicked();

private:

    /**
    * @brief coordinator Coordinador encargado de conectar las vistas con el modelo.
    */
    Coordinator * coordinator;
    /**
     * @brief ui Apuntador al Ui de esta clase.
     */
    Ui::HistogramForm *ui;
    /**
     * @brief hist Arreglo que contiene el histograma de la imagen.
     */
    vector<int> hist;
    /**
     * @brief lowerValue Valor de gris mínimo del histograma.
     */
    int lowerValue;
    /**
     * @brief upperValue Valor de gris máximo del histograma.
     */
    int upperValue;
    /**
     * @brief minBoxHUValueHasChange Variable para saber si aun no se han hecho cambios en el minBoxHUValue o maxBoxHUValue respectivamente.
     *True : El valor del boxHUValue ha cambiado almenos una vez.
     *False : El valor del boxHUValue no ha cambiado desde que se creó.
     */
    bool minBoxHUValueHasChange;
    bool maxBoxHUValueHasChange;
    /**
     * @brief chartType Indica el tipo de gráfico en que se presentará el histograma, puede ser continuo, barras, puntos o linea.
     */
    int chartType;
    /**
     * @brief view Vista en donde se configura la tabla que contiene los datos que van a ser dibujados.
     */
    vtkSmartPointer<vtkContextView> view;

};

#endif // HISTOGRAMFORM_H
