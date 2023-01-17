void resetVariables() {
  if (DEBUG == 2) {
    Serial.println("reset variables");
  }

  keyWasPressed = false;

  // reset Reverse   button
  if (millis() - lastReverseKeypressedTime > resetReverseInterval) {
    lastReverseKeypressedTime = 0;
  }
}
