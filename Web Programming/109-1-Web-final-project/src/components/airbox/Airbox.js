import React from 'react';
import airbox from './airbox.jpg';
import '../bootstrap.min.css'

function Airbox(){
    return (
        <>
            <div style={{height:'55px'}}/>
            <div style={{height:'80%', margin:'1%', borderRadius:'10px', backgroundColor:'rgb(247, 247, 249)'}}>
                <h1 style={{margin:'1%', fontSize:'30'}}>Airbox 空氣盒子</h1>
                <hr style={{margin:'0'}}/>
                <div style={{height:'650px', borderRadius:'10px', display:'flex', flexDirection:'row', justifyContent:'center'}} >
                    <div className="card text-white bg-primary mb-3" style={{maxWidth: "40%", maxHeight:"80%" , margin:"65px"}}>
                        <div className="card-header">
                            <h3>簡介：</h3>
                        </div>
                        <div className="card-body">
                            <h4 className="card-text" style={{textAlign:'left'}}> 空氣盒子計劃是LASS核心概念的延伸，由華碩雲端、瑞昱半導體與中研院資訊所合作，發展一套簡易安裝的空氣品質監控設備，對於缺乏軟硬開發能力的人，提供一個監測PM2.5、溫度與濕度的管道，目前已提供300個空氣盒子給市民與學校，再將環境資訊上傳至雲端平台進行彙整，開放給研究機構分析汙染來源以及Makers進行創新加值應用，真正落實民眾參與、共同提升城市生活品質。</h4>
                            <br></br>
                            <h4 className="card-text" style={{textAlign:'left'}}>https://lass-net.org/2016/06/airbox/</h4>
                        </div>
                    </div>
                    <img src={airbox} alt="" width='40%' style={{float:'right', margin:'5%'}}/>
                </div>
                
            </div>
        </>);
};

export default Airbox;