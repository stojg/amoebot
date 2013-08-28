#ifndef IR_H
#define IR_H


class IR
{
    public:
        /*
         * @brief Class constructor.
         */
        IR(int irpin) : irpin(irpin)
        {
            pinMode(irpin, OUTPUT);
            
        }
        
        float getDistance() {
          int value = analogRead(irpin);
          return 10650.08 * pow(value, -0.935) - 10;
        }
    private:
       int irpin;
};

#endif

