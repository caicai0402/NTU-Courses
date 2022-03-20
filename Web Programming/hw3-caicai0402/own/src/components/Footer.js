import React, {Component} from 'react';

class Footer extends Component {
    render(){
        if(this.props.box.All_box.length === 0){
            return null;
        }
        else{
        return (
            <footer className="todo-app__footer" id="todo-footer">
                <div className="todo-app__total">{this.props.box.Active_box.length} Left</div> 
                <ul className="todo-app__view-buttons">
                    <button onClick={this.props.all_function}>All</button>
                    <button onClick={this.props.active_function}>Active</button>
                    <button onClick={this.props.completed_function}>Completed</button>
                </ul>
                <div className="todo-app__clean">
                    <button onClick={this.props.clear_function}>Clear Completed</button>
                </div>
            </footer>
            );
        }
    }  
  }

  export default Footer;
