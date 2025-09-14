#include <Arduino.h>

#define lm35 A0
#define peltier 3

void setup() {
    Serial.begin(9600);
    analogReference(INTERNAL); // usa referência interna de 1,1V
    pinMode(lm35, INPUT);
    pinMode(peltier, OUTPUT);
}

int set = 40;
int pwm = 0;

double pid;

double error = 0, last_error = 0;
double last_temperature;
long last_process;

double kp = 10, ki = 1.0, kd = 0;
double p = 0, i = 0, d = 0;

void loop() {

    int leitura = analogRead(lm35);
    float tensao = leitura * (1.1 / 1023.0); // referência interna
    int temperature = tensao * 100.0;      // 10 mV/°C

    error = set - temperature;
    float delta = (millis() - last_process) / 1000.0;
    last_process = millis();
    
    p = error * kp;
    i += (error * ki) * delta;
    if (i > 255) i = 255;
    if (i < -255) i = -255;
    //d = (last_temperature - temperature) * kd * delta;
    d = ((error - last_error) / delta) * kd;

    pid = p + i + d;

    pwm = constrain(pid, 0, 255);

    analogWrite(peltier, pwm);

    Serial.println(temperature);
    last_error = error;
    delay(100);
}