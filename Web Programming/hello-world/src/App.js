import React, {Component} from 'react';
import logo from './logo.svg';
import './App.css';



class CustomTextInput extends React.Component { 
  constructor(props){
    super(props);
    this.textInput = React.createRef();
  }

  focusTextInput = () => this.textInput.current.focus();

  componentDidMount(){ 
    this.focusTextInput();
  }

  render() {
      return (
        <div>
            <input type="Text" ref={this.textInput}/>
            <input type="button" value="Focus the text input" onClick={this.focusTextInput}/>
        </div> );
  } }
  export default CustomTextInput;


/*
class Counter extends Component {
  constructor(props) {
    super(props);
    this.state = { count: 100 };
  }
  handleInc = () => this.setState(state => ({ count: state.count + 1 }));
  handleDec = () => this.setState(state => ({ count: state.count - 1 }));
  render() {
    return (
      <div className="App">
        <h1 className="App-display">{this.state.count}</h1>
        <span className="App-controls">
          <button onClick={this.handleInc}>+</button>
          <button onClick={this.handleDec}>-</button>
        </span>
      </div>
      );
    } 
  }
export default Counter;
*/


/*
function App() {
  return (<h1>Hello, world!</h1>);}
export default App;
*/

/*
class ScoreCard extends Component {
  render(){
    let records = this.props.scoreCard.records.map
    (e => e.map(g => <td>{g}</td>) );
    return (
      <table>
        <caption>{this.props.scoreCard.name}'s Score</caption>
        <thead>
          <tr>
            <th> {this.props.columnIndex.map(e => <th>{e}</th>)} </th>
          </tr>
        </thead>
        <tbody>
          {records.map(e => <tr>{e}</tr>)}
        </tbody>
      </table>);
  }  
}
export default ScoreCard;
*/