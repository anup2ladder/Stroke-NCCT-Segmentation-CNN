'''
This code handles post-processing of DeepMedic's output predicted segmentation probability maps,
including minimum cut-off of for volume of predicted segmentation connected components,
and hole-filling to help correct small gaps in segmentation. 
'''

from __future__ import print_function
import os
import SimpleITK as sitk
import itk

# Paths to segmentation predictions by DeepMedic
# E.g. "./input_cnn_segmentations"
dirPred = "./input_cnn_segmentations"

# Path to desired output directory of post-processed segmentations
# E.g. "./output_postprocessed"
dirPost = "./output_postprocessed"


if not os.path.exists(dirPost):
    os.makedirs(dirPost)


# Lower cut-off for volume, will conver to cm^3
# E.g. 1500 = 1.5 cm^3
# Optimized as per https://doi.org/10.1109/ACCESS.2020.2995632
minvol = 1500

# Radius for hole-filling, in voxels
# Optimized as per https://doi.org/10.1109/ACCESS.2020.2995632
rad = 3


for img in os.listdir(dirPred):

    #Adjust depending on length of patient ID compared to filename 
    patient = img[0:]     
    print("Processing patient scan: " + patient + "\n")

    inIm = os.path.join(dirPred, img) 
    outIm = os.path.join(dirPost, "postprocess_"+img)

    inputIm = sitk.ReadImage(inIm)

    #Threshold and binarize DeepMedic probability maps
    threshFilter = sitk.BinaryThresholdImageFilter()
    threshFilter.SetLowerThreshold(0.50)
    inputIm = threshFilter.Execute(inputIm)
    
    #So image type is compatible with certain filters
    castFilter = sitk.CastImageFilter()
    castFilter.SetOutputPixelType(4)
    inputIm = castFilter.Execute(inputIm)

    #Voxel volume needed for minimum size cut-off
    spacing = inputIm.GetSpacing()
    voxvol = spacing[0]*spacing[1]*spacing[2]
    
    size = inputIm.GetSize()
    
    #Get connected components of segmentation as distinguishable intensities
    conComFilter = sitk.ConnectedComponentImageFilter()
    conComFilter.SetFullyConnected(True)
    inputIm = conComFilter.Execute(inputIm)

    #Re-assign component label intensities by size
    #If largest component is >= 1.5 cm^3, threshold components below cut-off
    #Otherwise keep all components
    bySizeFilter = sitk.RelabelComponentImageFilter()
    bySizeFilter.Execute(inputIm)
    objectpix = bySizeFilter.GetSizeOfObjectsInPixels()
    if objectpix[0]*voxvol >= minvol: 
        bySizeFilter.SetMinimumObjectSize(int(minvol/voxvol))
    else:
        bySizeFilter.SetMinimumObjectSize(0)
    inputIm = bySizeFilter.Execute(inputIm)

    #Re-binarize label by setting object intensities back to 1
    threshFilter = sitk.BinaryThresholdImageFilter()
    threshFilter.SetOutsideValue(0)
    threshFilter.SetInsideValue(1)
    threshFilter.SetLowerThreshold(1)
    threshFilter.SetUpperThreshold(bySizeFilter.GetNumberOfObjects())
    inputIm = threshFilter.Execute(inputIm)
      
    sitk.WriteImage(inputIm, outIm, True)
    
    #Note: switching from sitk to itk for hole-filling - - - - - - - - - - -
    
    type1 = itk.Image[itk.UC,2]    # For 2-D image filter
    type2 = itk.Image[itk.UC,3]    # For 3-D image filter
    
    reader = itk.ImageFileReader[type2].New()
    reader.SetFileName(outIm)
    
    #Filling in small gaps in predicted segmentation
    #Voxel radius as optimized on validation dataset 
    fillFilter = itk.VotingBinaryIterativeHoleFillingImageFilter[type1].New()
    fillFilter.SetMaximumNumberOfIterations(10)
    fillFilter.SetRadius(rad)
    fillFilter.SetForegroundValue(1)
    fillFilter.SetBackgroundValue(0)
    
    #Hole-filling is applied slice-by-slice
    sliceFiller = itk.SliceBySliceImageFilter[type2, type2].New()
    sliceFiller.SetFilter(fillFilter)
    sliceFiller.SetInput(reader.GetOutput())
    
    writer = itk.ImageFileWriter[type2].New()
    writer.SetFileName(outIm)
    writer.SetInput(sliceFiller.GetOutput())
    
    writer.Update()
