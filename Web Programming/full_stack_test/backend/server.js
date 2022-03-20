import express from 'express';
import bodyParser from 'body-parser';
import cors from 'cors'
 
const app = express();
app.use(bodyParser.json());
app.use(cors())

const port = process.env.PORT || 4000
/*
app.get('/', (req, res) => {
  res.send('Hello, World!');
});
*/
app.get('/', (req, res) => {
  res.send('Received a GET HTTP method');
  console.log('GET is evoked');
});
app.post('/', (req, res) => {
  res.send('Received a POST HTTP method');
  //console.log(req.body.text);
});
app.put('/', (req, res) => {
  res.send('Received a PUT HTTP method');
});
app.delete('/', (req, res) => {
  res.send('Received a DELETE HTTP method');
});
app.listen(port, () =>
  console.log(`Example app listening on port ${port}!`),
);
