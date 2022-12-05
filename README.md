### [第十二次平時作業](https://flipclass.stust.edu.tw/course/homework/58930)
- ESP32 抓空汙大數據 按鈕控制顯示地區(2022/11/24)

請設計一個系統，可以透過網路讀取網路全台灣PM2.5的數據```https://data.epa.gov.tw/api/v2/aqx_p_02?api_key=e8dd42e6-9b8b-43f8-991e-b3dee723a52d&limit=1000&sort=datacreationdate%20desc&format=JSON```，目前共77站，可以透過外接按鈕切換，每按一次，系統會切換到下 一個地點，系統會讀取目前偵測到的地點進行PM2.5的讀取，會根據下圖的範圍讓彩色LED顯示對應的顏色。

![參考圖片](https://www.myair.net.tw/myAir/img/img_airqi.jpg)
