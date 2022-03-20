import { useState } from 'react'
import './App.css'
import { guess, startGame, restart } from './axios'

function App() {
  const [hasStarted, setHasStarted] = useState(false)
  const [hasWon, setHasWon] = useState(false)
  const [number, setNumber] = useState('')
  const [status, setStatus] = useState('')

  const startMenu = (
    <div>
      <button
        onClick={async () => {
          await startGame()
          setHasStarted(true)
        }}
      >
        start game
      </button>
    </div>
  )

  const game = (
    <div>
      {hasWon ? (
        <>
          <p>you won! the number was {number}.</p>
          <button
            onClick={async () => {
              await restart()
              setHasWon(false)
              setStatus('')
              setNumber('')
            }}
          >
            restart
          </button>
        </>
      ) : (
        <>
          <p>Guess a number between 1 to 100</p>
          <input
            onChange={(e) => setNumber(e.target.value)}
            value={number}
            onKeyUp = { 
              async (e) => {
                if(e.keyCode === 13 && e.target.value !== ''){
                  let msg = await guess(number)
                  setHasWon(msg === 'You Win!' ? true : false)
                  setNumber(msg === 'You Win!' ? number : '')
                  setStatus(msg)
                }
              }
            }
          ></input>
          <button
            // TODO: use async/await to call guess(number),
            // process the response to set the proper state values
            onClick={async () => {
              let msg = await guess(number)
              setHasWon(msg === 'You Win!' ? true : false)
              setNumber(msg === 'You Win!' ? number : '')
              setStatus(msg)
            }}
            disabled={!number}
          >
            guess!
          </button>
          <p>{status}</p>
        </>
      )}
    </div>
  )

  return <div className="App">{hasStarted ? game : startMenu}</div>
}

export default App
