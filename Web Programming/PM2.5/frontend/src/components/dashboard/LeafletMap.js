import React, { useEffect } from 'react';
import 'leaflet/dist/leaflet.css';
import L from 'leaflet';


function LeafletMap(){
    useEffect(()=>{
        const map = L.map('0_map').setView([25.017, 121.538], 16);
        L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png").addTo(map);
        /*
// 使用 leaflet-color-markers ( https://github.com/pointhi/leaflet-color-markers ) 當作 marker
        const greenIcon = new L.Icon({
            iconUrl: "https://cdn.rawgit.com/pointhi/leaflet-color-markers/master/img/marker-icon-2x-green.png",
            shadowUrl: "https://cdnjs.cloudflare.com/ajax/libs/leaflet/0.7.7/images/marker-shadow.png",
            iconSize: [25, 41],
            iconAnchor: [12, 41],
            popupAnchor: [1, -34],
            shadowSize: [41, 41]
            });
        const marker = L.marker([25.03418, 121.564517], { icon: greenIcon }).addTo(mymap);
        marker.bindPopup("<b>Hello world!</b><br>I am a popup.").openPopup();
        L.circle([25.03418, 121.564517], {
            color: "red",
            fillColor: "#f03",
            fillOpacity: 0.5,
            radius: 10
            }).addTo(mymap);
*/
    });
    
    return (
        <>
            <div id='0_map' style={{ height:"98%", width:"98%", margin:"1%"}}/>
        </>);
};

export default LeafletMap;
