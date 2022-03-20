import { GraphQLServer, PubSub } from 'graphql-yoga'
import Query from './resolvers/Query'
import Mutation from './resolvers/Mutation'
import Subscription from './resolvers/Subscription'
import Message from './resolvers/Message'
require('dotenv-defaults').config()

const mongoose = require('mongoose')
const Message_model = require('./models/message')

if (!process.env.MONGO_URL) {
  console.error('Missing MONGO_URL!!!')
  process.exit(1)
}

mongoose.connect(
  process.env.MONGO_URL, {
  useNewUrlParser: true,
  useUnifiedTopology: true
})

const db = mongoose.connection
const pubsub = new PubSub()
const Graphql = new GraphQLServer({
  typeDefs: 'server/schema.graphql',
  resolvers: {
//    Message,
    Query,
    Mutation,
//    Subscription
  },
  context: {
    //db
    pubsub
  }
})

db.on('error', (error) => {
  console.error(error)
})
db.once('open', () => {
  console.log('MongoDB connected!')})


const GRAPHQL_PORT = process.env.port || 4000
Graphql.start(GRAPHQL_PORT, () => {
  console.log(`The grapgql server is up on http://localhost:${GRAPHQL_PORT}!`)
})
  
