import React from "react";

export default ({ id }) => {
    return (
        <article>
            <h1>Post #{id}</h1>
            <p>This is the {id}-th post</p>
        </article>
    );
};
