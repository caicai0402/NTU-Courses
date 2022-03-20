import Message from '../models/message'

const Mutation = {
    async createMessage(parent, args, { db }, info) {
        return await Message.create(args)
    },
    async deleteMessage(parent, args, { db }, info) {
        let message = await Message.find({name: {$regex: args.name, $options: 'i'}, body: {$regex: args.body, $options: 'i'}})
        if(message.length === 0){
            console.log("error test")
            throw new Error('Query not found')
        }
        return await Message.findOneAndDelete( {name: {$regex: args.name, $options: 'i'}, body: {$regex: args.body, $options: 'i'}})
    },
    async updateMessage(parent, args, { db }, info) {
        const { name, body, update } = args
        let message = await Message.find({name: {$regex: name, $options: 'i'}, body: {$regex: body, $options: 'i'}})
        if (message.length === 0)
            throw new Error('Query not found')
        message.name = name
        message.body = update
        if (typeof update === 'string') {
            await Message.updateOne({name: {$regex: name, $options: 'i'}, body: {$regex: body, $options: 'i'}},
                {$set : {"body": update}}
            )
            return message
        }
    }
} 
  export { Mutation as default }