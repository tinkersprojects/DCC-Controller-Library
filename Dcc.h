#ifndef Dcc_h
#define Dcc_h

#ifndef DCC_Buffer_size
  #define DCC_Buffer_size 20
#endif


typedef void (*CallbackFunction) (uint8_t pinState);

class Dcc
{
  public:
    Dcc(void (*CallBack)(uint8_t pinState));
    Dcc(uint8_t _OutputA, uint8_t _OutputB);
    void begin();

    size_t set(uint8_t address, uint8_t Direction, uint8_t speed);
    size_t set(uint8_t address, uint8_t data);
    void Send();
    void SendAll();
    void flush();
    
  private:

    uint8_t OutputA;
    uint8_t OutputB;
    CallbackFunction runCallback = NULL;

    #if defined(ESP8266) || defined(ESP32) || defined(ARDUINO_ESP8266_NODEMCU) 
      uint32_t OutputABitMask;
      uint32_t OutputBBitMask;
      volatile uint32_t * OutputAPortRegister;
      volatile uint32_t * OutputBPortRegister;
    #else
      volatile uint8_t * OutputAPortRegister;
      volatile uint8_t * OutputBPortRegister;
      uint8_t OutputABitMask;
      uint8_t OutputBBitMask;
    #endif

    struct OutputBuffer {
        uint8_t address;
        uint8_t data;
        uint8_t send;
    } OutputBufferArray[DCC_Buffer_size];

    void writeOutput(uint8_t pinState);
    void writeBit(uint8_t bit);
    void output(uint8_t address, uint8_t data);
};


#endif
