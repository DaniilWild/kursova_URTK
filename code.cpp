#include <DOS.H>
#include <conio.h>
#include <stdio.h>
#include <emurtk.h>
#include <time.h>
#include <stdlib.h>
 
unsigned char SNS0, SNS1, SNS2;    
int SN[18], SNB[18];              
int move=0, cycle=0;
int X = 0, Y = 0, Z = 0;
unsigned char inputD()      
{
    unsigned char data = 0;
    outportb(0x37A, 0x2E);
    outportb(0x378, 0xFF);
    outportb(0x37A, 0x2F);
    delay(1);
    data = inportb(0x378);
    outportb(0x37A, 0x04);
    return data;
}
void writeRC(char data)    
{
    outportb(0x37A, 0x04);
    outportb(0x378, data);
    outportb(0x37A, 0x00);
    delay(1);
    outportb(0x37A, 0x04);
}
void writeRA(char data)    
{
    outportb(0x37A, 0x0E);
    outportb(0x378, data);
    outportb(0x37A, 0x0A);
    delay(1);
    outportb(0x37A, 0x0E);
}
void Doit(char DRV)        
{
    writeRC(0x11);
    writeRA(0x80);
    writeRA(DRV);
}
void RS()              
{
    writeRC(0x13);
    SNS0 = inputD();
    SNS1 = inputD();
    SNS2 = inputD();
    int SNS[3] = { SNS0 ,SNS1 ,SNS2 };
    int u = 0, i, j;
    for (i = 0; i<3; i++) {
        for (j = 0; j<6; j++) {
            SN[u] = (SNS[i] + 1) % 2;
            SNS[i] /= 2;
            u++;
        }
    }
}
void Stop()    
{
    writeRC(0x11);
    writeRA(0x00);
    writeRA(0x00);
}
void Gif() {
    cycle++;
    if (cycle == 20){
        clrscr();
        printf("Calibration.");
    }if (cycle == 40) {
        clrscr();
        printf("Calibration..");
    }if (cycle == 60){
        clrscr();
        printf("Calibration...");
        cycle = 0;
    }
 
}
void V3(int v) {
    if (v == 1) {
        clrscr();
        printf("--------------");
    }if (v == 2) {
        clrscr();
        printf("--------------\n              -\n              -\n             \n              -\n              -\n              -\n              -");
}
i(v == 3) {
      clrscr();
        printf("--------------\n              -\n              -\n             \n              -\n              -\n              -\n              -\n--------------");
}if (v == 4) {
      clrscr();
       printf("--------------\n-             -\n-             -\n-             -\n-            -\n-             -\n-             -\n-             -\n--------------");
    }
}

void Cycle() {          
 
    for (int timer = 0; timer<18; timer++) {
        SNB[timer] = SN[timer];
    }
}
 
void RassedWay()                          
{
 
    clrscr();
    printf("The X-axis was %d tags\n", X);
    printf("The Y-axis was %d tags\n", Y);
    printf("The Z-axis was %d tags\n", Z);
}
 
 
void Conclusion() {
 
    RS();
    if (SN[2] != SNB[2]) { X++; }
    if (SN[5] != SNB[5]) { Y++; }
    if (SN[8] != SNB[8]) { Z++; }
}
void Calibration()
{
    int flag = 1 ;
   
    while (flag == 1) {
       
            Gif();
            Doit(0x15);
            RS(); 
if ((SN[0] == 1) && (SN[3] == 1) && (SN[6] == 1)) {
                flag = 0;
                break;
            }
        }
   
    flag = 1;
   
    while (flag == 1) {
 
 
 
            Gif();
            Cycle();
            Doit(0x2A);
            RS();
            Conclusion();
 
            
     if ((SN[1] == 1) && (SN[4] == 1) && (SN[7] == 1)) {
             flag = 0;
             break;
            }
            Cycle();
        }
 
    Stop();
}
void Work() {
     int WayX, WayY, WayZ;
 
    WayX = X / 2;
    WayY = Y / 2;
    WayZ = Z / 2;
 
    X = 0;
    Y = 0;
    Z = 0;
 
    move = 0x15;
 
    int flag = 1;
    int flagX = 0, flagZ = 0, flagY = 0;
    int i = 0; 
    
while (flag == 1) {
           
        if (WayX <= X && WayY <= Y && WayZ <= Z) {
            Stop();
            flag = 0;
        }
            Cycle();
            Conclusion();
            Doit(move);
 
            i++;
            if (i == 20) {
                i = 0;
                RassedWay();
            }
 
 
            if (WayX <= X && flagX ==0){
                flagX = 1;
                move -= 0x01;
            }if (WayY <= Y && flagY == 0) {
                flagY = 1;
                move -= 0x04;
            }if (WayZ <= Z && flagZ == 0){
                flagZ = 1;
                move -= 0x10;
        }
    }
    delay(1000);
    flag = 1;
    i = 0;
    flagX = 0;
    flagZ = 0;
    flagY = 0;
    move = 0x16;
 
    while (flag == 1) {
 
        Doit(move);
        RS();
 
        if(SN[1] == 1 && SN[3] == 1 && SN[6] == 1){
            V3(1);
            i=1;
            move = 0x01;
            delay(1000);
            Doit(move);
        }if (SN[0] == 1 && SN[3] == 1 && SN[6] == 1) {
            V3(2);
            move = 0x20;
            flagX = 1;
        }if (SN[0] == 1 && SN[3] == 1 && SN[7] == 1){
            V3(3);
            move = 0x02;
            flagZ = 1;
        }if (SN[1] == 1 && SN[3] == 1 && SN[7] == 1) {
            V3(4);
            move = 0x10;
            flagY = 1;
        }
        if (i == 1 && flagY == 1 && flagX == 1 && flagZ == 1){
     Doit(move);
            delay(3000);
            flag = 0;
        }
    }
}
int main()
{  
    Calibration();
    Work();
    return 0;
 }
