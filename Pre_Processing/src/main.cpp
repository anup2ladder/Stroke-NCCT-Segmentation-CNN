#include <iostream> 
#include <cctype> 
#include <sstream> 
#include <string>


#include "imageManipulation.h"


int main(int arc, char* argv[])
{	
	typedef float FloatPixel; 
	typedef itk::Image< FloatPixel, 3 >  FloatImageType;
	typedef unsigned int IntPixel; 
	typedef itk::Image< IntPixel, 3 >  IntImageType;

	char fname[50]; int n; 	
	// reading in command line arguments..
	imagemanipulation::FloatImageType::Pointer scan; 	
	std::string scan_name = argv[1]; 
	scan = imagemanipulation::readFloat(scan_name);
	
	// Slice by slice gaussian smoothing
	imagemanipulation::FloatImageType::Pointer smooth_scan;
	double variance = 4.0; 
	smooth_scan = imagemanipulation::gaussianSmoothImage(scan, variance); 
	n = sprintf(fname, "%d_gauss.nii.gz",1); 
	imagemanipulation::write(smooth_scan,fname);


	// Global HU thresholding
	imagemanipulation::IntImageType::Pointer binary_scan;
	binary_scan = imagemanipulation::binaryThresholdImage(smooth_scan, 0, 100);
	n = sprintf(fname, "%d_bw.nii.gz",1); 
	imagemanipulation::writeInt(binary_scan,fname);

	// Slice by slice erosion 	
	imagemanipulation::IntImageType::Pointer eroded_scan;
	double radius = 5;
	eroded_scan = imagemanipulation::binaryErosionImage(binary_scan, radius);
  	n = sprintf(fname, "%d_bw_erode.nii.gz",1); 
	imagemanipulation::writeInt(eroded_scan,fname);


	// Connected components 
	imagemanipulation::IntImageType::Pointer brain_mask; 
	brain_mask = imagemanipulation::largestConnectedComponentMask(eroded_scan); 
	n = sprintf(fname, "%d_brain_mask.nii.gz",1); 
	imagemanipulation::writeInt(brain_mask,fname);


	// Slice by slice dilation
	imagemanipulation::IntImageType::Pointer dilated_mask; 
	dilated_mask = imagemanipulation::binaryDilationImage(brain_mask, radius); 
	n = sprintf(fname, "%d_dilated_mask.nii.gz",1); 
	imagemanipulation::writeInt(dilated_mask,fname);

	// Fill all holes Globally 
	imagemanipulation::IntImageType::Pointer final_mask; 
	final_mask = imagemanipulation::iterativeHoleFillingMask(dilated_mask, radius); 
	n = sprintf(fname, "%d_final_mask.nii.gz",1); 
	imagemanipulation::writeInt(final_mask,fname);



	// // Rescale HUs to 0 - 1024 range in all scans 
	// imagemanipulation::FloatImageType::Pointer norm_scan;
	// norm_scan =  imagemanipulation::normalizeCTintensityImage(scan, 0, 100);
	// n = sprintf(fname, "%d_norm.nii.gz",1); 
	// imagemanipulation::write(norm_scan,fname);

	
	return 0; 
}
