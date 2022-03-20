import React, { Component } from 'react';
import x_image from '../img/x.png'

class Section extends Component {
    render() {
        let box = this.props.box;
        let now = box.now;

        let doms = box[now].map((e, i) => 
            <li className="todo-app__item">
                <div className="todo-app__checkbox" >
                    <input id={i} type="checkbox" checked={box.Completed_box.indexOf(e)!==-1 ? true:false} onClick={this.props.done_function} />
                    <label for={i} />
                </div>
                <h1 className="todo-app__item-detail" style={box.Completed_box.indexOf(e)!==-1 ? {textDecoration:'line-through',opacity:0.5}:{}} >
                    {box.todo_list[e]}
                </h1>
                <img src={x_image} className="todo-app__item-x" onClick={this.props.delete_function} id={i+"img"} alt="X"/>
            </li>
            );

        return (
            <section className="todo-app__main">
                <input className="todo-app__input" onKeyUp={this.props.keyup_function} placeholder="What needs to be done?" id="todo-input" />
                <ul class="todo-app__list" id="todo-list">
                    {doms}
                </ul>
            </section>);
    }
}
export default Section;
