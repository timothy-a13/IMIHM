# [論文實作] IMIHM

本專案是以下論文方法的 C++/OpenCV 實作：

> Wenjie Li, Xuebin Liu, Jie Yang, Chongji Zhao, and Huan Deng, "A Color Correction Method Based on Incremental Multilevel Iterative Histogram Matching," IEEE Sensors Journal, vol. 24, no. 17, pp. 27892-27901, 2024.

論文連結：https://ieeexplore.ieee.org/document/10614104  
DOI：https://doi.org/10.1109/JSEN.2024.3432277

這是一份論文方法實作專案，不是該論文的官方頁面。

## 專案用途

本程式針對影像序列進行色彩校正，實作方向包含 histogram matching 與 incremental multilevel iterative histogram matching 流程。程式會讀取輸入影像資料夾與轉換矩陣資料夾，依序對目標影像做色彩校正，並輸出中間結果與 warp 後的結果影像。

## 環境需求

- CMake 3.20 或更新版本
- 支援 C++17 的編譯器
- OpenCV 4.x

目前 `CMakeLists.txt` 中的 `OpenCV_DIR` 設為：

```txt
C:/Program Files/OpenCV/opencv-4.10.0/build/install
```

如果你的 OpenCV 安裝在其他位置，請先修改 `CMakeLists.txt` 裡的 `OpenCV_DIR`，再進行 CMake 設定。

## 建置

在專案根目錄執行：

```powershell
cmake -S . -B build
cmake --build build --config Release
```

建置完成後，會在 `build` 目錄中產生 `IHM` 執行檔。

## 執行

用法：

```txt
IHM <Image Folder> <Tfm Matrix Folder> <Number of Image>
```

使用專案內範例資料夾的執行範例：

```powershell
.\build\IHM.exe .\dataset\img .\dataset\tfm 10
```

影像資料夾中應包含剛好 `Number of Image` 張影像。轉換矩陣資料夾中應包含：

- 每張影像對應的一個 `3 x 3` 轉換矩陣文字檔
- `shape.txt`
- `shift__H.txt`

程式會在處理前先排序檔案路徑，因此檔名需要維持一致且可排序的命名規則。

## 輸出

輸出路徑目前設定在 `src/global_folder_paths.cpp`。

預設會輸出到：

```txt
C:/Timothy/Code/IHM/result/
```

在其他電腦上執行前，請修改 `src/global_folder_paths.cpp` 裡的 `resultFolder`，或確認該輸出資料夾已存在。

輸出資料夾用途如下：

- `result/tmp/`：中間除錯影像
- `result/org_IHM/`：色彩校正後的序列影像
- `result/warped_IHM/`：warp 後的色彩校正影像

## 專案結構

```txt
include/   標頭檔
src/       程式實作
dataset/   範例輸入影像與轉換矩陣
assets/    範例結果圖
result/    產生的輸出影像
```

## 著作權注意事項

本專案僅引用 IEEE 論文作為出處說明，未包含論文 PDF、論文原文、論文圖片或論文表格。論文本身仍屬於其著作權持有人。若要公開散布此 repository，請不要加入 IEEE PDF，或重製 IEEE 提供的圖表，除非你已取得必要授權或使用許可。

若希望明確定義他人如何使用本實作程式碼，請另外加入 repository 授權條款。
