# Automatic Segmentation of Stroke Lesions in Non-contrast Computed Tomography Datasets with Convolutional Neural Networks
## Version 1.0 (June 10, 2020)

## Publication: [https://doi.org/10.1109/ACCESS.2020.2995632](https://doi.org/10.1109/ACCESS.2020.2995632)

Anup Tuladhar\#, Serena Schimert\#, Deepthi Rajashekar, Helge C. Kniep, Jens Fiehler, Nils D. Forkert, "Automatic Segmentation of Stroke Lesions in Non-Contrast Computed Tomography Datasets With Convolutional Neural Networks," in IEEE Access, vol. 8, pp. 94871-94879, 2020, doi:[10.1109/ACCESS.2020.2995632](https://doi.org/10.1109/ACCESS.2020.2995632).

## Model: [https://dx.doi.org/10.21227/jps9-0b57](https://dx.doi.org/10.21227/jps9-0b57)
Anup Tuladhar\#, Serena Schimert\#, Deepthi Rajashekar, Helge C. Kniep, Jens Fiehler, Nils D. Forkert, "Automatic Segmentation of Stroke Lesions in Non-contrast Computed Tomography Datasets with Convolutional Neural Networks", IEEE Dataport, 2020. [Online]. Available: [http://dx.doi.org/10.21227/jps9-0b57](http://dx.doi.org/10.21227/jps9-0b57). Accessed: Jun. 10, 2020.

\#: Co-first authors

The [publicly available model](https://dx.doi.org/10.21227/jps9-0b57) accompanying the [publication](https://doi.org/10.1109/ACCESS.2020.2995632) contains 3 parts:

* Pre-processing: Script to extraction brain segments from surrounding skull segments in non-contrast computed tomography (NCCT) scans and instructions for further pre-processing.
* Trained convolutional neural network (CNN) to perform automated segmentations
* **Post-processing script to improve CNN-based segmentations**

---

# Post-processing

This is the post-processing script to improve binary lesion segmentations produced by the trained CNN.

### From [Publication](https://doi.org/10.1109/ACCESS.2020.2995632) (Methods Section 2E)
The CNN-based binary lesion segmentations were post-processed to further improve the segmentation accuracy (Fig. 1). The implemented post-processing consists of a connected component analysis to exclude small lesion components, most likely caused by noise artifacts, and an automatic hole-filling approach. Post-processing was performed using the ITK toolkit. Using a connected components analysis, components smaller than an empirically determined cut-off were removed. The exception was in segmentations where the largest connected component was smaller than the cut-off value, in which case no cutoff was applied. Afterwards, a hole-filling algorithm was used to fill gaps within the segmentation.

The validation dataset was used to estimate the optimum minimum object size cut-off and the hole-filling kernel radius. The minimum object size cut-off was optimized first, by varying the cut-off range from 0.3 cm<sup>3</sup> to 2.5 cm<sup>3</sup>. The cut-off that maximized the DSC was 1.5 cm<sup>3</sup> and was used for post-processing.

Using this minimum object size cut-off, the hole-filling radius was optimized next using values of 2, 3, 5, 7, and 10 voxels. As hole-filling causes the segmented lesion volumes to grow, and subsequently increased the error in lesion volume estimates, both the DSC and lesion volume error were considered when choosing the optimal value. More precisely, the DSC was maximized while the lesion volume error was minimized. The optimal radius was found to be 3 voxels and was used for post-processing.

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


