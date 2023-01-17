int saveLastPlayedTrack(byte EEFolder, byte EETrack) {

  int addr = 0;  // addr=0 -> Folder , addr=1 -> Track

  if (DEBUG) {
    Serial.println("Save last played Track");
  }

  if (DEBUG) {
    Serial.println(" saving in EEProm:");
    Serial.print("folder: ");
    Serial.print(EEFolder);
    Serial.print(" track: ");
    Serial.println(EETrack);
    Serial.println("");
  }
  addr = 0;
  EEPROM.write(addr, EEFolder);

  addr = 1;
  EEPROM.write(addr, EETrack);
}