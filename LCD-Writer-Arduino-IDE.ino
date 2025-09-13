#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MCreatorLink.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(20);

  // Initialize MCreatorLink connection
  MCreatorLink.setup(Serial);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Waiting for msg...");
}

void loop() {
  // Keep MCreator Link alive
  MCreatorLink.loop();

  // Check if there's raw data waiting
  if (Serial.available()) {
    String msg = Serial.readStringUntil('\n');
    msg.trim();

    // Filter out handshake/control messages
    if (msg == "msg?" || msg == "ident?" || msg.length() == 0) {
      return;
    }

    // Split into up to 4 parts
    String parts[4];
    int idx = 0;
    int start = 0;
    for (int i = 0; i < msg.length() && idx < 4; i++) {
      if (msg.charAt(i) == '|') {
        parts[idx++] = msg.substring(start, i);
        start = i + 1;
      }
    }
    if (idx < 4) {
      parts[idx++] = msg.substring(start);
    }

    // Display parts on LCD
    lcd.clear();
    for (int row = 0; row < 4; row++) {
      lcd.setCursor(0, row);
      if (row < idx) {
        lcd.print(parts[row]);
      }
    }
  }
}
