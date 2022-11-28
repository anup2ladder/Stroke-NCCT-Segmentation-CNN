# Automatic Segmentation of Stroke Lesions in Non-contrast Computed Tomography Datasets with Convolutional Neural Networks
## Version 1.0 (June 10, 2020)

## Publication: [https://doi.org/10.1109/ACCESS.2020.2995632](https://doi.org/10.1109/ACCESS.2020.2995632)

Anup Tuladhar\#, Serena Schimert\#, Deepthi Rajashekar, Helge C. Kniep, Jens Fiehler, Nils D. Forkert, "Automatic Segmentation of Stroke Lesions in Non-Contrast Computed Tomography Datasets With Convolutional Neural Networks," in IEEE Access, vol. 8, pp. 94871-94879, 2020, doi:[10.1109/ACCESS.2020.2995632](https://doi.org/10.1109/ACCESS.2020.2995632).

## Model: [https://dx.doi.org/10.21227/jps9-0b57](https://dx.doi.org/10.21227/jps9-0b57)
Anup Tuladhar\#, Serena Schimert\#, Deepthi Rajashekar, Helge C. Kniep, Jens Fiehler, Nils D. Forkert, "Automatic Segmentation of Stroke Lesions in Non-contrast Computed Tomography Datasets with Convolutional Neural Networks", IEEE Dataport, 2020. [Online]. Available: [http://dx.doi.org/10.21227/jps9-0b57](http://dx.doi.org/10.21227/jps9-0b57). Accessed: Jun. 10, 2020.

\#: Co-first authors

The [publicly available model](https://dx.doi.org/10.21227/jps9-0b57) accompanying the [publication](https://doi.org/10.1109/ACCESS.2020.2995632) contains 3 parts:

* Pre-processing: Script to extraction brain segments from surrounding skull segments in non-contrast computed tomography (NCCT) scans and instructions for further pre-processing.
* **Trained convolutional neural network (CNN) to perform automated segmentations**
* Post-processing script to improve CNN-based segmentations

---

# Trained CNN

This is the trained CNN for automated stroke lesion segmentation in NCCT datasets. 

It uses the [DeepMedic](https://github.com/deepmedic/deepmedic) framework <b>v0.7.3</b>, which has been included along with the trained weights. 

All licensing pertaining to DeepMedic are contained within the <u>`deepmedic`</u> folder and apply to the included framework.

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

<br>
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


