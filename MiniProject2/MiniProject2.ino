// Libraries
#include <SoftwareSerial.h>
#include <UTFT.h>

// Led pin
#define Led1 7
#define Led2 6

// Software serial pin
#define TX_OUT 4
#define RX_IN 3
const SoftwareSerial mySerial(RX_IN, TX_OUT);

// Color aliases
#define Trans VGA_TRANSPARENT
#define Black VGA_BLACK
#define Silver VGA_SILVER
#define Gray VGA_GRAY
#define White VGA_WHITE
#define Red VGA_BLUE
#define Green VGA_LIME
#define Blue VGA_RED
#define Navy VGA_MAROON
#define Purple VGA_FUCHSIA
#define Yellow VGA_AQUA
#define Aqua VGA_YELLOW

// External font
extern uint8_t BigFont[];
// Initialize the display object.
// Constructor parameters: (display model, SDA, SCK, CS, RST, A0)
UTFT tft(ST7735, 12, 13, 9, 10, 11);

// Game state
// currentPlayer: 0 or 1 toggled to indicate turn; initial value 0
static bool currentPlayer = 0;
// board: 6 rows x 7 columns grid used for a Connect-Four style game
// 0 = empty, 1 = player 1 token, 2 = player 2 token
static int	board[6][7] = {
  {0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0,},
  {0, 0, 0, 0, 0, 0, 0,},
};

void setup() {
  // Initialize serial ports
  Serial.begin(9600);       // USB serial for debugging
  mySerial.begin(2400);     // Software serial for board-to-board comms
  Serial.setTimeout(100);
  mySerial.setTimeout(100);

  // Initialize TFT display
  tft.InitLCD();
  tft.setFont(BigFont);
  tft.clrScr();

  // Initialize LEDs
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  // Default LED state: Led1 on, Led2 off
  digitalWrite(Led1, LOW);
  digitalWrite(Led2, HIGH);

  Serial.println("Board 2 Ready");

  // Draw initial UI
  tft.fillScr(Black);                 // Screen background
  tft.setColor(Green);
  tft.fillRect(0, 0, 159, 18);        // Top banner showing the player
  tft.setBackColor(Trans);            // Make text background transparent
  tft.setColor(Black);                // Text color for "Player 1"
  tft.print(String("Player 1"), CENTER, 1);

  // Draw board area and empty holes
  tft.setColor(Navy);
  tft.fillRect(13, 19, 145, 127);     // Board background rectangle
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      tft.setColor(Black);  // Hole color (empty)
      // Draw circle for each cell position
      tft.fillCircle(25 + (18 * j), 28 + (18 * i), 8);
    }
  }
}

void loop() {
  // Main loop: wait for a serial update from the other board
  if (mySerial.available() > 0) {
    // Read board state string from software serial and populate board array
    serial_to_board();

    // Print the received board to the USB Serial for debugging
    display_board_serial();

    // If the board is empty (all zeros), set currentPlayer to 1
    if (check_zero()) {
      currentPlayer = 1;
    }

    // Toggle player indicator and update banner + LEDs
    if (!currentPlayer) {
      // Show Player 2's turn: Led1 OFF, Led2 ON, banner red
      digitalWrite(Led1, HIGH);
      digitalWrite(Led2, LOW);  // Player 2 indicator
      tft.setColor(Red);
      tft.fillRect(0, 0, 159, 18);  // Banner
      tft.setBackColor(Trans);
      tft.setColor(White);
      tft.print(String("Player 2"), CENTER, 1);
      currentPlayer = !currentPlayer; // flip turn state
    } else {
      // Show Player 1's turn: Led1 ON, Led2 OFF, banner green
      digitalWrite(Led1, LOW);  // Player 1 indicator
      digitalWrite(Led2, HIGH);
      tft.setColor(Green);
      tft.fillRect(0, 0, 159, 18);  // Banner
      tft.setBackColor(Trans);
      tft.setColor(Black);
      tft.print(String("Player 1"), CENTER, 1);
      currentPlayer = !currentPlayer;
    }

    // Redraw the board on the TFT based on the updated board array
    display_tft();
  }
}

void serial_to_board() {
  // Read a formatted string from software serial and fill the board array.
  String str;
  int row;
  int col;
  int num;

  str = mySerial.readString();
  row = 0;
  num = 0;
  while (row < 6) {
    col = 0;
    while (col < 7) {
      // Convert ascii digit to integer
      board[row][col] = str[num] - '0';
      col++;
      num += 2; // skip the delimiter
    }
    row++;
  }
}

void display_board_serial(void) {
  // Print the 6x7 board to the USB serial console
  Serial.println("Board Array:");
  for (int row = 0; row < 6; row++) {
    for (int col = 0; col < 7; col++) {
      Serial.print(board[row][col]);
      if (col < 6) {
        Serial.print(",");
      }
    }
    Serial.println();
  }
  Serial.println();
}

void display_tft(void) {
  // Draw each cell as a filled circle: empty, player1 (green), player2 (red)
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 7; j++) {
      if (board[i][j] == 0) {
        tft.setColor(Black);                     // empty hole
        tft.fillCircle(25 + (18 * j), 28 + (18 * i), 8);
      }
      if (board[i][j] == 1) {
        tft.setColor(Green);                     // player 1 token
        tft.fillCircle(25 + (18 * j), 28 + (18 * i), 8);
      }
      if (board[i][j] == 2) {
        tft.setColor(Red);                       // player 2 token
        tft.fillCircle(25 + (18 * j), 28 + (18 * i), 8);
      }
    }
  }
}

int check_zero(void) {
  // Return 1 if the entire board is empty (all zeros)
  int row;
  int col;

  row = 0;
  col = 0;
  while (row < 6) {
    col = 0;
    while (col < 7) {
      if (board[row][col] != 0)
        return (0);
      col++;
    }
    row++;
  }
  return (1);
}
