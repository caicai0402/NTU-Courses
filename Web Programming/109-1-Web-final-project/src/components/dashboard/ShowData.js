import React, {useState} from 'react';
import '../bootstrap.min.css';

function ShowData({airbox, station, indexlist, useIsscrap, scrapdata}){
    const [datatime, setDataTime] = useState(airbox.length!==0?airbox.length-1:0);
    return (
        <>
            <div style={{height:'45%', borderRadius:'10px', backgroundColor:'lavender', margin:'1%'}}>
                <h3 style={{margin:'2%'}}>測站：{useIsscrap.isscrap?scrapdata[0].name:airbox.length!==0?airbox[airbox.length-1].Data.feeds[station].SiteName:""}</h3>
                <h4 style={{margin:'2%'}}>資料時間：
                    <select className="custom-select" style={{width:'125px', margin:'0 2% 0 0.5%'}} onChange={(e) =>{setDataTime(e.target.value)}}>
                        <option selected disabled hidden>{useIsscrap.isscrap?scrapdata[0].time:airbox.length!==0?airbox[airbox.length-1].Data.feeds[station].time:""}</option>
                        {useIsscrap.isscrap?indexlist.map((i, j) =>(<option value={j}>{scrapdata[i].time}</option>)):airbox.length!==0?indexlist.map((i, j) =>(<option value={j}>{airbox[j].Data.feeds[i].time}</option>)):""}
                    </select>
                </h4>
                <h4>
                    <ul>
                        <li style={{margin:'2%'}}>PM2.5 : {useIsscrap.isscrap?scrapdata[datatime].s_d0:airbox.length!==0?airbox[datatime].Data.feeds[indexlist[datatime]].s_d0:""} ppm</li>
                        <li style={{margin:'2%'}}>PM10 : {useIsscrap.isscrap?scrapdata[datatime].s_d1:airbox.length!==0?airbox[datatime].Data.feeds[indexlist[datatime]].s_d1:""} ppm</li>
                        <li style={{margin:'2%'}}>PM1 : {useIsscrap.isscrap?scrapdata[datatime].s_d2:airbox.length!==0?airbox[datatime].Data.feeds[indexlist[datatime]].s_d2:""} ppm</li>
                        <li style={{margin:'2%'}}>Humidity : {useIsscrap.isscrap?scrapdata[datatime].s_h0:airbox.length!==0?airbox[datatime].Data.feeds[indexlist[datatime]].s_h0:""} %</li>
                        <li style={{margin:'2%'}}>CO2 : {useIsscrap.isscrap?scrapdata[datatime].s_t0:airbox.length!==0?airbox[datatime].Data.feeds[indexlist[datatime]].s_t0:""} ppm</li>
                    </ul>
                </h4>
            </div>
            
        </>);
};

export default ShowData;