#include <Arduino.h>
#include <roboremo.h>
#include <WiFiClient.h>
#include <WiFi.h>

#define BATTERY_PIN 35

const char *ssid = "mywifi";  // You will connect your phone to this Access Point
const char *pw = "qwerty123"; // and this is the password
IPAddress ip(192, 168, 0, 1); // From RoboRemo app, connect to this IP
IPAddress netmask(255, 255, 255, 0);
const int port = 9876; // and this port

const int chCount = 4;       // 4 channels, you can add more if you have GPIOs :)
int chPin[] = {2, 4, 5, 18}; // ESP pins: GPIO 0, 2, 14, 12
int chVal[] = {0, 0, 0, 0};  // default value (middle)
float batteryVoltage = 0;
int batteryPercentage = 100;

WiFiServer server(port);
WiFiClient client;

void roboremo::begin()
{

    cmdIndex = 0;

    Serial.begin(115200);
    delay(1000);
    WiFi.softAPConfig(ip, ip, netmask); // configure ip address for softAP
    WiFi.softAP(ssid, pw);              // configure ssid and password for softAP

    server.begin(); // start TCP server

    Serial.println("ESP8266 RC receiver 1.1 powered by RoboRemo");
    Serial.println((String) "SSID: " + ssid + "  PASS: " + pw);
    Serial.println((String) "RoboRemo app must connect to " + ip.toString() + ":" + port);
}
void roboremo::slider()
{
    int ch = cmd[2] - '0';
    if (ch >= 0 && ch <= 9 && cmd[3] == ' ')
    {
        sliderValue[ch] = (int)atof(cmd + 4);
        // Serial.print("ch:" + String(ch));
        // Serial.println("-" + String(sliderValue[ch]));
    }
}

void roboremo::button()
{
    int sh = cmd[2] - '0';
    if (sh >= 0 && sh <= 9 && cmd[3] == ' ')
    {
        buttonValue[sh] = (int)atof(cmd + 4);
        // the continue is not important (int)atof(cmd + 4)
        // Serial.print("ch:" + String(ch));
        // Serial.println("-" + String(sliderValue[ch]));
    }
}

// void checkbattry()
// {
//     float batteryVoltage = analogRead(BATTERY_PIN) * (3.3 / 4095.0 * 2);
// }
void roboremo::checkbattry()
{
    int raw = analogRead(BATTERY_PIN);
    // float voltage = raw * (3.3 / 4095.0 * 2); 
    // int mv = voltage * 1000; 

    int percentage = map(raw, 2600, 3300, 0, 100);
    percentage = constrain(percentage, 0, 100);

    Serial.println(percentage);
    batteryPercentage = percentage;

    if (percentage <= 15)
    {
        send("msg", " Low Battery! " + String(percentage) + "%");
        send("led", 1);
    }
    else
    {
        send("msg", String(percentage) + "% " + String(raw));
        send("led", 0);
    }
}

// void roboremo::checkbattry()
// {
//     float voltage = analogRead(BATTERY_PIN) * (3.3 / 4095.0 * 2);
//     int percentage = (voltage - 3.0) / (4.2 - 3.0) * 100;
//     percentage = constrain(percentage, 0, 100);
//     Serial.println(percentage);
//     batteryPercentage = percentage;

//     if (percentage <= 15)
//     {
//         send("msg", " Low Battery! " + String(percentage) + "%");
//         send("led", 1);
//         // client.print("led 1\n");
//     }
//     else
//     {
//         send("msg", String(percentage) + "% "+String(analogRead(BATTERY_PIN)));
//         send("led", 0);

//         // client.print("led 0\n");
//     }
// }

void roboremo::exeCmd()
{
    // lastCmdTime = millis();
    for (byte i = 0; i < 10; i++)
    {
        Serial.print(cmd[i]);
    }
    Serial.println("");

    if (cmdStartsWith("ch"))
    {
        slider();
    }
    else if (cmdStartsWith("sh"))
    {
        Serial.println("Button:");

        button();
    }
}

void roboremo::send(String lable, String value)
{
    client.print(lable + " " + value + "\n");
    // client.println(value);
    //  client.print("%\n");
}

void roboremo::handel()
{

    if (!client.connected())

    {
        client = server.available();
        return;
    }

    // here we have a connected client

    if (client.available())
    {
        char c = (char)client.read(); // read char from client (RoboRemo app)

        if (c == '\n')
        { // if it is command ending
            cmd[cmdIndex] = 0;
            exeCmd();     // execute the command
            cmdIndex = 0; // reset the cmdIndex
        }
        else
        {
            cmd[cmdIndex] = c; // add to the cmd buffer
            if (cmdIndex < 99)
                cmdIndex++;
        }
    }

    if (millis() - aliveSentTime > 500)
    { // every 500ms
        client.write("alive 1\n");
        // send the alibe signal, so the "connected" LED in RoboRemo will stay ON
        // (the LED must have the id set to "alive")

        aliveSentTime = millis();
        // if the connection is lost, the RoboRemo will not receive the alive signal anymore,
        // and the LED will turn off (because it has the "on timeout" set to 700 (ms) )
    }
    static unsigned long lastBatCheck = 0;
    if (millis() - lastBatCheck > 2000)
    {
        checkbattry();
        lastBatCheck = millis();
    }
}