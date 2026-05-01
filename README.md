# CHARGER Card

## A compact, interactable business card created for The Univeristy of Alabama in Huntsville Electrical and Computer Engineering Department.

This project was created by Aadam Saafi, Austin Nash, John D'Alessandro, and Keelan Lynch. It utilizes a TFT LCD display, capacative touch buttons, USB interface for flashing/power, and an ESP32-S3-WROOM1.
The code for the project was written in Arduino IDE and the PCB design/layout was created using KiCad. Some of the features of the board in its current state include:

* Three educational activities relating to ECE disciplines included below:
* A Cyber Security activity of brute force byte decryption (similar to mastermind or wordle)
* An Elictical Engineering connect-the-dots activity wheree user connect electrical components to create an electrical connection
* A Computer Engineering activity adjusting inputs and logic gates to get the desired output
* Capacative touch calibration
* Menu selection
* Bitmap rendering

Some additions that could be made using the capabilities of the ESP32-S3:
* Bluetooth utilization
* Wi-Fi utilization
* Artifical Intelligence
* Additional games
* Resume display
* Web browser
* Animated display
* Etc.

## Demo Videos

https://www.youtube.com/watch?v=7o9A-FuoEWo

## How to install this project

1. Clone this project
2. Install Arduino IDE - https://www.arduino.cc/en/software/
3. Install KiCad - https://www.kicad.org/download/

## Opening this project in KiCad (we recommend using V2 of the design)

1. Open Buisness_Card_PCB.kicad_pro

## Opening this project in Ardunio

1. Open ChargerCardSoftware.ino
2. In Board Manager, install esp32 by Espressif Systems
3. In Library Manager, install "Adafruit GFX Library" and "Adafruit ST7735 and ST7789 Library"
4. Ensure that the CHARGER Card is plugged into your computer via USB
5. From the top menu: Tools > Board: > esp32 > ESP32S3 Dev Module
6. Select the port that the board is currently connected to
7. Once set up, you can upload any changes made in code
8. If this is your first time connecting the board: Hold down the boot switch (lower push button), press and release the reset switch (upper push button), release boot switch. This will put the board into boot mode.

# Altering the code

It will be easiest if you edit this code within an installed verion of the Arduino IDE. The ChargerCardSoftware.ino file is where all code for the project currently resides. 
The bitmaps.h file has all the bitmaps used by the primary ino file. 

The setup() and loop() functions are the core of any Arduino code and are located at the very bottom of ChargerCardSoftware.ino. Additional functions and their descriptions are listed below:

* calibrateTouch() - ran in setup, establishes a baseline for the capacative touch inputs
* updateTouchReadings() - ran in loop to get a current reading from the touch inputs
* touchTest(int i) - compares the given touch point i with the established baseline
* displayLogo() - called to display full sized UAH logo
* drawGithub() - displays bitmap of QR code to this github
* drawFaculty(int f) - called to display image and description for the given UAH ECE faculty member f
* drawMenuSelect() - draws/updates the left side of main menu when the cursor is moved
* drawBonusMenuSelect() - draws/updates the left side of bonus menu when the cursor is moved
* drawMainMenu() - draws main menu
* drawBonusMenu() - draws bonus menu
* drawAboutPage() - draws about page
* returnMainMenu() - used to make main menu our current scene
* moveSelectionLeft() and moveSelectionRight() - used by the CS and EE activity to update the cursor left or right
* toggleSelectedBit() - used by CS activity to toggle the bit highlighted by the cursor
* submitGuess() - used by CS activity when the user presses A
* drawUI() - used by CS activity to draw graphical user interface
* drawTopBar() - component of the CS activity gui
* drawCurrentGuess() - component of the CS activity gui
* drawPreviousGuesses() - component of the CS activity gui
* drawHintBar() - component of the CS activity gui
* drawStats() - component of the CS activity gui
* drawKeyboard() - component of the CS activity gui
* uint8_t countCorrectBits(uint8_t guess, uint8_t target) - CS activity function for counting how many bits are correct
* showVictory() - CS activity function to draw victory message
* showDefeat() - CS activity function to draw game over message
* String getBinaryString(uint8_t num) - string conversion in CS activity
* String getHexString(uint8_t num) - string conversion in CS activity
* newCyberGame() - called when CS activity begins or is reset
* selectCurrentNode() - EE activity function called when user presses A
* initLevel(int level) - EE activity function for level initialization
* setupLevelX() - X is a number, EE activity functions used to initialize each level
* handleNodeTouch(int nodeId) - EE activity function that handles logic for touch on current node
* checkCircuitComplete() - EE activity function that checks if circuit is complete
* drawConnection(int node1, int node2, uint16_t color, int offset) - EE activity function to draw wire connections
* drawComponentSymbol(int x, int y, ComponentType type, bool active) - EE activity function for drawing components
* drawCircuit() - top level EE activity function called when drawing the circuit
* drawNodeLabel(int nodeIndex, uint16_t textColor) - EE activity function that draws a label for each component
* drawControls() - EE activity GUI showing controls
* showError(const char* message) - EE activity error message
* showSuccess() - EE activity level win message
* showGameComplete() - congratulations message shown at the end of EE activity
* resetLevel() - EE activity function to reset current level
* checkVal(int i) - CPE activity recursive function used to test logical output from edited diagram
* drawCursor() - CPE activity function used to draw updated cursor position
* drawWire(int x0, int y0, int i) - CPE activity function to draw wire between current node pos (x0, y0) and other node i
* drawLogic(int i) - CPE activity function to draw/update an input component
* drawNot(int i) - CPE activity function to draw NOT gate
* drawAnd(int i) - CPE activity function to draw AND gate
* drawOr(int i) - CPE activity function to draw OR gate
* drawSwitch(int i) - CPE activity function to draw/update a switch
* drawLED(int i) - CPE activity function to draw output component
* drawComponent(int i) - top level CPE activity function for drawing logic components
* drawWin() - CPE activity function to draw level complete message
* drawCPELevel() - CPE activity function to draw current level
* componentInteract(int i) - CPE activity function that handles if user presses A on component
* startCPELevel(int l) - initializes current CPE level l
* updateCPE() - CPE activity function to refresh screen when an edit was made
* startCPE() - called to begin the CPE activity
* cpeInputHandling(int t) - CPE activity function to handle all acceptable touch button inputs
* enterScene(int s) - used in main menu to switch scene and initialize new scene
* enterBonusScene(int s) - used in main menu to enter bonus menu
* update_mm(int d) - update main menu
* update_bm(int d) - update bonus menu
* update_faculty(int f) - update faculty page
* update_about(int a) - update about page

## Adding Custom Bitmaps

Custom bitmaps can be created from any image and drawn very easily using the Adafruit GFX library. The steps for converting and displaying the image of your choice will be listed below:

1. download your desired image (.png or .jpeg would be best)
2. go to https://javl.github.io/image2cpp/
3. select your file
4. alter canvas size (max size is 320x170 for this device)
5. scale to fit or scale to fill
6. adjust brightness/alpha threshold to get your desired image
7. ensure that code output format is set to Arduino Code, single bitmap
8. change the name of the array to something simple
9. copy and paste array into bitmaps.h file
10. tft.drawBitmap([x_pos],[y_pos],[array name],[width],[height],[color])
