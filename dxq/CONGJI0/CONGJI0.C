#include <reg51.h>
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

sbit start = P2 ^ 3;
sbit oe = P2 ^ 4;
sbit eoc = P2 ^ 5;
uchar temp = 0, rec_addr, rec_data, getdata;
uchar ADDR = 0x20;
void init();
void capture_num();
void Delay(uint m);
void fs();
void send(uchar c);

void main()
{
    init();

    while (1)
    {
        while (RI == 0)
            ;
        RI = 0;
        rec_addr = SBUF;
        if (rec_addr == ADDR)
        {
            SM2 = 0;
            // while (RI == 0)
            //     ;
            // RI = 0;
            // rec_data = SBUF;
            // if (rec_data == 0xFF)
            // {
            capture_num();
            send(temp);
            // }
        }
    }
}
void Delay(uint m)
{
    while (m--)
        ;
}
void init()
{
    SCON = 0xF0;
    TMOD = 0x20;

    TH1 = 0xf3;
    TL1 = 0xf3;
    TR1 = 1;

    start = 0;
    oe = 0;
}

void capture_num()
{
        start = 1;
        start = 0;
        //if (eoc == 1)
        // / {
        oe = 1;
        temp = P0;
        P1 = temp;
        // oe = 0;
        // / }
}

void send(uchar c)
{
    TB8 = 0;
    SBUF = c;
    while (TI == 0)
        ;
    TI = 0;
}
