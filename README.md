# Automatic Segmentation of Stroke Lesions in Non-contrast Computed Tomography Datasets with Convolutional Neural Networks
## Version 1.0 (June 10, 2020)

## Publication: [https://doi.org/10.1109/ACCESS.2020.2995632](https://doi.org/10.1109/ACCESS.2020.2995632)

Anup Tuladhar\#, Serena Schimert\#, Deepthi Rajashekar, Helge C. Kniep, Jens Fiehler, Nils D. Forkert, "Automatic Segmentation of Stroke Lesions in Non-Contrast Computed Tomography Datasets With Convolutional Neural Networks," in IEEE Access, vol. 8, pp. 94871-94879, 2020, doi:[10.1109/ACCESS.2020.2995632](https://doi.org/10.1109/ACCESS.2020.2995632).

## Model: [https://dx.doi.org/10.21227/jps9-0b57](https://dx.doi.org/10.21227/jps9-0b57)
Anup Tuladhar\#, Serena Schimert\#, Deepthi Rajashekar, Helge C. Kniep, Jens Fiehler, Nils D. Forkert, "Automatic Segmentation of Stroke Lesions in Non-contrast Computed Tomography Datasets with Convolutional Neural Networks", IEEE Dataport, 2020. [Online]. Available: [http://dx.doi.org/10.21227/jps9-0b57](http://dx.doi.org/10.21227/jps9-0b57). Accessed: Jun. 10, 2020.

\#: Co-first authors

The [publicly available model](https://dx.doi.org/10.21227/jps9-0b57) accompanying the [publication](https://doi.org/10.1109/ACCESS.2020.2995632) contains 3 parts:

* Pre-processing: Script to extract brain volume from surrounding skull in non-contrast computed tomography (NCCT) scans and instructions for further pre-processing.
* Trained convolutional neural network (CNN) to perform automated segmentations
* Post-processing script to improve CNN-based segmentations

<div style="page-break-after: always"></div>
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


## 2. Thresholding and normalizing
1. The extracted images must be thresholded again between 0 and 100 Hounsfield units to remove the remaining high-intensity tissue artifacts resulting from the morphological erosion and dilation.
2. The images must then be normalized to zero (0) mean and unit (1) variance to account for potential differences in scanner tube potential and different reconstruction algorithms.

<div style="page-break-after: always"></div>
----

# Trained CNN

This is the trained CNN for automated stroke lesion segmentation in NCCT datasets. 

It uses the [DeepMedic](https://github.com/deepmedic/deepmedic) framework <b>v0.7.3</b>, which has been included along with the trained weights. 

All licensing pertaining to DeepMedic are contained within the <u>`deepmedic`</u> folder and apply to the included framework.

<br>
### From [Publication](https://doi.org/10.1109/ACCESS.2020.2995632) (Methods Sections 2C,D)

#### C. CNN Architecture
The CNN used in this work is based on the DeepMedic model proposed by [Kamnitsas et al.](https://doi.org/10.1016/j.media.2016.10.004) and modified for NCCT stroke lesion segmentation. The network parameters were optimized with cross-validation. We used a total of 11 layers. The first eight layers consist of three parallel convolutional pathways for processing the images at multiple scales. The multi-scale pathways were created by using down-sampled versions of the NCCT images (by factors of 3× and 5× ) as inputs to the parallel convolutional pathways, in addition to the original image. Each parallel pathway has eight convolutional layers consisting of 30, 30, 40, 40, 40, 40, 50, and 50 feature maps and uses convolutional kernels of size 3×3×3 . Additionally, residual skip connections between layers two and four, between layers four and six, and between layers six and eight are used in each parallel pathway. The ninth layer combines the three multi-scale pathways together by using the concatenated outputs from layer eight of each parallel pathway. Layer nine uses 3×3×3 convolutional kernels and has 250 feature maps. Layer ten is a fully-connected convolutional layer with 1×1×1 convolutional kernels and 250 feature maps. Additionally, a residual skip connection between layers eight and ten was used. The final softmax classification layer, layer eleven, produces the lesion probability maps. A threshold of >0.5 is used to binarize the probability map to a final lesion segmentation. 

#### D. CNN Training

All CNN model training was performed in Python 2.7 on Compute Canada and Calcul Quebec computing clusters. The DeepMedic framework (v.0.7.3), available from [https://github.com/deepmedic/deepmedic](https://github.com/deepmedic/deepmedic), was used for model training. The DeepMedic framework performs model training on image segments extracted from the original image, rather than the entire image. In this work, segments of 37×37×37 were used. The network was trained for 35 epochs with a batch size of 10. Each epoch was divided into 20 sub-epochs, within which 1000 image segments were extracted and used for model training. An initial learning rate of 0.001, which decreases through training using a polynomial decay function, was employed. Root mean square propagation was used as the optimizer. L1 and L2 regularizations of 10<sup>−6</sup> and 10<sup>−4</sup> were used, respectively. Data augmentation consisted of mirroring along the sagittal axis. The CNN model achieved a mean Dice similarity coefficient (DSC) of 0.52 in the training set evaluated by 10-fold cross-validation.

The DSC scores and lesion volumes for the automatic segmentations of the validation and holdout test sets were obtained using a single CNN model that was trained on the entire training data.

---
## Prerequisites

The CNN was trained and tested with the following libraries and versions:

* [Python](https://www.python.org/downloads/) 2.7 and 3.5
* [TensorFlow](https://www.tensorflow.org/) 1.12
* [NiBabel](http://nipy.org/nibabel/) 2.5.1
* [numpy](http://www.numpy.org/) 1.16.2
* [scipy](http://www.scipy.org/) 1.2.1

Please see the [DeepMedic](https://github.com/deepmedic/deepmedic) framework for further instructions and details on setting up your system to run the DeepMedic framework.

## File and Folder Structure

* ./trained_model <b>(Do not modify)</b>
* ./deepmedic <b>(Do not modify)</b>
* deepMedicRun <b>(Do not modify)</b>
* ./data <b>(Update)</b>
    * ./preprocessed_images
        * EXAMPLE_IMAGE1.nii.gz
        * EXAMPLE_IMAGE2.nii.gz
        * EXAMPLE_IMAGE3.nii.gz
    * ./groundtruth_segmentations
        * EXAMPLE_GroundTruth1.nii.gz
        * EXAMPLE_GroundTruth2.nii.gz
        * EXAMPLE_GroundTruth3.nii.gz
* ./filelists <b>(Update)</b>
    * TestChannels_NCCT.cfg
    * TestGroundTruthLabels.cfg
    * TestNamesOfPredictions.cfg
* ./test_configuration <b>(Update)</b>
    * TestConfig.cfg
* ./output
    * ./logs
        * EXAMPLE_TestSession.txt
    * ./predictions
        * ./EXAMPLE_TestSession
            * ./features
            * ./predictions
                * EXAMPLE_IMAGE1_Segm.nii.gz
                * EXAMPLE_IMAGE2_Segm.nii.gz
                * EXAMPLE_IMAGE3_Segm.nii.gz

## Preparing the Files and Folders
1. Prepare the `data` folder.
    a. Put pre-processed brain scans in  `./data/preprocessed_images`
    b. (Optional) If available, put ground truth segmentations in  `./data/groundtruth_segmentations`

2. Update the **.cfg** files in `./filelists` with the names of the data files prepared in step 1. Note: The order of filenames in each .cfg file should be the same.
    a. **TestChannels_NCCT.cfg** : Path to files in `../data/preprocessed_images/`
    b. **TestNamesOfPredictions.cfg** : Names given to segmentations produced and saved by the CNN.
    c. (Optional) **TestGroundTruthLabels.cfg** : Names of ground truth segmentations (see step 1b) in `../data/groundtruth_segmentations/`

3. Update **TestConfig.cfg** in `./test_configuration`.
    a. (Line 6) Provide a name for the testing session (e.g. "EXAMPLE_TestSession")
    b. The following variables should point to these files by default:
        * (Line 17) channels : **TestChannels_NCCT.cfg**
        * (Line 20) namesForPredictionsPerCase : **TestNamesOfPredictions.cfg**
        * (Optional) (Line 27) gtLabels : **TestGroundTruthLabels.cfg**
            * <u>Note</u>: Comment this out with a "#" at the beginning of Line 27 if ground truth labels are not provided in steps 1b and 2c.

## Running the CNN

Run one of the following commands from the root folder of "Trained_CNN", depending on whether your set-up can or cannot use a GPU.

<u>Note</u>: **deepMedicRun** must be executable (e.g. `chmod +x deepMedicRun`).

### CPU-only
~~~
./deepMedicRun \
    -model ./trained_model/ModelConfig_TuladharSchimert_deepMedic_strokeNCCTsegmentation_20200515.cfg \
    -test ./test_configuration/TestConfig.cfg \
    -load ./trained_model/TrainedWeights_TuladharSchimert_deepMedic_strokeNCCTsegmentation_20200515.model.ckpt
~~~

### GPU accelerated
~~~
./deepMedicRun \
    -model ./trained_model/ModelConfig_TuladharSchimert_deepMedic_strokeNCCTsegmentation_20200515.cfg \
    -test ./test_configuration/TestConfig.cfg \
    -load ./trained_model/TrainedWeights_TuladharSchimert_deepMedic_strokeNCCTsegmentation_20200515.model.ckpt \
    -dev cuda0
~~~
<u>Note:</u> Replace `cuda0` with the device name of your GPU, if it differs. In most systems with a single GPU, the default device name will likely be `cuda0`.

<br>
## Output

* The console outputs will be saved in `./output/logs/` as a **.txt** file matching the session name provided in Step 3a
    * e.g. **EXAMPLE_TestSession.txt**

* The segmentations will be saved in `./output/predictions/session_name/predictions`, with session_name replaced with the session name provided in Step 3a.
    * e.g. `./output/predictions/EXAMPLE_TestSession/predictions/`
    * Each segmentation will use the filename given in Step 2b, **TestNamesOfPredictions.cfg**, and be appended with "_Segm"

<div style="page-break-after: always"></div>
---

# Post-processing

This is the post-processing script to improve binary lesion segmentations produced by the trained CNN.

<br>
### From [Publication](https://doi.org/10.1109/ACCESS.2020.2995632) (Methods Section 2E)
The CNN-based binary lesion segmentations were post-processed to further improve the segmentation accuracy (Fig. 1). The implemented post-processing consists of a connected component analysis to exclude small lesion components, most likely caused by noise artifacts, and an automatic hole-filling approach. Post-processing was performed using the ITK toolkit. Using a connected components analysis, components smaller than an empirically determined cut-off were removed. The exception was in segmentations where the largest connected component was smaller than the cut-off value, in which case no cutoff was applied. Afterwards, a hole-filling algorithm was used to fill gaps within the segmentation.

The validation dataset was used to estimate the optimum minimum object size cut-off and the hole-filling kernel radius. The minimum object size cut-off was optimized first, by varying the cut-off range from 0.3 cm<sup>3</sup> to 2.5 cm<sup>3</sup>. The cut-off that maximized the DSC was 1.5 cm<sup>3</sup> and was used for post-processing.

Using this minimum object size cut-off, the hole-filling radius was optimized next using values of 2, 3, 5, 7, and 10 voxels. As hole-filling causes the segmented lesion volumes to grow, and subsequently increased the error in lesion volume estimates, both the DSC and lesion volume error were considered when choosing the optimal value. More precisely, the DSC was maximized while the lesion volume error was minimized. The optimal radius was found to be 3 voxels and was used for post-processing.
s

---
## Prerequisites

The script was tested with the following libraries and versions:

* [Python](https://www.python.org/downloads/) 2.7 and 3.5
* [SimpleITK](https://simpleitk.org/) 1.2.0
* [ITK](https://itk.org/) 5.1.0


## File and Folder Structure

* postProcessing.py

* ./input_cnn_segmentations <b>(Update)</b>
    * EXAMPLE_IMAGE1_Segm.nii.gz
    * EXAMPLE_IMAGE2_Segm.nii.gz
    * EXAMPLE_IMAGE3_Segm.nii.gz

* ./output_postprocessed
    * postprocess_EXAMPLE_IMAGE1_Segm.nii.gz
    * postprocess_EXAMPLE_IMAGE2_Segm.nii.gz
    * postprocess_EXAMPLE_IMAGE3_Segm.nii.gz

## Preparing the Files and Folders
Put the binary segmentations produced by the CNN in the folder `input_cnn_segmentations`

<br>
## Parameters
The post-processing script has 2 parameters that can be changed in **postProcessing.py**. By default they are set to the parameters from the associated publication.

* (Line 28) minvol : The lower cut-off for minimum connected-component object volume, in mm<sup>3</sup>. This will be converted to cm<sup>3</sup>
    * Default: 1500 mm<sup>3</sup> (i.e. 1.5 cm<sup>3</sup>)

* (Line 32) rad : The size of the hole-filling radius, in voxels.
    * Default: 3 voxels


## Running the script
~~~
python ./postProcessing.py
~~~


## Output
The post-processed segmentations will be saved in `output_postprocessed_segmentations` with the prefix **postprocess_** added to the input filenames..

* e.g. **postprocess_EXAMPLE_IMAGE1_Segm.nii.gz**

