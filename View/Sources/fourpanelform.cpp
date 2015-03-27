#include "fourpanelform.h"
#include "ui_fourpanelform.h"

/**
 * @brief The vtkResliceCursorCallback class Clase que realiza la interacción de los cuatro paneles al momento de cambiar parámetros como el contraste de la imágen.
 * Ejemplo de 4 páneles de QT-VTK
 */
class vtkResliceCursorCallback : public vtkCommand
{
public:
  static vtkResliceCursorCallback *New()
  {
      return new vtkResliceCursorCallback;
  }

  void Execute( vtkObject *caller, unsigned long ev, void *callData )
  {
    if (ev == vtkResliceCursorWidget::WindowLevelEvent ||
        ev == vtkCommand::WindowLevelEvent ||
        ev == vtkResliceCursorWidget::ResliceThicknessChangedEvent)
    {
      // Render everything
      for (int i = 0; i < 3; i++){
        this->RCW[i]->Render();
      }
      this->IPW[0]->GetInteractor()->GetRenderWindow()->Render();
      return;
    }

    vtkImagePlaneWidget* ipw = dynamic_cast< vtkImagePlaneWidget* >( caller );
    if (ipw)
    {
        double* wl = static_cast<double*>( callData );
        if ( ipw == this->IPW[0] )
        {
            this->IPW[1]->SetWindowLevel(wl[0],wl[1],1);
            this->IPW[2]->SetWindowLevel(wl[0],wl[1],1);

        }else if( ipw == this->IPW[1] ){
            this->IPW[0]->SetWindowLevel(wl[0],wl[1],1);
            this->IPW[2]->SetWindowLevel(wl[0],wl[1],1);

        }else if (ipw == this->IPW[2]){
            this->IPW[0]->SetWindowLevel(wl[0],wl[1],1);
            this->IPW[1]->SetWindowLevel(wl[0],wl[1],1);
        }
    }

    vtkResliceCursorWidget *rcw = dynamic_cast<vtkResliceCursorWidget * >(caller);
    if (rcw) {
        cout<<"here"<<endl;

        vtkResliceCursorLineRepresentation *rep = dynamic_cast<vtkResliceCursorLineRepresentation * >(rcw->GetRepresentation());
        // Although the return value is not used, we keep the get calls
        // in case they had side-effects
        rep->GetResliceCursorActor()->GetCursorAlgorithm()->GetResliceCursor();
        for (int i = 0; i < 3; i++) {
            vtkPlaneSource *ps = static_cast< vtkPlaneSource * > (this->IPW[i]->GetPolyDataAlgorithm());
            ps->SetOrigin(this->RCW[i]->GetResliceCursorRepresentation()->GetPlaneSource()->GetOrigin());
            ps->SetPoint1(this->RCW[i]->GetResliceCursorRepresentation()->GetPlaneSource()->GetPoint1());
            ps->SetPoint2(this->RCW[i]->GetResliceCursorRepresentation()->GetPlaneSource()->GetPoint2());

            // If the reslice plane has modified, update it on the 3D widget
            this->IPW[i]->UpdatePlacement();
        }
    }
    // Render everything
    for (int i = 0; i < 3; i++) {
        this->RCW[i]->Render();
    }
    this->IPW[0]->GetInteractor()->GetRenderWindow()->Render();
  }

  vtkResliceCursorCallback() {}
  vtkImagePlaneWidget* IPW[3];
  vtkResliceCursorWidget *RCW[3];
};


FourPanelForm::FourPanelForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FourPanelForm)
{
    ui->setupUi(this);

    //Conecta la señal del scrollBar (cambio del valor del slice en la barra) con el slot del spinBox (cambio del valor del slice en el item).
    //Cuando se cambia el valor en el scroolbar de vista 1, se ve reflejado ese valor en el scrollbar de vista1.
    connect(this->ui->scroll_view1,SIGNAL(valueChanged(int)),
            this->ui->spinBoxView1,SLOT(setValue(int)));

    //Conecta la señal del scrollBar (cambio del valor del slice en la barra) con el slot del spinBox (cambio del valor del slice en el item).
    //Cuando se cambia el valor en el scroolbar de vista 2, se ve reflejado ese valor en el scrollbar de vista2.
    connect(this->ui->scroll_view2,SIGNAL(valueChanged(int)),
            this->ui->spinBoxView2,SLOT(setValue(int)));

    //Conecta la señal del scrollBar (cambio del valor del slice en la barra) con el slot del spinBox (cambio del valor del slice en el item).
    //Cuando se cambia el valor en el scroolbar de vista 3, se ve reflejado ese valor en el scrollbar de vista3.
    connect(this->ui->scroll_view3,SIGNAL(valueChanged(int)),
            this->ui->spinBoxView3,SLOT(setValue(int)));
}

FourPanelForm::~FourPanelForm()
{
    delete ui;
}

void FourPanelForm::setUpImages(vtkImageData * image){

      int imageDims[3];
      image->GetDimensions(imageDims);

      for (int i = 0; i < 3; i++)
      {
        riw[i] = vtkSmartPointer< vtkResliceImageViewer >::New();
      }

      this->ui->qvtkWindow1->SetRenderWindow(riw[0]->GetRenderWindow());
      riw[0]->SetupInteractor(this->ui->qvtkWindow1->GetRenderWindow()->GetInteractor());

      this->ui->qvtkWindow2->SetRenderWindow(riw[1]->GetRenderWindow());
      riw[1]->SetupInteractor(this->ui->qvtkWindow2->GetRenderWindow()->GetInteractor());

      this->ui->qvtkWindow3->SetRenderWindow(riw[2]->GetRenderWindow());
      riw[2]->SetupInteractor(this->ui->qvtkWindow3->GetRenderWindow()->GetInteractor());

      for (int i = 0; i < 3; i++)
      {
         // make them all share the same reslice cursor object.
         vtkResliceCursorLineRepresentation *rep = vtkResliceCursorLineRepresentation::SafeDownCast(
                                                    riw[i]->GetResliceCursorWidget()->GetRepresentation());
         riw[i]->SetResliceCursor(riw[0]->GetResliceCursor());

         rep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(i);

         riw[i]->SetInputData(image);
         riw[i]->SetSliceOrientation(i);
         riw[i]->SetResliceModeToAxisAligned();
      }

      vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
      picker->SetTolerance(0.005);

      vtkSmartPointer<vtkProperty> ipwProp = vtkSmartPointer<vtkProperty>::New();

      vtkSmartPointer< vtkRenderer > ren = vtkSmartPointer< vtkRenderer >::New();


      //Si el qvtkwidget4 no tiene algún renderer no se hace nada.
      //Si el qvtkwidget4 tiene algún renderer, entonces se elimina el anterior y se sigue el proceso.
      //Es necesario eliminar cualquier otro renderer que exista en el widget, ya que de lo contrario no se podría hacer una interacción con este.
      if (this->ui->qvtkWindow4->GetRenderWindow()->GetRenderers()->GetFirstRenderer() != NULL){
          this->ui->qvtkWindow4->GetRenderWindow()->RemoveRenderer(
                      this->ui->qvtkWindow4->GetRenderWindow()->GetRenderers()->GetFirstRenderer()
                      );
      }


      this->ui->qvtkWindow4->GetRenderWindow()->AddRenderer(ren);
      vtkRenderWindowInteractor *iren = this->ui->qvtkWindow4->GetInteractor();


      for (int i = 0; i < 3; i++)
      {
         planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
         planeWidget[i]->SetInteractor( iren );
         planeWidget[i]->SetPicker(picker);
         planeWidget[i]->RestrictPlaneToVolumeOn();
         double color[3] = {0, 0, 0};
         color[i] = 1;
         planeWidget[i]->GetPlaneProperty()->SetColor(color);

         color[0] /= 4.0;
         color[1] /= 4.0;
         color[2] /= 4.0;
         riw[i]->GetRenderer()->SetBackground( color );

         planeWidget[i]->SetTexturePlaneProperty(ipwProp);
         planeWidget[i]->TextureInterpolateOff();
         planeWidget[i]->SetResliceInterpolateToLinear();
         planeWidget[i]->SetInputData(image);
         planeWidget[i]->SetPlaneOrientation(i);
         planeWidget[i]->SetSliceIndex(imageDims[i]/2);
         planeWidget[i]->DisplayTextOn();
         planeWidget[i]->SetDefaultRenderer(ren);
         planeWidget[i]->SetWindowLevel(1358, -27);
         planeWidget[i]->On();
         planeWidget[i]->InteractionOn();
      }

      vtkSmartPointer<vtkResliceCursorCallback> cbk = vtkSmartPointer<vtkResliceCursorCallback>::New();
      for (int i = 0; i < 3; i++)
      {
          cbk->IPW[i] = planeWidget[i];
          cbk->RCW[i] = riw[i]->GetResliceCursorWidget();
          riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );
          riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::WindowLevelEvent, cbk );
          riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResliceThicknessChangedEvent, cbk );
          riw[i]->GetResliceCursorWidget()->AddObserver(vtkResliceCursorWidget::ResetCursorEvent, cbk );
          riw[i]->GetInteractorStyle()->AddObserver(vtkCommand::WindowLevelEvent, cbk );

          // Make them all share the same color map.
          riw[i]->SetLookupTable(riw[0]->GetLookupTable());
          planeWidget[i]->GetColorMap()->SetLookupTable(riw[0]->GetLookupTable());
          //planeWidget[i]->GetColorMap()->SetInput(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap()->GetInput());
          planeWidget[i]->SetColorMap(riw[i]->GetResliceCursorWidget()->GetResliceCursorRepresentation()->GetColorMap());
      }

      this->ui->qvtkWindow1->show();
      this->ui->qvtkWindow2->show();
      this->ui->qvtkWindow3->show();


      //Se asignan los rangos a los ScrollViews y a los SpinBox.
      //Se les asigna un valor +1 para que se inicie desde 1 y no desde 0.
      this->ui->scroll_view1->setRange(riw[0]->GetSliceMin()+1,riw[0]->GetSliceMax()+1);
      this->ui->scroll_view1->setValue(imageDims[0]/2+1);
      this->ui->spinBoxView1->setRange(riw[0]->GetSliceMin()+1,riw[0]->GetSliceMax()+1);
      this->ui->spinBoxView1->setValue(imageDims[0]/2+1);

      this->ui->scroll_view2->setRange(riw[1]->GetSliceMin()+1,riw[1]->GetSliceMax()+1);
      this->ui->scroll_view2->setValue(imageDims[1]/2+1);
      this->ui->spinBoxView2->setRange(riw[1]->GetSliceMin()+1,riw[1]->GetSliceMax()+1);
      this->ui->spinBoxView2->setValue(imageDims[1]/2+1);

      this->ui->scroll_view3->setRange(riw[2]->GetSliceMin()+1,riw[2]->GetSliceMax()+1);
      this->ui->scroll_view3->setValue(imageDims[2]/2+1);
      this->ui->spinBoxView3->setRange(riw[2]->GetSliceMin()+1,riw[2]->GetSliceMax()+1);
      this->ui->spinBoxView3->setValue(imageDims[2]/2+1);

}

void FourPanelForm::setCoordinator (Coordinator * &coordinator){
    this->coordinator = coordinator;
}

void FourPanelForm::modifyViews(int view){

    //Transversal
    if (view == 1){

        this->ui->frameView1->setVisible(false);
        this->ui->frameView2->setVisible(false);
        this->ui->frameView4->setVisible(false);

        if (!this->ui->frameView3->isVisible()){
            this->ui->frameView3->setVisible(true);
        }
    }

    //Coronal
    if (view == 2){
        this->ui->frameView1->setVisible(false);
        this->ui->frameView3->setVisible(false);
        this->ui->frameView4->setVisible(false);


        if (!this->ui->frameView2->isVisible()){
            this->ui->frameView2->setVisible(true);
        }
    }

    //Sagital
    if (view == 3){
        this->ui->frameView2->setVisible(false);
        this->ui->frameView3->setVisible(false);
        this->ui->frameView4->setVisible(false);

        if (!this->ui->frameView1->isVisible()){
            this->ui->frameView1->setVisible(true);
        }
    }

    //Volumen
    if (view == 4) {
        this->ui->frameView1->setVisible(false);
        this->ui->frameView2->setVisible(false);
        this->ui->frameView3->setVisible(false);

        if (!this->ui->frameView4->isVisible()){
            this->ui->frameView4->setVisible(true);
        }
    }

    //Todas las vistas
    if (view == 5) {
        this->ui->frameView1->setVisible(true);
        this->ui->frameView2->setVisible(true);
        this->ui->frameView3->setVisible(true);
        this->ui->frameView4->setVisible(true);
    }
}

void FourPanelForm::changeViews(int value, int element){

    //Se le resta un valor de 1 (-1) al slice que debe mostrarse, ya que el valor dado por el scrollView o el spinBox da un valor mas.
    if (element!=3){
        this->riw[element]->SetSlice(value-1);
        this->planeWidget[element]->SetSliceIndex(this->riw[element]->GetSlice());
        this->ui->qvtkWindow4->update();
    }
    else {
        this->riw[0]->SetSlice(riw[0]->GetSliceMax()/2);
        this->riw[1]->SetSlice(riw[1]->GetSliceMax()/2);
        this->riw[2]->SetSlice(riw[2]->GetSliceMax()/2);

        this->ui->scroll_view1->setValue(riw[0]->GetSliceMax()/2);
        this->ui->scroll_view2->setValue(riw[1]->GetSliceMax()/2);
        this->ui->scroll_view3->setValue(riw[2]->GetSliceMax()/2);
    }

    this->ui->qvtkWindow4->update();
}

void FourPanelForm::on_scroll_view1_valueChanged(int value)
{
    changeViews(value, 0);
}

void FourPanelForm::on_scroll_view2_valueChanged(int value)
{
    changeViews(value, 1);
}

void FourPanelForm::on_scroll_view3_valueChanged(int value)
{
    changeViews(value, 2);
}

void FourPanelForm::on_pb_resetViews_2_clicked()
{
    changeViews(0, 3);
}

void FourPanelForm::on_spinBoxView1_valueChanged(int value)
{
    this->ui->scroll_view1->setValue(value);
}

void FourPanelForm::on_spinBoxView2_valueChanged(int value)
{
    this->ui->scroll_view2->setValue(value);
}

void FourPanelForm::on_spinBoxView3_valueChanged(int value)
{
    this->ui->scroll_view3->setValue(value);
}
