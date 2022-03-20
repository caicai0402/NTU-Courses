import React, { useEffect } from 'react';
import mapboxgl from 'mapbox-gl';


function MapboxMap({airbox, station, useIsscrap, scrapdata}){
    mapboxgl.accessToken = 'pk.eyJ1IjoiY2FpY2FpMDQwMiIsImEiOiJja2p6eGZ4d2swYnVkMnZueWozM21nd3V2In0.KrnmI7BZAeeZ-ETTpZow5A';
    useEffect(() =>{
        //style: 'mapbox://styles/mapbox/light-v10'
        const map = new mapboxgl.Map({
            container: '0_map',
            style: 'mapbox://styles/mapbox/streets-v11',
            center: useIsscrap.isscrap?[scrapdata[0].gps_lon,scrapdata[0].gps_lat]:airbox.length!==0? [airbox[airbox.length-1].Data.feeds[station].gps_lon, airbox[airbox.length-1].Data.feeds[station].gps_lat] : [121.538517, 25.02718],
            zoom: 13
            });
        map.addControl(new mapboxgl.NavigationControl(), "top-right");
        if(airbox.length !== 0)
            new mapboxgl.Marker({color:"red"}).setLngLat(useIsscrap.isscrap?[scrapdata[0].gps_lon,scrapdata[0].gps_lat]:[airbox[airbox.length-1].Data.feeds[station].gps_lon, airbox[airbox.length-1].Data.feeds[station].gps_lat]).addTo(map);
        });
    return (
        <>
            <link href='https://api.mapbox.com/mapbox-gl-js/v1.10.1/mapbox-gl.css' rel='stylesheet' />
            <div id='0_map' style={{height:"98%", width:"98%", margin:"1%"}} />
        </>);
    
    return <></>
}

export default MapboxMap;
