import express from 'express';
import path from 'path';
import http from 'http';
import bodyParser from 'body-parser';
import WebSocket from 'ws';
import mongoose from 'mongoose';
import "dotenv-defaults/config.js"
import Airbox from'./server/models/airbox.js';
import puppeteer from 'puppeteer';
import apiRoute from './src/route/api.js'


const app = express();
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });
const port = process.env.PORT || 80;
//const apiRoute = require('./src/route/api');
var __dirname="";
app.use(express.static(path.join(__dirname, 'build')));
app.use('/api', apiRoute);
app.use(bodyParser.json());
app.get('/ping', function (req, res) {
  return res.send('pong');
});
app.get('/*', function (req, res) {
  res.sendFile(path.join(__dirname, 'build', 'index.html'));
});


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
    var today = new Date();
    Airbox.find({Date:{$eq: `${today.getFullYear()}-${today.getMonth()+1}-${today.getDate()}`}})
        .limit()
        .sort({ _id: 1 })
        .exec((err, res) => {
            if (err) throw err
                sendData(['init', res])
        });
    ws.onmessage = (message) => {

        async function par(device_id) {
            const browser = await puppeteer.launch();
            const page = await browser.newPage();
            await page.goto('https://pm25.lass-net.org/data/history.php?device_id=' + device_id);
            await page.waitFor(3000);
            const result = await page.evaluate(() => {
                let airbox_data = document.querySelector('body > pre').innerText;
                return airbox_data;
            }); 
            await browser.close();
            return result;
        };

        const { data } = message;
        const payload = JSON.parse(data);
        switch (payload[0]) {
            case 'search': {
                Airbox.find({Date:{$eq: payload[1]}})
                    .limit()
                    .sort({ _id: 1 })
                    .exec((err, res) => {
                        if (err) throw err
                            sendData(['search', res])
                    });
                break};
            case 'scrap': {
                const p2 = payload[2];
                par(payload[1]).then(async(value) => {
                    let temp = [];
                    value = JSON.parse(value);
                    for (var i=0;i < value.num_of_records-1;i++) {
                         let k = Object.keys(value.feeds[0].AirBox[i]);
                         if(value.feeds[0].AirBox[i][k].date===p2)
                             {temp.push(value.feeds[0].AirBox[i][k]);}
                    }
                    if(temp.length!==0){sendData(['scrap', [temp , temp.length]]);}
                    else{sendData(['error', 1]);}
                });
            }
            default:
                break
        };
    };
});

server.listen(port);
console.log(`Server ready and listening on port ${port}!`)

/*
server.listen(port, () =>
    console.log(`Listening on port ${port}!`),
);
*/
