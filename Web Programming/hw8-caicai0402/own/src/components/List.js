import React from 'react'
import { Menu } from 'antd'
import { MailOutlined } from '@ant-design/icons'
import '../App.css'

function List({friends, selectedKey, setSelectedKey, setTalk_to, refetch}){
    return (
        <>
            <div className="App-list">
                <Menu
                selectedKeys={[selectedKey]}
                mode="inline"
                theme="dark"
                >
                    <Menu.Item
                        key="1"
                        icon={<MailOutlined />}
                        onClick={
                            async(e)=>{
                                setSelectedKey(e.key)
                                await setTalk_to("")
                                refetch()}}
                    >
                        All
                    </Menu.Item>
                    {friends.map((item, i) => (
                        <Menu.Item key={(i+2).toString()} icon={<MailOutlined/>} onClick={async(e)=>{setSelectedKey(e.key); await setTalk_to(item)}}>
                            {item}
                        </Menu.Item>
                    ))}
                </Menu>
            </div>
        </>
    )
}

export default List