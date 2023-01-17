size_t arrayLength = 128;
char fileName[128];
int numberOfLinesInOldPlaylist = 0;
char playListFilePath[128];
String playTrackString;
#define PLAYLIST_FILE_NAME "/playlist.m3u"
#define PLAYLIST_UNSORTED "/playlist_unsorted.m3u"

void createSortedPlaylist(int dirNameInt) {  // Open directory
  if (DEBUG) {
    Serial.println("Create Sorted Playlist");
  }

  char playlistFiles[128];
  char completePath[3] = "/";

  char dirName = dirNameInt + '0';
  sprintf(completePath, "/%c", dirName);
  if (DEBUG) {
    Serial.print("Opening: ");
    Serial.println(completePath);
  }
  int fileCount = 0;
  if (!dir.open(completePath)) {
    if (DEBUG) {
      Serial.print("dir.open failed: ");
      Serial.println(completePath);
      while (1) {};
    }
  }

  strcpy(playlistFiles, completePath);
  strcat(playlistFiles, PLAYLIST_UNSORTED);

  if (DEBUG) {
    Serial.print("Writung into: ");
    Serial.println(playlistFiles);
  }
  SD.remove(playlistFiles);
  // Create a file in Folder using a path.
  if (!playlistfile.open(playlistFiles, O_RDWR | O_CREAT)) {
    if (DEBUG) {
      Serial.println("create playlist.m3u failed");
    }
  }
  fileCount = 0;
  while (file.openNext(&dir, O_RDWR)) {
    //   file.printFileSize(&Serial);
    //   Serial.write(' ');
    //   file.printModifyDateTime(&Serial);
    //   Serial.write(' ');
    char fileName[arrayLength];
    size_t fileNameLength;
    fileNameLength = file.getName(&fileName[0], arrayLength);

    // write supported file types: mp3, ogg, wav, wma, flac, aac (only mp3 and flac tested)
    if (strcmp("mp3", &fileName[fileNameLength - 3]) == 0
        || strcmp("ogg", &fileName[fileNameLength - 3]) == 0
        || strcmp("wav", &fileName[fileNameLength - 3]) == 0
        || strcmp("wma", &fileName[fileNameLength - 3]) == 0
        //      || strcmp("flac", &fileName[fileNameLength - 4]) == 0  flac not working .-(
        || strcmp("aac", &fileName[fileNameLength - 3]) == 0) {
      playlistfile.write(&fileName[0], fileNameLength);
      playlistfile.write("\n", 1);
      fileCount++;
      if (DEBUG == 2) {
        Serial.print("song: ");
        Serial.println(fileName);
      }
      if (file.isDir()) {
        // Indicate a directory.
        Serial.write('/');
      }
    }
    //  Anzahl der Dateien in array schreiben, um später für REWIND, FORWARD Gesamtanzahl zu kennen
    maxTrackCount[dirNameInt] = fileCount;

    file.close();
  }

  if (!playlistfile.close()) {
    if (DEBUG) {
      Serial.println("playlist.close failed");
    }
  }


  // check if existing playlist.m3u has same number of tracks as the unsorted
  // if number is equal, use the old file

  numberOfLinesInOldPlaylist = 0;
  sprintf(playListFilePath, "/%c%s", dirName, PLAYLIST_FILE_NAME);
  if (DEBUG) {
    Serial.print("Reading old playlist from: ");
    Serial.println(playListFilePath);
  }
  playlistfile = SD.open(playListFilePath, O_RDONLY);


  if (playlistfile) {
    if (DEBUG) {
      Serial.print("Opening old playlist from: ");
      Serial.println(playListFilePath);
    }
    // read from the file until playTrack number is reached:
    // if playTrack Number greater than lines i playlist, last line will be used
    while (playlistfile.available()) {
      playTrackString = playlistfile.readStringUntil('\n');
      if (DEBUG) {
        Serial.print("Reading Track from playlist: ");
        Serial.println(playTrackString);
      }
      numberOfLinesInOldPlaylist++;
    }

    if (DEBUG) {
      Serial.print("Number of Lines in playlist: ");
      Serial.println(numberOfLinesInOldPlaylist);
    }
    // close the file:
    if (!playlistfile.close()) {
      if (DEBUG) {
        Serial.println("playlist.close failed in playActualTrack");
      }
    }
  } else {
    // if the file not exist:
    if (DEBUG) {
      Serial.println("old playlist file not exists");
    }
    numberOfLinesInOldPlaylist = 0;
  }


  if (numberOfLinesInOldPlaylist == fileCount) {
    if (DEBUG) {
      Serial.println("use existing playlist.m3u file");
    }

  } else {
    if (DEBUG) {
      Serial.println("sort unsorted file");
    }

    sprintf(playListFilePath, "/%c/%s", dirName, PLAYLIST_UNSORTED);
    if (!playlistfile.open(playListFilePath, O_WRONLY | O_CREAT)) {
      if (DEBUG) {
        Serial.println("open unsorted playlist failed");
      }
    }
    if (DEBUG) {
      sprintf(playListFilePath, "/%c%s", dirName, PLAYLIST_FILE_NAME);
      Serial.print("renaming unsorted playlist to: ");
      Serial.println(playListFilePath);
    }
    playlistfile.rename(playListFilePath);
    playlistfile.close();

    sortLines(completePath);
    if (!dir.close()) {
      if (DEBUG) {
        Serial.println("dir.close failed");
      }
    }
  }
}

File openTempAndListFile(char* dirName) {
  char fileName[128];

  //create sorted entries temp filelistfile
  File tempFile;
  strcpy(fileName, dirName);
  strcat(fileName, "/temp.txt");

  if (!tempFile.open(fileName, O_WRONLY | O_CREAT)) {
    if (DEBUG) {
      Serial.println("create temp.txt failed");
    }
  }
  //open list files
  fileName[0] = '\0';
  strcpy(fileName, dirName);
  strcat(fileName, PLAYLIST_FILE_NAME);

  if (!playlistfile.open(fileName, O_RDWR | O_CREAT)) {
    if (DEBUG) {
      Serial.println("open playlist.m3u failed");
    }
  }
  return tempFile;
}

void deleteplaylistFilesRenameTemp(char* dirName, File tempFile) {
  playlistfile.close();
  char fileName[128];
  strcpy(fileName, dirName);
  strcat(fileName, PLAYLIST_FILE_NAME);
  SD.remove(fileName);
  tempFile.rename(fileName);
  tempFile.close();
}

void sortLines(char* dirName) {

  int lengthEntry1 = 1;
  char biggerLine[arrayLength];
  int lengthEntry2 = 1;
  char newLine[arrayLength];
  bool sorted = false;

  while (!sorted) {
    File tempFile = openTempAndListFile(dirName);
    sorted = true;
    lengthEntry1 = playlistfile.fgets(biggerLine, sizeof(biggerLine));
    while ((lengthEntry2 = playlistfile.fgets(newLine, sizeof(newLine))) > 0) {
      if (strcmp(biggerLine, newLine) > 0) {  // >0 means str1 is greater than str2
        sorted = false;
        tempFile.print(newLine);
      } else {
        tempFile.print(biggerLine);
        strcpy(biggerLine, newLine);
      }
    }
    tempFile.print(biggerLine);
    deleteplaylistFilesRenameTemp(dirName, tempFile);
  }
}