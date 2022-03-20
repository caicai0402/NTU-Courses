const mongoose = require('mongoose')
const Schema = mongoose.Schema

// Creating a schema, sort of like working with an ORM
const UserSchema = new Schema({
	name: {
		type: String,
		required: [true, 'Name field is required.']
	},
	password: {
		type: String,
		required: [true, 'Password field is required.']
    },
    friends: [{
        type: String,
		required: [true, 'Friends field is required.']
    }]
})

// Creating a table within database with the defined schema
const User = mongoose.model('user', UserSchema)

// Exporting table for querying and mutating
module.exports = User