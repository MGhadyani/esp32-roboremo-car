#ifndef ROBOREMO_H
#define ROBOREMO_H
#include <Arduino.h>

class roboremo
{
public:
    void begin();
    void slider();
    void button();
    void exeCmd();
    void handel();
    void checkbattry();
    void send(String lable, String value);
    void send(String lable, int value)
    {
        send(lable, String(value));
    }

    int sliderValue[10];
    int buttonValue[10];
    int batteryPercentage = 100;

private:
    char cmd[100]; // stores the command chars received from RoboRemo
    int cmdIndex;
    unsigned long lastCmdTime = 60000;
    unsigned long aliveSentTime = 0;

    boolean cmdStartsWith(const char *st)
    { // checks if cmd starts with st
        for (int i = 0;; i++)
        {
            if (st[i] == 0)
                return true;
            if (cmd[i] == 0)
                return false;
            if (cmd[i] != st[i])
                return false;
            ;
        }
        return false;
    }
};
#endif