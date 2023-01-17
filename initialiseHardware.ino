void initialiseHardware() {

  // Volume Poti
  pinMode(volumePotiPin, INPUT);

  // Keypad
  Wire.begin();
  Wire.setClock(400000);
  if (keyPad.begin() == false) {
    if (DEBUG) {
      Serial.println("\nERROR: cannot communicate to keypad.\nPlease reboot.\n");
    }
    while (1) {};
  }
  keyPad.loadKeyMap(keymap);

  //SD Card
  if (!SD.begin(CARDCS)) {
    if (DEBUG) {
      Serial.println(F("\nSD failed, or not present\n"));
    }
    while (1) {};
  }
  if (DEBUG) {
    Serial.println(F("SD Card initialized"));
  }

  // Music Maker
  if (!musicPlayer.begin()) {  // initialise the music player
    if (DEBUG) {
      Serial.println("\nCouldn't find VS1053, do you have the right pins defined?\n");
    }
    while (1) {};
  }
  if (DEBUG) {
    Serial.println(F("VS1053 found"));
  }

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int

  // Set volume for left, right channels. actually muted
  musicPlayer.setVolume(254, 254);
}