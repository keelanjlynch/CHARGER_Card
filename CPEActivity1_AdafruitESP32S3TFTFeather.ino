#include "arduino_secrets.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Built-in TFT pins are provided by the Adafruit ESP32-S3 TFT Feather board definition.
// (TFT_CS, TFT_DC, TFT_RST, TFT_BACKLITE, TFT_I2C_POWER)

// Touch button pins (capacitive touch): use touch-capable GPIOs (1-14 on ESP32-S3).
#define TOUCH1 5   // D5
#define TOUCH2 6   // D6
#define TOUCH3 9   // D9
#define TOUCH4 10  // D10
#define TOUCH5 11  // D11
#define TOUCH6 12  // D12

// Display dimensions for Adafruit ESP32-S3 TFT Feather (ST7789 240x135)
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 135

// The original layout was designed for a 320x170 (1.9") display.
#define BASE_WIDTH  320
#define BASE_HEIGHT 170

// Coordinate scaling helpers (map old 320x170 layout onto 240x135 display)
static inline int SX(int x) { return (x * SCREEN_WIDTH) / BASE_WIDTH; }
static inline int SY(int y) { return (y * SCREEN_HEIGHT) / BASE_HEIGHT; }
static inline int SW(int w) { int v = (w * SCREEN_WIDTH) / BASE_WIDTH;  return v < 1 ? 1 : v; }
static inline int SH(int h) { int v = (h * SCREEN_HEIGHT) / BASE_HEIGHT; return v < 1 ? 1 : v; }
static inline int SR(int r) { int v = (r * ((SCREEN_WIDTH < SCREEN_HEIGHT) ? SCREEN_WIDTH : SCREEN_HEIGHT)) / ((BASE_WIDTH < BASE_HEIGHT) ? BASE_WIDTH : BASE_HEIGHT); return v < 1 ? 1 : v; }



// Game settings
#define MAX_NODES 6
#define NODE_RADIUS 6
#define TOUCH_THRESHOLD 40

// Custom color
#define COLOR_DARKGREY 0x4208

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Logic gate types
enum GateType {
  NONE,
  AND_GATE,
  OR_GATE,
  NOT_GATE,
  NAND_GATE,
  NOR_GATE,
  XOR_GATE,
  XNOR_GATE
};

// Node structure
struct Node {
  int x, y;
  int id;
  bool isInput;      // Input signal (replaces isStart)
  bool isOutput;     // Output signal (replaces isEnd)
  bool isGate;       // Is this a logic gate
  GateType gateType;
  bool visited;
  bool signalValue;  // HIGH (1) or LOW (0)
  int connectedTo[3];
  int connectionCount;
};

Node nodes[MAX_NODES];
int touchPins[MAX_NODES] = {TOUCH1, TOUCH2, TOUCH3, TOUCH4, TOUCH5, TOUCH6};
int currentNode = -1;
int pathSequence[MAX_NODES];
int pathLength = 0;
bool gameComplete = false;
int currentLevel = 0;
int totalLevels = 6;

// Level configurations
struct Level {
  const char* title;
  const char* objective;
  GateType primaryGate;
  int nodeCount;
};

Level levels[6] = {
  {"Level 1: AND Gate", "Both inputs HIGH = HIGH", AND_GATE, 5},
  {"Level 2: OR Gate", "Any input HIGH = HIGH", OR_GATE, 4},
  {"Level 3: NOT Gate", "Inverts input signal", NOT_GATE, 3},
  {"Level 4: XOR Gate", "Different inputs = HIGH", XOR_GATE, 5},
  {"Level 5: NAND Gate", "NOT both HIGH = HIGH", NAND_GATE, 5},
  {"Level 6: NOR Gate", "All inputs LOW = HIGH", NOR_GATE, 5}
};

// Forward declarations
void drawConnection(int node1, int node2, uint16_t color, int offset = 0);
void drawGateSymbol(int x, int y, GateType type, bool active);
void drawCircuit();
void showError(const char* message);
void showSuccess();
void resetLevel();

void setup() {
  Serial.begin(115200);

  // Turn on TFT backlight + TFT/I2C power (board-specific pins are provided by the variant)
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // Initialize display (Adafruit ESP32-S3 TFT Feather: ST7789 240x135)
  tft.init(SCREEN_HEIGHT, SCREEN_WIDTH); // init(height, width)
  tft.setRotation(3);                    // landscape
  tft.fillScreen(ST77XX_BLACK);

  Serial.println("Logic Gate Learning Game Started");

  // Initialize game
  initLevel(currentLevel);
  drawCircuit();
}


void loop() {
  if (gameComplete) {
    // Check for any button press to continue
    int activeNodes = levels[currentLevel].nodeCount;
    for (int i = 0; i < activeNodes; i++) {
      if (touchRead(touchPins[i]) < TOUCH_THRESHOLD) {
        delay(300);
        currentLevel++;
        if (currentLevel >= totalLevels) {
          showGameComplete();
          delay(3000);
          currentLevel = 0;  // Reset to level 1 after showing complete screen
        }
        initLevel(currentLevel);
        drawCircuit();
        break;
      }
    }
    return;
  }
  
  // Check touch buttons for active nodes
  int activeNodes = levels[currentLevel].nodeCount;
  for (int i = 0; i < activeNodes; i++) {
    if (touchRead(touchPins[i]) < TOUCH_THRESHOLD) {
      handleNodeTouch(i);
      delay(250);
    }
  }
}

void initLevel(int level) {
  gameComplete = false;
  currentNode = -1;
  pathLength = 0;
  
  // Clear all nodes
  for (int i = 0; i < MAX_NODES; i++) {
    nodes[i].visited = false;
    nodes[i].connectionCount = 0;
    nodes[i].isInput = false;
    nodes[i].isOutput = false;
    nodes[i].isGate = false;
    nodes[i].gateType = NONE;
    nodes[i].signalValue = false;
    pathSequence[i] = -1;
  }
  
  // Configure nodes based on level
  switch(level) {
    case 0: setupLevel1(); break;
    case 1: setupLevel2(); break;
    case 2: setupLevel3(); break;
    case 3: setupLevel4(); break;
    case 4: setupLevel5(); break;
    case 5: setupLevel6(); break;
  }
}

void setupLevel1() {
  // AND gate: Must visit BOTH inputs before reaching the gate
  // Path: Input A -> Connection -> Input B -> AND gate -> Output
  nodes[0] = {50, 60, 0, true, false, false, NONE, false, true, {1}, 1};     // Input A (HIGH)
  nodes[1] = {120, 85, 1, false, false, false, NONE, false, false, {0, 2}, 2}; // Connection point
  nodes[2] = {50, 110, 2, true, false, false, NONE, false, true, {1, 3}, 2};  // Input B (HIGH)
  nodes[3] = {200, 85, 3, false, false, true, AND_GATE, false, false, {1, 2, 4}, 3};  // AND gate
  nodes[4] = {280, 85, 4, false, true, false, NONE, false, false, {3}, 1};   // Output
}

void setupLevel2() {
  // OR gate: Two inputs (one HIGH) -> OR -> Output
  nodes[0] = {50, 60, 0, true, false, false, NONE, false, true, {2}, 1};     // Input A (HIGH)
  nodes[1] = {50, 110, 1, true, false, false, NONE, false, false, {2}, 1};   // Input B (LOW)
  nodes[2] = {160, 85, 2, false, false, true, OR_GATE, false, false, {0, 1, 3}, 3};   // OR gate
  nodes[3] = {270, 85, 3, false, true, false, NONE, false, false, {2}, 1};   // Output
}

void setupLevel3() {
  // NOT gate: Input (HIGH) -> NOT -> Output (LOW)
  nodes[0] = {70, 85, 0, true, false, false, NONE, false, true, {1}, 1};     // Input (HIGH)
  nodes[1] = {170, 85, 1, false, false, true, NOT_GATE, false, false, {0, 2}, 2};  // NOT gate
  nodes[2] = {270, 85, 2, false, true, false, NONE, false, false, {1}, 1};   // Output
}

void setupLevel4() {
  // XOR gate: Must collect both different inputs
  // Path: Input A (HIGH) -> Connection -> Input B (LOW) -> XOR -> Output
  nodes[0] = {50, 60, 0, true, false, false, NONE, false, true, {1}, 1};     // Input A (HIGH)
  nodes[1] = {120, 85, 1, false, false, false, NONE, false, false, {0, 2, 3}, 3}; // Connection
  nodes[2] = {50, 110, 2, true, false, false, NONE, false, false, {1, 3}, 2}; // Input B (LOW)
  nodes[3] = {200, 85, 3, false, false, true, XOR_GATE, false, false, {1, 2, 4}, 3};  // XOR gate
  nodes[4] = {280, 85, 4, false, true, false, NONE, false, false, {3}, 1};   // Output
}

void setupLevel5() {
  // NAND gate: (1,0) -> NAND -> 1 (Output HIGH because NOT both inputs are HIGH)
  nodes[0] = {40, 85, 0, true, false, false, NONE, false, true, {2}, 1};     // Input A (HIGH)
  nodes[1] = {110, 60, 1, true, false, false, NONE, false, false, {2}, 2};   // Input B (LOW)
  nodes[2].x = 180; nodes[2].y = 85; nodes[2].id = 2;
  nodes[2].isInput = false; nodes[2].isOutput = false; nodes[2].isGate = true;
  nodes[2].gateType = NAND_GATE; nodes[2].visited = false; nodes[2].signalValue = false;
  nodes[2].connectedTo[0] = 0; nodes[2].connectedTo[1] = 1; 
  nodes[2].connectedTo[2] = 3; nodes[2].connectionCount = 3;
  nodes[3] = {240, 110, 3, false, false, false, NONE, false, false, {2, 4}, 2};  // Connection
  nodes[4] = {280, 85, 4, false, true, false, NONE, false, false, {2, 3}, 2}; // Output
}

void setupLevel6() {
  // NOR gate: Both inputs LOW -> NOR -> HIGH output
  // NOR is TRUE only when ALL inputs are LOW (0)
  nodes[0] = {50, 60, 0, true, false, false, NONE, false, false, {1}, 1};    // Input A (LOW=0)
  nodes[1] = {120, 85, 1, false, false, false, NONE, false, false, {0, 2, 3}, 3}; // Connection
  nodes[2] = {50, 110, 2, true, false, false, NONE, false, false, {1, 3}, 2}; // Input B (LOW=0)  
  nodes[3] = {200, 85, 3, false, false, true, NOR_GATE, false, false, {1, 2, 4}, 3};  // NOR gate
  nodes[4] = {280, 85, 4, false, true, false, NONE, false, false, {3}, 1};   // Output
}

void handleNodeTouch(int nodeId) {
  // First touch - must be input node
  if (currentNode == -1) {
    if (nodes[nodeId].isInput) {
      currentNode = nodeId;
      nodes[nodeId].visited = true;
      pathSequence[pathLength++] = nodeId;
      drawCircuit();
      Serial.print("Started at input node ");
      Serial.println(nodeId);
    } else {
      showError("Must start at INPUT!");
    }
    return;
  }
  
  // Check if touched node is connected to current node
  bool isConnected = false;
  for (int i = 0; i < nodes[currentNode].connectionCount; i++) {
    if (nodes[currentNode].connectedTo[i] == nodeId) {
      isConnected = true;
      break;
    }
  }
  
  if (!isConnected) {
    showError("Not connected!");
    return;
  }
  
  // Check if already visited
  if (nodes[nodeId].visited) {
    showError("Already visited!");
    return;
  }
  
  // Valid move
  nodes[nodeId].visited = true;
  pathSequence[pathLength++] = nodeId;
  
  // Draw connection
  drawConnection(currentNode, nodeId, ST77XX_GREEN);
  
  currentNode = nodeId;
  drawCircuit();
  
  // Check if reached output
  if (nodes[nodeId].isOutput) {
    if (checkCircuitComplete()) {
      gameComplete = true;
      showSuccess();
    } else {
      showError("Wrong logic output!");
      delay(1500);
      resetLevel();
    }
  }
  
  Serial.print("Moved to node ");
  Serial.println(nodeId);
}

bool checkCircuitComplete() {
  // Must reach output
  if (!nodes[currentNode].isOutput) return false;
  
  // Check if path includes at least one gate
  bool hasGate = false;
  for (int i = 0; i < pathLength; i++) {
    if (nodes[pathSequence[i]].isGate) {
      hasGate = true;
      break;
    }
  }
  if (!hasGate) return false;
  
  // Evaluate logic: Check if the correct inputs were used
  return evaluateLogic();
}

bool evaluateLogic() {
  // Count how many HIGH inputs are in the path
  int highInputCount = 0;
  int totalInputCount = 0;
  
  for (int i = 0; i < pathLength; i++) {
    if (nodes[pathSequence[i]].isInput) {
      totalInputCount++;
      if (nodes[pathSequence[i]].signalValue) {
        highInputCount++;
      }
    }
  }
  
  // Find which gate was used
  GateType usedGate = NONE;
  for (int i = 0; i < pathLength; i++) {
    if (nodes[pathSequence[i]].isGate) {
      usedGate = nodes[pathSequence[i]].gateType;
      break;
    }
  }
  
  // Evaluate based on gate type
  switch(usedGate) {
    case AND_GATE:
      // AND requires ALL inputs to be HIGH
      return (totalInputCount >= 2 && highInputCount == totalInputCount);
      
    case OR_GATE:
      // OR requires AT LEAST ONE input to be HIGH
      return (highInputCount >= 1);
      
    case NOT_GATE:
      // NOT inverts the input (we're checking the path, so any input works)
      return (totalInputCount == 1);
      
    case XOR_GATE:
      // XOR requires EXACTLY ONE input to be HIGH (for 2 inputs)
      return (totalInputCount == 2 && highInputCount == 1);
      
    case NAND_GATE:
      // NAND is opposite of AND - FALSE only when ALL inputs are HIGH
      return !(totalInputCount >= 2 && highInputCount == totalInputCount);
      
    case NOR_GATE:
      // NOR is opposite of OR - TRUE only when ALL inputs are LOW
      return (totalInputCount >= 2 && highInputCount == 0);
      
    case XNOR_GATE:
      // XNOR is opposite of XOR - TRUE when inputs are SAME
      return (totalInputCount == 2 && (highInputCount == 0 || highInputCount == 2));
      
    default:
      return false;
  }
}

void drawConnection(int node1, int node2, uint16_t color, int offset) {
  int x1 = SX(nodes[node1].x);
  int y1 = SY(nodes[node1].y + offset);
  int x2 = SX(nodes[node2].x);
  int y2 = SY(nodes[node2].y + offset);
  tft.drawLine(x1, y1, x2, y2, color);
}

void drawCircuit() {
  tft.fillScreen(ST77XX_BLACK);
  
  // Draw title and level info
  tft.setCursor(5, 5);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(1);
  tft.print(levels[currentLevel].title);
  
  tft.setCursor(5, SCREEN_HEIGHT - 10);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.print(levels[currentLevel].objective);
  
  int activeNodes = levels[currentLevel].nodeCount;
  
  // Draw all connections (wires) first
  for (int i = 0; i < activeNodes; i++) {
    for (int j = 0; j < nodes[i].connectionCount; j++) {
      int targetId = nodes[i].connectedTo[j];
      if (targetId > i) {
        drawConnection(i, targetId, COLOR_DARKGREY);
      }
    }
  }
  
  // Draw completed path with thicker lines
  for (int i = 1; i < pathLength; i++) {
    drawConnection(pathSequence[i-1], pathSequence[i], ST77XX_GREEN);
    drawConnection(pathSequence[i-1], pathSequence[i], ST77XX_GREEN, 1);
  }
  
  // Draw nodes and gates
  for (int i = 0; i < activeNodes; i++) {
    if (nodes[i].isGate) {
      // Draw logic gate symbol
      drawGateSymbol(nodes[i].x, nodes[i].y, nodes[i].gateType, nodes[i].visited);
    } else {
      // Draw regular node (input/output/connection)
      uint16_t color;
      if (nodes[i].visited) {
        color = ST77XX_GREEN;
      } else if (nodes[i].isInput) {
        color = nodes[i].signalValue ? ST77XX_RED : ST77XX_MAGENTA;  // RED=HIGH, MAGENTA=LOW
      } else if (nodes[i].isOutput) {
        color = ST77XX_BLUE;
      } else {
        color = ST77XX_YELLOW;
      }
      
      int nx = SX(nodes[i].x);
      int ny = SY(nodes[i].y);
      int nr = SR(NODE_RADIUS);
      tft.fillCircle(nx, ny, nr, color);
      tft.drawCircle(nx, ny, nr + 1, ST77XX_WHITE);
      
      // Label inputs/outputs
      if (nodes[i].isInput) {
        tft.setCursor(nx - SW(8), ny + SH(10));
        tft.setTextColor(nodes[i].signalValue ? ST77XX_RED : ST77XX_MAGENTA);
        tft.setTextSize(1);
        tft.print(nodes[i].signalValue ? "1" : "0");
      } else if (nodes[i].isOutput) {
        tft.setCursor(nx - SW(12), ny + SH(10));
        tft.setTextColor(ST77XX_BLUE);
        tft.setTextSize(1);
        tft.print("OUT");
      }
    }
  }
}

void drawGateSymbol(int x, int y, GateType type, bool active) {
  // Map from the original coordinate system (320x170) to the Feather TFT (240x135)
  int cx = SX(x);
  int cy = SY(y);

  uint16_t fg = active ? ST77XX_GREEN : ST77XX_WHITE;
  uint16_t bg = ST77XX_BLACK;

  int w = SW(56);
  int h = SH(30);
  int r = SR(6);

  // Clear area and draw a rounded "gate" box
  tft.fillRoundRect(cx - w / 2, cy - h / 2, w, h, r, bg);
  tft.drawRoundRect(cx - w / 2, cy - h / 2, w, h, r, fg);

  // Gate label
  tft.setTextSize(1);
  tft.setTextColor(fg);
  tft.setCursor(cx - SW(18), cy - SH(4));

  switch (type) {
    case AND_GATE:  tft.print("AND"); break;
    case OR_GATE:   tft.print("OR"); break;
    case NOT_GATE:  tft.print("NOT"); break;
    case NAND_GATE: tft.print("NAND"); break;
    case NOR_GATE:  tft.print("NOR"); break;
    case XOR_GATE:  tft.print("XOR"); break;
    default:        tft.print("?"); break;
  }
}


void showError(const char* message) {
  int boxW = SCREEN_WIDTH - 20;
  int boxH = 28;
  int x = 10;
  int y = (SCREEN_HEIGHT / 2) - (boxH / 2);

  tft.fillRect(x, y, boxW, boxH, ST77XX_BLACK);
  tft.drawRect(x, y, boxW, boxH, ST77XX_RED);

  tft.setCursor(x + 6, y + 10);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.print(message);

  delay(1000);
  drawCircuit();
}

void showSuccess() {
  delay(300);

  int boxW = SCREEN_WIDTH - 20;
  int boxH = 44;
  int x = 10;
  int y = (SCREEN_HEIGHT / 2) - (boxH / 2);

  tft.fillRect(x, y, boxW, boxH, ST77XX_BLACK);
  tft.drawRect(x, y, boxW, boxH, ST77XX_GREEN);

  tft.setCursor(x + 12, y + 10);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.print("DONE");

  tft.setCursor(x + 12, y + 30);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  if (currentLevel < totalLevels - 1) {
    tft.print("Touch to continue");
  } else {
    tft.print("Touch to restart");
  }
}

void showGameComplete() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);

  tft.setCursor(10, 30);
  tft.print("ALL");
  tft.setCursor(10, 55);
  tft.print("LEVELS");
  tft.setCursor(10, 80);
  tft.print("DONE!");

  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(10, 105);
  tft.print("Logic Master!");
}

void resetLevel() {
  initLevel(currentLevel);
  drawCircuit();
}