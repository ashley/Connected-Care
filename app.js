var twilio = require('twilio');
 
// Find your account sid and auth token in your Twilio account Console.
var client = twilio('AC23d7f43213b313e84b9bfcdb54290c9e', '976d70816a1daa0250764020f75fa6a6');
 
function sendText(){
// Send the text message.
client.sendMessage({
  to: '+18082386658',
  from: '+18084270637',
  body: 'DANGER'
});
}
