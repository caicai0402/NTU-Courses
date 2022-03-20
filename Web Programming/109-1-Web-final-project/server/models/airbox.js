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

/*
{
	"$schema":"http://json-schema.org/draft-04/schema#",
	"title":"Product",
	"type":"object",
	"properties":{
		"source":{
			"type":"string"},
		"c_d0_source":{"type":"string"},
		"num_of_records":{"type":"number"},
		"feeds":{
			"type":"array",
			"items":{
				"type":"object",
				"properties":{
					"time":{"type":"string"},
					"SiteAddr":{"type":"string"},
					"SiteName":{"type":"string"},
					"app":{"type":"string"},
					"area":{"type":"string"},
					"date":{"type":"string"},
					"gps_alt":{"type":"number"},
					"gps_fix":{"type":"number"},
					"gps_lat":{"type":"number"},
					"gps_lon":{"type":"number"},
					"gps_num":{"type":"number"},
					"name":{"type":"string"},
					"s_d0":{"type":"number"},
					"s_d1":{"type":"number"},
					"s_d2":{"type":"number"},
					"s_h0":{"type":"number"},
					"s_t0":{"type":"number"},
					"timestamp":{"type":"string"},
					"device_id":{"type":"string"},
					"c_d0":{"type":["number","string"]},
					"c_d0_method":{"type":"string"},
					"fw_ver":{"type":"string"},
					"hcho":{"type":"number"},
					"model":{"type":"string"}
				},
				"required":["time","SiteAddr","SiteName","app","area","date","gps_alt","gps_fix","gps_lat","gps_lon","gps_num","name","s_d0","s_d1","s_d2","s_h0","s_t0","timestamp","device_id","c_d0","c_d0_method","fw_ver","hcho","model"]
			}
		},
		"descriptions":{
			"type":"object",
			"properties":{
				"URL":{"type":"string"},
				"c_d0_method":{"type":"string"},
				"c_d0":{"type":"string"}
			}
		},
		"version":{"type":"string"}
	}
}
*/

/*
source : {type : String},
		c_d0_source : {type : String},
		num_of_records : {type : Number},
		feeds : {
				type : Array,
				"items":{
					"type":"object",
					"properties":{
						"time":{"type":"string"},
						"SiteAddr":{"type":"string"},
						"SiteName":{"type":"string"},
						"app":{"type":"string"},
						"area":{"type":"string"},
						"date":{"type":"string"},
						"gps_alt":{"type":"number"},
						"gps_fix":{"type":"number"},
						"gps_lat":{"type":"number"},
						"gps_lon":{"type":"number"},
						"gps_num":{"type":"number"},
						"name":{"type":"string"},
						"s_d0":{"type":"number"},
						"s_d1":{"type":"number"},
						"s_d2":{"type":"number"},
						"s_h0":{"type":"number"},
						"s_t0":{"type":"number"},
						"timestamp":{"type":"string"},
						"device_id":{"type":"string"},
						"c_d0":{"type":["number","string"]},
						"c_d0_method":{"type":"string"},
						"fw_ver":{"type":"string"},
						"hcho":{"type":"number"},
						"model":{"type":"string"}
					},
				"required":["time","SiteAddr","SiteName","app","area","date","gps_alt","gps_fix","gps_lat","gps_lon","gps_num","name","s_d0","s_d1","s_d2","s_h0","s_t0","timestamp","device_id","c_d0","c_d0_method","fw_ver","hcho","model"]
				}
			},
			"descriptions":{
				"type":"object",
				"properties":{
					"URL":{"type":"string"},
					"c_d0_method":{"type":"string"},
					"c_d0":{"type":"string"}
				}
			},
			"version":{"type":"string"}
		}
		*/