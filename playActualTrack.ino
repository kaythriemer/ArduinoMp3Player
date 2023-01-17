void playActualTrack() {
  int i = 1;
  char playListFilePath[128];
  char actualMusicFileFullPath[128];
  String playTrackString;


  if (DEBUG) {
    Serial.println("PlayActualTrack");
  }

  // ermittle Filename aus playlist.m3u für playFolder und playTrack und starte damit Player
  // open the playlistfile for reading:
  if (DEBUG) {
    Serial.print("PlayActualTrack from Folder: ");
    Serial.println(lastPlayedFolder[0]);
  }
  sprintf(playListFilePath, "/%c/%s", lastPlayedFolder[0], "playlist.m3u");
  if (DEBUG) {
    Serial.print("Reading playlist from: ");
    Serial.println(playListFilePath);
  }
  playlistfile = SD.open(playListFilePath, O_RDONLY);


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
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening Playlist File, will retry");
    playActualTrack();
  }


  // build filename
  sprintf(actualMusicFileFullPath, "/%c/%s", lastPlayedFolder[0], playTrackString.c_str());
  if (DEBUG) {
    Serial.print("Created full Filepath: ");
    Serial.println(actualMusicFileFullPath);
  }

  if (!SD.exists(actualMusicFileFullPath)) {
    if (DEBUG) {
      Serial.println("Retry to read Playlistfile ");
      playActualTrack();
    }
  }

  musicPlayer.stopPlaying();

  musicPlayer.startPlayingFile(actualMusicFileFullPath);

  // write playFolder and playTrack into lastplayedtrackfile
  saveLastPlayedTrack(playFolder, playTrack);
}
