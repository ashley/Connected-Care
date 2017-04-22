var Particle = require('particle-api-js');
var particle = new Particle();
var token;

particle.login({username: 'yuann.chen@simmons.edu', password: 'password123'}).then(
  function(data) {
    token = data.body.access_token;
    console.log(token);
  },
  function (err) {
    console.log('Could not log in.', err);
  }
);
