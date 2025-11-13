#include <M5Unified.h>
#include <ESP32Servo.h>

// 3つのサーボ
Servo servo1;
Servo servo2;
Servo servo3;

// サーボピン
const int SERVO1_PIN = 5;  // G5
const int SERVO2_PIN = 6;  // G6
const int SERVO3_PIN = 7;  // G7

// 角度設定
const int ANGLE_0 = 0;
const int ANGLE_90 = 90;

// 速度設定（ミリ秒）
const int SPEED_SLOW = 700;  // 遅い（0.7秒間隔）
const int SPEED_FAST = 300;  // 速い（0.3秒間隔）

// テスト設定
const int PULSE_COUNT = 5;  // 各速度で5回パルス

void testPulse(Servo &servo, int servoNum, int pin, int speed, const char* speedName) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("Servo %d\n", servoNum);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.printf("%s (%dms)\n", speedName, speed);
  M5.Display.println("");
  M5.Display.println("Testing...");
  
  Serial.printf("=== Servo%d G%d: %s (%dms) ===\n", servoNum, pin, speedName, speed);
  
  for (int i = 0; i < PULSE_COUNT; i++) {
    // 0°
    servo.write(ANGLE_0);
    M5.Display.fillRect(0, 80, 128, 48, BLACK);
    M5.Display.setCursor(0, 80);
    M5.Display.setTextSize(2);
    M5.Display.printf("Pulse %d/5\n", i + 1);
    M5.Display.printf("0 deg");
    Serial.printf("Pulse %d: 0 deg\n", i + 1);
    delay(speed);
    
    // 90°
    servo.write(ANGLE_90);
    M5.Display.fillRect(0, 80, 128, 48, BLACK);
    M5.Display.setCursor(0, 80);
    M5.Display.setTextSize(2);
    M5.Display.printf("Pulse %d/5\n", i + 1);
    M5.Display.printf("90 deg");
    Serial.printf("Pulse %d: 90 deg\n", i + 1);
    delay(speed);
  }
  
  // 最後は0°に戻す
  servo.write(ANGLE_0);
  delay(500);
}

void testServo(Servo &servo, int servoNum, int pin) {
  // 0.7秒テスト
  testPulse(servo, servoNum, pin, SPEED_SLOW, "SLOW");
  delay(1000);
  
  // 0.3秒テスト
  testPulse(servo, servoNum, pin, SPEED_FAST, "FAST");
  delay(1000);
}

void runAllTests() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("Test...");
  delay(1000);
  
  Serial.println("=== Starting 3 Servo Pulse Test ===");
  
  // Servo 1 (G5)
  testServo(servo1, 1, SERVO1_PIN);
  
  // Servo 2 (G6)
  testServo(servo2, 2, SERVO2_PIN);
  
  // Servo 3 (G7)
  testServo(servo3, 3, SERVO3_PIN);
  
  Serial.println("=== All Tests Complete ===");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("All Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to test again");
  
  delay(2000);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.println("3 Servo Pulse Test");
  M5.Display.println("==================");
  M5.Display.println("");
  M5.Display.println("Pins: G5,G6,G7");
  M5.Display.println("");
  M5.Display.println("Each servo:");
  M5.Display.println(" SLOW 5 pulses");
  M5.Display.println(" FAST 5 pulses");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  Serial.begin(115200);
  Serial.println("3 Servo Pulse Auto Test");
  
  // サーボ初期化
  Serial.printf("Init Servo1: Pin=%d\n", SERVO1_PIN);
  servo1.attach(SERVO1_PIN);
  servo1.write(ANGLE_0);
  
  Serial.printf("Init Servo2: Pin=%d\n", SERVO2_PIN);
  servo2.attach(SERVO2_PIN);
  servo2.write(ANGLE_0);
  
  Serial.printf("Init Servo3: Pin=%d\n", SERVO3_PIN);
  servo3.attach(SERVO3_PIN);
  servo3.write(ANGLE_0);
  
  Serial.println("Servo Init Complete");
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3 Servo Pulse Test");
  M5.Display.println("==================");
  M5.Display.println("");
  M5.Display.println("Pins: G5,G6,G7");
  M5.Display.println("");
  M5.Display.println("Each servo:");
  M5.Display.println(" SLOW 5 pulses");
  M5.Display.println(" FAST 5 pulses");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void loop() {
  M5.update();
  
  // ボタン押下：テスト開始
  if (M5.BtnA.wasPressed()) {
    Serial.println("Button pressed - Starting test");
    runAllTests();
  }
  
  delay(10);
}