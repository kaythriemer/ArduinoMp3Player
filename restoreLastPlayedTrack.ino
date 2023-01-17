void restoreLastPlayedTrack() {

  if (DEBUG) {
    Serial.print("Start restoreLastPlayedTrack ");
  }
  byte EEFolder;
  byte EETrack;

  EEFolder = EEPROM.read(0);
  EETrack = EEPROM.read(1);

  if (EEFolder < 1) {EEFolder = 1;}
  if (EETrack < 1) {EETrack = 1;}

  playTrack = EETrack;
  playFolder = EEFolder;

  itoa(playTrack, lastPlayedTrack, 10);
  itoa(playFolder, lastPlayedFolder, 10);

  if (DEBUG) {
    Serial.print("last played folder(int): ");
    Serial.println(playFolder);
    Serial.print("last played track(int): ");
    Serial.println(playTrack);
    Serial.print("last played folder(char): ");
    Serial.println(lastPlayedFolder);
    Serial.print("last played track(char): ");
    Serial.println(lastPlayedTrack);
  }
}