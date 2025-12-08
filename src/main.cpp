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
const int PULSE_COUNT = 5;  // フルテスト：各速度で5回パルス

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

// 10パターンのパルス回数（3-5回）とインターバル（0-500ms）
const int patternPulseCounts[10] = {4, 3, 5, 3, 4, 5, 3, 4, 5, 4};  // 回
const int patternIntervals[10] = {400, 250, 150, 350, 50, 450, 200, 300, 0, 100};  // ms (0-500ms)

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
  M5.Display.println("Full Test");
  delay(1000);
  
  Serial.println("=== Starting Full Servo Test ===");
  
  // Servo 1 (G5)
  testServo(servo1, 1, SERVO1_PIN);
  
  // Servo 2 (G6)
  testServo(servo2, 2, SERVO2_PIN);
  
  // Servo 3 (G7)
  testServo(servo3, 3, SERVO3_PIN);
  
  Serial.println("=== Full Test Complete ===");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Full Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  delay(2000);
}

void executePattern(int servoNum, int angle, int speed, int moveNum, int pulseCount, int intervalTime) {
  // 全サーボを0°に戻す
  servo1.write(ANGLE_0);
  servo2.write(ANGLE_0);
  servo3.write(ANGLE_0);
  delay(100);
  
  Servo* targetServo;
  int pin;
  
  switch(servoNum) {
    case 1:
      targetServo = &servo1;
      pin = SERVO1_PIN;
      break;
    case 2:
      targetServo = &servo2;
      pin = SERVO2_PIN;
      break;
    case 3:
      targetServo = &servo3;
      pin = SERVO3_PIN;
      break;
  }
  
  const char* speedName = (speed == SPEED_SLOW) ? "SLOW" : "FAST";
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/10\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("Servo %d\n", servoNum);
  M5.Display.println(speedName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d: %dms\n", pin, speed);
  M5.Display.printf("Count: %d\n", pulseCount);
  M5.Display.printf("Wait: %dms", intervalTime);
  
  Serial.printf("Move %d/10: Servo%d G%d %s (%dms) Count:%d Wait:%dms\n", 
                moveNum, servoNum, pin, speedName, speed, pulseCount, intervalTime);
  
  // パルス動作
  for (int i = 0; i < pulseCount; i++) {
    targetServo->write(ANGLE_0);
    delay(speed);
    targetServo->write(ANGLE_90);
    delay(speed);
    Serial.printf("  Pulse %d/%d\n", i + 1, pulseCount);
  }
  
  // 最後は0°に戻す
  targetServo->write(ANGLE_0);
  
  delay(intervalTime);
}

void run10Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("10x Pattern");
  delay(1000);
  
  Serial.println("\n=== 10 Pattern Fixed Sequence ===");
  
  // 固定の10パターン（順番と速度は固定、パルス回数とインターバルだけバラバラ）
  executePattern(2, ANGLE_90, SPEED_FAST, 1, patternPulseCounts[0], patternIntervals[0]);   // Servo2 速い 4回 / 400ms
  executePattern(3, ANGLE_90, SPEED_SLOW, 2, patternPulseCounts[1], patternIntervals[1]);   // Servo3 遅い 3回 / 250ms
  executePattern(1, ANGLE_90, SPEED_FAST, 3, patternPulseCounts[2], patternIntervals[2]);   // Servo1 速い 5回 / 150ms
  executePattern(2, ANGLE_90, SPEED_SLOW, 4, patternPulseCounts[3], patternIntervals[3]);   // Servo2 遅い 3回 / 350ms
  executePattern(1, ANGLE_90, SPEED_SLOW, 5, patternPulseCounts[4], patternIntervals[4]);   // Servo1 遅い 4回 / 50ms
  executePattern(3, ANGLE_90, SPEED_FAST, 6, patternPulseCounts[5], patternIntervals[5]);   // Servo3 速い 5回 / 450ms
  executePattern(2, ANGLE_90, SPEED_FAST, 7, patternPulseCounts[6], patternIntervals[6]);   // Servo2 速い 3回 / 200ms
  executePattern(1, ANGLE_90, SPEED_FAST, 8, patternPulseCounts[7], patternIntervals[7]);   // Servo1 速い 4回 / 300ms
  executePattern(3, ANGLE_90, SPEED_SLOW, 9, patternPulseCounts[8], patternIntervals[8]);   // Servo3 遅い 5回 / 0ms
  executePattern(1, ANGLE_90, SPEED_SLOW, 10, patternPulseCounts[9], patternIntervals[9]);  // Servo1 遅い 4回 / 100ms
  
  // 全サーボを0°に戻す
  servo1.write(ANGLE_0);
  servo2.write(ANGLE_0);
  servo3.write(ANGLE_0);
  
  Serial.println("=== 10 Pattern Complete ===\n");
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("10x Pattern");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 10x pattern");
  
  delay(2000);
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
  M5.Display.println("Short press:");
  M5.Display.println(" Full test");
  M5.Display.println("");
  M5.Display.println("Long press:");
  M5.Display.println(" 10x pattern");
  M5.Display.println("");
  M5.Display.println("Press to start");
}

void loop() {
  M5.update();
  
  // ボタンが押された瞬間の時刻を記録
  if (M5.BtnA.wasPressed()) {
    unsigned long pressStartTime = millis();
    
    // ボタンが離されるまで待つ
    while (M5.BtnA.isPressed()) {
      M5.update();
      delay(10);
    }
    
    // 押していた時間を計算
    unsigned long pressDuration = millis() - pressStartTime;
    
    if (pressDuration >= LONG_PRESS_TIME) {
      // 長押し：10パターン実行
      Serial.printf("Long press detected (%lums)\n", pressDuration);
      run10Pattern();
    } else {
      // 短押し：フルテスト実行
      Serial.printf("Short press detected (%lums)\n", pressDuration);
      runAllTests();
    }
  }
  
  delay(10);
}