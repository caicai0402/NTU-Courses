import {clickToGet} from './axios'
function App(){
    return(
        <div className="App">
            <button
                onClick={async()=>{
                    await clickToGet()
                }}
            >
            Click Me !
            </button>
        </div>
    );
}

