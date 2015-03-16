#include "histogramForm.h"
#include "ui_histogramForm.h"

/**
 * @brief HistogramForm::HistogramForm Constructor.
 * @param parent Padre que crea esta ventana.
 * @param lowerValue Valor mínimo de gris que tiene el histograma. 0 por defecto.
 * @param upperValue Valor máximo de gris que tiene el histograma. 0 por defecto.
 */
HistogramForm::HistogramForm(QWidget *parent, int lowerValue , int upperValue) :
    QDialog(parent),
    ui(new Ui::HistogramForm)
{
    ui->setupUi(this);

    //Valores iniciales de variables. Falso ya que no se ha mostrado la ventana.
    minBoxHUValueHasChange = false;
    maxBoxHUValueHasChange = false;
    chartType=vtkChart::BAR;

    this->upperValue = upperValue;
    this->lowerValue = lowerValue;

    //Se les da el valor inicial y el valor final de cada uno de los sliders y de los spinbox.
    //A los que corresponden al valor inicial se les pone un valor de -1 al máximo.
    this->ui->minSliderHUValue->setMinimum(this->lowerValue);
    this->ui->minSliderHUValue->setMaximum(this->upperValue-1);
    this->ui->minBoxHUValues->setMinimum(this->lowerValue);
    this->ui->minBoxHUValues->setMaximum(this->upperValue-1);
    //A los que corresponden al valor final se les pone un valor de +1 al mínimo.
    this->ui->maxSliderHUValue->setMinimum(this->lowerValue+1);
    this->ui->maxSliderHUValue->setMaximum(this->upperValue);
    this->ui->maxBoxHUValues->setMinimum(this->lowerValue+1);
    this->ui->maxBoxHUValues->setMaximum(this->upperValue);

    view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderer()->SetBackground(0.2, 0.2, 0.2);

    //Conecta la señal del slider (cambio del valor de HU mínimo en la barra) con el slot del spinBox (cambio del valor de HU mínimo en el item).
    //Cuando se cambia el valor en el slider de valor Mínimo, se ve reflejado ese valor en el spinBox de valor Mínimo.
    connect(this->ui->minSliderHUValue,SIGNAL(valueChanged(int)),
            this->ui->minBoxHUValues,SLOT(setValue(int)));

    //Conecta la señal del spinBox (cambio del valor de HU mínimo en el item) con el slot del spinBox (cambio del valor de HU mínimo en la barra).
    //Cuando se cambia el valor en el spinBox de valor Mínimo, se ve reflejado ese valor en el slider de valor Mínimo.
    connect(this->ui->minBoxHUValues,SIGNAL(valueChanged(int)),
            this->ui->minSliderHUValue,SLOT(setValue(int)));

    //Conecta la señal del slider (cambio del valor de HU máximo en la barra) con el slot del spinBox (cambio del valor de HU máximo en el item).
    //Cuando se cambia el valor en el slider de valor Máximo, se ve reflejado ese valor en el spinBox de valor Máximo.
    connect(this->ui->maxSliderHUValue,SIGNAL(valueChanged(int)),
            this->ui->maxBoxHUValues,SLOT(setValue(int)));

    //Conecta la señal del spinBox (cambio del valor de HU máximo en el item) con el slot del spinBox (cambio del valor de HU máximo en la barra).
    //Cuando se cambia el valor en el spinBox de valor Máximo, se ve reflejado ese valor en el slider de valor Máximo.
    connect(this->ui->maxBoxHUValues,SIGNAL(valueChanged(int)),
            this->ui->maxSliderHUValue,SLOT(setValue(int)));

}

/**
 * @brief HistogramForm::~HistogramForm Destructor.
 */
HistogramForm::~HistogramForm()
{
    delete ui;
}

/**
 * @brief HistogramForm::setCoordinator Asigna el coordinador al objeto de esta clase.
 * @param coordinator Coordinador.
 */
void HistogramForm::setCoordinator (Coordinator * coordinator){
    this->coordinator = coordinator;
}

/**
 * @brief HistogramForm::setHistogram Asigna un histograma al histograma de esta ventana.
 * @param hist Arreglo que contiene un histograma.
 */
void HistogramForm::setHistogram(vector<int> hist){
    this->hist = hist;
}

/**
 * @brief HistogramForm::setLowerValue Asigna el valor de gris mínimo.
 * @param lowerValue Valor de gris mínimo.
 */
void HistogramForm::setLowerValue(int lowerValue){
    this->lowerValue = lowerValue;
}

/**
 * @brief HistogramForm::setUpperValue Asigna el valor de gris máximo.
 * @param upperValue Valor de gris máximo.
 */
void HistogramForm::setUpperValue(int upperValue){
    this->upperValue = upperValue;
}
/**
 * @brief HistogramForm::on_minBoxHUValues_valueChanged Cuando hay un cambio en el valor de gris mínimo, se verifica que dicho valor no sobrepase el valor de gris máximo, de ser así el valor máximo siempre estará un valor de gris mas arriba.
 * @param actualValue valor actual en que se encuentra el valor de gris mínimo.
 */
void HistogramForm::on_minBoxHUValues_valueChanged(int actualValue)
{
    if(actualValue >=  this->ui->maxBoxHUValues->value()) {
        this->ui->maxBoxHUValues->setValue(actualValue+1);
    }

    if (!minBoxHUValueHasChange) {
        minBoxHUValueHasChange = true;
    } else {
         changeHistogramBarView(actualValue , this->ui->maxBoxHUValues->value());
    }
}

/**
 * @brief HistogramForm::on_maxBoxHUValues_valueChanged Cuando hay un cambio en el valor de gris máximo, se verifica que dicho valor no sobrepase el valor de gris mínimo, de ser así el valor mínimo siempre estará un valor de gris mas abajo.
 * @param actualValue valor actual en que se encuentra el valor de gris máximo.
 */
void HistogramForm::on_maxBoxHUValues_valueChanged(int actualValue)
{
    if(actualValue <=  this->ui->minBoxHUValues->value()) {
        this->ui->minBoxHUValues->setValue(actualValue-1);
    }

    if (!maxBoxHUValueHasChange) {
        maxBoxHUValueHasChange = true;
    } else {
         changeHistogramBarView(this->ui->minBoxHUValues->value() , actualValue);
    }

}

/**
 * @brief HistogramForm::changeHistogramView Dibuja un diagrama (histograma) en el qvtkWidget. El tipo de diagrama depende del valor de Chart.
 * @param minValue Valor mínimo desde el cual se dibujará el histograma.
 * @param maxValue Valor máximo hasta el cual se dibujará el histograma.
 * Tomado de : http://www.paraview.org/Wiki/VTK/Examples/Cxx/Plotting/BarChart
 */
void HistogramForm:: changeHistogramBarView(int minValue , int maxValue ) {

    //Para quitar las anteriores tablas que pueden existir en la view.
    view->GetScene()->ClearItems();

    //Se configura la escena 2D y se agrega un diagrama XY.
    vtkSmartPointer<vtkChartXY> chart = vtkSmartPointer<vtkChartXY>::New();
    view->GetScene()->AddItem(chart);
    chart->SetBarWidthFraction(1.0);

    // Creación de la tabla.
    vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

    vtkSmartPointer<vtkIntArray> arrHUValue = vtkSmartPointer<vtkIntArray>::New();
    arrHUValue->SetName("HU Value");
    table->AddColumn(arrHUValue);

    vtkSmartPointer<vtkIntArray> freq = vtkSmartPointer<vtkIntArray>::New();
    freq->SetName("Frequency");
    table->AddColumn(freq);

     // Se llena la tabla con los valores del histograma
    table->SetNumberOfRows(maxValue-minValue+1);
    for (int i = 0; i < maxValue-minValue+1; i++)
    {
        table->SetValue(i,0,i+minValue);
        table->SetValue(i,1,hist[i+(minValue-lowerValue)]);
    }

    // Se agregan las caracteristicas del diagrama.
    vtkPlot *line = 0;

    line = chart->AddPlot(chartType);
    #if VTK_MAJOR_VERSION <= 5
        line->SetInput(table, 0, 1);
    #else
        line->SetInputData(table, 0, 1);
    #endif
        line->SetColor(0, 255, 0, 255);

    //Se renderiza la imagen
    this->view->SetInteractor(this->ui->qvtkhistogram->GetInteractor());
    this->view->GetRenderWindow()->SetMultiSamples(0);
    this->ui->qvtkhistogram->SetRenderWindow(this->view->GetRenderWindow());

    //Métodos para decirle a los componentes que se han hecho cambios.
    this->view->Modified();
    this->view->Update();
    this->ui->qvtkhistogram->update();
}

/**
 * @brief HistogramForm::on_rb_line_clicked Cuando se presiona el radioButton correspondiente, se cambia al diagrama de linea.
 */
void HistogramForm::on_rb_line_clicked()
{
    if (this->ui->rb_line->isChecked()){
        this->chartType = chartType=vtkChart::LINE;
        changeHistogramBarView(this->ui->minBoxHUValues->value() , this->ui->maxBoxHUValues->value());
    }
}

/**
 * @brief HistogramForm::on_rb_bar_clicked Cuando se presiona el radioButton correspondiente, se cambia al diagrama de barras.
 */
void HistogramForm::on_rb_bar_clicked()
{
    if (this->ui->rb_bar->isChecked()){
        this->chartType = chartType=vtkChart::BAR;
        changeHistogramBarView(this->ui->minBoxHUValues->value() , this->ui->maxBoxHUValues->value());
    }
}

/**
 * @brief HistogramForm::on_rb_points_clicked Cuando se presiona el radioButton correspondiente, se cambia al diagrama de puntos.
 */
void HistogramForm::on_rb_points_clicked()
{
    if (this->ui->rb_points->isChecked()){
        this->chartType = chartType=vtkChart::POINTS;
        changeHistogramBarView(this->ui->minBoxHUValues->value() , this->ui->maxBoxHUValues->value());
    }
}

/**
 * @brief HistogramForm::on_rb_stacked_clicked Cuando se presiona el radioButton correspondiente, se cambia al diagrama acumulado.
 */
void HistogramForm::on_rb_stacked_clicked()
{
    if (this->ui->rb_stacked->isChecked()){
        this->chartType = chartType=vtkChart::STACKED;
        changeHistogramBarView(this->ui->minBoxHUValues->value() , this->ui->maxBoxHUValues->value());
    }
}

/**
 * @brief HistogramForm::on_pb_exit_clicked Se cierra la ventana.
 */
void HistogramForm::on_pb_exit_clicked()
{
    this->close();
}
