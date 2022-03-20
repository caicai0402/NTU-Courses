import React, { Component } from 'react';
import ReactLoading from "react-loading";
import { Fireworks } from 'fireworks/lib/react'

import "./Sudoku.css"
import Header from '../components/Header';
import Grid_9x9 from '../components/Grid_9x9';
import ScreenInputKeyBoard from '../components/ScreenInputKeyBoard'
import { problemList } from "../problems"

class Sudoku extends Component {
    constructor(props) {
        super(props);
        this.state = {
            loading: true, // Return loading effect if this is true.
            problem: null, // Stores problem data. See "../problems/" for more information.This is the origin problem and should not be modified. This is used to distinguish the fixed numbers from the editable values
            gridValues: null,  // A 2D array storing the current values on the gameboard. You should update this when updating the game board values.
            selectedGrid: { row_index: -1, col_index: -1 }, // This objecct store the current selected grid position. Update this when a new grid is selected.
            gameBoardBorderStyle: "8px solid #000", // This stores the gameBoarderStyle and is passed to the gameboard div. Update this to have a error effect (Bonus #2).
            completeFlag: false, // Set this flag to true when you wnat to set off the firework effect.
            conflicts: [{ row_index: -1, col_index: -1 }] // The array stores all the conflicts positions triggered at this moment. Update the array whenever you needed.
        }
    }

    handle_grid_1x1_click = (row_index, col_index) => {
        if (this.state.problem.content[row_index][col_index] === "0") {
            this.setState({ selectedGrid: { row_index: row_index, col_index: col_index } });
        }
    }

    handleKeyDownEvent = (e) => {
        if (this.state.gridValues !== null && this.state.selectedGrid.row_index !== -1 && this.state.selectedGrid.col_index !== -1 && ((e.keyCode >= 48 && e.keyCode <= 57) || (e.keyCode >= 96 && e.keyCode <= 105) || e.code === "Space")) {
            if (this.state.problem.content[this.state.selectedGrid.row_index][this.state.selectedGrid.col_index] === "0") {
                if (this.findConflicts(this.state.selectedGrid.row_index, this.state.selectedGrid.col_index, e.key === " " ? "0" : e.key)) {
                    let new_gridValues = this.state.gridValues;
                    new_gridValues[this.state.selectedGrid.row_index][this.state.selectedGrid.col_index] = e.key === " " ? "0" : e.key;
                    this.setState({ gridValues: new_gridValues })
                    if (this.checkCompleted(new_gridValues)) {
                        this.setState({ completeFlag: true });
                        setTimeout(() => { this.setState({ completeFlag: false }); }, 2500);
                    };
                } else {
                    this.setState({ gameBoardBorderStyle: "8px solid #E77" });
                    setTimeout(() => { this.setState({ gameBoardBorderStyle: "8px solid #333" }); }, 1000);
                }
            }
        }
    }

    handleScreenKeyboardInput = (num) => {
        this.handleKeyDownEvent({ key: String(num), keyCode: num + 48 });
    }

    findConflicts = (row_index, col_index, value) => {
        let rt = true;
        let conflicts = [];
        if (value === "0")
            return true;
        for (let i = 0; i < 9; ++i) {
            if (i !== row_index && this.state.gridValues[i][col_index] === value) {
                rt = false;
                conflicts.push({ row_index: i, col_index: col_index });
            }
            if (i !== col_index && this.state.gridValues[row_index][i] === value) {
                rt = false;
                conflicts.push({ row_index: row_index, col_index: i });
            }
            for (let j = 0; j < 9; ++j) {
                const target_row_index = Math.floor(row_index / 3) * 3 + Math.floor(j / 3);
                const target_col_index = Math.floor(col_index / 3) * 3 + j % 3;
                if (target_row_index !== row_index && target_col_index !== col_index && this.state.gridValues[target_row_index][target_col_index] === value) {
                    rt = false;
                    conflicts.push({ row_index: target_row_index, col_index: target_col_index });
                }
            }
        }
        this.setState({ conflicts: conflicts });
        setTimeout(() => { this.setState({ conflicts: [] }); }, 1000);
        return rt;
    }

    checkCompleted = (gridValue) => {
        for (let i = 0; i < gridValue.length; ++i) {
            for (let j = 0; j < gridValue[i].length; ++j) {
                if (gridValue[i][j] === "0") {
                    return false;
                }
            }
        }
        return true;
    }

    handleResetGame = () => {
        if (this.state.problem !== null && this.state.problem !== null) {
            this.setState({ gridValues: this.state.problem.content });
        }
    }

    handleAutoComplete = () => {
        if (this.state.gridValues === null) return;
        let candidateList = [];
        for (let i = 0; i < 9; ++i) {
            for (let j = 0; j < 9; ++j) {
                if (this.state.problem.content[i][j] === "0") {
                    let candidates = [];
                    for (let k = 1; k < 10; ++k) {
                        if (this.isValid(this.state.gridValues, i, j, String(k))) {
                            candidates.push(String(k));
                        }
                    }
                    candidateList.push({ tartgetIndex: { row: i, col: j }, candidates: candidates });
                }
            }
        }
        if(candidateList.length === 0) return
        this.search(0, candidateList, this.state.gridValues);
    }

    search = (index, candidateList, gridValues) => {
        let new_gridValues = [];
        for (let i = 0; i < gridValues.length; i++)
            new_gridValues[i] = gridValues[i].slice();

        for (const property in candidateList[index].candidates) {
            if (this.isValid(new_gridValues, candidateList[index].tartgetIndex.row, candidateList[index].tartgetIndex.col, candidateList[index].candidates[property])) {
                new_gridValues[candidateList[index].tartgetIndex.row][candidateList[index].tartgetIndex.col] = candidateList[index].candidates[property];
                if (index === candidateList.length - 1) {
                    this.setState({ gridValues: new_gridValues });
                    if (this.checkCompleted(new_gridValues)) {
                        this.setState({ completeFlag: true });
                        setTimeout(() => { this.setState({ completeFlag: false }); }, 2500);
                    };
                    return true;
                } else if (this.search(index + 1, candidateList, new_gridValues)) {
                    return true;
                }
            }
        }
        return false;
    }

    isValid = (gridValues, row_index, col_index, value) => {
        for (let i = 0; i < 9; ++i) {
            if (i !== row_index && gridValues[i][col_index] === value) {
                return false;
            }
            if (i !== col_index && gridValues[row_index][i] === value) {
                return false;
            }
            for (let j = 0; j < 9; ++j) {
                const target_row_index = Math.floor(row_index / 3) * 3 + Math.floor(j / 3);
                const target_col_index = Math.floor(col_index / 3) * 3 + j % 3;
                if (target_row_index !== row_index && target_col_index !== col_index && this.state.gridValues[target_row_index][target_col_index] === value) {
                    return false;
                }
            }
        }
        return true;
    }

    componentDidMount = () => {
        window.addEventListener('keydown', this.handleKeyDownEvent);
    }

    loadProblem = async (name) => {
        this.setState({
            loading: true,
            problem: null,
            gridValues: null,
            selectedGrid: { row_index: -1, col_index: -1 }
        });

        const problem = await require(`../problems/${name}`)
        if (problem.content !== undefined) {
            let gridValues = [];
            for (let i = 0; i < problem.content.length; i++)
                gridValues[i] = problem.content[i].slice();
            this.setState({ problem: problem, gridValues: gridValues, loading: false });
        }
    }

    extractArray(array, col_index, row_index) {
        let rt = []
        for (let i = row_index; i < row_index + 3; i++) {
            for (let j = col_index; j < col_index + 3; j++) {
                rt.push(array[i][j])
            }
        }
        return rt;
    }

    render() {
        const fxProps = {
            count: 3,
            interval: 700,
            canvasWidth: window.innerWidth,
            canvasHeight: window.innerHeight,
            colors: ['#cc3333', '#81C784'],
            calc: (props, i) => ({
                ...props,
                x: (i + 1) * (window.innerWidth / 3) * Math.random(),
                y: window.innerHeight * Math.random()
            })
        }
        return (
            <>
                <Header
                    problemList={problemList}
                    loadProblem={this.loadProblem}
                    handleResetGame={this.handleResetGame}
                    handleAutoComplete={this.handleAutoComplete} />

                {this.state.loading ? (<ReactLoading type={"bars"} color={"#777"} height={"40vh"} width={"40vh"} />) : (
                    <div id="game-board" className="gameBoard" style={{ border: this.state.gameBoardBorderStyle }}>
                        <div className="row">
                            <Grid_9x9 row_offset={0} col_offset={0}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 0, 0)}
                                fixedValue={this.extractArray(this.state.problem.content, 0, 0)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />

                            <Grid_9x9 row_offset={0} col_offset={3}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 3, 0)}
                                fixedValue={this.extractArray(this.state.problem.content, 3, 0)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />

                            <Grid_9x9 row_offset={0} col_offset={6}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 6, 0)}
                                fixedValue={this.extractArray(this.state.problem.content, 6, 0)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />
                        </div>
                        <div className="row">
                            <Grid_9x9 row_offset={3} col_offset={0}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 0, 3)}
                                fixedValue={this.extractArray(this.state.problem.content, 0, 3)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />

                            <Grid_9x9 row_offset={3} col_offset={3}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 3, 3)}
                                fixedValue={this.extractArray(this.state.problem.content, 3, 3)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />

                            <Grid_9x9 row_offset={3} col_offset={6}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 6, 3)}
                                fixedValue={this.extractArray(this.state.problem.content, 6, 3)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />
                        </div>
                        <div className="row">
                            <Grid_9x9 row_offset={6} col_offset={0}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 0, 6)}
                                fixedValue={this.extractArray(this.state.problem.content, 0, 6)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />

                            <Grid_9x9 row_offset={6} col_offset={3}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 3, 6)}
                                fixedValue={this.extractArray(this.state.problem.content, 3, 6)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />

                            <Grid_9x9 row_offset={6} col_offset={6}
                                handle_grid_1x1_click={this.handle_grid_1x1_click}
                                value={this.extractArray(this.state.gridValues, 6, 6)}
                                fixedValue={this.extractArray(this.state.problem.content, 6, 6)}
                                selectedGrid={this.state.selectedGrid}
                                conflicts={this.state.conflicts} />
                        </div>
                    </div>
                )}
                {this.state.completeFlag ? (<Fireworks {...fxProps} />) : null}
                {this.state.loading ? null : (<ScreenInputKeyBoard handleScreenKeyboardInput={this.handleScreenKeyboardInput} />)}
            </>
        );
    }
}

export default Sudoku;