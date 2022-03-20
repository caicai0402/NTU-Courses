import axios from 'axios'
const instance = axios.create({baseURL : 'http://localhost:4000' });
const clickToGet = async () => {
    const msg = await instance.get('/');
    console.log(msg);
    return msg;
}
export{clickToGet};

