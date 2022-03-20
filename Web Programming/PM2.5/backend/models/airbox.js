import mongoose from 'mongoose';
const Schema = mongoose.Schema;

const AirboxSchema = new Schema({
	Data: {
		type: Object,
		required: [true, 'Data field is required.']	
	},
	Date: {
		type: String,
		required: [true, 'Time field is required.']
	}
});

const Airbox = mongoose.model('airbox', AirboxSchema);

export default Airbox;