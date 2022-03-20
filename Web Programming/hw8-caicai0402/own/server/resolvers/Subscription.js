import Message from '../models/message'

const Subscription = {
    message: {
      async subscribe(parent, args, { pubsub }, info) {
        return pubsub.asyncIterator('message')
      }
    },
    user: {
      async subscribe(parent, args, { pubsub }, info) {
        return pubsub.asyncIterator('user')
      }
    }
  }
  
  export { Subscription as default }
  