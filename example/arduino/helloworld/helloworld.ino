#include <SerialBridge.hpp>
#include <InoHardwareSerial.hpp>

#include <Message.hpp>

typedef struct{
    char c[32];
} words_t;

typedef sb::Message<words_t> Words;

Words hello;

SerialDev *dev = new InoHardwareSerial(&Serial);
SerialBridge serial(dev);

void setup()
{
    Serial.begin(9600);
    serial.add_frame(123, &hello);

    sprintf(hello.data.c, "Hello world from ino.\n");

    serial.write(123);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    static bool read_success = serial.read() == 0;
    if(read_success){
        digitalWrite(LED_BUILTIN, HIGH);
    }

    serial.update();
}