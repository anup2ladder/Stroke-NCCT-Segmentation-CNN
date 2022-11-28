#include "imageManipulation.h" 
#include "itkImageFileReader.h" 
#include "itkSliceBySliceImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkLabelSelectionLabelMapFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkVotingBinaryIterativeHoleFillingImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

namespace imagemanipulation
{
	
	FloatImageType::Pointer readFloat(std::string filename)
	{

		typedef itk::ImageFileReader < FloatImageType > FloatImageReaderType; 
		FloatImageReaderType::Pointer reader = FloatImageReaderType::New();
		reader->SetFileName(filename);
		reader->Update();
   		reader->UpdateLargestPossibleRegion();
		return reader->GetOutput();
	}

	IntImageType::Pointer readInt(std::string filename)
	{

		typedef itk::ImageFileReader < IntImageType > IntImageReaderType; 
		IntImageReaderType::Pointer reader = IntImageReaderType::New();
		reader->SetFileName(filename);
		reader->Update();
   		reader->UpdateLargestPossibleRegion();
		return reader->GetOutput();
	}


	void write(FloatImageType::Pointer image, std::string fileprefix)
	{
		typedef itk::ImageFileWriter< FloatImageType > WriterType;
		WriterType::Pointer writer = WriterType::New(); 
		writer->SetFileName(fileprefix);
		writer->SetInput(image); 
		writer->Update(); 

	}
  	
	void writeInt(IntImageType::Pointer image, std::string fileprefix)
	{
		typedef itk::ImageFileWriter< IntImageType > WriterType;
		WriterType::Pointer writer = WriterType::New(); 
		writer->SetFileName(fileprefix);
		writer->SetInput(image); 
		writer->Update(); 

	}  	

	FloatImageType::Pointer normalizeCTintensityImage(FloatImageType::Pointer image, int min, int max)
	{
		typedef itk::RescaleIntensityImageFilter< FloatImageType, FloatImageType > RescaleFilterType;
  		RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  		rescaleFilter->SetInput(image);
  		rescaleFilter->SetOutputMinimum(min);
  		rescaleFilter->SetOutputMaximum(max);
  		rescaleFilter->Update(); 

  		return rescaleFilter->GetOutput(); 

	}

	FloatImageType::Pointer gaussianSmoothImage(FloatImageType::Pointer image, double variance)
	{
		typedef itk::DiscreteGaussianImageFilter< FloatKernelType, FloatKernelType >  SmoothFilterType;
		SmoothFilterType::Pointer gaussianFilter = SmoothFilterType::New();
  		gaussianFilter->SetVariance(variance);

		typedef itk::SliceBySliceImageFilter<FloatImageType, FloatImageType> SliceFilterType;
		SliceFilterType::Pointer slice_walker = SliceFilterType::New();
		slice_walker->SetInput(image); 	
  		slice_walker->SetFilter(gaussianFilter);
		slice_walker->Update();

  		return slice_walker->GetOutput();
 
	}



	IntImageType::Pointer binaryThresholdImage(FloatImageType::Pointer image, int LowerThreshold, int UpperThreshold)
	{
		typedef itk::BinaryThresholdImageFilter< FloatImageType, IntImageType > ThresholdFilterType;
		ThresholdFilterType::Pointer thresholdFilter = ThresholdFilterType::New();
		thresholdFilter->SetInput(image); 
		thresholdFilter->SetLowerThreshold( LowerThreshold );
  		thresholdFilter->SetUpperThreshold( UpperThreshold );
  		thresholdFilter->SetOutsideValue( 0 );
		thresholdFilter->SetInsideValue( 1 );
		thresholdFilter->Update();;

		return thresholdFilter->GetOutput();
	}


	IntImageType::Pointer binaryErosionImage(IntImageType::Pointer image, double radius)
	{

		typedef itk::BinaryBallStructuringElement<IntPixel, 2> StructuringElementType;
		StructuringElementType structuringElement;
		structuringElement.SetRadius(radius);
		structuringElement.CreateStructuringElement();

		typedef itk::BinaryErodeImageFilter <IntKernelType, IntKernelType, StructuringElementType>  BinaryErodeImageFilterType;
  		BinaryErodeImageFilterType::Pointer erodeFilter = BinaryErodeImageFilterType::New();
  		erodeFilter->SetKernel(structuringElement);
  		erodeFilter->SetErodeValue( 1 ); 


		typedef itk::SliceBySliceImageFilter<IntImageType, IntImageType> SliceFilterType;
		SliceFilterType::Pointer slice_walker = SliceFilterType::New();
		slice_walker->SetInput(image); 	
  		slice_walker->SetFilter(erodeFilter);
		slice_walker->Update();

  		return slice_walker->GetOutput();
	}

	IntImageType::Pointer largestConnectedComponentMask(IntImageType::Pointer image)
	{
		// label all CC
		typedef itk::ConnectedComponentImageFilter <IntImageType, IntImageType > ConnectedComponentImageFilterType;
  		ConnectedComponentImageFilterType::Pointer connected =	ConnectedComponentImageFilterType::New ();
  		connected->SetInput(image);	
  		connected->Update();
		
		// relabel all CC 
		typedef itk::RelabelComponentImageFilter<IntImageType, IntImageType> RelabelFilterType;
  		RelabelFilterType::Pointer relabelFilter = RelabelFilterType::New();
  		relabelFilter->SetInput(connected->GetOutput());
  		relabelFilter->Update();

  		// convert relabled image to label map 
  		typedef itk::LabelObject <IntPixel, 3 > LabelObjectType ; 
  		typedef itk::LabelMap < LabelObjectType> LabelMapType;
  		typedef itk::LabelImageToLabelMapFilter <IntImageType, LabelMapType > LabelImageToLabelMapFilterType; 
  		LabelImageToLabelMapFilterType::Pointer labelMapConverter = LabelImageToLabelMapFilterType::New();
  		labelMapConverter->SetInput( relabelFilter->GetOutput() );
  		labelMapConverter->SetBackgroundValue( itk::NumericTraits< IntPixel >::Zero );

  		// select the label of interest (in this case 1 )
  		typedef itk::LabelSelectionLabelMapFilter< LabelMapType > SelectorType;
  		SelectorType::Pointer selector = SelectorType::New();
  		selector->SetInput( labelMapConverter->GetOutput() );
  		selector->SetLabel( 1 ); // one becuase the relabel ensures the largest volume is 1

  		// make it an image and return it 
		typedef itk::LabelMapToLabelImageFilter< LabelMapType, IntImageType > LabelMapToLabelImageFilterType;
  		LabelMapToLabelImageFilterType::Pointer labelImageConverter = LabelMapToLabelImageFilterType::New();
    	labelImageConverter->SetInput( selector->GetOutput( 0 ) );
    	labelImageConverter->Update();

    	return labelImageConverter->GetOutput();

	}


	IntImageType::Pointer binaryDilationImage(IntImageType::Pointer image, double radius)
	{

		typedef itk::BinaryBallStructuringElement<IntPixel, 2> StructuringElementType;
		StructuringElementType structuringElement;
		structuringElement.SetRadius(radius);
		structuringElement.CreateStructuringElement();

		typedef itk::BinaryDilateImageFilter <IntKernelType, IntKernelType, StructuringElementType>  BinaryDilateImageFilterType;
  		BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
  		dilateFilter->SetKernel(structuringElement);
  		dilateFilter->SetDilateValue( 1 ); 


		typedef itk::SliceBySliceImageFilter<IntImageType, IntImageType> SliceFilterType;
		SliceFilterType::Pointer slice_walker = SliceFilterType::New();
		slice_walker->SetInput(image); 	
  		slice_walker->SetFilter(dilateFilter);
		slice_walker->Update();

  		return slice_walker->GetOutput();
	}

	IntImageType::Pointer iterativeHoleFillingMask(IntImageType::Pointer image, double radius)
	{

		typedef itk::VotingBinaryIterativeHoleFillingImageFilter< IntImageType > FilterType;
	  	FilterType::InputSizeType extent;
	  	extent.Fill( radius );

	  	FilterType::Pointer filter = FilterType::New();
	  	filter->SetInput( image );
	  	filter->SetRadius( extent );
	  	filter->SetMajorityThreshold( 1 );
	  	filter->SetBackgroundValue( itk::NumericTraits< IntPixel >::Zero );
	  	filter->SetForegroundValue( itk::NumericTraits< IntPixel >::One );
	  	filter->SetMaximumNumberOfIterations( 3 );
	  	filter->Update();

	  	return filter->GetOutput();
	
	}


}
