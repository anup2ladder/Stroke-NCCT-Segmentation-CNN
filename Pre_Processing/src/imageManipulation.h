#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMultiplyImageFilter.h"


namespace imagemanipulation
{
	typedef float FloatPixel; 
	typedef unsigned int IntPixel;

	typedef itk::Image< FloatPixel, 3 >  FloatImageType;
	typedef itk::Image< IntPixel, 3 >  IntImageType;
	typedef itk::Image< FloatPixel, 2>  FloatKernelType;
	typedef itk::Image< IntPixel, 2>  IntKernelType;


	void write(FloatImageType::Pointer image, std::string fileprefix);
	void writeInt(IntImageType::Pointer image, std::string fileprefix);

	FloatImageType::Pointer readFloat(std::string filename);
	IntImageType::Pointer readInt(std::string filename);
	IntImageType::Pointer binaryThresholdImage(FloatImageType::Pointer image, int LowerThreshold, int UpperThreshold);
	FloatImageType::Pointer normalizeCTintensityImage(FloatImageType::Pointer image, int min, int max);
	FloatImageType::Pointer gaussianSmoothImage(FloatImageType::Pointer image, double variance);
	IntImageType::Pointer binaryErosionImage(IntImageType::Pointer image, double radius);
	IntImageType::Pointer largestConnectedComponentMask(IntImageType::Pointer image);
	IntImageType::Pointer binaryDilationImage(IntImageType::Pointer image, double radius);
	IntImageType::Pointer iterativeHoleFillingMask(IntImageType::Pointer image, double radius);
}

