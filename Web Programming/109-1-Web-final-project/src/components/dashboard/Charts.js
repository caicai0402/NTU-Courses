import React, {useState} from 'react';
import {Line} from 'react-chartjs-2';

function Charts({airbox, indexlist, useIsscrap, scrapdata}){
    const items = [["PM2.5", "PM10", "PM1", "Humidity", "CO2"], ["s_d0", "s_d1", "s_d2", "s_h0", "s_t0"]];
    const [item, setItem] = useState(0);
    var x = [];
    var y = [];
    if (useIsscrap.isscrap) {
        for(var i = 0; i < scrapdata.length; i++){
            x.push(scrapdata[i].time);
            y.push(scrapdata[i][items[1][item]]);
        }
    }
    else if(airbox.length !== 0)
        for(var i = 0; i < airbox.length; i++){
            x.push(airbox[i].Data.feeds[indexlist[i]].time);
            y.push(airbox[i].Data.feeds[indexlist[i]][items[1][item]]);
        }
    const state = {
        labels: x,
        datasets: [{
            label: 'Rainfall',
            fill: false,
            lineTension: 0,
            backgroundColor: 'pink',
            borderColor: 'navy',
            borderWidth: 3,
            data: y
          }]
    };
    return (
        <>
            <div style={{height:'50%', borderRadius:'10px', backgroundColor:'lavender', margin:'1%'}}>
                <select className="custom-select" style={{width:'125px', margin:'1%'}} onChange={(e) =>{setItem(e.target.value)}}>
                    <option selected disabled hidden>{items[0][item]}</option>
                    {items[0].map((i, j) =>(<option value={j}>{i}</option>))}
                </select>
                <div style={{height:'80%', margin:'1%', background:'rgb(247, 247, 249)'}}>
                    <Line
                        data={state}
                        options={{
                            title:{
                                display:true,
                                text:`${items[0][item]}`,
                                fontSize:20
                            },
                            legend:{
                                display:false,
                            },
                            responsive: true,
                            maintainAspectRatio: false,
                            
                        }}
                    />
                </div>
            </div>
        </>);
};

export default Charts;