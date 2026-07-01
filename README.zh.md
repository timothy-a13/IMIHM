# [論文實作] IMIHM

[中文](./README.zh.md) | [English](./README.md)

IMIHM 是以 C++/OpenCV 實作的影像序列色彩校正流程。此專案對應下列 IEEE 論文方法，主要處理已排序的影像資料夾與預先計算好的轉換矩陣。

## 論文

- 題名：A Color Correction Method Based on Incremental Multilevel Iterative Histogram Matching
- IEEE Xplore：https://ieeexplore.ieee.org/document/10614104
- DOI：https://doi.org/10.1109/JSEN.2024.3432277
- 出版資訊：IEEE Sensors Journal, Vol. 24, No. 17, pp. 27892-27901, Sept. 1, 2024

## 專案內容

程式會讀取影像序列與其對應的轉換矩陣，執行 incremental multilevel iterative histogram matching，並輸出校正後影像與 warp 後影像。

主要流程包含：

1. 讀取輸入影像與 `3 x 3` 轉換矩陣。
2. 使用提供的 homography 矩陣對齊相鄰影像。
3. 估計重疊區域，並以 histogram matching 進行色彩校正。
4. 在目標影像的局部切片區域中逐步傳遞校正結果。
5. 輸出校正後序列影像與 warp 後影像，供後續 mosaic 檢視。

## 執行結果範例

| 原始 mosaic | IMIHM 校正結果 |
| --- | --- |
| ![原始 mosaic](assets/original.png) | ![IMIHM 校正結果](assets/IMIHM.png) |

## 建置需求

- Windows
- CMake 3.20 或更新版本
- 支援 C++17 的編譯器
- OpenCV 4.10.0（目前預設 CMake 設定）

專案也可以改用其他 OpenCV 4.x 版本建置，但目前 `CMakeLists.txt` 預設指向 OpenCV 4.10.0。

## OpenCV 設定

預設 CMake 設定會尋找以下 OpenCV 路徑：

```text
C:/Program Files/OpenCV/opencv-4.10.0/build/install
```

此路徑設定在 `CMakeLists.txt`：

```cmake
set(OpenCV_DIR "C:/Program Files/OpenCV/opencv-4.10.0/build/install")
```

如果 OpenCV 安裝在其他位置，請在 CMake 設定前修改 `OpenCV_DIR`。

執行時也需要讓程式找得到 OpenCV runtime DLL，可將 OpenCV 的 binary 目錄加入 `PATH`，或將 DLL 放在產生出的執行檔旁邊。

## 建置方式

在專案根目錄執行：

```powershell
cmake -S . -B build
cmake --build build --config Release
```

建置完成後，會在 `build` 目錄中產生 `IHM` 執行檔。

## 輸入資料格式

`src/main.cpp` 需要三個命令列參數：

```text
IHM <Image Folder> <Tfm Matrix Folder> <Number of Image>
```

專案內範例輸入資料使用以下格式：

```text
dataset/
  img/
    IMG_0073.JPG
    IMG_0074.JPG
    ...
  tfm/
    00__H_.txt
    01__H_.txt
    ...
    shape.txt
    shift__H.txt
```

影像資料夾中必須包含剛好 `Number of Image` 張影像。轉換矩陣資料夾中必須包含每張影像對應的一個 `3 x 3` 矩陣文字檔，以及 `shape.txt` 和 `shift__H.txt`。

程式會在處理前先排序檔案路徑，因此檔名需要維持一致且可排序的命名規則。

輸出路徑目前硬編碼在 `src/global_folder_paths.cpp`：

```cpp
std::string resultFolder = "C:/Timothy/Code/IHM/result/";
```

若希望輸出到其他位置，執行前請先修改此路徑。

## 執行方式

建置完成後，使用影像資料夾、轉換矩陣資料夾與影像數量執行：

```powershell
.\build\IHM.exe .\dataset\img .\dataset\tfm 10
```

校正後結果會輸出到：

```text
<result-folder>/org_IHM/
<result-folder>/warped_IHM/
```

中間除錯影像會輸出到：

```text
<result-folder>/tmp/
```

## 專案結構

- `src/main.cpp`：命令列參數解析與整體影像序列處理流程。
- `src/ihm.cpp`、`include/ihm.hpp`：incremental multilevel iterative histogram matching 流程。
- `src/hm.cpp`、`include/hm.hpp`：histogram 計算、CDF 對應與色彩校正工具。
- `src/loader.cpp`、`include/loader.hpp`：影像與轉換矩陣載入。
- `src/util.cpp`、`include/util.hpp`：perspective transformation、重疊區域估計與輸出資料夾清理。
- `src/global_folder_paths.cpp`、`include/global_folder_paths.hpp`：硬編碼輸出資料夾設定。
- `dataset/`：範例影像與轉換矩陣。
- `assets/`：本 README 使用的範例結果圖。

## 著作權注意事項

此 repository 目標是保存論文方法的獨立程式實作，不應未經授權散布 IEEE 論文 PDF、論文中的圖表、表格、截圖，或大量逐字引用論文內容。

本 README 只提供必要的書目資訊與 IEEE Xplore/DOI 連結。若要上傳資料集、產生出的影像結果或評估檔案，也請先確認來源資料集的授權或散布條款。

目前專案尚未包含 `LICENSE` 檔案。若此 repository 要公開釋出，建議在上傳前加入授權檔，讓使用者知道此實作程式碼可如何使用。
