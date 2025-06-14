
#include <Servo.h>
#include <ezButton.h>

#define VRX_PIN      A0 // Arduino pin connected to VRX pin
#define VRY_PIN      A1 // Arduino pin connected to VRY pin
#define SW_PIN   13  // Arduino pin connected to SW  pin
#define SERVO_X_PIN  10  // Arduino pin connected to Servo motor 1
#define SERVO_Y_PIN  11  // Arduino pin connected to Servo motor 2

#define COMMAND_NO     0x00
#define COMMAND_LEFT   0x01
#define COMMAND_RIGHT  0x02
#define COMMAND_UP     0x04
#define COMMAND_DOWN   0x08

#define LEFT_THRESHOLD  400
#define RIGHT_THRESHOLD 800
#define UP_THRESHOLD    400
#define DOWN_THRESHOLD  800

#define UPDATE_INTERVAL 100 // 100ms

ezButton button(SW_PIN);
Servo xServo;  // create servo object to control a servo 1
Servo yServo;  // create servo object to control a servo 2

int xValue = 0 ; // To store value of the X axis
int yValue = 0 ; // To store value of the Y axis
int xAngle = 90; // the center position of servo #1
int yAngle = 90; // the center position of servo #2
int command = COMMAND_NO;

unsigned long lastUpdateTime = 0;

void setup() {
  Serial.begin(9600) ;

  xServo.attach(SERVO_X_PIN);
  yServo.attach(SERVO_Y_PIN);

  button.setDebounceTime(50); // set debounce time to 50 milliseconds
}

void loop() {
  button.loop(); // MUST call the loop() function first

  if (millis() - lastUpdateTime > UPDATE_INTERVAL) {
    lastUpdateTime = millis() ;

    // read analog X and Y analog values
    xValue = analogRead(VRX_PIN);
    yValue = analogRead(VRY_PIN);

    // converts the analog value to commands
    // reset commands
    command = COMMAND_NO;

    // check left/right commands
    if (xValue < LEFT_THRESHOLD)
      command = command | COMMAND_LEFT;
    else if (xValue > RIGHT_THRESHOLD)
      command = command | COMMAND_RIGHT;

    // check up/down commands
    if (yValue < UP_THRESHOLD)
      command = command | COMMAND_UP;
    else if (yValue > DOWN_THRESHOLD)
      command = command | COMMAND_DOWN;

    // NOTE: AT A TIME, THERE MAY BE NO COMMAND, ONE COMMAND OR TWO COMMANDS

    // print command to serial and process command
    if (command & COMMAND_LEFT) {
      Serial.println("COMMAND LEFT");
      xAngle--;
    }

    if (command & COMMAND_RIGHT) {
      Serial.println("COMMAND RIGHT");
      xAngle++;
    }

    if (command & COMMAND_UP) {
      Serial.println("COMMAND UP");
      yAngle--;
    }

    if (command & COMMAND_DOWN) {
      Serial.println("COMMAND DOWN");
      yAngle++;
    }
  }

  if (button.isPressed()) {
    Serial.println("The button is pressed");
    xAngle = 90; // the center position of servo #1
    yAngle = 90; // the center position of servo #2
  }

  xServo.write(xAngle); // rotate servo motor 1
  yServo.write(yAngle); // rotate servo motor 2

  // print data to Serial Monitor on Arduino IDE
  Serial.print("Servo Motor's Angle: ");
  Serial.print(xAngle);
  Serial.print("°, ");
  Serial.print(xAngle);
  Serial.println("°");
}
