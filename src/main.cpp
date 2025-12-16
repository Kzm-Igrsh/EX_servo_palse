#include <M5Unified.h>
#include <ESP32Servo.h>

// 3つのサーボ
Servo servoLeft;
Servo servoCenter;
Servo servoRight;

// サーボピン
const int SERVO_LEFT_PIN = 5;    // G5 - Left
const int SERVO_CENTER_PIN = 6;  // G6 - Center
const int SERVO_RIGHT_PIN = 7;   // G7 - Right

// 角度設定
const int ANGLE_0 = 0;
const int ANGLE_90 = 90;

// 速度設定（ミリ秒）
const int SPEED_WEAK = 600;   // Weak（0.6秒間隔）
const int SPEED_STRONG = 200; // Strong（0.2秒間隔）

// テスト設定
const int PULSE_COUNT = 5;  // フルテスト：各速度で5回パルス

// 長押し判定時間（ミリ秒）
const int LONG_PRESS_TIME = 1000;  // 1秒以上で長押し

// 10パターンのパルス回数（3-5回）とインターバル（0-500ms）
const int patternPulseCounts[10] = {4, 3, 5, 3, 4, 5, 3, 4, 5, 4};  // 回
const int patternIntervals[10] = {450, 1050, 1500, 1350, 300, 200, 1500, 300, 900, 150};  // ms (0-500ms)

// ========================================
// シリアル通信用ヘルパー関数
// PC側のPython GUIが期待する厳密なフォーマットで出力
// ========================================
void sendStimMessage(const char* position, const char* strength) {
  Serial.printf("%s,%s\n", position, strength);
}

void testPulse(Servo &servo, const char* position, int pin, int speed, const char* strengthName) {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.printf("Pin: G%d\n", pin);
  M5.Display.printf("%s (%dms)\n", strengthName, speed);
  M5.Display.println("");
  M5.Display.println("Testing...");
  
  // ========================================
  // パルス刺激開始 - PC側へメッセージ送信
  // ========================================
  sendStimMessage(position, strengthName);
  
  for (int i = 0; i < PULSE_COUNT; i++) {
    // 0°
    servo.write(ANGLE_0);
    M5.Display.fillRect(0, 80, 128, 48, BLACK);
    M5.Display.setCursor(0, 80);
    M5.Display.setTextSize(2);
    M5.Display.printf("Pulse %d/5\n", i + 1);
    M5.Display.printf("0 deg");
    delay(speed);
    
    // 90°
    servo.write(ANGLE_90);
    M5.Display.fillRect(0, 80, 128, 48, BLACK);
    M5.Display.setCursor(0, 80);
    M5.Display.setTextSize(2);
    M5.Display.printf("Pulse %d/5\n", i + 1);
    M5.Display.printf("90 deg");
    delay(speed);
  }
  
  // 最後は0°に戻す
  servo.write(ANGLE_0);
  
  // ========================================
  // パルス刺激終了 - インターバル開始
  // ========================================
  sendStimMessage("none", "none");
  delay(500);
}

void testServo(Servo &servo, const char* position, int pin) {
  // Weakテスト
  testPulse(servo, position, pin, SPEED_WEAK, "Weak");
  delay(1000);
  
  // Strongテスト
  testPulse(servo, position, pin, SPEED_STRONG, "Strong");
  delay(1000);
}

void runAllTests() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("Full Test");
  delay(1000);
  
  // Left (G5)
  testServo(servoLeft, "Left", SERVO_LEFT_PIN);
  
  // Center (G6)
  testServo(servoCenter, "Center", SERVO_CENTER_PIN);
  
  // Right (G7)
  testServo(servoRight, "Right", SERVO_RIGHT_PIN);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Full Test");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
  delay(2000);
}

void executePattern(const char* position, int speed, int moveNum, int pulseCount, int intervalTime) {
  // 全サーボを0°に戻す
  servoLeft.write(ANGLE_0);
  servoCenter.write(ANGLE_0);
  servoRight.write(ANGLE_0);
  delay(100);
  
  Servo* targetServo;
  int pin;
  
  if (strcmp(position, "Left") == 0) {
    targetServo = &servoLeft;
    pin = SERVO_LEFT_PIN;
  } else if (strcmp(position, "Center") == 0) {
    targetServo = &servoCenter;
    pin = SERVO_CENTER_PIN;
  } else {
    targetServo = &servoRight;
    pin = SERVO_RIGHT_PIN;
  }
  
  const char* strengthName = (speed == SPEED_WEAK) ? "Weak" : "Strong";
  
  // 画面表示
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(1);
  M5.Display.printf("Move %d/20\n", moveNum);
  M5.Display.println("==============");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.printf("%s\n", position);
  M5.Display.printf("%s\n", strengthName);
  M5.Display.setTextSize(1);
  M5.Display.printf("G%d: %dms\n", pin, speed);
  M5.Display.printf("Count: %d\n", pulseCount);
  M5.Display.printf("Wait: %dms", intervalTime);
  
  // ========================================
  // パルス刺激開始 - PC側へメッセージ送信
  // ========================================
  sendStimMessage(position, strengthName);
  
  // パルス動作
  for (int i = 0; i < pulseCount; i++) {
    targetServo->write(ANGLE_0);
    delay(speed);
    targetServo->write(ANGLE_90);
    delay(speed);
  }
  
  // 最後は0°に戻す
  targetServo->write(ANGLE_0);
  
  // ========================================
  // インターバル開始 - "none,none" を送信
  // ========================================
  if (intervalTime > 0) {
    sendStimMessage("none", "none");
    
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(1);
    M5.Display.printf("Move %d/20\n", moveNum);
    M5.Display.println("==============");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.printf("Wait:%dms", intervalTime);
    
    delay(intervalTime);
  } else {
    // インターバルが0msの場合も "none,none" を送信
    sendStimMessage("none", "none");
  }
}

void run20Pattern() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("Starting");
  M5.Display.println("20x Pattern");
  delay(1000);
  
  // 1~10回目
  executePattern("Center", SPEED_STRONG, 1, patternPulseCounts[0], patternIntervals[0]);  // Center Strong 4回 / 150ms
  executePattern("Right", SPEED_WEAK, 2, patternPulseCounts[1], patternIntervals[1]);     // Right Weak 3回 / 350ms
  executePattern("Left", SPEED_STRONG, 3, patternPulseCounts[2], patternIntervals[2]);    // Left Strong 5回 / 0ms
  executePattern("Center", SPEED_WEAK, 4, patternPulseCounts[3], patternIntervals[3]);    // Center Weak 3回 / 450ms
  executePattern("Left", SPEED_WEAK, 5, patternPulseCounts[4], patternIntervals[4]);      // Left Weak 4回 / 100ms
  executePattern("Right", SPEED_STRONG, 6, patternPulseCounts[5], patternIntervals[5]);   // Right Strong 5回 / 250ms
  executePattern("Center", SPEED_STRONG, 7, patternPulseCounts[6], patternIntervals[6]);  // Center Strong 3回 / 500ms
  executePattern("Right", SPEED_STRONG, 8, patternPulseCounts[7], patternIntervals[7]);   // Right Strong 4回 / 200ms
  executePattern("Right", SPEED_WEAK, 9, patternPulseCounts[8], patternIntervals[8]);     // Right Weak 5回 / 300ms
  executePattern("Left", SPEED_WEAK, 10, patternPulseCounts[9], patternIntervals[9]);     // Left Weak 4回 / 50ms
  
  // 11~20回目（配列の順番をめちゃくちゃに、位置と強度も変更）
  executePattern("Left", SPEED_STRONG, 11, patternPulseCounts[6], patternIntervals[4]);   // Left Strong 3回 / 100ms
  executePattern("Center", SPEED_WEAK, 12, patternPulseCounts[2], patternIntervals[7]);   // Center Weak 5回 / 200ms
  executePattern("Right", SPEED_WEAK, 13, patternPulseCounts[8], patternIntervals[1]);    // Right Weak 5回 / 350ms
  executePattern("Left", SPEED_WEAK, 14, patternPulseCounts[5], patternIntervals[9]);     // Left Weak 5回 / 50ms
  executePattern("Right", SPEED_STRONG, 15, patternPulseCounts[1], patternIntervals[5]);  // Right Strong 3回 / 250ms
  executePattern("Center", SPEED_STRONG, 16, patternPulseCounts[9], patternIntervals[2]); // Center Strong 4回 / 0ms
  executePattern("Left", SPEED_STRONG, 17, patternPulseCounts[4], patternIntervals[8]);   // Left Strong 4回 / 300ms
  executePattern("Center", SPEED_WEAK, 18, patternPulseCounts[7], patternIntervals[6]);   // Center Weak 4回 / 500ms
  executePattern("Right", SPEED_STRONG, 19, patternPulseCounts[0], patternIntervals[3]);  // Right Strong 4回 / 450ms
  executePattern("Left", SPEED_WEAK, 20, patternPulseCounts[3], patternIntervals[0]);     // Left Weak 3回 / 150ms
  
  // 全サーボを0°に戻す
  servoLeft.write(ANGLE_0);
  servoCenter.write(ANGLE_0);
  servoRight.write(ANGLE_0);
  
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  M5.Display.println("20x Pattern");
  M5.Display.println("Complete!");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
  
  delay(2000);
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // シリアル通信初期化（115200bps）
  Serial.begin(115200);
  
  // サーボ初期化（デバッグ出力なし）
  servoLeft.attach(SERVO_LEFT_PIN);
  servoLeft.write(ANGLE_0);
  
  servoCenter.attach(SERVO_CENTER_PIN);
  servoCenter.write(ANGLE_0);
  
  servoRight.attach(SERVO_RIGHT_PIN);
  servoRight.write(ANGLE_0);
  
  // 初期画面表示
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("3 Servo Pulse Test");
  M5.Display.println("==================");
  M5.Display.println("");
  M5.Display.setTextSize(2);
  M5.Display.println("None");
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("Short: Full test");
  M5.Display.println("Long: 20x pattern");
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
      // 長押し：20パターン実行
      run20Pattern();
    } else {
      // 短押し：フルテスト実行
      runAllTests();
    }
    
    // テスト完了後、Noneを表示
    M5.Display.clear();
    M5.Display.setTextSize(1);
    M5.Display.setCursor(0, 0);
    M5.Display.println("3 Servo Pulse Test");
    M5.Display.println("==================");
    M5.Display.println("");
    M5.Display.setTextSize(2);
    M5.Display.println("None");
    M5.Display.println("");
    M5.Display.setTextSize(1);
    M5.Display.println("Short: Full test");
    M5.Display.println("Long: 20x pattern");
  }
  
  delay(10);
}