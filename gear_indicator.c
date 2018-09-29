#define RESET_BIT PORTB.b0
#define CLOCK_BIT PORTB.b1
#define NEUTRAL_BIT PORTB.b2
#define UP_BIT PORTB.b3
#define DOWN_BIT PORTB.b4
#define RESET_DDR TRISB.b0
#define CLOCK_DDR TRISB.b1
#define DEBOUNCE_TIME 60
#define LOCK_TIME 250
#define TOP_GEAR 5

int Gear;

void ShowDigit(int Digit)
{
    int i;

        PORTB |= (1 << RESET_BIT);       // RESET pin high
        PORTB &= ~(1 << RESET_BIT);      // RESET pin low
    for (i=0; i<Digit; i++)
    {
            PORTB |= (1 << CLOCK_BIT);   // CLOCK pin high
        PORTB &= ~(1 << CLOCK_BIT);      // CLOCK pin low
    }
}

void Init()
{
    int i;

    PORTB |= 1 << NEUTRAL_BIT;             // NEUTRAL pin enable pull-up
        PORTB |= 1 << UP_BIT;              // UP pin enable pull-up
        PORTB |= 1 << DOWN_BIT;            // DOWN pin enable pull-up

    TRISB |= 1 << RESET_DDR;               // RESET pin as output
        TRISB |= 1 << CLOCK_DDR;          // CLOCK pin as output

        PORTB &= ~(1 << RESET_BIT);       // RESET pin low

                                          // Self test effect

        ShowDigit(0);
        Delay_ms(500);

        for (i=1; i<10; i++)
        {
            ShowDigit(i);
                Delay_ms(100);
        }

        ShowDigit(0);
        Delay_ms(500);

    Gear=EEPROM_Read((int*)0);  // Read initial value from EEPROM
        ShowDigit(Gear);                       // Show initial value
}

int Released(int Bit)
{
   // if (bit_is_clear(PORTB,Bit))
    if (!(PORTB,Bit))

        {
          Delay_ms(DEBOUNCE_TIME);
          //  if (bit_is_set(PORTB,Bit))
            if (PORTB,Bit)

            {
            return 1;
            }
        }
    return 0;
}

int main(void)
{
    Init();

        while(1)
    {
                if (Released(UP_BIT))
        {
                    if ((Gear > 0) && (Gear < TOP_GEAR)) Gear++;
                     if (Gear == 0) Gear = 2;
                       // if (bit_is_clear(PORTB,NEUTRAL_BIT)) Gear = 0;
                          if (!(PORTB,NEUTRAL_BIT)) Gear = 0;

                        EEPROM_Write((int*)0,Gear);
                        ShowDigit(Gear);
                        Delay_ms(LOCK_TIME);
                }

                if (Released(DOWN_BIT))
                {
                 if (Gear > 1) Gear--;
                        if (Gear == 0)
                        {
                         Gear = 1;
                        }
                        
                       // if (bit_is_clear(PORTB,NEUTRAL_BIT))
                        if (!(PORTB,NEUTRAL_BIT))
                        {
                        Gear = 0;
                        }
                        
                        EEPROM_Write((int*)0,Gear);
                        ShowDigit(Gear);
                        Delay_ms(LOCK_TIME);
                }
        }
}