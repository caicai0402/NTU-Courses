import React, {useState} from 'react';
import '../bootstrap.min.css';

function Top({airbox, useStation, useYear, useMonth, useDay, useLoading, sendData}){
    const [temp_year, setTemp_year] = useState(useYear.year);
    const [temp_month, setTemp_month] = useState(useMonth.month+1);
    const [temp_day, setTemp_day] = useState(useDay.day);
    var temp_station = useStation.station;
    const handle_date_list = (task)=>{
        switch(task){
            case 'year':
                return useYear.year-2018;
                break;
            case 'month':
                if(temp_year===useYear.year)
                    return useMonth.month;
                else return 12;
                break;
            case 'day':
                if(temp_year===useYear.year && temp_month-1===useMonth.month)
                    return useDay.day;
                var d = new Date(temp_year, temp_month, 0);
                return d.getDate();
                break;
            default:
                break;
        };
    }
    const handle_data_search = ()=>{
        useStation.setStation(temp_station);
        useLoading.setLoading(true);
        sendData(["search", `${temp_year}-${temp_month}-${temp_day}`]);
    }
    const handle_data_scrap = () =>{
        /*
        useStation.setStation(temp_station);
        useLoading.setLoading(true);
        sendData(["scrap", airbox.length!==0?airbox[airbox.length-1].Data.feeds[useStation.station].device_id:""]); //74DA38F7C64C
        */
    }

    return (
        <>
            <nav className="navbar navbar-expand-lg navbar-light bg-light" style={{margin: '1% 1%', borderRadius:'10px'}}>
                <b className="navbar-brand" href="#">{airbox.length!==0? airbox[airbox.length-1].Data.feeds[useStation.station].SiteName:""}</b>
                <div className="collapse navbar-collapse" id="navbarColor03"></div>
                <select name="test" className="custom-select" style={{width:'200px', margin:'0 1%'}} onChange={(e)=>temp_station = e.target.value}>
                    <option selected disabled hidden>{airbox.length!==0? airbox[airbox.length-1].Data.feeds[temp_station].SiteName:""}</option>
                    {airbox.length!==0? airbox[airbox.length-1].Data.feeds.map((i, j) =>(<option value={j}>{i.SiteName}</option>)):""}
                </select>
                <select className="custom-select" style={{width:'100px', margin:'0 0.5% 0 1%'}} onChange={(e)=>setTemp_year(parseInt(e.target.value))}>
                    <option selected disabled hidden>{`${temp_year}年`}</option>
                    {Array.from(Array(handle_date_list('year')), (i,j) =>j+2019).map((i) =>(<option value={i}>{`${i}年`}</option>))}
                </select>
                <select id="select_month" className="custom-select" style={{width:'100px', margin:'0 0.5% 0 0.5%'}} onChange={(e)=>setTemp_month(parseInt(e.target.value))}>
                    <option selected disabled hidden>{`${temp_month}月`}</option>
                    {Array.from(Array(handle_date_list('month')), (i,j) =>j+1).map((i) =>(<option value={i}>{`${i}月`}</option>))}
                </select>
                <select className="custom-select" style={{width:'100px', margin:'0 1% 0 0.5%'}} onChange={(e)=>setTemp_day(parseInt(e.target.value))}>
                    <option selected disabled hidden>{`${temp_day}日`}</option>
                    {Array.from(Array(handle_date_list('day')), (i,j) =>j+1).map((i) =>(<option value={i}>{`${i}日`}</option>))}
                </select>       
                <button type="button" className="btn btn-secondary" disabled={useLoading.loading} onClick={handle_data_search}>Search</button>
                <button type="button" className="btn btn-warning" disabled={true} onClick={handle_data_scrap} style={{margin:'0 0 0 1%'}}>Scrap</button>
            </nav>
        </>);
};

export default Top;