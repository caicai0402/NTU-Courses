import '../App.css'
import React, { useEffect, useRef, useState, useCallback } from 'react'
import { useQuery, useMutation } from '@apollo/react-hooks'
import { Button, Input, message, Tag } from 'antd'
import {
    MESSAGES_QUERY,
    CREATE_MESSAGE_MUTATION,
    DELETE_MESSAGE_MUTATION,
    MESSAGES_SUBSCRIPTION,
    USER_QUERY,
    USER_WITH_PASSWORD_QUERY,
    UPDATE_USER_MUTATION
} from '../graphql'
import List from './List'

function Chat({username, password}){
    const [talk_to, setTalk_to] = useState('')
    const [body, setBody] = useState('')
    const { loading, error, data, subscribeToMore, refetch } = useQuery(MESSAGES_QUERY, {variables:{name: username, talk_to: talk_to, body:""}})
    const user_data = useQuery(USER_QUERY, {variables:{name: talk_to, password: ""}}, [talk_to])
    const my_user_data = useQuery(USER_WITH_PASSWORD_QUERY, {variables:{name: username, password: password}}, [username, password])
    const [friends, setFriends] = useState([])
    const [addMessage] = useMutation(CREATE_MESSAGE_MUTATION)
    const [clearMessage] = useMutation(DELETE_MESSAGE_MUTATION)
    const [updateUser] = useMutation(UPDATE_USER_MUTATION)
    const [selectedKey, setSelectedKey] = useState("1")
    const bodyRef = useRef(null)

    const handleSetfriends = useCallback(() => {
        if (my_user_data.loading) return null;
        if (my_user_data.error) return `Error! ${my_user_data.error}`;
        setFriends(my_user_data.data.users_with_password[0].friends)
    }, [my_user_data, setFriends])

    const displayStatus = (s) => {
        if (s.msg) {
            const { type, msg } = s
            const content = {
                content: msg,
                duration: 1}
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
                break}
        }    
    }
    useEffect(() => {
        subscribeToMore({
            document: MESSAGES_SUBSCRIPTION,
            updateQuery: (prev, { subscriptionData }) => {
            if (!subscriptionData.data) return prev
                const newMessage = subscriptionData.data
            console.log(newMessage)
            if(newMessage.message.mutation === "CREATED")
                if(newMessage.message.data.name === username || newMessage.message.data.talk_to === username)
                    return {
                        ...prev,
                        messages: [...prev.messages, newMessage.message.data]
                    }
            else if(newMessage.message.mutation === "DELETED")
                return {messages: []}
            }
        })
    }, [subscribeToMore])

    useEffect(() => {
        handleSetfriends()
    }, [handleSetfriends])

    useEffect(
        ()=>{
            let index = friends.indexOf(talk_to)
            if(index !== -1)
                setSelectedKey((index+2).toString())
        }, [friends, talk_to])
    
    const handleMessageSubmit = useCallback(
        async () => {
            if (!talk_to || !body){
                displayStatus({
                    type: 'error',
                    msg: 'Please enter a username and a message body.'
                })
                return
            }
            if (user_data.loading) return null;
            if (user_data.error) return `Error! ${user_data.error}`;
            if (user_data.data.users.length === 0){
                displayStatus({
                    type: 'error',
                    msg: 'User is not exit.'
                })
                return
            }
            if(my_user_data.data.users_with_password[0].friends.indexOf(talk_to) === -1){
                await updateUser({
                    variables: {
                        name: username,
                        password: password,
                        friends: friends,
                        update: talk_to}
                })
                setFriends(friends.concat(talk_to))
            }
            await addMessage({
                variables: {
                    name: username,
                    talk_to: talk_to,
                    body: body}
            })
            setBody('')
        }, [updateUser, addMessage, talk_to, body, friends, user_data, my_user_data])
    
    const handleMessageClear = useCallback(
        async () => {
        if (loading) return null;
        if (error) return `Error! ${error}`;
        if (data.messages.length === 0){
            displayStatus({
                type: 'error',
                msg: 'You do not have any message.'
            })
            return
        }
        var times = data.messages.length
        for(var i = 0; i < times; i++)
            await clearMessage({variables:{name:username, talk_to:talk_to, body:""}})
        setBody("")
    }, [clearMessage, loading, error, data, username, talk_to])

    return (
        <div className="App-test">
            <List
                friends={friends}
                selectedKey={selectedKey}
                setSelectedKey={setSelectedKey}
                setTalk_to={setTalk_to}
                refetch={refetch}
            />
            <div className="App" >
                <div className="App-title">
                    <h1>Simple Chat</h1>
                    <Button type="primary" danger onClick={handleMessageClear}> 
                        Clear
                    </Button>
                </div>
                <div className="App-messages">
                    {loading ? (
                        <p style={{ color: '#ccc' }}>
                            {'Loading...'}
                        </p>
                        ) : (data.messages.length === 0 ? (
                            <p style={{ color: '#ccc' }}>
                                {'No messages...'}
                            </p>
                            ) : (data.messages.map(({ name, body }, i) => (
                                <p className="App-message" key={i}>
                                    {name === username? 
                                    (<Tag color="purple">{name}</Tag>) : (<Tag color="blue">{name}</Tag>)}
                                    {body}
                                </p>))
                                ))}
                </div>
                <Input
                    placeholder="User name"
                    value={talk_to}
                    onChange={(e) => setTalk_to(e.target.value)}
                    style={{ marginBottom: 10 }}
                    onKeyDown={(e) => {
                    if (e.key === 'Enter') {
                        bodyRef.current.focus()}
                    }}
                ></Input>
                <Input.Search
                    rows={4}
                    value={body}
                    ref={bodyRef}
                    enterButton="Send"
                    onChange={(e) => setBody(e.target.value)}
                    placeholder="Type a message here..."
                    onSearch={handleMessageSubmit}
                ></Input.Search>
            </div>
            
        </div>
    )
}

export default Chat