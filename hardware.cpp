// This header has all the (extern) declarations of the globals.
// "extern" means "this is instantiated somewhere, but here's what the name
// means.
#include "globals.h"

#include "hardware.h"

// We need to actually instantiate all of the globals (i.e. declare them once
// without the extern keyword). That's what this file does!

// Hardware initialization: Instantiate all the things!
uLCD_4DGL uLCD(p9,p10,p11);             // LCD Screen (tx, rx, reset)
//SDFileSystem sd(p5, p6, p7, p8, "sd");  // SD Card(mosi, miso, sck, cs)
Nav_Switch myNav(p12, p15, p14, p16, p13); // initialize nav switch
DigitalIn button1(p21);                 // Pushbuttons (pin)
DigitalIn button2(p22);
DigitalIn button3(p23);
DigitalIn button4(p24);
SDBlockDevice sd_block(p5,p6,p7,p8);
FATFileSystem fs("fs");
PwmOut speaker(p25); 
AnalogOut DACout(p18);
wave_player waver(&DACout);



// Some hardware also needs to have functions called before it will set up
// properly. Do that here.
int hardware_init()
{   
    uLCD.baudrate(3000000);
    button1.mode(PullUp); 
    button2.mode(PullUp);
    button3.mode(PullUp);
    return ERROR_NONE;
}

// Implement this function.
GameInputs read_inputs() 
{
    //TODO: Complete this
    GameInputs in;
    in.fire = !button1.read();  // player fires missile
    in.pause = !button2.read(); // pauses the game
    in.start = !button3.read(); // saves the game
    in.control = !button4.read(); // shows the control menu

    in.up = myNav.up();
    in.down = myNav.down();
    in.left = myNav.left();
    in.right = myNav.right(); 
    return in;
}

void mount_sd() {
    sd_block.init();
    fs.mount(&sd_block);
}

void unmount_sd() {
    fs.unmount();
    sd_block.deinit();
}
