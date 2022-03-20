import React, {useState} from 'react';
import Top from './Top';
import MapboxMap from './MapboxMap.js';
import ShowData from './ShowData.js';
import Charts from './Charts.js';

function Dashboard({airbox, sendData, useLoading}){
    const today = new Date()
    const [station, setStation] = useState(0);
    const [year, setYear] = useState(today.getFullYear());
    const [month, setMonth] = useState(today.getMonth());
    const [day, setDay] = useState(today.getDate());
    var indexlist = [];
    if(airbox.length !== 0)
        for(var i = 0; i < airbox.length; i++)
            indexlist.push(airbox[i].Data.feeds.findIndex(x => x.SiteName === `${airbox[airbox.length-1].Data.feeds[station].SiteName}`));
    return (
        <>
            <div style={{height:'55px'}}/>
            <Top
                airbox={airbox}
                useStation={{station, setStation}}
                useYear={{year, setYear}}
                useMonth={{month, setMonth}}
                useDay={{day, setDay}}
                useLoading={useLoading}
                sendData={sendData}
            />
            <div style={{display:'flex', flexDirection:'row', height:'650px'}}>
                <div style={{display:'flex', flexDirection:'column', alignItems:'center', textAlign:'left', height: '100%', width:'70%', margin: '0% 0.5% 1% 1%', borderRadius:'10px', backgroundColor:'rgb(247, 247, 249)'}}>
                    <MapboxMap airbox={airbox} station={station}/>
                </div>
                <div style={{textAlign:'left', height:'100%', width:'30%', margin: '0% 1% 1% 0.5%', borderRadius:'10px', backgroundColor:'rgb(247, 247, 249)'}}>
                    <ShowData airbox={airbox} station={station} indexlist={indexlist}/>
                    <hr style={{margin:'2%'}}/>
                    <Charts airbox={airbox} indexlist={indexlist}/>
                </div>

            </div>
        </>
    );
};

export default Dashboard;