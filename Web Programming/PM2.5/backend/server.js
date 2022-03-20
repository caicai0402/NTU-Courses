import express from 'express';
import http from 'http';
import WebSocket from 'ws';
import mongoose from 'mongoose';
import "dotenv-defaults/config.js"
import Airbox from'./models/airbox.js';
import puppeteer from 'puppeteer';

const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });
const port = process.env.PORT || 4000;


if(!process.env.MONGO_URL){
    console.error('Missing MONGO_URL!!!')
    process.exit(1)
}

const db = mongoose.connect(
    process.env.MONGO_URL, {
    useNewUrlParser: true,
    useUnifiedTopology: true
}).then(res => {
    console.log('mongo db connection created!')
})

wss.on('connection', ws => {
    const sendData = (data) => {
        ws.send(JSON.stringify(data));
    };
    const par = async(device_id)=>{
        if(device_id.length === 0)
            return [];
        const browser = await puppeteer.launch();
        const page = await browser.newPage();
        await page.goto('https://pm25.lass-net.org/data/history.php?device_id=' + device_id);
        await page.waitForSelector('body > pre');
        const result = await page.evaluate(() => {
            let airbox_data = document.querySelector('body > pre').innerText;
            return airbox_data;
        }); 
        await browser.close();
        return JSON.parse(result);
    };
    var today = new Date();
    Airbox.find({Date:{$eq: `${today.getFullYear()}-${today.getMonth()+1}-${today.getDate()}`}})
        .limit()
        .sort({ _id: 1 })
        .exec((err, res) => {
            if (err) throw err
                sendData(['init', res])
        });
    ws.onmessage = (message) => {
        const { data } = message;
        const payload = JSON.parse(data);
        switch (payload[0]) {
            case 'search':
                Airbox.find({Date:{$eq: payload[1]}})
                    .limit()
                    .sort({ _id: 1 })
                    .exec((err, res) => {
                        if (err) throw err
                            sendData(['search', res])
                    });
                break;
            case 'scrap':
                /*
                if(payload[1].length===0)
                    sendData(['scrap', []]);
                par(payload[1]).then((value)=>{
                    sendData(['scrap', [{Data:value}]]);
                });
                */
                break;
            default:
                break
        };
    };
});

server.listen(port, () =>
    console.log(`Listening on port ${port}!`),
);