void setVolume() {
  int lastvolume;
  int volume;
  // set volume while playing

  volume = getVolumeValue();
  if (volume != lastvolume) {
    musicPlayer.setVolume(volume, volume);
    lastvolume = volume;
  }
}



// FUNKTION get value of the volume poti
int getVolumeValue() {
  // read PotiData
  int volPotiValue = analogRead(volumePotiPin);  // 0-1023
  int vol;
  if (DEBUG == 2) {
    Serial.print("poti Value: ");
    Serial.println(volPotiValue);
  }

  // Volume Mapping
  vol = map(volPotiValue, 0, 1023, maximalVolume, minimalVolume);
  vol = constrain(vol, maximalVolume, minimalVolume);

  if (DEBUG == 2) {
    Serial.print("Volume Value:");
    Serial.println(vol);
  }
  return vol;
}