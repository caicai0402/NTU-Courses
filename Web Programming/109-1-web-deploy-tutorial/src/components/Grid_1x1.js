import React from 'react';
import "./Grid.css"

export default function Grid_1x1(props) {
    const gridStyle = {
        color: (props.selectedGrid.row_index === props.row_index && props.selectedGrid.col_index === props.col_index) || props.conflicted ? "#FFF" : props.fixed ? "#666" : "#6CC",
        backgroundColor: props.selectedGrid.row_index === props.row_index && props.selectedGrid.col_index === props.col_index ? "#333" : props.conflicted ? "#E77" : "#FFF",
        borderLeft: props.col_index % 3 === 0 ? "1.5px solid transparent" : "1.5px solid #999",
        borderRight: props.col_index % 3 === 2 ? "1.5px solid transparent" : "1.5px solid #999",
        borderTop: props.row_index % 3 === 0 ? "1.5px solid transparent" : "1.5px solid #999",
        borderBottom: props.row_index % 3 === 2 ? "1.5px solid transparent" : "1.5px solid #999",
    };
    return (
        <div className="grid_1x1" id={`grid-${props.row_index}*${props.col_index}`} tabindex="1" style={gridStyle} onClick={() => props.handle_grid_1x1_click(props.row_index, props.col_index)}>
            { props.value === "0" ? "" : props.value}
        </div>
    );
}