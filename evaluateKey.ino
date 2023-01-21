
void evaluateKey(char pressedKey) {


  if (DEBUG) {
    Serial.println("Evaluate pressed key");
    Serial.print("Pressed key: ");
    Serial.println(pressedKey);
  }

  if (DEBUG==2) {
    Serial.print("Start: last played folder(int): ");
    Serial.println(playFolder);
    Serial.print("last played track(int): ");
    Serial.println(playTrack);
  }

  if (pressedKey == '1' || pressedKey == '2' || pressedKey == '3' || pressedKey == '4' || pressedKey == '5' || pressedKey == '6' || pressedKey == '7' || pressedKey == '8' || pressedKey == '9') {


    playTrack = 1;
    playFolder = pressedKey - '0';

    if (DEBUG==2) {
      Serial.print("Pressed Key 1-9:  next folder(int): ");
      Serial.println(playFolder);
      Serial.print("next track(int): ");
      Serial.println(playTrack);
    }    
    playActualTrack();
  } else if (pressedKey == 'F') {

    // Forward
    if (playTrack < maxTrackCount[playFolder]) {  // wenn bereits beim letzten Lied im Folder wird beim ersten Lied angefangen
      playTrack = playTrack + 1;
    } else {
      playTrack = 1;
    }

    playFolder = playFolder;

    if (DEBUG==2) {
      Serial.print("Pressed Key F:  next folder(int): ");
      Serial.println(playFolder);
      Serial.print("nexttrack(int): ");
      Serial.println(playTrack);
      Serial.print("Maximale Anzahl Lieder in Folder: ");
      Serial.println(maxTrackCount[playFolder]);
    }
    playActualTrack();
  } else if (pressedKey == 'R') {

    // Back
    // erstes mal Reverse
    if (millis() - lastReverseKeypressedTime > restartplayTrackInterval) {
      playTrack = playTrack;
      playFolder = playFolder;
      lastReverseKeypressedTime = millis();
      // Reverse innerhalb einer bestimmten Zeit wieder gedrückt
    } else {
      if (playTrack > 1) {  // wenn bereits beim ersten Lied in Folder wird mit letztem Lied in Folder fortgesetzt
        playTrack = playTrack - 1;
      } else {
        playTrack = maxTrackCount[playFolder];
      }
      playFolder = playFolder;
      lastReverseKeypressedTime = millis();
    }

    if (DEBUG == 2) {
      Serial.print("Pressed Key R:  nextfolder(int): ");
      Serial.println(playFolder);
      Serial.print("nexttrack(int): ");
      Serial.println(playTrack);
      Serial.print("Maximale Anzahl Lieder in Folder: ");
      Serial.println(maxTrackCount[playFolder]);
    }
    playActualTrack();

  } else if (pressedKey == 'P') {

    // Pause

    if (!musicPlayer.paused()) {
      if (DEBUG) {
        Serial.println("Paused");
      }
      musicPlayer.pausePlaying(true);
    } else {
      if (DEBUG) {
        Serial.println("Resumed");
      }
      musicPlayer.pausePlaying(false);
    }
    if (DEBUG == 2) {
      Serial.print("Pressed Key P:  next folder(int): ");
      Serial.println(playFolder);
      Serial.print("next track(int): ");
      Serial.println(playTrack);
      Serial.print("Maximale Anzahl Lieder in Folder: ");
      Serial.println(maxTrackCount[playFolder]);
    }
  } else {

    if (DEBUG) {
      Serial.println("Unknown Key");
      playTrack = 1;
      playFolder = 1;
    }
  }

  if (DEBUG == 2) {
    Serial.print("Ende: next folder(int): ");
    Serial.println(playFolder);
    Serial.print("next track(int): ");
    Serial.println(playTrack);
  }
}
