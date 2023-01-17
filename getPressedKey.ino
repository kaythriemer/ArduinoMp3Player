char getPressedKey() {
  if (DEBUG == 2) {
    Serial.println("Get pressed key");
  }

  int lastkey;
  char lastch;
  int key;
  char ch;

  if (millis() - lastKeypressedTime > debounce && keyPad.isPressed()) {
    keyWasPressed = true;

    lastkey = keyPad.getLastKey();
    lastch = keyPad.getLastChar();
    key = keyPad.getKey();
    ch = keyPad.getChar();

    lastKeypressedTime = millis();

    if (DEBUG==2) {
      Serial.print("lastkey: ");
      Serial.print(lastkey);
      Serial.print(" \t");
      Serial.print("key: ");
      Serial.print(key);
      Serial.print(" \t");
      Serial.print("lastch: ");
      Serial.print(lastch);
      Serial.print(" \t");
      Serial.print("ch: ");
      Serial.println(ch);
      Serial.println("");
    }
    return (ch);
  }
}