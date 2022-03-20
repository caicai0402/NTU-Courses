
const Subscription = {
    Message: {
      subscribe(parent, { postId }, { db, pubsub }, info) {
          /*
        const post = db.posts.find(post => post.id === postId && post.published)
        if (!post) {
          throw new Error('Name not found')
        }
        return pubsub.asyncIterator(`comment ${postId}`)
        */
       return "test"
      }
    }
  }
  
  export { Subscription as default }
  