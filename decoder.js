function Decoder(bytes, port) {
  // Decode an uplink message from a buffer
  // (array) of bytes to an object of fields.
  var decoded = {};

  // Decode bytes to int
  var temperatureInt = (bytes[0] << 8) | bytes[1];
  var hunidityLvlInt = (bytes[2] << 8) | bytes[3]; 
  var windSpeed = (bytes[4] << 8) | bytes[5]; 
  
  // Decode int to float
  decoded.temperature = temperatureInt / 100;
  decoded.humdityLvl= hunidityLvlInt / 100;
  decoded.windSpeed= windSpeed / 100;
  
  return decoded;
}
