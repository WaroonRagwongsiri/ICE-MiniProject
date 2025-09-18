// Include Serial
#include <SoftwareSerial.h>
#include <UTFT.h>

#define Led1 7
#define Led2 6
#define Buzz 5
#define TX_OUT 4
#define RX_IN 3
const SoftwareSerial mySerial(RX_IN, TX_OUT);

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

extern uint8_t BigFont[];
//(Model,SDA,SCK,CS,RST,A0)
UTFT tft(ST7735, 12, 13, 9, 10, 11);

bool currentPlayer = 0;
//insert
static int	board[6][7] = {
	{0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0,},
	{0, 0, 0, 0, 0, 0, 0,},
};

void setup() {
  Serial.begin(9600);
  mySerial.begin(2400);
  Serial.setTimeout(50);
  mySerial.setTimeout(50);
  tft.InitLCD();
  tft.setFont(BigFont);
  tft.clrScr();
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  digitalWrite(Led1, HIGH);
  digitalWrite(Led2, HIGH);
  Serial.println("Board 2 Ready");
  tft.fillScr(Black);
  tft.setColor(Green);
  tft.fillRect(0, 0, 159, 18);  //Banner
  tft.setBackColor(Trans);
  tft.setColor(Black);
  tft.print(String("Player 1"), CENTER, 1);
  tft.setColor(Navy);
  tft.fillRect(16, 19, 142, 127);  //Perimeter
  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 7; j++) {
      tft.setColor(Black);
      tft.fillCircle(25 + (18 * j), 28 + (18 * i), 8);
    }
  }
}

void loop() {
  if (mySerial.available() > 0) {
    serial_to_board();
    display_board_serial();
    if (!currentPlayer) {
      digitalWrite(Led1, HIGH);
      digitalWrite(Led2, LOW);
      tft.setColor(Red);
      tft.fillRect(0, 0, 159, 18);  //Banner
      tft.setBackColor(Trans);
      tft.setColor(White);
      tft.print(String("Player 2"), CENTER, 1);
      currentPlayer = !currentPlayer;
    } else {
      digitalWrite(Led1, LOW);
      digitalWrite(Led2, HIGH);
      tft.setColor(Green);
      tft.fillRect(0, 0, 159, 18);  //Banner
      tft.setBackColor(Trans);
      tft.setColor(Black);
      tft.print(String("Player 1"), CENTER, 1);
      currentPlayer = !currentPlayer;
    }
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 7; j++) {
        if (board[i][j] == 1) {
          tft.setColor(Green);
          tft.fillCircle(25 + (18 * j), 28 + (18 * i), 8);
        }
        if (board[i][j] == 2) {
          tft.setColor(Red);
          tft.fillCircle(25 + (18 * j), 28 + (18 * i), 8);
        }
      }
    }
  }
}

void serial_to_board() {
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
      board[row][col] = str[num] - '0';
      col++;
      num += 2;
    }
    row++;
  }
}

void display_board_serial(void) {
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
