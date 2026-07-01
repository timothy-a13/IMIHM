# [Paper Implementation] IMIHM

This repository is a C++/OpenCV implementation of the method described in:

> Wenjie Li, Xuebin Liu, Jie Yang, Chongji Zhao, and Huan Deng, "A Color Correction Method Based on Incremental Multilevel Iterative Histogram Matching," IEEE Sensors Journal, vol. 24, no. 17, pp. 27892-27901, 2024.

Paper link: https://ieeexplore.ieee.org/document/10614104  
DOI: https://doi.org/10.1109/JSEN.2024.3432277

This is an implementation repository, not the official paper page.

## What It Does

The program performs image color correction for an image sequence using histogram matching and an incremental multilevel iterative histogram matching workflow. It reads a folder of input images and a folder of transformation matrices, corrects the target images sequentially, and writes intermediate and warped output images.

## Requirements

- CMake 3.20 or newer
- C++17 compiler
- OpenCV 4.x

The current `CMakeLists.txt` sets `OpenCV_DIR` to:

```txt
C:/Program Files/OpenCV/opencv-4.10.0/build/install
```

If OpenCV is installed somewhere else, update `OpenCV_DIR` in `CMakeLists.txt` before configuring the project.

## Build

From the repository root:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

This builds the `IHM` executable in the `build` directory.

## Run

Usage:

```txt
IHM <Image Folder> <Tfm Matrix Folder> <Number of Image>
```

Example using the included sample folders:

```powershell
.\build\IHM.exe .\dataset\img .\dataset\tfm 10
```

The image folder should contain exactly `Number of Image` images. The transformation matrix folder should contain:

- one `3 x 3` transformation matrix text file for each image
- `shape.txt`
- `shift__H.txt`

The program sorts the file paths before processing them, so keep file names ordered consistently.

## Output

Output paths are currently configured in `src/global_folder_paths.cpp`.

By default, the code writes to:

```txt
C:/Timothy/Code/IHM/result/
```

Before running on another machine, update `resultFolder` in `src/global_folder_paths.cpp` or make sure the configured folders exist.

The output folders are:

- `result/tmp/`: intermediate debug images
- `result/org_IHM/`: color-corrected sequence images
- `result/warped_IHM/`: warped color-corrected images

## Project Structure

```txt
include/   Header files
src/       Implementation files
dataset/   Sample input images and transformation matrices
assets/    Example visual results
result/    Generated output images
```

## Copyright Notes

This repository references the IEEE paper for attribution and does not include the paper PDF, paper text, paper figures, or paper tables. The paper remains owned by its respective copyright holders. If you distribute this repository publicly, do not add the IEEE PDF or reproduce IEEE-provided figures/tables unless you have the required permission or license.

Add a separate repository license if you want to define how others may use this implementation code.
