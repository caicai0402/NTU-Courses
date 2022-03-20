import Message from '../models/message'
import User from '../models/user'

const Query = {
    async messages(parent, args, { db }, info) {
        if(args.talk_to === ""){
            return await Message.find({$or: [ {name : {$eq: args.name}}, {talk_to: {$eq: args.name}} ] })
        }
        return await Message.find({name : {$in: [args.name, args.talk_to]}, talk_to: {$in: [args.talk_to, args.name]}, body: {$regex: args.body, $options: 'i'}})
    },
    async messages_test(parent, args, { db }, info) {
        return await Message.find({name : {$regex: args.name, $options: 'i'}, talk_to: {$regex: args.talk_to, $options: 'i'}, body: {$regex: args.body, $options: 'i'}})
    },
    async users(parent, args, { db }, info) {
        return await User.find({name : {$eq: args.name}, password : {$regex: args.password, $options: 'i'}})
    },
    async users_with_password(parent, args, { db }, info) {
        return await User.find({name : {$eq: args.name}, password : {$eq: args.password}})
    },
    async users_test(parent, args, { db }, info) {
        return await User.find({name : {$regex: args.name, $options: 'i'}, password : {$regex: args.password, $options: 'i'}})
    },
}

export { Query as default }