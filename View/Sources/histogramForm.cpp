#include "histogramForm.h"
#include "ui_histogramForm.h"


HistogramForm::HistogramForm(QWidget *parent, int lowerValue , int upperValue) :
    QDialog(parent),
    ui(new Ui::HistogramForm)
{
    ui->setupUi(this);

    //Valores iniciales de variables. Falso ya que no se ha mostrado la ventana.
    minBoxHUValueHasChange = false;
    maxBoxHUValueHasChange = false;
    chartType=vtkChart::BAR;

    //Para indicar que no se esta usando máscara.
    this->usingMask = false;

    //Para ajustar los valores inicial y final de los sliders.
    this->upperValue = upperValue;
    this->lowerValue = lowerValue;
    this->setLimitValues();

    view = vtkSmartPointer<vtkContextView>::New();
    view->GetRenderer()->SetBackground(0.2, 0.2, 0.2);

    //Se deshabilita el combobox de máscaras ya que inicialmente solo se va a ver el histograma de la imagen original.
    this->ui->cb_masks->setEnabled(false);
    this->ui->rb_noMask->setChecked(true);


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

HistogramForm::~HistogramForm()
{
    delete ui;
}

void HistogramForm::setCoordinator (Coordinator * &coordinator){
    this->coordinator = coordinator;
}

void HistogramForm::setHistogram(vector<int> hist){
    this->hist = hist;
}

void HistogramForm::setMasksList (QStringList masksList){
     this->ui->cb_masks->addItems(masksList);
}

void HistogramForm::setLimitValues (){

    int lower, upper;

    if (!this->usingMask){
        lower = this->lowerValue;
        upper = this->upperValue;
    }
    else {
        lower = this->lowerValueMask;
        upper = this->upperValueMask;
    }

    //Se les da el valor inicial y el valor final de cada uno de los sliders y de los spinbox.
    //A los que corresponden al valor inicial se les pone un valor de -1 al máximo.
    this->ui->minSliderHUValue->setMinimum(lower);
    this->ui->minSliderHUValue->setMaximum(upper-1);
    this->ui->minBoxHUValues->setMinimum(lower);
    this->ui->minBoxHUValues->setMaximum(upper-1);
    //A los que corresponden al valor final se les pone un valor de +1 al mínimo.
    this->ui->maxSliderHUValue->setMinimum(lower+1);
    this->ui->maxSliderHUValue->setMaximum(upper);
    this->ui->maxBoxHUValues->setMinimum(lower+1);
    this->ui->maxBoxHUValues->setMaximum(upper);

    //Valores iniciales:
    this->ui->minSliderHUValue->setValue(lower);
    this->ui->maxSliderHUValue->setValue(upper);
    this->ui->minBoxHUValues->setValue(lower);
    this->ui->maxBoxHUValues->setValue(upper);

}

void HistogramForm:: changeHistogramView(int minValue , int maxValue ) {

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

        if (!this->usingMask){
            table->SetValue(i,1,hist[i+(minValue-lowerValue)]);
        }
        else {
            table->SetValue(i,1,histMask[i+(minValue-lowerValueMask)]);
        }
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

void HistogramForm::on_minBoxHUValues_valueChanged(int actualValue)
{
    if(actualValue >=  this->ui->maxBoxHUValues->value()) {
        this->ui->maxBoxHUValues->setValue(actualValue+1);
    }

    if (!minBoxHUValueHasChange) {
        minBoxHUValueHasChange = true;
    } else {
         changeHistogramView(actualValue , this->ui->maxBoxHUValues->value());
    }
}

void HistogramForm::on_maxBoxHUValues_valueChanged(int actualValue)
{
    if(actualValue <=  this->ui->minBoxHUValues->value()) {
        this->ui->minBoxHUValues->setValue(actualValue-1);
    }

    if (!maxBoxHUValueHasChange) {
        maxBoxHUValueHasChange = true;
    } else {
         changeHistogramView(this->ui->minBoxHUValues->value() , actualValue);
    }

}

void HistogramForm::on_rb_line_clicked()
{
    if (this->ui->rb_line->isChecked()){
        this->chartType = chartType=vtkChart::LINE;
        changeHistogramView(this->ui->minBoxHUValues->value() , this->ui->maxBoxHUValues->value());
    }
}

void HistogramForm::on_rb_bar_clicked()
{
    if (this->ui->rb_bar->isChecked()){
        this->chartType = chartType=vtkChart::BAR;
        changeHistogramView(this->ui->minBoxHUValues->value() , this->ui->maxBoxHUValues->value());
    }
}

void HistogramForm::on_rb_points_clicked()
{
    if (this->ui->rb_points->isChecked()){
        this->chartType = chartType=vtkChart::POINTS;
        changeHistogramView(this->ui->minBoxHUValues->value() , this->ui->maxBoxHUValues->value());
    }
}

void HistogramForm::on_rb_stacked_clicked()
{
    if (this->ui->rb_stacked->isChecked()){
        this->chartType = chartType=vtkChart::STACKED;
        changeHistogramView(this->ui->minBoxHUValues->value() , this->ui->maxBoxHUValues->value());
    }
}

void HistogramForm::on_pb_exit_clicked()
{
    this->close();
}

void HistogramForm::on_rb_yesMask_clicked()
{
    this->ui->cb_masks->setEnabled(true);
}

void HistogramForm::on_rb_noMask_clicked()
{
    this->ui->cb_masks->setEnabled(false);
    this->usingMask = false;
    this->setLimitValues();
}

void HistogramForm::on_cb_masks_currentIndexChanged(int index)
{
    vector<int> data;

    if (index == 0) {
        this->usingMask = false;
        this->setLimitValues();
    }
    else {
        int lower,upper;
        data = this->coordinator->getHistogramData(lower,upper,index);

        this->upperValueMask = upper;
        this->lowerValueMask = lower;

        this->usingMask = true;

        this->histMask = data;
        this->setLimitValues();
    }
}
