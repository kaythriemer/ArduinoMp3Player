#define DEBUG 1

// include  libraries
#include <EEPROM.h>
// needs the adopted library VS1053_SDFat
#include <SPI.h>
#include <VS1053_SdFat.h>
#include "SdFat.h"
#include "Wire.h"
#include "I2CKeyPad.h"

SdFat SD;
File playlistfile;
File dir;
File file;


int maxTrackCount[10];  // max Anzahl der Lieder pro Folder 1 bis 9

// These are the pins used for the music maker shield
#define SHIELD_RESET -1  // VS1053 reset pin (unused!)
#define SHIELD_CS 7      // VS1053 chip select pin (output)
#define SHIELD_DCS 6     // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4  // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3  // VS1053 Data request, ideally an Interrupt pin


VS1053_SdFat_FilePlayer musicPlayer =
  VS1053_SdFat_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// settings for Volume Poti
int volumePotiPin = A2;  // select the input pin for the potentiometer
int minimalVolume = 55;
int maximalVolume = 0;

// keypad
const uint8_t KEYPAD_ADDRESS = 0x38;
I2CKeyPad keyPad(KEYPAD_ADDRESS);
char key;
char keymap[19] = "369F258P147R    NF";  // N = NoKey
unsigned long lastKeypressedTime;
const long debounce = 300;
bool keyWasPressed = false;

unsigned long lastReverseKeypressedTime = 0;
const long restartplayTrackInterval = 3000;
const long resetReverseInterval = 10000;

unsigned long lastMusicPlayerStopTime = 0;
const long pauseBetween2Tracks = 1000;

//
int playTrack;
int playFolder;

void setup() {
  Serial.begin(115200);

  if (DEBUG) {
    Serial.println("Setup for mp3 player V2");
  }

  // initialise SD card and music maker
  initialiseHardware();

  if (DEBUG == 2) {
    SD.ls("/", LS_R);
  }

  // create Playlist for every Directory
  createPlaylist();

  //set actual track to previously stored value (ROM or SDCard ?)
  restoreLastPlayedTrack();
  resetVariables();
  setVolume();
  // play last track
  playActualTrack();
}

void loop() {


  resetVariables();
  setVolume();
  key = getPressedKey();

  // aus key neuen Track ermitteln und playTrack und playFolder setzen, ruft intern playActualTrack
  if (keyWasPressed) {
    evaluateKey(key);
  }
  // wenn aktuelles lied zu Ende nach pauseBetween2Tracks -> nächstes starten mit Track +1 mit F-Logik

  if (musicPlayer.stopped()) {
    if (lastMusicPlayerStopTime == 0) {
      lastMusicPlayerStopTime = millis();
    } else if (millis() - lastMusicPlayerStopTime > pauseBetween2Tracks) {
      lastMusicPlayerStopTime = 0;
      if (DEBUG) {
        Serial.println("############# Play next track automatically.  #################");
      }
      evaluateKey('F');
    }
  } else {
    lastMusicPlayerStopTime = 0;
  }
}