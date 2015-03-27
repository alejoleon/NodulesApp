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

//Lista de combobox
#include "QStringList"

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

    /**
     * @brief HistogramForm Constructor.
     * @param parent Padre que crea esta ventana.
     * @param lowerValue Valor mínimo de gris que tiene el histograma. 0 por defecto.
     * @param upperValue Valor máximo de gris que tiene el histograma. 0 por defecto.
     */
    explicit HistogramForm(QWidget *parent = 0 , int lowerValue = 0 , int upperValue = 0);
    /**
     * @brief ~HistogramForm Destructor por defecto.
     */
    ~HistogramForm();

    /**
     * @brief setCoordinator Asigna el coordinador al objeto de esta clase.
     * @param coordinator Coordinador.
     */
    void setCoordinator (Coordinator * &coordinator);

    /**
     * @brief setHistogram Asigna un histograma a esta esta ventana.
     * @param hist Arreglo que contiene un histograma.
     */
    void setHistogram(vector<int> hist);

    /**
     * @brief setMasksList Llena el combobox con las máscaras disponibles.
     * @param masksList Lista con los nombres de las máscaras que existen en memoria.
     */
    void setMasksList (QStringList masksList);

protected:

    /**
     * @brief changeHistogramView Dibuja un diagrama (histograma) en el qvtkWidget. El tipo de diagrama depende del valor de Chart.
     * @param minValue Valor mínimo desde el cual se dibujará el histograma.
     * @param maxValue Valor máximo hasta el cual se dibujará el histograma.
     * Tomado de : http://www.paraview.org/Wiki/VTK/Examples/Cxx/Plotting/BarChart
     */
    void changeHistogramView(int minValue , int maxValue );

    /**
     * @brief setLimitValues Ajusta los valores mínimos y máximos de los controles dependiendo del histograma a mostrar, de acuerdo a la variable usingMask.
     */
    void setLimitValues ();

private slots:
    /**
     * @brief on_minBoxHUValues_valueChanged Cuando hay un cambio en el valor de gris mínimo, se verifica que dicho valor no sobrepase el valor de gris máximo, de ser así el valor máximo siempre estará un valor de gris mas arriba.
     * @param arg1 Valor actual en que se encuentra el valor de gris mínimo.
     */
    void on_minBoxHUValues_valueChanged(int arg1);

    /**
     * @brief on_maxBoxHUValues_valueChanged Cuando hay un cambio en el valor de gris máximo, se verifica que dicho valor no sobrepase el valor de gris mínimo, de ser así el valor mínimo siempre estará un valor de gris mas abajo.
     * @param arg1 Valor actual en que se encuentra el valor de gris máximo.
     */
    void on_maxBoxHUValues_valueChanged(int arg1);

    /**
     * @brief on_rb_line_clicked Cuando se presiona el radioButton correspondiente, se cambia al diagrama de linea.
     */
    void on_rb_line_clicked();

    /**
     * @brief on_rb_bar_clicked Cuando se presiona el radioButton correspondiente, se cambia al diagrama de barras.
     */
    void on_rb_bar_clicked();

    /**
     * @brief on_rb_points_clicked Cuando se presiona el radioButton correspondiente, se cambia al diagrama de puntos.
     */
    void on_rb_points_clicked();

    /**
     * @brief on_rb_stacked_clicked Cuando se presiona el radioButton correspondiente, se cambia al diagrama acumulado.
     */
    void on_rb_stacked_clicked();

    /**
     * @brief on_pb_exit_clicked Se cierra la ventana.
     */
    void on_pb_exit_clicked();

    /**
     * @brief on_rb_yesMask_clicked Se habilita el ComboBox de selección de máscara.
     */
    void on_rb_yesMask_clicked();

    /**
     * @brief on_rb_noMask_clicked Se deshabilita el ComboBox de selección de máscara y se deja el gráfico correspondiente al histograma de la imagen original.
     */
    void on_rb_noMask_clicked();

    /**
     * @brief on_cb_masks_currentIndexChanged Cada vez que exista un cambio en la selección del ComboBox, se realiza el cálculo del nuevo histograma y de cambia el gráfico actual.
     * @param index
     */
    void on_cb_masks_currentIndexChanged(int index);

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
     * @brief hist Arreglo que contiene el histograma de la imagen con máscara.
     */
    vector<int> histMask;
    /**
     * @brief lowerValue Valor de gris mínimo del histograma.
     */
    int lowerValue;
    /**
     * @brief upperValue Valor de gris máximo del histograma.
     */
    int upperValue;
    /**
     * @brief lowerValue Valor de gris mínimo del histograma con máscara.
     */
    int lowerValueMask;
    /**
     * @brief upperValue Valor de gris máximo del histograma con máscara.
     */
    int upperValueMask;
    /**
     * @brief minBoxHUValueHasChange Variable para saber si aun no se han hecho cambios en el minBoxHUValue.
     *True : El valor del boxHUValue ha cambiado almenos una vez.
     *False : El valor del boxHUValue no ha cambiado desde que se creó.
     */
    bool minBoxHUValueHasChange;
    /**
     * @brief maxBoxHUValueHasChange Variable para saber si aun no se han hecho cambios en el maxBoxHUValue.
     *True : El valor del boxHUValue ha cambiado almenos una vez.
     *False : El valor del boxHUValue no ha cambiado desde que se creó.
     */
    bool maxBoxHUValueHasChange;
    /**
     * @brief chartType Indica el tipo de gráfico en que se presentará el histograma, puede ser continuo, barras, puntos o linea.
     */
    int chartType;
    /**
     * @brief view Vista en donde se configura la tabla que contiene los datos que van a ser dibujados.
     */
    vtkSmartPointer<vtkContextView> view;
    /**
     * @brief usingMask Variable que indica si se está usando el histograma de la imagen original o de la imagen con máscara.
     * True: Si se usa imagen con máscara.
     * False : Se usa únicamente la imagen original.
     */
    bool usingMask;

};

#endif // HISTOGRAMFORM_H
