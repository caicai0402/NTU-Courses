import React, {useState} from 'react';
import Top from './Top';
import MapboxMap from './MapboxMap.js';
import ShowData from './ShowData.js';
import Charts from './Charts.js';

function Dashboard({airbox, sendData, useIsscrap, scrapdata, scrapnum, setScrapdata, error, useLoading}){
    const today = new Date()
    const [station, setStation] = useState(0);
    const [year, setYear] = useState(today.getFullYear());
    const [month, setMonth] = useState(today.getMonth());
    const [day, setDay] = useState(today.getDate());
    const [device_id, setDevice_id] = useState("");
    var indexlist = [];
    if(airbox.length !==0){
        for(var i = 0; i < airbox.length; i++)
            {indexlist.push(airbox[i].Data.feeds.findIndex(x => x.SiteName === `${airbox[airbox.length-1].Data.feeds[station].SiteName}`));}
    }
    if(useIsscrap.isscrap){
        indexlist = [];
        for(var i = 0; i < scrapnum; i++)
            {indexlist.push(i);}
    }

    return (
        <>
            <div style={{height:'55px'}}/>
            <Top
                airbox={airbox}
                useStation={{station, setStation}}
                useYear={{year, setYear}}
                useMonth={{month, setMonth}}
                useDay={{day, setDay}}
                useDevice_id={{device_id, setDevice_id}}
                sendData={sendData}
                useIsscrap={useIsscrap}
                scrapdata={scrapdata}
                setScrapdata={setScrapdata}
                error={error}
                useLoading={useLoading}
            />
            <div style={{display:'flex', flexDirection:'row', height:'650px'}}>
                <div style={{display:'flex', flexDirection:'column', alignItems:'center', textAlign:'left', height: '100%', width:'70%', margin: '0% 0.5% 1% 1%', borderRadius:'10px', backgroundColor:'rgb(247, 247, 249)'}}>
                    <MapboxMap airbox={airbox} station={station} useIsscrap={useIsscrap} scrapdata={scrapdata}/>
                </div>
                <div style={{textAlign:'left', height:'100%', width:'30%', margin: '0% 1% 1% 0.5%', borderRadius:'10px', backgroundColor:'rgb(247, 247, 249)'}}>
                    <ShowData airbox={airbox} station={station} indexlist={indexlist} useIsscrap={useIsscrap} scrapdata={scrapdata}/>
                    <hr style={{margin:'2%'}}/>
                    <Charts airbox={airbox} indexlist={indexlist} useIsscrap={useIsscrap} scrapdata={scrapdata}/>
                </div>
            </div>
        </>
    );
};

export default Dashboard;