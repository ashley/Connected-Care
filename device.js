//Photon ID: NWE4
var Particle = require('particle-api-js');
var particle = new Particle();

var token = "d29bc4705d9e065189d1b1c0a32193d9905ac376";
var devicesPr = particle.listDevices({ auth: token });

devicesPr.then(
  function(devices){
    console.log('Devices: ', devices);
  },
  function(err) {
    console.log('List devices call failed: ', err);
  }
);
