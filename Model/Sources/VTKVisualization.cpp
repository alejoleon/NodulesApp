#include "VTKVisualization.h"

//Variable global:
ImageType::Pointer imagePrueba1;
ImageBinaryType::Pointer imagePrueba2;

// Ejemplo de : http://www.vtk.org/Wiki/VTK/Examples/Cxx/IO/ReadDICOMSeries
// helper class to format slice status message
class StatusMessage
{
public:
    static std::string Format(int slice, int maxSlice)
    {
	std::stringstream tmp;
	tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
	return tmp.str();
    }
};

// Define own interaction style
class myVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
    static myVtkInteractorStyleImage* New();
    vtkTypeMacro(myVtkInteractorStyleImage, vtkInteractorStyleImage);

protected:
    vtkImageViewer2* _ImageViewer;
    vtkTextMapper* _StatusMapper;
    int _Slice;
    int _MinSlice;
    int _MaxSlice;

public:
    void SetImageViewer(vtkImageViewer2* imageViewer)
    {
	_ImageViewer = imageViewer;
	_MinSlice = imageViewer->GetSliceMin();
	_MaxSlice = imageViewer->GetSliceMax();
	_Slice = _MinSlice;
	cout << "Slicer: Min = " << _MinSlice << ", Max = " << _MaxSlice << std::endl;
    }

    void SetStatusMapper(vtkTextMapper* statusMapper)
    {
	_StatusMapper = statusMapper;
    }

protected:
    void MoveSliceForward()
    {
	if(_Slice < _MaxSlice) {
	    _Slice += 1;
	    cout << "MoveSliceForward::Slice = " << _Slice << std::endl;
	    _ImageViewer->SetSlice(_Slice);
	    std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
	    _StatusMapper->SetInput(msg.c_str());
	    _ImageViewer->Render();
	}
    }

    void MoveSliceBackward()
    {
	if(_Slice > _MinSlice) {
	    _Slice -= 1;
	    cout << "MoveSliceBackward::Slice = " << _Slice << std::endl;
	    _ImageViewer->SetSlice(_Slice);
	    std::string msg = StatusMessage::Format(_Slice, _MaxSlice);
	    _StatusMapper->SetInput(msg.c_str());
	    _ImageViewer->Render();
	}
    }

    virtual void OnKeyDown()
    {
	std::string key = this->GetInteractor()->GetKeySym();
	if(key.compare("Up") == 0) {
	    // cout << "Up arrow key was pressed." << endl;
	    MoveSliceForward();
	} else if(key.compare("Down") == 0) {
	    // cout << "Down arrow key was pressed." << endl;
	    MoveSliceBackward();
	}
	// forward event
	vtkInteractorStyleImage::OnKeyDown();
    }

    virtual void OnMouseWheelForward()
    {
	// std::cout << "Scrolled mouse wheel forward." << std::endl;
	MoveSliceForward();
	// don't forward events, otherwise the image will be zoomed
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelForward();
    }
	
    virtual void OnMouseWheelBackward()
    {
	// std::cout << "Scrolled mouse wheel backward." << std::endl;
	if(_Slice > _MinSlice) {
	    MoveSliceBackward();
	}
	// don't forward events, otherwise the image will be zoomed
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelBackward();
    }

    // Ejemplo en : http://www.vtk.org/Wiki/VTK/Examples/Cxx/Visualization/CaptionActor2D
	// http://www.paraview.org/Wiki/index.php?title=VTK/Examples/Cxx/Interaction/PointPicker&redirect=no
    virtual void OnLeftButtonDown()
    {
		cout<<"||||||||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
	std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " "
	          << this->Interactor->GetEventPosition()[1] << std::endl;
	this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
	                                    this->Interactor->GetEventPosition()[1],
	                                    0, // always zero.
	                                    this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
	double picked[3];
	this->Interactor->GetPicker()->GetPickPosition(picked);
	std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
	// Forward events
	// vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

	ImageType::IndexType currentIndex;
	currentIndex[0] = picked[0];
	currentIndex[1] = picked[1];
	currentIndex[2] = picked[2];
	
	/* ???
	ImageBinaryType::IndexType currentIndex2;
	currentIndex2[0] = picked[0];
	currentIndex2[1] = picked[1];
	currentIndex2[2] = picked[2];
	*/
	if(!imagePrueba2) {
		ImageType::PixelType currentValueIn = imagePrueba1->GetPixel(currentIndex);
		cout<<"valor en imagen de entrada: "<<currentValueIn<<endl;
	}else if (!imagePrueba1){

		ImageBinaryType::PixelType currentValueOut = imagePrueba2->GetPixel(currentIndex);
		cout<<"valor en imagen de salida: "<<(int)currentValueOut<<endl;
	}else{

		ImageType::PixelType currentValueIn = imagePrueba1->GetPixel(currentIndex);
		cout<<"valor en imagen de entrada: "<<currentValueIn<<endl;
		
		ImageBinaryType::PixelType currentValueOut = imagePrueba2->GetPixel(currentIndex);
		cout<<"valor en imagen de salida: "<<(int)currentValueOut<<endl;
	}
	
	cout<<"----------------------------------------------------"<<endl;
    }
};

vtkStandardNewMacro(myVtkInteractorStyleImage);


VTKVisualization::VTKVisualization()
{
	this->dir1="";
	this->dir2="";
}

VTKVisualization::~VTKVisualization()
{
}

void VTKVisualization::SetDir1(const string& dir1)
{
	this->dir1 = dir1;
}
void VTKVisualization::SetDir2(const string& dir2)
{
	this->dir2 = dir2;
}
const string& VTKVisualization::GetDir1() const
{
	return dir1;
}
const string& VTKVisualization::GetDir2() const
{
	return dir2;
}

/**
 * @brief Muestra las dos imágenes de los parámetros en dos ventanas de VTK para poder interactuar con ellas.
 * @param img1 Imagen de entrada.
 * @param img2 Imagen de salida.
 */
void VTKVisualization::readImages(ImageType::Pointer img1,ImageBinaryType::Pointer img2){
	imagePrueba1=img1;
	imagePrueba2=img2;
	
	string folder = this->dir1;
	string folder2 = this->dir2;
	//Entrada
   vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
   reader->SetDirectoryName(folder.c_str());
   reader->Update();
 
   // Visualize
   vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
   imageViewer->SetInputConnection(reader->GetOutputPort());
 
   // slice status message
   vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
   sliceTextProp->SetFontFamilyToCourier();
   sliceTextProp->SetFontSize(20);
   sliceTextProp->SetVerticalJustificationToBottom();
   sliceTextProp->SetJustificationToLeft();
 
   vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
   std::string msg = StatusMessage::Format(imageViewer->GetSliceMin(), imageViewer->GetSliceMax());
   sliceTextMapper->SetInput(msg.c_str());
   sliceTextMapper->SetTextProperty(sliceTextProp);
 
   vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
   sliceTextActor->SetMapper(sliceTextMapper);
   sliceTextActor->SetPosition(15, 10);
 
   // usage hint message
   vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
   usageTextProp->SetFontFamilyToCourier();
   usageTextProp->SetFontSize(14);
   usageTextProp->SetVerticalJustificationToTop();
   usageTextProp->SetJustificationToLeft();
 
   vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
   //usageTextMapper->SetInput("- Slice with mouse wheel\n  or Up/Down-Key\n- Zoom with pressed right\n  mouse button while dragging");
    usageTextMapper->SetInput("Entrada");
   usageTextMapper->SetTextProperty(usageTextProp);
 
   vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
   usageTextActor->SetMapper(usageTextMapper);
   usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
   usageTextActor->GetPositionCoordinate()->SetValue( 0.05, 0.95);
 
   // create an interactor with our own style (inherit from vtkInteractorStyleImage)
   // in order to catch mousewheel and key events
   vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
 
   vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
 
   // make imageviewer2 and sliceTextMapper visible to our interactorstyle
   // to enable slice status message updates when scrolling through the slices
   myInteractorStyle->SetImageViewer(imageViewer);
   myInteractorStyle->SetStatusMapper(sliceTextMapper);
 
   imageViewer->SetupInteractor(renderWindowInteractor);
   // make the interactor use our own interactorstyle
   // cause SetupInteractor() is defining it's own default interatorstyle 
   // this must be called after SetupInteractor()
   renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
   // add slice status message and usage hint message to the renderer
   imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
   imageViewer->GetRenderer()->AddActor2D(usageTextActor);
 
   // initialize rendering and interaction
   //imageViewer->GetRenderWindow()->SetSize(400, 300);
   //imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
   imageViewer->Render();
   imageViewer->GetRenderer()->ResetCamera();
   imageViewer->Render();
   
   //Salida
   vtkSmartPointer<vtkDICOMImageReader> reader2 = vtkSmartPointer<vtkDICOMImageReader>::New();
   reader2->SetDirectoryName(folder2.c_str());
   reader2->Update();
 
   // Visualize
   vtkSmartPointer<vtkImageViewer2> imageViewer2 = vtkSmartPointer<vtkImageViewer2>::New();
   imageViewer2->SetInputConnection(reader2->GetOutputPort());
 
   // slice status message
   vtkSmartPointer<vtkTextProperty> sliceTextProp2 = vtkSmartPointer<vtkTextProperty>::New();
   sliceTextProp2->SetFontFamilyToCourier();
   sliceTextProp2->SetFontSize(20);
   sliceTextProp2->SetVerticalJustificationToBottom();
   sliceTextProp2->SetJustificationToLeft();
 
   vtkSmartPointer<vtkTextMapper> sliceTextMapper2 = vtkSmartPointer<vtkTextMapper>::New();
   std::string msg2 = StatusMessage::Format(imageViewer2->GetSliceMin(), imageViewer2->GetSliceMax());
   sliceTextMapper2->SetInput(msg2.c_str());
   sliceTextMapper2->SetTextProperty(sliceTextProp2);
 
   vtkSmartPointer<vtkActor2D> sliceTextActor2 = vtkSmartPointer<vtkActor2D>::New();
   sliceTextActor2->SetMapper(sliceTextMapper2);
   sliceTextActor2->SetPosition(15, 10);
 
   // usage hint message
   vtkSmartPointer<vtkTextProperty> usageTextProp2 = vtkSmartPointer<vtkTextProperty>::New();
   usageTextProp2->SetFontFamilyToCourier();
   usageTextProp2->SetFontSize(14);
   usageTextProp2->SetVerticalJustificationToTop();
   usageTextProp2->SetJustificationToLeft();
 
   vtkSmartPointer<vtkTextMapper> usageTextMapper2 = vtkSmartPointer<vtkTextMapper>::New();
   //usageTextMapper->SetInput("- Slice with mouse wheel\n  or Up/Down-Key\n- Zoom with pressed right\n  mouse button while dragging");
   usageTextMapper2->SetInput("Salida");
   usageTextMapper2->SetTextProperty(usageTextProp2);
 
   vtkSmartPointer<vtkActor2D> usageTextActor2 = vtkSmartPointer<vtkActor2D>::New();
   usageTextActor2->SetMapper(usageTextMapper2);
   usageTextActor2->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
   usageTextActor2->GetPositionCoordinate()->SetValue( 0.05, 0.95);
 
   // create an interactor with our own style (inherit from vtkInteractorStyleImage)
   // in order to catch mousewheel and key events
   vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor2 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
 
   vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle2 = vtkSmartPointer<myVtkInteractorStyleImage>::New();
 
   // make imageviewer2 and sliceTextMapper visible to our interactorstyle
   // to enable slice status message updates when scrolling through the slices
   myInteractorStyle2->SetImageViewer(imageViewer2);
   myInteractorStyle2->SetStatusMapper(sliceTextMapper2);
 
   imageViewer2->SetupInteractor(renderWindowInteractor2);
   // make the interactor use our own interactorstyle
   // cause SetupInteractor() is defining it's own default interatorstyle 
   // this must be called after SetupInteractor()
   renderWindowInteractor2->SetInteractorStyle(myInteractorStyle2);
   // add slice status message and usage hint message to the renderer
   imageViewer2->GetRenderer()->AddActor2D(sliceTextActor2);
   imageViewer2->GetRenderer()->AddActor2D(usageTextActor2);
 
   // initialize rendering and interaction
   //imageViewer->GetRenderWindow()->SetSize(400, 300);
   //imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
   imageViewer2->Render();
   imageViewer2->GetRenderer()->ResetCamera();
   imageViewer2->Render();
      
	  
	//Para mirar el valor del pixel// el metodo del click ya se puso en el estilo principal
/*
	vtkSmartPointer<MouseInteractorStylePP> style = vtkSmartPointer<MouseInteractorStylePP>::New();
	vtkSmartPointer<MouseInteractorStylePP> style2 = vtkSmartPointer<MouseInteractorStylePP>::New();
	  
	renderWindowInteractor->SetInteractorStyle( style );   
	renderWindowInteractor2->SetInteractorStyle( style2 );

  */
   renderWindowInteractor->Start();
   renderWindowInteractor2->Start();
}

/**
 * @brief Muestra la imagen de entrada que es pasada en el parametro, en una ventana de VTK para poder interactuar con ella.
 * @param img1 Imagen de entrada.
 */
 
void VTKVisualization::readImages(ImageType::Pointer img1){
	imagePrueba1=img1;
	
	string folder = this->dir1;
	
	//Entrada
   vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
   reader->SetDirectoryName(folder.c_str());
   reader->Update();
 
   // Visualize
   vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
   imageViewer->SetInputConnection(reader->GetOutputPort());
 
   // slice status message
   vtkSmartPointer<vtkTextProperty> sliceTextProp = vtkSmartPointer<vtkTextProperty>::New();
   sliceTextProp->SetFontFamilyToCourier();
   sliceTextProp->SetFontSize(20);
   sliceTextProp->SetVerticalJustificationToBottom();
   sliceTextProp->SetJustificationToLeft();
 
   vtkSmartPointer<vtkTextMapper> sliceTextMapper = vtkSmartPointer<vtkTextMapper>::New();
   std::string msg = StatusMessage::Format(imageViewer->GetSliceMin(), imageViewer->GetSliceMax());
   sliceTextMapper->SetInput(msg.c_str());
   sliceTextMapper->SetTextProperty(sliceTextProp);
 
   vtkSmartPointer<vtkActor2D> sliceTextActor = vtkSmartPointer<vtkActor2D>::New();
   sliceTextActor->SetMapper(sliceTextMapper);
   sliceTextActor->SetPosition(15, 10);
 
   // usage hint message
   vtkSmartPointer<vtkTextProperty> usageTextProp = vtkSmartPointer<vtkTextProperty>::New();
   usageTextProp->SetFontFamilyToCourier();
   usageTextProp->SetFontSize(14);
   usageTextProp->SetVerticalJustificationToTop();
   usageTextProp->SetJustificationToLeft();
 
   vtkSmartPointer<vtkTextMapper> usageTextMapper = vtkSmartPointer<vtkTextMapper>::New();
   //usageTextMapper->SetInput("- Slice with mouse wheel\n  or Up/Down-Key\n- Zoom with pressed right\n  mouse button while dragging");
    usageTextMapper->SetInput("Entrada");
   usageTextMapper->SetTextProperty(usageTextProp);
 
   vtkSmartPointer<vtkActor2D> usageTextActor = vtkSmartPointer<vtkActor2D>::New();
   usageTextActor->SetMapper(usageTextMapper);
   usageTextActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
   usageTextActor->GetPositionCoordinate()->SetValue( 0.05, 0.95);
 
   // create an interactor with our own style (inherit from vtkInteractorStyleImage)
   // in order to catch mousewheel and key events
   vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
 
   vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle = vtkSmartPointer<myVtkInteractorStyleImage>::New();
 
   // make imageviewer2 and sliceTextMapper visible to our interactorstyle
   // to enable slice status message updates when scrolling through the slices
   myInteractorStyle->SetImageViewer(imageViewer);
   myInteractorStyle->SetStatusMapper(sliceTextMapper);
 
   imageViewer->SetupInteractor(renderWindowInteractor);
   // make the interactor use our own interactorstyle
   // cause SetupInteractor() is defining it's own default interatorstyle 
   // this must be called after SetupInteractor()
   renderWindowInteractor->SetInteractorStyle(myInteractorStyle);
   // add slice status message and usage hint message to the renderer
   imageViewer->GetRenderer()->AddActor2D(sliceTextActor);
   imageViewer->GetRenderer()->AddActor2D(usageTextActor);
 
   // initialize rendering and interaction
   //imageViewer->GetRenderWindow()->SetSize(400, 300);
   //imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
   imageViewer->Render();
   imageViewer->GetRenderer()->ResetCamera();
   imageViewer->Render();
   

	//Para mirar el valor del pixel// el metodo del click ya se puso en el estilo principal
/*
	vtkSmartPointer<MouseInteractorStylePP> style = vtkSmartPointer<MouseInteractorStylePP>::New();
	vtkSmartPointer<MouseInteractorStylePP> style2 = vtkSmartPointer<MouseInteractorStylePP>::New();
	  
	renderWindowInteractor->SetInteractorStyle( style );   
	renderWindowInteractor2->SetInteractorStyle( style2 );

  */
   renderWindowInteractor->Start();

}

/**
 * @brief Muestra la imagen de salida que es pasada en el parametro, en una ventana de VTK para poder interactuar con ella.
 * @param img2 Imagen de salida.
 */

void VTKVisualization::readImages(ImageBinaryType::Pointer img2){
	imagePrueba2=img2;
	
	string folder2 = this->dir2;
	
   //Salida
   vtkSmartPointer<vtkDICOMImageReader> reader2 = vtkSmartPointer<vtkDICOMImageReader>::New();
   reader2->SetDirectoryName(folder2.c_str());
   reader2->Update();
 
   // Visualize
   vtkSmartPointer<vtkImageViewer2> imageViewer2 = vtkSmartPointer<vtkImageViewer2>::New();
   imageViewer2->SetInputConnection(reader2->GetOutputPort());
 
   // slice status message
   vtkSmartPointer<vtkTextProperty> sliceTextProp2 = vtkSmartPointer<vtkTextProperty>::New();
   sliceTextProp2->SetFontFamilyToCourier();
   sliceTextProp2->SetFontSize(20);
   sliceTextProp2->SetVerticalJustificationToBottom();
   sliceTextProp2->SetJustificationToLeft();
 
   vtkSmartPointer<vtkTextMapper> sliceTextMapper2 = vtkSmartPointer<vtkTextMapper>::New();
   std::string msg2 = StatusMessage::Format(imageViewer2->GetSliceMin(), imageViewer2->GetSliceMax());
   sliceTextMapper2->SetInput(msg2.c_str());
   sliceTextMapper2->SetTextProperty(sliceTextProp2);
 
   vtkSmartPointer<vtkActor2D> sliceTextActor2 = vtkSmartPointer<vtkActor2D>::New();
   sliceTextActor2->SetMapper(sliceTextMapper2);
   sliceTextActor2->SetPosition(15, 10);
 
   // usage hint message
   vtkSmartPointer<vtkTextProperty> usageTextProp2 = vtkSmartPointer<vtkTextProperty>::New();
   usageTextProp2->SetFontFamilyToCourier();
   usageTextProp2->SetFontSize(14);
   usageTextProp2->SetVerticalJustificationToTop();
   usageTextProp2->SetJustificationToLeft();
 
   vtkSmartPointer<vtkTextMapper> usageTextMapper2 = vtkSmartPointer<vtkTextMapper>::New();
   //usageTextMapper->SetInput("- Slice with mouse wheel\n  or Up/Down-Key\n- Zoom with pressed right\n  mouse button while dragging");
   usageTextMapper2->SetInput("Salida");
   usageTextMapper2->SetTextProperty(usageTextProp2);
 
   vtkSmartPointer<vtkActor2D> usageTextActor2 = vtkSmartPointer<vtkActor2D>::New();
   usageTextActor2->SetMapper(usageTextMapper2);
   usageTextActor2->GetPositionCoordinate()->SetCoordinateSystemToNormalizedDisplay();
   usageTextActor2->GetPositionCoordinate()->SetValue( 0.05, 0.95);
 
   // create an interactor with our own style (inherit from vtkInteractorStyleImage)
   // in order to catch mousewheel and key events
   vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor2 = vtkSmartPointer<vtkRenderWindowInteractor>::New();
 
   vtkSmartPointer<myVtkInteractorStyleImage> myInteractorStyle2 = vtkSmartPointer<myVtkInteractorStyleImage>::New();
 
   // make imageviewer2 and sliceTextMapper visible to our interactorstyle
   // to enable slice status message updates when scrolling through the slices
   myInteractorStyle2->SetImageViewer(imageViewer2);
   myInteractorStyle2->SetStatusMapper(sliceTextMapper2);
 
   imageViewer2->SetupInteractor(renderWindowInteractor2);
   // make the interactor use our own interactorstyle
   // cause SetupInteractor() is defining it's own default interatorstyle 
   // this must be called after SetupInteractor()
   renderWindowInteractor2->SetInteractorStyle(myInteractorStyle2);
   // add slice status message and usage hint message to the renderer
   imageViewer2->GetRenderer()->AddActor2D(sliceTextActor2);
   imageViewer2->GetRenderer()->AddActor2D(usageTextActor2);
 
   // initialize rendering and interaction
   //imageViewer->GetRenderWindow()->SetSize(400, 300);
   //imageViewer->GetRenderer()->SetBackground(0.2, 0.3, 0.4);
   imageViewer2->Render();
   imageViewer2->GetRenderer()->ResetCamera();
   imageViewer2->Render();
      
	  
	//Para mirar el valor del pixel// el metodo del click ya se puso en el estilo principal
/*
	vtkSmartPointer<MouseInteractorStylePP> style = vtkSmartPointer<MouseInteractorStylePP>::New();
	vtkSmartPointer<MouseInteractorStylePP> style2 = vtkSmartPointer<MouseInteractorStylePP>::New();
	  
	renderWindowInteractor->SetInteractorStyle( style );   
	renderWindowInteractor2->SetInteractorStyle( style2 );

  */
   renderWindowInteractor2->Start();
}

void VTKVisualization::quickView(ImageType::Pointer image){
	
	typedef itk::ImageToVTKImageFilter<ImageType>       ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	
	
	connector->SetInput(image);
	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	#if VTK_MAJOR_VERSION <= 5
		actor->SetInput(connector->GetOutput());
	#else
		connector->Update();
		actor->GetMapper()->SetInputData(connector->GetOutput());
	#endif
  
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->ResetCamera();
 
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
 
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	
	renderWindowInteractor->SetInteractorStyle(style);
 
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
 
	renderWindowInteractor->Start();
}

void VTKVisualization::quickView(ImageBinaryType::Pointer image){
	
	typedef itk::ImageToVTKImageFilter<ImageBinaryType>       ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	
	
	connector->SetInput(image);
	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	#if VTK_MAJOR_VERSION <= 5
		actor->SetInput(connector->GetOutput());
	#else
		connector->Update();
		actor->GetMapper()->SetInputData(connector->GetOutput());
	#endif
  
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->ResetCamera();
 
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
 
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	
	renderWindowInteractor->SetInteractorStyle(style);
 
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
 
	renderWindowInteractor->Start();
}


/**
 * @brief Dibuja un histograma en forma de barras.
 * @param hist :Arreglo que contiene la información correspondiente al histograma de la imagen.
 * @param lower :Valor de gris mínimo desde donde se dibujará el histograma.
 * @param upper :Valor de gris máximo hasta donde se dibujará el histograma.
 * @param minValue :Valor de gris que corresponde al primer elemento del histograma o arreglo pasado por parametro.
 */
void VTKVisualization::readHistogram (const int* hist, int lower, int upper, int minValue){
	
	
	// Set up a 2D scene, add an XY chart to it
	vtkSmartPointer<vtkContextView> view = vtkSmartPointer<vtkContextView>::New();
	view->GetRenderer()->SetBackground(0.2, 0.2, 0.2);
	view->GetRenderWindow()->SetSize(400, 300);
  
	vtkSmartPointer<vtkChartXY> chart = vtkSmartPointer<vtkChartXY>::New();
	view->GetScene()->AddItem(chart);
	chart->SetBarWidthFraction(1.0);	

	// Create a table with some points in it...
	vtkSmartPointer<vtkTable> table = vtkSmartPointer<vtkTable>::New();

	vtkSmartPointer<vtkIntArray> arrHUValue= vtkSmartPointer<vtkIntArray>::New();
	arrHUValue->SetName("HU Value");
	table->AddColumn(arrHUValue);

	vtkSmartPointer<vtkIntArray> freq = vtkSmartPointer<vtkIntArray>::New();
	freq->SetName("Frequency");
	table->AddColumn(freq);

	table->SetNumberOfRows(upper-lower);
	for (int i = 0; i < upper-lower; i++)
	{
		table->SetValue(i,0,i+lower);
		table->SetValue(i,1,hist[i+(lower-minValue)]);
	}

	// Add multiple line plots, setting the colors etc
	vtkPlot *line = 0;

	line = chart->AddPlot(vtkChart::BAR);
	#if VTK_MAJOR_VERSION <= 5
		line->SetInput(table, 0, 1);
	#else
		line->SetInputData(table, 0, 1);
	#endif
		line->SetColor(0, 255, 0, 255);

	//Finally render the scene and compare the image to a reference image
	view->GetRenderWindow()->SetMultiSamples(0);
	view->GetInteractor()->Initialize();
	view->GetInteractor()->Start();
	
}
