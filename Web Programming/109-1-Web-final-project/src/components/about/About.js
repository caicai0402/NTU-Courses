import React from 'react';
import Giphy from './giphy.gif';

function About(){
    return (
        <>
            <div style={{height:'55px'}}/>
            <div style={{height:'80%', margin:'1%', borderRadius:'10px', backgroundColor:'rgb(247, 247, 249)'}}>
                <h1 style={{margin:'1%', fontSize:'30'}}>空氣監測網</h1>
                <hr style={{margin:'0'}}/>
                <div style={{height:'650px', borderRadius:'10px', display:'flex', flexDirection:'column', justifyContent:'center'}} >
                    <h4>空氣中飄浮著許多灰塵狀的粒狀物，我們稱之為懸浮微粒。其中以PM2.5大小對人體健康的影響最大，濃度過高容易引起嚴重的免疫反應，例如咳嗽、哮喘，甚至支氣管炎、小支氣管炎等。目前已經有許多流行病學研究都已顯示，長期暴露於 PM2.5會增加罹患肺癌、心血管疾病、白血病等風險，因此我們必須審慎看待，不可忽略它對健康影響的重要性。</h4>
                    <h4></h4>
                    <h4>目前，許多空氣監測平台相繼建置，讓一般大眾得以即時查看空氣品質，及早做好保護措施、預防受到危害。用來偵測空氣品質的空氣盒子逐漸普及，目前已漸漸涵蓋全台範圍，因此我們透過實際蒐集、網路爬蟲等方式，實際建置一網站，試著以資料視覺化的方式，呈現出單一測站在各個不同時間點的監測數據。</h4>
                    <h4></h4>
                    <h4>本網站的功能有二：手動上傳資料到資料庫的search功能、以及即時利用網路爬蟲下載全台開放資料的scrap功能。需要注意的是，search需要由後台上傳，由於目前空氣盒子實體機器還在建置中，故我們利用隨機上傳至資料庫幾筆資料，來模擬真正的情況。另外，由於開放資料平台的限制，我們只能取得過去七天內的監測資料，若超出範圍則會顯示錯誤。</h4>
                </div>
            </div>
        </>
    );
};

export default About;