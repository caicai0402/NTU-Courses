import React, {useState, useEffect} from 'react';
import '../bootstrap.min.css';

function Top({airbox, useStation, useYear, useMonth, useDay, useDevice_id, sendData, useIsscrap, scrapdata, scrapnum, setScrapdata, error, useLoading}){
    const [temp_year, setTemp_year] = useState(useYear.year);
    const [temp_month, setTemp_month] = useState(useMonth.month+1);
    const [temp_day, setTemp_day] = useState(useDay.day);
    var temp_station = useStation.station;
    const changeStation = (e) =>{temp_station = e.target.value;};
    const changeYear = (e) =>{setTemp_year(e.target.value);};
    const changeMonth = (e) =>{setTemp_month(e.target.value);};
    const changeDay = (e) =>{setTemp_day(e.target.value);};
    const how_many_days = () =>{
        if(temp_year===useYear.year && temp_month-1===useMonth.month)
            return useDay.day;
        var d = new Date(temp_year, temp_month, 0);
        return d.getDate();
    }
    const handle_data_search = () =>{ 
        useIsscrap.setIsscrap(false);
        useStation.setStation(temp_station);
        useLoading.setLoading(true);
        useDevice_id.setDevice_id(airbox.length!==0?airbox[airbox.length-1].Data.feeds[useStation.station].device_id:"");
        sendData(["search", `${temp_year}-${temp_month}-${temp_day}`]);
    }
    const handle_data_scrap = async() =>{
        useStation.setStation(temp_station);
        useLoading.setLoading(true);
        useDevice_id.setDevice_id(airbox.length!==0?airbox[airbox.length-1].Data.feeds[useStation.station].device_id:"");
        if(airbox.length===0){await sendData(["search", '2021-1-18']);}
        sendData(["scrap", airbox.length!==0?airbox[airbox.length-1].Data.feeds[useStation.station].device_id:"", `${temp_year}-0${temp_month}-${temp_day}`]);
    }

    return (
        <>
            <nav className="navbar navbar-expand-lg navbar-light bg-light" style={{margin: '1% 1%', borderRadius:'10px'}}>
                <b className="navbar-brand" href="#">{airbox.length!==0? airbox[airbox.length-1].Data.feeds[useStation.station].SiteName:""}</b>
                <h4 style={{color:'#FF0000'}}>{error?" ERROR! NO DATA!":""}</h4>
                <div className="collapse navbar-collapse" id="navbarColor03"></div>
                <select name="test" className="custom-select" style={{width:'200px', margin:'0 1%'}} onChange={changeStation}>
                    <option selected disabled hidden>{airbox.length!==0? airbox[airbox.length-1].Data.feeds[temp_station].SiteName:""}</option>
                    {airbox.length!==0? airbox[airbox.length-1].Data.feeds.map((i, j) =>(<option value={j}>{i.SiteName}</option>)):""}
                </select>
                <select className="custom-select" style={{width:'100px', margin:'0 0.5% 0 1%'}} onChange={changeYear}>
                    <option selected disabled hidden>{`${temp_year}年`}</option>
                    {Array.from(Array(useYear.year-2018), (i,j) =>j+2019).map((i) =>(<option value={i}>{`${i}年`}</option>))}
                </select>
                <select id="select_month" className="custom-select" style={{width:'100px', margin:'0 0.5% 0 0.5%'}} onChange={changeMonth}>
                    <option selected disabled hidden>{`${temp_month}月`}</option>
                    {Array.from(Array(temp_year===useYear.year? useMonth.month:12), (i,j) =>j+1).map((i) =>(<option value={i}>{`${i}月`}</option>))}
                </select>
                <select className="custom-select" style={{width:'100px', margin:'0 2% 0 0.5%'}} onChange={changeDay}>
                    <option selected disabled hidden>{`${temp_day}日`}</option>
                    {Array.from(Array(how_many_days()), (i,j) =>j+1).map((i) =>(<option value={i}>{`${i}日`}</option>))}
                </select>       
                <button type="button" className="btn btn-secondary" disabled={useLoading.loading} onClick={handle_data_search}>Search</button>
                <button type="button" className="btn btn-warning" disabled={useLoading.loading} onClick={handle_data_scrap} style={{margin:'0 0.5% 0 0.5%'}}>Scrap</button>
            </nav>
        </>);
};

export default Top;