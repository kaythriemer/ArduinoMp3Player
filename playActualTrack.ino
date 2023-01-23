void playActualTrack() {
  int i = 1;
  char playListFilePath[256];
  char actualMusicFileFullPath[256];
  String playTrackString;


  if (DEBUG) {
    Serial.println("PlayActualTrack");
  }

  sprintf(playListFilePath, "/%i/%s", playFolder, "playlist.m3u");
  if (DEBUG) {
    Serial.print("Reading playlist from: ");
    Serial.println(playListFilePath);
  }
  playlistfile = SD.open(playListFilePath, O_RDONLY);

while (!playlistfile) {
    playlistfile = SD.open(playListFilePath, O_RDONLY);
    
    // if the file didn't open, print an error:
    Serial.println("############### ######################################################");
    Serial.println("############### error opening Playlist File, will retry ##############");
    Serial.println("############### ######################################################");  
}


  if (playlistfile) {
    // read from the file until playTrack number is reached:
    // if playTrack Number greater than lines i playlist, last line will be used
    while (playlistfile.available() and i <= playTrack) {

      playTrackString = playlistfile.readStringUntil('\n');
      if (DEBUG) {
        Serial.print("Reading Track from playlist: ");
        Serial.println(playTrackString);
      }
      i++;
    }
    if (DEBUG) {
      Serial.print("Playing Track from playlist: ");
      Serial.println(playTrackString);
    }
    // close the file:
    if (!playlistfile.close()) {
      if (DEBUG) {
        Serial.println("playlist.close failed in playActualTrack");
      }
    }
  } 

  // build filename
  sprintf(actualMusicFileFullPath, "/%i/%s", playFolder, playTrackString.c_str());
  if (DEBUG) {
    Serial.print("Created full Filepath: ");
    Serial.println(actualMusicFileFullPath);
  }

  while (!SD.exists(actualMusicFileFullPath)) {
    if (DEBUG) {
      Serial.println("################  Retry find Music file #############");
      
    }
  }

  

  musicPlayer.startPlayingFile(actualMusicFileFullPath);

  // write playFolder and playTrack into lastplayedtrackfile
  saveLastPlayedTrack(playFolder, playTrack);
  delay (1000);
}
