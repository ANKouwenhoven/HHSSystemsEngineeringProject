#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

class IOController {
  private:
    bool sendRawInfo;
    bool sendInfoVisual;
    bool sendColorInfo;
    bool receiveCommands;
    bool runningMode;

  public:
    void init();  
    void printToSerial();
    void printAsVisual();
    void readAndProcessSerial();
}

#endif