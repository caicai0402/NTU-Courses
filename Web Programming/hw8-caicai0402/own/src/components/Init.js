import '../App.css'
import React from 'react'
import { useQuery, useMutation } from '@apollo/react-hooks'
import { Button, Input, message, Tooltip } from 'antd'
import { EyeInvisibleOutlined, EyeTwoTone, InfoCircleOutlined, UserOutlined } from '@ant-design/icons'
import {
    USER_QUERY,
    USER_WITH_PASSWORD_QUERY,
    CREATE_USER_MUTATION,
} from '../graphql'

function Init({username, setUsername, setLogin, password, setPassword}) {
    const { loading, error, data } = useQuery(USER_WITH_PASSWORD_QUERY, {variables:{
        name: username,
        password: password
    }}, [username, password])
    const register_data = useQuery(USER_QUERY, {variables:{
        name: username,
        password: ""
    }}, [username])
    const [addUser] = useMutation(CREATE_USER_MUTATION)

    const displayStatus = (s) => {
        if (s.msg) {
            const { type, msg } = s
            const content = {
                content: msg,
                duration: 1
            }
        switch (type) {
            case 'success':
                message.success(content)
                break
            case 'info':
                message.info(content)
                break
            case 'danger':
                default:
                    message.error(content)
                break
            }
        }    
    }   
  
    const handleLogin = async () => {
        if( username === '' || password === '' ){
            displayStatus({
                type: 'error',
                msg: 'Please enter your name and password.'
            })
            return
        }
        else{
            if (register_data.loading) return null;
            if (register_data.error) return `Error! ${error}`;
            if(register_data.data.users.length === 0){
                displayStatus({
                    type: 'error',
                    msg: 'Name not found.'
                })
                return
            }
            if (loading) return null;
            if (error) return `Error! ${error}`;
            if(data.users_with_password.length === 0)
                displayStatus({
                    type: 'error',
                    msg: 'Password is incorrect.'
                })
            else if(data.users_with_password.length === 1)
                setLogin(true)
            return
        }
    }
  
    const handleRegister = async () => {
        if( username === '' || password === '' ){
            displayStatus({
                type: 'error',
                msg: 'Please enter your name and password.'
            })
            return
        }
        else{
            if (register_data.loading) return null;
            if (register_data.error) return `Error! ${error}`;
            if(register_data.data.users.length !== 0){
                displayStatus({
                    type: 'error',
                    msg: 'Name has been used.'
                })
                return
            }
            await addUser({
                variables:{
                    name: username,
                    password: password,
                    friends: []}
            })
            displayStatus({
                type: 'success',
                msg: 'You have already registered an account!'
            })
            setUsername("")
            setPassword("")
            setTimeout(window.location.reload(), 1000); 
        }
    }
  
    return (
        <div className="App">
            <h1>Simple Chat</h1>
            <Input
                placeholder="Enter your name"
                prefix={<UserOutlined className="site-form-item-icon" />}
                suffix={
                    <Tooltip title="You have to login!!">
                    <InfoCircleOutlined style={{ color: 'rgba(0,0,0,.45)' }} />
                    </Tooltip>}
                style={{ marginBottom: 20 }}
                value={username}
                onChange={(e) => setUsername(e.target.value)}
            />
            <Input.Password
                placeholder="Input password"
                iconRender={visible => (visible ? <EyeTwoTone /> : <EyeInvisibleOutlined />)}
                style={{ marginBottom: 20 }}
                value={password}
                onChange={(e) => setPassword(e.target.value)}
            />
            <div>
                <Button
                    type="primary"
                    float="left"
                    style={{ marginRight: 10 }}
                    onClick={handleLogin}
                >
                    Login
                </Button>
                <Button
                    type="primary"
                    float="right"
                    style={{ marginLeft: 10 }}
                    onClick={handleRegister}
                >
                    Register
                </Button>
            </div>
      </div>
    )    
    
}

export default Init