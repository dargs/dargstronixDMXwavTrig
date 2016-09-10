// dargs tronix testing wavTrigger 
// dargs.com.au
// 4/9/2016
// arduino 1.6.5
//
// based on code & lirbary by Jamie Robertson, jamie@robertsonics.com
// 
// wavTrigger Serial protocol . can probably not use the wavTrig libray if i send descrete commands.

// http://robertsonics.com/wav-trigger-online-user-guide/#chapter9


#include <AltSoftSerial.h>    // include AtlSoftSerial library. ATmega328 uses Pin 8 to TX, Pin 8 to RX and Pin 10 becomes unuseable for PWM
#include <wavTrigger.h>       // include wavTrigger library (Apr 26, 2015 commit - due to 2 day comms issues in Sept 2016 version)

wavTrigger wTrig;             //  WAV Trigger object

#include <DMXSerial.h>        // include DMXserial library



void setup() {
   
  wTrig.start();                      // WAV Trigger startup at default 57600

  DMXSerial.init(DMXReceiver);        // initalise DMX
  
  delay(1000);                        // wait 1sec for WAV Trig to power up.
  
  wTrig.stopAllTracks();              // Stop all tracks just in case.  
  wTrig.masterGain(0);                // Reset the master gain to 0dB                 
  

}

void loop() {
  
 // Calculate how long no data backet was received
  unsigned long lastPacket = DMXSerial.noDataSince();
  
  
  if (lastPacket < 5000) { // if DMX received
    for (int x = 0; x < 20; x++){ // checking dmx channels 0 to 20. To Do: add dmx start address in future versions.
    
      if (DMXSerial.read(x) >= 0 && DMXSerial.read(x) <= 10 ) wTrig.trackStop(x);          // Stop Track if DMX Value is between 0-10
      if (DMXSerial.read(x) >=11 && DMXSerial.read(x) <= 20 ) wTrig.trackPause(x);         // Pause Track if DMX Value is between 11-20
      if (DMXSerial.read(x) >=21 && DMXSerial.read(x) <= 30 ) wTrig.trackResume(x);        // Resume Track if DMX Value is between 21-30
      if (DMXSerial.read(x) >=31 && DMXSerial.read(x) <= 40 ) wTrig.trackPlayPoly(x);      // Play Track if DMX value 31-40
      if (DMXSerial.read(x) >=41 && DMXSerial.read(x) <= 255 )                             // Volume
        {
          signed int tempLevel = DMXSerial.read(x);
          signed int Level = map(tempLevel,21,255,-70,10);
          wTrig.trackFade(x, Level, 10, 0); // where (a,b,c,d) = a= track #, b=Level, c= fade time in milliseconds, d=0=contine playing, 1=stop playing
        }
 //
      
      


//      wTrig.trackFade(4, -50, 5000, 1);     // Fade Track 4 to -50dB and stop
// trackPause 
    
    }  
} 
  
}

          
//          Level = map((DMXSerial.read(x),  take the dmx value 21-255 and turn into audio levels for wav trig, -70 to +10dB 
/*
TRACK_FADE
(Note 2)
Message Code = 0x0a, Length = 12
Data = Track Number (2 bytes), Target Volume (2 bytes, signed int, -70dB to +10dB), Milliseconds (2 bytes), StopFlag (1 byte)


TRACK_VOLUME
(Note 1)
Message Code = 0x08, Length = 9
Data = Track Number (2 bytes), Volume (2 bytes, signed int, -70dB to +10dB)
Response = none
Comments: Updates the volume of a track with the specified gain in dB
Example: 0xf0, 0xaa, 0x09, 0x08, 0x01, 0x00, 0x00, 0x00, 0x55
          
*/
