/*
  Bad USB v1.1

  Ducky script interpreter for use on arduino boards. This version of a bad USB has an onboard oled
  to display all of the files on an onboard SD card, and a switch to navigate and deploy. The idea behind being
  not all tasks need stealth - so why not have a non-stealth version.

  Bad USB Circuit:

  ** Adafruit Micro-SD Breakout
    CLK    - Pin 15
    D0     - Pin 14
    D1     - Pin 16
    CS     - Pin 10

  ** SSD1306 Display
    SDA    - Pin 2
    SCL    - Pin 3

  ** Momentary Switch
    Right  - Pin 7
    Center - Pin 8
    Left   - Pin 9


  Created September 2021
  Bill Bassett
  www.billbassett.com

*/

#include <Keyboard.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <ACROBOTIC_SSD1306.h>

// Root directory of the SD card
File rootDir;

// The next file in the directory
File nextFile;

void setup() {

  Serial.begin(9600); // Begin communications at 9600 baud
  Wire.begin(); // Start up Wire (for use with ACROBOTIC_SSD1306 library)
  Keyboard.begin(); // Start emulating a keyboard on the computer

  // Initialize the display, change the library font to the smaller size and display the splash message.
  oled.init();
  oled.setFont(font5x7);
  display_text("Bad USB v1.1", "");

  // Initialize the digital pins as an input for the scrolling switch.
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

  // Wait for the chip select to be set on the SD breakout
  if (!SD.begin(10)) {
    display_text("Bad USB v1.1", "Error reading SD card");
    while (1);
  }

  // Open the sd card root directory
  rootDir = SD.open("/");
  delay(50);

  display_text("Bad USB v1.1", "Ready");
}

void loop() {

  // Momentary switch scroll
  if (digitalRead(7) == 0) {

    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);

    // Go to the net file in the root dir, if there isn't a next we are at the end so start over.
    nextFile =  rootDir.openNextFile();

    if (!nextFile) {
      rootDir.rewindDirectory();
    }

    // Show the file name on the oled
    display_text(nextFile.name(), "Ready");

    // Don't leave a file reader open
    nextFile.close();
  }

  // Momentry switch center - This will execute the selected script
  if (digitalRead(8) == 0) {

    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);

    // Indicate what file is executing on the oled
    display_text(nextFile.name(), "Executing");

    // Open the file
    nextFile = SD.open(nextFile.name());

    /*
       If the file was opened then we need to read it in line by line to get the
       command to run followed by the string to execute. Each line should be in the
       form of:
          COMMAND TEXT_TO_TYPE
       ex: GUI r would have a command of press KEY_LEFT_GUI then type the char r
    */
    if (nextFile) {

      String next_line = ""; // Will be building one whole line at a time

      String command = ""; // A variable to hold the command value
      String text = ""; // A variable to hold the text to type

      while (nextFile.available()) {

        char next_char = nextFile.read(); // Read in the next letter

        /*
           If the next char isn't a new line, then we know we need to append it to the line.
           Once we hit the new line we then need to break it up into the command and text to run
        */
        if (next_char != '\n') {
          next_line.concat(next_char); // Add to the next_line variable
        } else {
          // The command should start at the beginning of the line and go to the first whitespace
          command = next_line.substring(0, next_line.indexOf(' '));
          command.trim(); // Remove any whitespaces
          
          // The text to type should start at the first whitespace and go to the end of next_line
          text = next_line.substring(next_line.indexOf(' '));

          /*
             There are three special commands we need to check for before we execute them:
           *  *DELAY - We will set a delay for the value sent in the text variable
           *  *STRING - We will just type the value sent in the text variable
           *  *REM - Don't do anything because this is intended as a comment in Ducky Script
          */
          if (command.equals("DELAY")) {
            delay(text.toInt()); // Set a delay equal to the variable `text` ex. DELAY 100
          }
          else if (command.equals("STRING")) {
            Keyboard.print(text); // Just type the letters
          }
          else if (!command.equals("REM")) {
            press_command(command); // Send the command
            delay(100); // Give it a bit
            if (text.length() > 1) {
              Keyboard.print(text); // Just type the letters
            }

          }

          // Reset the variables for the next line
          command = "";
          next_line = "";
          text = "";
        }

      } // While loop

      // Don't leave a file reader open
      nextFile.close();

      // indicate the process is done
      display_text(nextFile.name(), "Done");

    } // If file

  } // Digital read

  delay(500); // For debounce

} // loop

/*
   Method to display text to multiple lines on the display. Takes in two strings and will display them at the same
   time on lines 1 and 3 (for spacing). Can send "" to not display anything.
*/
void display_text(String line_one, String line_two) {
  oled.clearDisplay();
  oled.setTextXY(1, 0);
  oled.putString(line_one);
  oled.setTextXY(3, 0);
  oled.putString(line_two);
}

/*
   Method to press the specific commands from the command variable. This is needed to map the string value of the
   Ducky Script to the key value of the Keyboard library.
*/
void press_command(String command) {
  if (command.equals("ENTER")) {
    Keyboard.press(KEY_RETURN);
  }
  else if (command.equals("CTRL")) {
    Keyboard.press(KEY_LEFT_CTRL);
  }
  else if (command.equals("SHIFT")) {
    Keyboard.press(KEY_LEFT_SHIFT);
  }
  else if (command.equals("ALT")) {
    Keyboard.press(KEY_LEFT_ALT);
  }
  else if (command.equals("GUI")) {
    Keyboard.press(KEY_LEFT_GUI);
  }
  else if (command.equals("UP") || command.equals("UPARROW")) {
    Keyboard.press(KEY_UP_ARROW);
  }
  else if (command.equals("DOWN") || command.equals("DOWNARROW")) {
    Keyboard.press(KEY_DOWN_ARROW);
  }
  else if (command.equals("LEFT") || command.equals("LEFTARROW")) {
    Keyboard.press(KEY_LEFT_ARROW);
  }
  else if (command.equals("RIGHT") || command.equals("RIGHTARROW")) {
    Keyboard.press(KEY_RIGHT_ARROW);
  }
  else if (command.equals("DELETE")) {
    Keyboard.press(KEY_DELETE);
  }
  else if (command.equals("PAGEUP")) {
    Keyboard.press(KEY_PAGE_UP);
  }
  else if (command.equals("PAGEDOWN")) {
    Keyboard.press(KEY_PAGE_DOWN);
  }
  else if (command.equals("HOME")) {
    Keyboard.press(KEY_HOME);
  }
  else if (command.equals("ESC")) {
    Keyboard.press(KEY_ESC);
  }
  else if (command.equals("INSERT")) {
    Keyboard.press(KEY_INSERT);
  }
  else if (command.equals("TAB")) {
    Keyboard.press(KEY_TAB);
  }
  else if (command.equals("END")) {
    Keyboard.press(KEY_END);
  }
  else if (command.equals("CAPSLOCK")) {
    Keyboard.press(KEY_CAPS_LOCK);
  }
  else if (command.equals("F1")) {
    Keyboard.press(KEY_F1);
  }
  else if (command.equals("F2")) {
    Keyboard.press(KEY_F2);
  }
  else if (command.equals("F3")) {
    Keyboard.press(KEY_F3);
  }
  else if (command.equals("F4")) {
    Keyboard.press(KEY_F4);
  }
  else if (command.equals("F5")) {
    Keyboard.press(KEY_F5);
  }
  else if (command.equals("F6")) {
    Keyboard.press(KEY_F6);
  }
  else if (command.equals("F7")) {
    Keyboard.press(KEY_F7);
  }
  else if (command.equals("F8")) {
    Keyboard.press(KEY_F8);
  }
  else if (command.equals("F9")) {
    Keyboard.press(KEY_F9);
  }
  else if (command.equals("F10")) {
    Keyboard.press(KEY_F10);
  }
  else if (command.equals("F11")) {
    Keyboard.press(KEY_F11);
  }
  else if (command.equals("F12")) {
    Keyboard.press(KEY_F12);
  }
  else if (command.equals("SPACE")) {
    Keyboard.press(' ');
  }
  Keyboard.releaseAll(); // Make sure to let up on the keys
}
