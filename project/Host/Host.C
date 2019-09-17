#include <reg51.h>
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;
uchar table[] =
    {
        0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0xEE, 0x3E, 0x9C, 0x7A, 0x9E, 0x8E};
uchar address[] =
    {
        0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

sbit P26 = P2 ^ 6;
sbit P27 = P2 ^ 7;
sbit P22 = P2 ^ 2;
sbit P23 = P2 ^ 3;

uchar run = 0;
uchar temp = 0;
uchar num_cj = 0;
uchar temp_data = 0;
uchar CongJi0 = 0x20, CongJi1 = 0x30;
uchar CongJi = 0x20;
uchar count = 0;

void Delay(uint m);
void Display();
void init();
void check_cj(uchar num_cj);
void addr_send(uchar CongJi);
void data_rec();
void DA();

void ex0_int() interrupt 0
{
    run = !run;
    TR0 = !TR0;
}

void time0_int() interrupt 1
{
    TH0 = 0xd8;
    TL0 = 0xf0;
    count++;
    if (count == 100)
    {
        count = 0;
        num_cj = !num_cj;
    }
}

void main()
{
    init();

    while (1)
    {
        if (run)
        {
            if (num_cj == 0)
                CongJi = CongJi0;
            else
                CongJi = CongJi1;

            addr_send(CongJi);
            // TB8 = 1;
            // SBUF = CongJi;
            // while (TI == 0)
            //     ;
            // TI = 0;

            data_rec();
            // while (RI == 0)
            //     ;
            // RI = 0;
            // temp = SBUF;
        }

        Display();
        DA();
    }
}
void Delay(uchar m)
{
    unsigned char a, b;
    for (; m > 0; m--)
        for (b = 142; b > 0; b--)
            for (a = 2; a > 0; a--)
                ;
}

void addr_send(uchar CongJi)
{
    TB8 = 1;
    SBUF = CongJi;
    while (TI == 0)
        ;
    TI = 0;
}

void data_rec()
{
    while (RI == 0)
        ;
    RI = 0;
    temp = SBUF;
}
void init()
{
    SCON = 0xD0;
    TMOD = 0x21;

    TH0 = 0xd8;
    TL0 = 0xf0;

    TH1 = 0xf3;
    TL1 = 0xf3;

    EA = 1;
    EX0 = 1;
    ET0 = 1;
    IT0 = 1;

    TR0 = 0;
    TR1 = 1;

    P22= 0;

    count = 0;
    num_cj = 0;
}
void Display()
{

    P0 = 0x7f;
    P27 = 1;
    P27 = 0;
    P0 = table[num_cj];
    P26 = 1;
    // Delay(10);
    P26 = 0;
    Delay(1);

    temp_data = temp % 10;
    P0 = 0xfe;
    P27 = 1;
    P27 = 0;
    P0 = table[temp_data];
    P26 = 1;
    // Delay(10);
    P26 = 0;
    Delay(1);

    temp_data = temp / 10 % 10;

    P0 = 0xfd;
    P27 = 1;
    P27 = 0;
    P0 = table[temp_data];
    P26 = 1;
    // Delay(10);
    P26 = 0;
    Delay(1);

    temp_data = temp / 100 % 10;

    P0 = 0xfb;
    P27 = 1;
    P27 = 0;
    P0 = table[temp_data];
    P26 = 1;
    // Delay(10);
    P26 = 0;
    Delay(1);
}

void DA()
{

    P1 = temp / 2;
    P23 = 1;
    P23 = 0;
}