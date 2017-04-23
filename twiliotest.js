var config = require('./node_modules/config');
var client = require('twilio')("AP026a2b46ec61346b7f44634d6a9548e7", "976d70816a1daa0250764020f75fa6a6");

module.exports.sendSms = function(to, message) {
  client.messages.create({
    body: "hi",
    to: "808-238-6658",
    from: "(808) 427-0637"
//  mediaUrl: imageUrl
  }, function(err, data) {
    if (err) {
      console.error('Could not notify administrator');
      console.error(err);
    } else {
      console.log('Administrator notified');
    }
  });
};