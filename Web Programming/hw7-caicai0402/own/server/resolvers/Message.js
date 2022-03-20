const Message = {
    name(parent, args, { db }, info) {
      return db.messages.filter(message => {
        return message.name === parent.id
      })
    },
    body(parent, args, { db }, info) {
      return db.messages.filter(message => {
        return message.body === parent.id
      })
    }
  }
  
  export { Message as default }
  