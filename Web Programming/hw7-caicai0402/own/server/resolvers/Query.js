import Message from '../models/message'

const Query = {
    async name(parent, args, { db }, info) {
        if (!args.query) {
            return await Message.find()
        }
        return await Message.find({name : {$regex: args.query, $options: 'i'}})
    },
    async body(parent, args, { db }, info) {
        if (!args.query) {
            return await Message.find()
        }
        return await Message.find({body : {$regex: args.query, $options: 'i'}})
    }
}

export { Query as default }