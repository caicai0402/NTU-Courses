# 空氣盒子監測網

用資料庫或網路爬蟲的方式，取得空氣偵測器的數據，並以資料視覺化的方式呈現出來。

## 主要功能介紹

網站的主要功能為顯示空氣盒子定時回傳的數據，包括PM2.5,CO2等氣體的逐時變化圖，以及測站所在的位置地圖。
數據取得有兩種方式：

### search模式
需先手動將數據加入至mongoDB，即可以特定日期、時間做查詢。由於空氣盒子實體機器還正在建置階段，因此我們隨機上傳資料至MongoDB，來模擬真正的情況，故目前只有2020/1/18日有資料。

### scrap模式
自動以puppeteer爬蟲下載開放資料平台的測站數據，也利用日期及時間做查詢。不過由於平台的限制，最多只能取得過去七天內的數據。

若該日期查無資料，則會在畫面左上方顯示Error! No Data!提醒字樣。

## 使用之第三方套件、框架

### Frondend
react chart.js mapbox-gl dotenv bootstrap

### Backend 
Express.js websocket MongoDB nodemon puppeteer
