import React from 'react';
import './bootstrap.min.css';

/*
<li className="nav-item active">
    <a className="nav-link" href="#home">Home
        <span className="sr-only">(current)</span>
    </a>
</li>
*/

function NavBar(){
    return (
        <>
            <nav className="navbar navbar-expand-lg navbar-dark bg-primary" style={{position:'fixed', top:'0', left:'0', width:'100%', zIndex:'999'}}>
                <a className="navbar-brand" href="#home">PM2.5 OPEN DATA</a>
                <button className="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarColor01" aria-controls="navbarColor01" aria-expanded="false" aria-label="Toggle navigation">
                    <span className="navbar-toggler-icon"></span>
                </button>
                <div className="collapse navbar-collapse" id="navbarColor01">
                    <ul className="navbar-nav mr-auto">
                        <li className="nav-item active">
                            <a className="nav-link" href="#about">About</a>
                        </li>
                        <li className="nav-item active">
                            <a className="nav-link" href="#airbox">Airbox</a>
                        </li>
                        <li className="nav-item active">
                            <a className="nav-link" href="#aqi">AQI</a>
                        </li>
                        <li className="nav-item active">
                            <a className="nav-link" href="#dashboard">Dashboard</a>
                        </li>
                    </ul>
                    <form className="form-inline my-2 my-lg-0">
                        <input className="form-control mr-sm-2" type="text" placeholder="Search"/>
                        <button className="btn btn-warning" type="button">Search</button>
                    </form>
                </div>
            </nav>
        </>);
};

export default NavBar;