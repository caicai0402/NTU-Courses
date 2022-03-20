import Message from '../models/message'
import User from '../models/user'

const Mutation = {
    async createMessage(parent, args, { db, pubsub }, info) {
        pubsub.publish('message', {
            message: {
            mutation: 'CREATED',
            data: args
            }
        })
        return await Message.create(args)
    },
    async deleteMessage(parent, args, { db, pubsub }, info) {
        let message = await Message.find({name : {$in: [args.name, args.talk_to]}, talk_to: {$in: [args.talk_to, args.name]}, body: {$regex: args.body, $options: 'i'}})
        if(message.length === 0)
            throw new Error('Message delete query not found')
        pubsub.publish('message', {
            message: {
            mutation: 'DELETED',
            data: args
            }
        })
        return await Message.findOneAndDelete({name : {$in: [args.name, args.talk_to]}, talk_to: {$in: [args.talk_to, args.name]}, body: {$regex: args.body, $options: 'i'}})
    },
    async updateMessage(parent, args, { db }, info) {
        const { name, talk_to, body, update } = args
        let message = await Message.findOne({name: {$regex: name, $options: 'i'}, talk_to: {$regex: talk_to, $options: 'i'}, body: {$regex: body, $options: 'i'}})
        if (message === null)
            throw new Error('Message update query not found')
        message.body = update
        if (typeof update === 'string') {
            await Message.updateOne({name: {$regex: name, $options: 'i'}, talk_to: {$regex: talk_to, $options: 'i'}, body: {$regex: body, $options: 'i'}},
                {$set : {"body": update}}
            )
            return message
        }
    },
    async createUser(parent, args, { db, pubsub }, info) {
        pubsub.publish('user', {
            user: {
            mutation: 'CREATED',
            data: args
            }
        })
        return await User.create(args)
    },
    async deleteUser(parent, args, { db, pubsub }, info) {

        let user = await User.find({name: {$regex: args.name, $options: 'i'}, password: {$regex: args.password, $options: 'i'}})
        if(user.length === 0)
            throw new Error('User delete query not found')
        pubsub.publish('user', {
            user: {
            mutation: 'DELETED',
            data: args
            }
        })
        return await User.findOneAndDelete( {name: {$regex: args.name, $options: 'i'}, password: {$regex: args.password, $options: 'i'}})
    },
    async updateUser(parent, args, { db }, info) {
        const { name, password, friends, update } = args
        let user = await User.findOne({name: {$eq: name}, password: {$eq: password}})
        if (user === null)
            throw new Error('User update query not found')
        user.friends = friends.concat(update)
        if (typeof update === 'string') {
            await User.updateOne({name: {$eq: name}, password: {$eq: password}},
                {$set : {"friends": friends.concat(update)}}
            )
            return user
        }
    }
} 
  export { Mutation as default }