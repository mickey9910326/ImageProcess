BMP的格式：「File Header + Info Header + (optional palette) + Raw Data」，不過我遇到的例子都是「File Header + Info Header + Raw Data」  

BMP   
1.  Bitmap file header
2.  DIB header

| 結構名稱 | 中文 | 必用 | 大小 | 功能 |附註|
| :--     | :-- | :-- | :-- | :-- | :-- |
| **Bitmap file header** | 點陣圖檔案頭    | O | 14 Bytes | 存放點陣圖檔案通用資訊 |  |
| **DIB header**         | DIB頭          | O | 7種格式 | 存放點陣圖詳細資訊及像素格式 |  |
| Extra bit masks        | 附加位遮罩      |   | 12 or 16 bytes | 定義像素格式 |  |
| Color table            | 調色盤          |   | 不定 | 定義圖像資料（像素陣列）所用顏色 |  |
| Gap1                   | 填充區A         |   | 不定 | 結構體對齊 |  |
| **Pixel array**        | 像素陣列        | O | 不定 | 存放像素數值 |  |
| Gap2                   | 填充區B         |   | 不定 | 結構體對齊 |  |
| ICC color profile      | ICC色彩特性資料 |   | 不定 | 定義色彩特性 |  |  


https://zh.wikipedia.org/wiki/BMP  
http://www.pcschool.com.tw/campus/share/lib/160/  
