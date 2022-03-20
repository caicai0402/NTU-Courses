import React, { Component } from "react";

import Post from "../../../components/Post/Post";

export default class PostRender extends Component {
    render() {
        const postIDs = ["1", "3", "5", "7", "9"];
        const { id } = this.props.match.params;
        return id && postIDs.includes(id) ? (
            <Post id={id} />
        ) : (
            <div>
                <h3>Error: Post #{id} NOT FOUND</h3>
            </div>
        );
    }
}
