import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import * as serviceWorker from './serviceWorker';
import CustomTextInput from './App';



ReactDOM.render(
  <CustomTextInput />,
  document.getElementById('root'));


/*
ReactDOM.render(
  <Counter />,
  document.getElementById('root'));
*/

/*
const columnIndex = [ 'Subject', 'Score' ];
const scoreCard = {
  name: 'Cai Cai',
  records: [
    [ 'Chinese', 98 ],
    [ 'English', 99 ],
    [ 'Math', 100 ]
  ]
};

ReactDOM.render(
<ScoreCard scoreCard = {scoreCard} columnIndex = {columnIndex} / >,
document.getElementById('root'));
*/





// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: https://bit.ly/CRA-PWA
serviceWorker.unregister();
