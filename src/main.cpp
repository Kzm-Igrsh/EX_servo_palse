#include <M5Unified.h>
#include <ESP32Servo.h>

Servo servo;

// サーボピン（AtomS3のGrove互換ピン）
const int SERVO_PIN = 5;  // G5 (Grove Yellow)

// 角度設定
const int ANGLE_0 = 0;
const int ANGLE_90 = 90;

// 速度設定（ミリ秒）
const int SPEED_STOP = 0;    // 停止
const int SPEED_SLOW = 700;  // 遅い（0.7秒間隔）
const int SPEED_FAST = 300;  // 速い（0.3秒間隔）

int speedMode = 0; // 0=停止, 1=遅い, 2=速い
int currentSpeed = SPEED_STOP;
int currentAngle = ANGLE_0;
unsigned long lastChangeTime = 0;

void updateDisplay() {
  M5.Display.clear();
  M5.Display.setCursor(0, 0);
  M5.Display.setTextSize(2);
  
  switch(speedMode) {
    case 0:
      M5.Display.println("STOP");
      break;
    case 1:
      M5.Display.println("SLOW");
      M5.Display.println("");
      M5.Display.setTextSize(1);
      M5.Display.println("0.7 sec interval");
      break;
    case 2:
      M5.Display.println("FAST");
      M5.Display.println("");
      M5.Display.setTextSize(1);
      M5.Display.println("0.3 sec interval");
      break;
  }
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // サーボ初期化
  servo.attach(SERVO_PIN);
  servo.write(ANGLE_0);
  
  M5.Display.clear();
  M5.Display.setTextSize(1);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Servo Pulse");
  M5.Display.println("===========");
  M5.Display.println("");
  M5.Display.println("Press button");
  M5.Display.println("to change speed");
  M5.Display.println("");
  M5.Display.println("Stop -> Slow");
  M5.Display.println("-> Fast -> Stop");
  M5.Display.println("");
  M5.Display.println("Status: Stop");
  
  delay(2000);
  updateDisplay();
  
  lastChangeTime = millis();
}

void loop() {
  M5.update();
  
  // ボタン押下：速度切り替え
  if (M5.BtnA.wasPressed()) {
    speedMode++;
    if (speedMode > 2) {
      speedMode = 0;
    }
    
    switch(speedMode) {
      case 0:
        currentSpeed = SPEED_STOP;
        break;
      case 1:
        currentSpeed = SPEED_SLOW;
        break;
      case 2:
        currentSpeed = SPEED_FAST;
        break;
    }
    
    updateDisplay();
    lastChangeTime = millis();
  }
  
  // サーボ制御（停止以外）
  if (currentSpeed > 0) {
    unsigned long currentTime = millis();
    
    if (currentTime - lastChangeTime >= currentSpeed) {
      // 角度を切り替え
      if (currentAngle == ANGLE_0) {
        currentAngle = ANGLE_90;
      } else {
        currentAngle = ANGLE_0;
      }
      
      servo.write(currentAngle);
      lastChangeTime = currentTime;
      
      // 画面更新（角度表示）
      M5.Display.fillRect(0, 100, 128, 28, BLACK);
      M5.Display.setCursor(0, 100);
      M5.Display.setTextSize(2);
      M5.Display.printf("%d deg", currentAngle);
    }
  }
  
  delay(10);
}