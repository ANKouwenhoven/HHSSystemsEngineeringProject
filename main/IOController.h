#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <Arduino.h>
#include "ZumoController.h"

class IOController {
  private:
    bool sendRawInfo;
    bool sendInfoVisual;
    bool sendColorInfo;
    bool receiveCommands;
    bool runningMode;

  public:
    IOController();
    IOController(bool, bool, bool, bool, bool);
    ~IOController();
    void init();  
    void printToSerial(int[]);
    void printAsVisual(int[], int, int, int, int);
    void printPerceivedColors(int[], int, int, int, int);
    void readAndProcessInput();
    void printDebugMessage(string);
}

#endif