# Automatic Segmentation of Stroke Lesions in Non-contrast Computed Tomography Datasets with Convolutional Neural Networks
## Version 1.0 (June 10, 2020)

## Publication: [https://doi.org/10.1109/ACCESS.2020.2995632](https://doi.org/10.1109/ACCESS.2020.2995632)

Anup Tuladhar\#, Serena Schimert\#, Deepthi Rajashekar, Helge C. Kniep, Jens Fiehler, Nils D. Forkert, "Automatic Segmentation of Stroke Lesions in Non-Contrast Computed Tomography Datasets With Convolutional Neural Networks," in IEEE Access, vol. 8, pp. 94871-94879, 2020, doi:[10.1109/ACCESS.2020.2995632](https://doi.org/10.1109/ACCESS.2020.2995632).

## Model: [https://dx.doi.org/10.21227/jps9-0b57](https://dx.doi.org/10.21227/jps9-0b57)
Anup Tuladhar\#, Serena Schimert\#, Deepthi Rajashekar, Helge C. Kniep, Jens Fiehler, Nils D. Forkert, "Automatic Segmentation of Stroke Lesions in Non-contrast Computed Tomography Datasets with Convolutional Neural Networks", IEEE Dataport, 2020. [Online]. Available: [http://dx.doi.org/10.21227/jps9-0b57](http://dx.doi.org/10.21227/jps9-0b57). Accessed: Jun. 10, 2020.

\#: Co-first authors

The [publicly available model](https://dx.doi.org/10.21227/jps9-0b57) accompanying the [publication](https://doi.org/10.1109/ACCESS.2020.2995632) contains 3 parts:

* **Pre-processing: Script to extract brain volume from surrounding skull in non-contrast computed tomography (NCCT) scans and instructions for further pre-processing.**
* Trained convolutional neural network (CNN) to perform automated segmentations
* Post-processing script to improve CNN-based segmentations

---

# Pre-processing

There are two steps for pre-processing NCCT images before segmentation with the trained CNN:

1. **Brain Extraction**: Bone structures must be removed from each dataset, retaining only the brain tissue in the images. The source code for this script is provided.

2. **Thresholding and Normalization**: The extracted brain segments must be thresholded and normalized to remove artifacts resulting from morphological erosion and dilation, and to account for potential differences in scanner tube potential and different reconstruction algorithms, respectively. Instructions for this are provided below.

<br>
### From [Publication](https://doi.org/10.1109/ACCESS.2020.2995632) (Methods Section 2B)
As NCCT images were acquired from multiple centers with differing scanners and imaging protocols, training a CNN directly on NCCT images without pre-processing resulted in very poor performance on the training set (data not shown). Thus, the datasets were pre-processed to ensure consistency between NCCT images collected from different centers.

First, the bone structures were removed from each dataset, retaining only the brain tissue in the images. To remove the bone structures, which have high Hounsfield values, a six-step procedure following the approach described by [Muschelli et al.](https://doi.org/10.1016/j.neuroimage.2015.03.074) was performed in a slice-wise manner. This approach was implemented using the [Insight Segmentation and Registration Toolkit (ITK)](http://doi.org/10.3233/978-1-60750-929-5-586). Briefly described, a Gaussian filter with a variance of 4 pixels was used to smooth each slice. In the next step, the intensities were thresholded between 0 and 100 Hounsfield units, which removes most of the artifacts from bone and other high-intensity tissues. After this, a circular structural element with a radius of 1 pixel was used to erode the resulting segmentation. Subsequently, the largest connected component in each slice was extracted and a circular structural element with a radius of 1 pixel was used to dilate this component in order to create a brain mask for the slice. After performing the these three steps in each slice, the masks from each slice are combined into a final mask for the entire volume and any holes in this final mask were filled using the “Voting Binary Hole Filling Image Filter” in ITK. Finally, the images were thresholded again between 0 and 100 Hounsfield units to remove the remaining high-intensity tissue artifacts resulting from the morphological erosion and dilation. The brain volumes were then normalized to zero mean and unit variance to account for potential differences in scanner tube potential and different reconstruction algorithms. All images in the training, validation, and holdout test datasets underwent the same pre-processing procedure.

---

## 1. Brain Extraction

### 1.1 Prerequisites

The brain extraction script was tested with the following libraries and versions:

* [ITK](https://itk.org/) 5.1.0


### 1.2 File and Folder Structure

* ./src **(Do not modify)**

* ./bin **(For compiled script)**

* ./unprocessed_data
    * EXAMPLE_UNPROCESSED_IMAGE1.nii.gz
    * EXAMPLE_UNPROCESSED_IMAGE2.nii.gz
    * EXAMPLE_UNPROCESSED_IMAGE3.nii.gz

* ./extracted_images
    * EXAMPLE_BRAINEXTRACTION_IMAGE1.nii.gz
    * EXAMPLE_BRAINEXTRACTION_IMAGE2.nii.gz
    * EXAMPLE_BRAINEXTRACTION_IMAGE3.nii.gz
    
* ./preprocessed_images
    * EXAMPLE_IMAGE1.nii.gz
    * EXAMPLE_IMAGE2.nii.gz
    * EXAMPLE_IMAGE3.nii.gz

    
### 1.3 Preparing the Files and Folders
* Put the unprocessed NCCT images in the folder `unprocessed_data`

* Compile the executable from `./src` into `./bin`
    * cd ./bin
    * cmake ../src
    * make
    * ./bin/ct-brain-extraction


### 1.4 Running the script
~~~
./bin/ct-brain-extraction ./unprocessed_data/EXAMPLE_UNPROCESSED_IMAGE1.nii.gz
~~~

### 1.5 Output of brain extraction script
The images with the brain extracted from the surrounding skull can be saved in `./extracted_images`. 

**<u>Note</u>: These must be further pre-processed before segmenting with the trained CNN.**


<br>
## 2. Thresholding and normalizing
1. The extracted images must be thresholded again between 0 and 100 Hounsfield units to remove the remaining high-intensity tissue artifacts resulting from the morphological erosion and dilation.
2. The images must then be normalized to zero (0) mean and unit (1) variance to account for potential differences in scanner tube potential and different reconstruction algorithms.


