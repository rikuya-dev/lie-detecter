// Lie Detector System: Weighted Scoring Lie Judgement
const int pinPressure = A0; // Respiration (Pressure Sensor)
const int pinPulse = A1;    // Heartbeat (Pulse Sensor)
const int pinSweat = A2;    // Skin Conductance
const int pinVoice = A3;    // Voice Sensor（使ってないが残しとく）
const int pinFace = A4;     // Facial (Vibration Sensor)
const int pinMotionX = A5;  // Motion (Accelerometer X)
int lastPulse = 0;
unsigned long lastBeatTime = 0;
unsigned long currentBeatTime = 0;
int lastPulsePeak = 0;
void setup() { Serial.begin(9600); }
void loop()
{
    int lieScore = 0;
    lieScore += checkRespiration(); // 0〜10
    lieScore += checkHeartbeat();   // 0〜20
    lieScore += checkSweat();       // 0〜40
    lieScore += checkFacial();      // 0〜10
    lieScore += checkMotion();      // 0〜20
    if (lieScore > 100)
        lieScore = 100;
    Serial.print("Lie Score: ");
    Serial.print(lieScore);
    if (lieScore >= 70)
    {
        Serial.print(" [💥 嘘確定]");
    }
    else if (lieScore >= 40)
    {
        Serial.print(" [⚠️ 疑わしい]");
    }
    else if (lieScore > 0)
    {
        Serial.print(" [🟡 軽度]");
    }
    else
    {
        Serial.print(" [🟢 正常]");
    }
    Serial.println();
    Serial.println(lieScore);
    delay(100);
} // --- 各センサから得点を返すバージョン（0～個別最大スコア） ---
int checkRespiration()
{
    int breath = analogRead(pinPressure); // 例: 400〜700
    int score = map(constrain(breath, 400, 700), 400, 700, 0, 10);
    Serial.print("Resp: ");
    Serial.print(breath);
    Serial.print(" ");
    return score;
}
int checkHeartbeat()
{
    int pulse = analogRead(pinPulse);
    Serial.print("Pulse: ");
    Serial.print(pulse);
    Serial.print(" ");
    lastPulse = pulse;
    int score = 0;
    if (pulse - lastPulsePeak > 20)
    {
        lastPulsePeak = pulse;
        currentBeatTime = millis();
        unsigned long interval = currentBeatTime - lastBeatTime;
        lastBeatTime = currentBeatTime;
        Serial.print("Interval: ");
        Serial.print(interval);
        Serial.print(" "); // BPM範囲: 1000ms〜400ms → スコア 0〜20（速いほどスコア高）
        if (interval >= 400 && interval <= 1000)
        {
            score = map(interval, 1000, 400, 0, 20);
        }
    }
    return score;
}
int checkSweat()
{
    int sweat = analogRead(pinSweat); // 300〜800想定
    int score = map(constrain(sweat, 300, 800), 300, 800, 0, 40);
    Serial.print("Sweat: ");
    Serial.print(sweat);
    Serial.print(" ");
    return score;
}
int checkFacial()
{
    int face = analogRead(pinFace); // 100〜700想定
    int score = map(constrain(face, 100, 700), 100, 700, 0, 10);
    Serial.print("Facial: ");
    Serial.print(face);
    Serial.print(" ");
    return score;
}
int checkMotion()
{
    int motion = analogRead(pinMotionX); // 500〜1000想定
    int score = map(constrain(motion, 500, 1000), 500, 1000, 0, 20);
    Serial.print("Motion: ");
    Serial.print(motion);
    Serial.print(" ");
    return score;
}