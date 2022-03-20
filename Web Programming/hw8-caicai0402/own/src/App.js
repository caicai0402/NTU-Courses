import './App.css'
import React, { useState } from 'react'
import Chat from './components/Chat'
import Init from './components/Init'

function App() {
    const [username, setUsername] = useState('')
    const [password, setPassword] = useState('')
    const [login, setLogin] = useState(false)

return (
  <>
    {!login?
      (
      <Init
        username={username} 
        setUsername={setUsername}
        login={login}
        setLogin={setLogin}
        password={password}
        setPassword={setPassword}
      />
      )
      : (
        <>
          <Chat
            username={username}
            password={password}
          />
        </>
      )
    }
  </>)
}

export default App
