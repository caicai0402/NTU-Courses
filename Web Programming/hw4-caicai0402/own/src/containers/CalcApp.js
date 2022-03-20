import React from 'react';
import CalcButton from '../components/CalcButton';

var l = 0;

// 計算機 App
class CalcApp extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      input_1 : 0,
      input_2 : 0,
      output : 0,
      operator : "=",
      AC_C : "AC",
      zero : 0,
      float : 0
    };
  }
  resetState = () => {
    if(this.state.AC_C === "AC"){
      this.setState({ input_1 : 0 });
      this.setState({ input_2 : 0 });
      this.setState({ output : 0 });
      this.setState({ operator : "=" });
      if(this.state.float !== 0){
        this.setState({ float : 0 });
        l = 0;}
      }
    else if(this.state.AC_C === "C"){
      this.setState({ input_2 : 0 });
      this.setState({ output : 0 });
      this.setState({ AC_C : "AC" });
      if(this.state.float !== 0){
        this.setState({ float : 0 });
        l = 0;}
      }
  }
  operate_fun = (e) => {
    let operator = e.target.innerText;
    this.setState({ zero : 1 });
    if(operator === "="){
      if(this.state.operator === "+"){
        let value = this.state.input_1 + this.state.input_2
        this.setState({ input_2 : value });
        this.setState({ output : value });}
      else if(this.state.operator === "-"){
        let value = this.state.input_1 - this.state.input_2
        this.setState({ input_2 : value });
        this.setState({ output : value });}
      else if(this.state.operator === "x"){
        let value = this.state.input_1 * this.state.input_2
        this.setState({ input_2 : value });
        this.setState({ output : value });}
      else if(this.state.operator === "÷"){
        let value = this.state.input_1/this.state.input_2
        this.setState({ input_2 : value });
        this.setState({ output : value });}}
    else{
      let value = this.state.input_2
      this.setState({ input_1 : value });}
    if(this.state.float !== 0){
      this.setState({ float : 0 });
      l = 0;}
    this.setState({ operator : operator });
  }
  float_fun = () => {
    this.setState({ float : 1 });
  }
  percentage_fun = () => {}
  opposite_fun = () => {}

  input_fun = (e) => {
    let num_btn = e.target.innerText;
    this.setState({ AC_C : "C" });
    var num;
    if(this.state.float === 1){
      num = this.state.input_2;
      num = num * 10 ** l;
      num = num * 10 + parseInt(num_btn);
      l += 1;
      num = num * 0.1 ** l;
      this.setState({ input_2: num });
      this.setState({ output: num });
    }
    else{
      if(this.state.zero === 0){
        num = this.state.input_2;
        num = num * 10 + parseInt(num_btn);
        this.setState({ input_2: num });
        this.setState({ output: num });}
      else{
        num = parseInt(num_btn);
        this.setState({ input_2: num });
        this.setState({ output: num });
        this.setState({ zero : 0 });}}
  }

  render() {
    return (
      <div className="calc-app">
        <div className="calc-container">
          <div className="calc-output">
            <div className="calc-display">{this.state.output}</div>
          </div>
          <div className="calc-row">
            <CalcButton onClick={this.resetState}>{this.state.AC_C}</CalcButton>
            <CalcButton onClick={this.opposite_fun}>+/-</CalcButton>
            <CalcButton onClick={this.percentage_fun}>%</CalcButton>
            <CalcButton className="calc-operator" onClick={this.operate_fun}>÷</CalcButton>
          </div>
          <div className="calc-row">
            <CalcButton className="calc-number" onClick={this.input_fun}>7</CalcButton>
            <CalcButton className="calc-number" onClick={this.input_fun}>8</CalcButton>
            <CalcButton className="calc-number" onClick={this.input_fun}>9</CalcButton>
            <CalcButton className="calc-operator" onClick={this.operate_fun}>x</CalcButton>
          </div>
          <div className="calc-row">
            <CalcButton className="calc-number" onClick={this.input_fun}>4</CalcButton>
            <CalcButton className="calc-number" onClick={this.input_fun}>5</CalcButton>
            <CalcButton className="calc-number" onClick={this.input_fun}>6</CalcButton>
            <CalcButton className="calc-operator" onClick={this.operate_fun}>-</CalcButton>
          </div>
          <div className="calc-row">
            <CalcButton className="calc-number" onClick={this.input_fun}>1</CalcButton>
            <CalcButton className="calc-number" onClick={this.input_fun}>2</CalcButton>
            <CalcButton className="calc-number" onClick={this.input_fun}>3</CalcButton>
            <CalcButton className="calc-operator" onClick={this.operate_fun}>+</CalcButton>
          </div>
          <div className="calc-row">
            <CalcButton className="bigger-btn" onClick={this.input_fun}>0</CalcButton>
            <CalcButton className="calc-number" onClick={this.float_fun}>.</CalcButton>
            <CalcButton className="calc-operator" onClick={this.operate_fun}>=</CalcButton>
          </div>
        </div>
      </div>
    );
  }
}

export default CalcApp;
