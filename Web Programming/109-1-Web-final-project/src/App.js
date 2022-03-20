import './App.css';
import React, {useState} from 'react';
import NavBar from './components/NavBar.js';
import About from './components/about/About.js';
import Airbox from './components/airbox/Airbox.js';
import Aqi from './components/aqi/Aqi.js';
import Dashboard from './components/dashboard/Dashboard.js';

//https://bootswatch.com/minty/

const client = new WebSocket('ws://localhost:80');

function App(){
    const [airbox, setAirbox] = useState([]);
    const [loading, setLoading] = useState(false);
    const [isscrap, setIsscrap] = useState(false);
    const [scrapdata, setScrapdata] = useState([]);
    const [scrapnum, setScrapnum] = useState([]);
    const [error, setError] = useState(false);
    client.onopen = () => {
        console.log("Websocket has connected!")};
    const sendData = (data) => {client.send(JSON.stringify(data));};
    client.onmessage = (message) => {
        const { data } = message;
        const [task, payload] = JSON.parse(data);
        switch (task) {
            case 'init': {
                setAirbox(payload);
                break};
            case 'search': {
                setAirbox(payload);
                setError(false);
                setLoading(false);
                break};
            case 'scrap': {
                setScrapdata(payload[0]);
                setScrapnum(payload[1]);
                setIsscrap(true);
                setError(false);
                setLoading(false);
                break};
            case 'error': {
                    setError(true);
                break};
            default:
                break;
          }
    };

    return (
        <>
            <div className="App">
                <NavBar/>
                <header id="about" className="App-header">
                    <About/>
                </header>
                <header id="airbox" className="App-header">
                    <Airbox/>
                </header>
                <header id="aqi" className="App-header">
                    <Aqi/>
                </header>
                <header id="dashboard" className="App-header">
                    <Dashboard airbox={airbox} sendData={sendData} useIsscrap={{isscrap, setIsscrap}} scrapdata={scrapdata} scrapnum={scrapnum} setScrapdata={setScrapdata} useLoading={{loading, setLoading}} error={error}/>
                </header>
            </div>
        </>);
};

export default App;