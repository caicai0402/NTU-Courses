import React from 'react';
import AQI from './AQI.png';

function Aqi(){
    return (
        <>
            <div style={{height:'55px'}}/>
            <div style={{height:'80%', margin:'1%', borderRadius:'10px', backgroundColor:'rgb(247, 247, 249)'}}>
                <h1 style={{margin:'1% 0 0 0', fontSize:'30'}}>AQI 指標</h1>
                <hr style={{margin:'0'}}/>
                <img src={AQI} alt="" width="88%" style={{margin:'1%'}}/>
            </div>
        </>);
};

export default Aqi;