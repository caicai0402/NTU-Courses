import './App.css';
import React, {useState} from 'react';
import NavBar from './components/NavBar.js';
import About from './components/about/About.js';
import Airbox from './components/airbox/Airbox.js';
import Aqi from './components/aqi/Aqi.js';
import Dashboard from './components/dashboard/Dashboard.js';


//https://bootswatch.com/minty/

const client = new WebSocket('ws://localhost:4000');

function App(){
    const [airbox, setAirbox] = useState([]);
    const [loading, setLoading] = useState(false);
    client.onopen = () => {console.log("Websocket has connected!")};
    const sendData = (data) => {client.send(JSON.stringify(data));};
    client.onmessage = (message) => {
        const { data } = message;
        const [task, payload] = JSON.parse(data);
        switch (task) {
            case 'init':
                setAirbox(payload);
                break;
            case 'search':
                setAirbox(payload);
                setLoading(false);
                break;
            case 'scrap':
                //setAirbox(payload);
                console.log(airbox, payload);
                setLoading(false);
                break;
            default:
                break;
          }
    };
    return (
        <>
            <div className="App" id="home">
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
                    <Dashboard
                        airbox={airbox}
                        sendData={sendData}
                        useLoading={{loading, setLoading}}
                    />
                </header>
                
            </div>
        </>);
};

export default App;