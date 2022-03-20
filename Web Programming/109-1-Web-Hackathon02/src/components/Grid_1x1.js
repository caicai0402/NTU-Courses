import "./Grid.css"
import React, {Component} from 'react';

//TODO

class Grid_1x1 extends Component {
    render(){
    let left = "1.5px #999";
    let right = "1.5px #999";
    let top = "1.5px #999";
    let bottom = "1.5px #999";
    if(this.props.row_index === 0)
        left = "1.5px solidtransparent";
    if(this.props.row_index === 2)
        right = "1.5px solidtransparent";
    if(this.props.col_index ===0)
        top = "1.5px solidtransparent";
    if(this.props.col_index === 2)
        bottom = "1.5px solidtransparent";
    let gridStyle = {
        color: (this.props.selectedGrid.row_index === this.props.row_index && this.props.selectedGrid.col_index === this.props.col_index) || this.props.conflicted ? "#FFF" : this.props.fixed ? "#666" : "#6CC",
        backgroundColor: this.props.selectedGrid.row_index === this.props.row_index && this.props.selectedGrid.col_index === this.props.col_index ? "#333" : this.props.conflicted ? "#E77" : "#FFF",
        borderTop : top,
        borderBottom : bottom,
        borderLeft : left,
        borderRight : right    
    };
    return (
        <div className="grid_1x1" id={`grid-${this.props.row_index}*${this.props.col_index}`} tabindex="1" style={gridStyle} onClick={() => this.props.handle_grid_1x1_click(this.props.row_index, this.props.col_index)}>
            { this.props.value === "0" ? "" : this.props.value}
        </div>
    );
    }
}
 export default Grid_1x1; 

// Useful hints:
// for React Component:
// const gridStyle = {
//     color: (this.props.selectedGrid.row_index === this.props.row_index && this.props.selectedGrid.col_index === this.props.col_index) || this.props.conflicted ? "#FFF" : this.props.fixed ? "#666" : "#6CC",
//     backgroundColor: this.props.selectedGrid.row_index === this.props.row_index && this.props.selectedGrid.col_index === this.props.col_index ? "#333" : this.props.conflicted ? "#E77" : "#FFF",
// };
// return (
//     <div className="grid_1x1" id={`grid-${this.props.row_index}*${this.props.col_index}`} tabindex="1" style={gridStyle} onClick={() => this.props.handle_grid_1x1_click(this.props.row_index, this.props.col_index)}>
//         { this.props.value === "0" ? "" : this.props.value}
//     </div>
// );

// for React Hook:
// const gridStyle = {
//     color: (props.selectedGrid.row_index === props.row_index && props.selectedGrid.col_index === props.col_index) || props.conflicted ? "#FFF" : props.fixed ? "#666" : "#6CC",
//     backgroundColor: props.selectedGrid.row_index === props.row_index && props.selectedGrid.col_index === props.col_index ? "#333" : props.conflicted ? "#E77" : "#FFF",
// };
// return (
//     <div className="grid_1x1" id={`grid-${props.row_index}*${props.col_index}`} tabindex="1" style={gridStyle} onClick={() => props.handle_grid_1x1_click(props.row_index, props.col_index)}>
//         { props.value === "0" ? "" : props.value}
//     </div>
// );