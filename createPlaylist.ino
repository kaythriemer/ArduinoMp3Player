//create ordered m3u files for every  directory (1 - 9), File format: mp3, ogg, wav, wma, flac, aac is allowed
void createPlaylist() {
  if (DEBUG) {
    Serial.println("Creating playlists in every directory");
  }

  if (DEBUG) {
    if (!SD.exists("1")
        || !SD.exists("2")
        || !SD.exists("3")
        || !SD.exists("4")
        || !SD.exists("5")
        || !SD.exists("6")
        || !SD.exists("7")
        || !SD.exists("8")
        || !SD.exists("9")) {
      Serial.println("Please create correct folder structure");
      while (1) {};
    }
  }

  // go in every directory from 01 to 09
  for (int i = 1; i < 10; i++) {
    if (DEBUG) {
      Serial.print("creating playlist.m3u in folder ");
      Serial.println(i);
    }


    createSortedPlaylist(i);
  }
}
