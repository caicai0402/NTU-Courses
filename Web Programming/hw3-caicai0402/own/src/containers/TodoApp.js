import React, { Component } from "react";
import Header from "../components/Header";
import Section from "../components/Section";
import Footer from "../components/Footer";

class TodoApp extends Component {
    constructor(props) {
        super(props);
        this.state = { 
            todo_list : [], 
            All_box : [], 
            Active_box : [], 
            Completed_box : [], 
            now : "All_box",
            t : 0};
    }

    keyup_function = (e) => {
        if(e.keyCode===13 && e.target.value!==''){
            let value = e.target.value;
            this.setState(state => ({todo_list : state.todo_list.concat(value) }));
            this.setState(state => ({All_box : state.All_box.concat(this.state.t) }));
            this.setState(state => ({Active_box : state.Active_box.concat(this.state.t) }));
            this.setState(state => ({t : state.t + 1}));
            e.target.value='';
        }    
    };

    done_function = (e) => {
        let value = e.target.id;
        let index = this.state[this.state.now][value];
        if(this.state.Active_box.indexOf(index)!==-1){
            this.setState(state => ({Completed_box : state.Completed_box.concat(index).sort() }));
            this.setState(state => ({Active_box : state.Active_box.filter(e => e !== index) }));
        }
        else{
            this.setState(state => ({Active_box : state.Active_box.concat(index).sort() }));
            this.setState(state => ({Completed_box : state.Completed_box.filter(e => e !== index) }));
        }
    };
    delete_function = (e) => {
        let value = e.target.id;
        value = parseInt(value);
        let index = this.state[this.state.now][value];
        this.setState(state => ({All_box : state.All_box.filter(e => e !== index) }));
        if(this.state.Active_box.indexOf(index) !== -1)
            this.setState(state => ({Active_box : state.Active_box.filter(e => e !== index) }));
        else if(this.state.Completed_box.indexOf(index) !== -1)
            this.setState(state => ({Completed_box : state.Completed_box.filter(e => e !== index) }));
        if(this.state.All_box.length === 1){
            this.setState(() => ({t : 0 }));
            this.setState(() => ({todo_list : [] }));}
    };
    all_function = () => this.setState({ now : "All_box"});
    active_function = () => this.setState({ now : "Active_box"});
    completed_function = () => this.setState({ now : "Completed_box"});
    clear_function = () => {
        this.setState(state => ({All_box : state.Active_box }));
        this.setState(() => ({Completed_box : [] }));
        if(this.state.All_box.length === 1){
            this.setState(() => ({t : 0 }));
            this.setState(() => ({todo_list : [] }));}
        };
    
    render(){
        return (
            <>
                <Header text="todos" />
                <Section 
                    box={this.state}
                    keyup_function={this.keyup_function}
                    done_function={this.done_function}
                    delete_function={this.delete_function}
                />
                <Footer
                    box={this.state}
                    all_function={this.all_function}
                    active_function={this.active_function}
                    completed_function={this.completed_function}
                    clear_function={this.clear_function}
                />

            </>
        );
    }
}

export default TodoApp;
