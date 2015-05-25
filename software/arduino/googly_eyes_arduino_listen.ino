// include stepper library header file
#include <AccelStepper.h>

// motor speed in steps per second
#define motorSpeed 3200

// motor acceleration in steps per second per second
#define motorAccel 9600

// left eyeball step and direction pins
#define xstep 2
#define xdir  3

// right eyeball step and direction pins
#define ystep 4
#define ydir  5

// left eyeball accelstepper instance
AccelStepper stepper1 (1, xstep, xdir);

// right eyeball accelstepper instance
AccelStepper stepper2 (1, ystep, ydir);

// serial input buffer
#define CMD_MAXLEN 72
unsigned char cmd_buffer[CMD_MAXLEN];
unsigned char cmd_length = 0;
unsigned char cmd_state = 0;
unsigned char word_begin = 0;
unsigned char word_end = 0;
unsigned char word_length = 0;

void InitCommand (void);
void GetCommand (void);
void ProcessCommand (void);
unsigned char IsCommand (char *s1, unsigned char l1);
long atoi (void);
void MoveToCommand (void);


void setup()
{
    // set step and direction pins to outputs
    pinMode (xstep, OUTPUT);
    pinMode (xdir,  OUTPUT);
    pinMode (ystep, OUTPUT);
    pinMode (ydir,  OUTPUT);

    // left eyeball stepper setup
    stepper1.setMaxSpeed(motorSpeed);
    stepper1.setSpeed(motorSpeed);
    stepper1.setAcceleration(motorAccel);

    // right eyeball stepper setup
    stepper2.setMaxSpeed(motorSpeed);
    stepper2.setSpeed(motorSpeed);
    stepper2.setAcceleration(motorAccel);
    
    // serial port setup
    Serial.begin (19200);
    
    // command interpreter setup
    InitCommand ();
}

void loop()
{
    stepper1.run();
    stepper2.run();
    GetCommand ();
    if (cmd_state == 2) {
        ProcessCommand ();
    }
}

void InitCommand (void)
{
    cmd_length = 0;
    cmd_state = 0;
    word_begin = 0;
    word_end = 0;
    word_length = 0;
}

void GetCommand (void)
{
    uint8_t ch;

    if (cmd_state == 0) {
        cmd_length = 0;
        cmd_buffer[cmd_length] = 0;
        Serial.write ("EYES> ");
        cmd_state++;
    } else if (cmd_state == 1) {
        // if character is available, get it and try to add it to command buffer
        if (Serial.available() > 0) {
            // get character
            ch = Serial.read ();
            // process character
            if (ch == 0x0d) {                           // return
                // carriage return and linefeed
                Serial.write (0x0d);
                Serial.write (0x0a);
                cmd_state++;
            } else if (ch == 0x08) {                    // backspace
                if (cmd_length > 0) {
                    Serial.write (0x08);
                    Serial.write (' ');
                    Serial.write (0x08);
                    cmd_buffer[--cmd_length] = 0;
                }
            } else if (ch == 0x15) {                    // ctrl-u is rub out
                while (cmd_length > 0) {
                    Serial.write (0x08);
                    Serial.write (' ');
                    Serial.write (0x08);
                    cmd_buffer[--cmd_length] = 0;
                }
            } else if (ch >= 0x20 && ch <= 0x7e) {      // printable characters
                if (cmd_length < (CMD_MAXLEN - 1)) {
                    Serial.write (ch);
                    cmd_buffer[cmd_length++] = ch;
                    cmd_buffer[cmd_length] = 0;
                }
            }
        }
    } else if (cmd_state != 2) {
        // handle bad states
        cmd_state = 0;
    }
}

void ProcessCommand (void)
{
    // find beginning of first word
    word_begin = 0;
    while ((cmd_buffer[word_begin] == ' ') && (word_begin < cmd_length)) {
        word_begin++;
    }

    // find end of first word
    word_end = word_begin;
    while ((cmd_buffer[word_end] != ' ') && (word_end < cmd_length)) {
        word_end++;
    }

    // find length of first word
    word_length = word_end - word_begin;

    // IsCommand accesses the global variables word_begin and word_length
    if (IsCommand ("mt", 2)) {
        MoveToCommand ();
    }

    // reset command interpreter 
    cmd_state = 0;
}

unsigned char IsCommand (char *s1, unsigned char l1)
{
    unsigned char s2;
    unsigned char l2;

    s2 = word_begin;
    l2 = word_length;

    if (l1 != l2) {
        return 0;
    }

    while (l1--) {
        if (*s1++ != cmd_buffer[s2++]) {
            return 0;
        }
    }

    return 1;
}

long atoi (void)
{
    char sign = 1;
    long result = 0;
    unsigned char index = 0;

    // find beginning of first word
    word_begin = word_end;
    while ((cmd_buffer[word_begin] == ' ') && (word_begin < cmd_length)) {
        word_begin++;
    }

    // find end of first word
    word_end = word_begin;
    while ((cmd_buffer[word_end] != ' ') && (word_end < cmd_length)) {
        word_end++;
    }

    // find length of first word
    word_length = word_end - word_begin;

    for (index = word_begin; index < word_end; index++) {
        if (cmd_buffer[index] == '-') {
            sign = -1;
        } else {
            result = (int32_t)10 * result + (cmd_buffer[index] - '0');
        }
    }

    return (int)sign * result;
}

void MoveToCommand (void)
{
    int xpos, ypos;

    xpos = -atoi ();
    ypos = -atoi ();
    
    stepper1.moveTo (xpos);
    stepper2.moveTo (ypos);
}

