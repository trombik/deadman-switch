/*
 * Deadman switch. the code works with ATMega328p, or arduino board.
 *
 * When powered on, the microcontroller turns the load on, and a timer starts.
 *
 * When the defined interval * 0.2 passes, it beeps twice.
 *
 * When the defined interval passes, it turns off the load.
 *
 * When GPIO_ON_BUTTON is pressed, the timer resets, effectively extend the
 * interval.
 *
 * When GPIO_OFF_BUTTON is pressed, it turns the load off.
 *
 */

/* ATtiny13
 *
 * MicroCore (https://github.com/MCUdude/MicroCore) does not work well with
 * tone() and millis() because the chip has just a single timer. ATtiny85 with
 * ATTinyCore should work.
 *
 * 2 -> PB2 (pin 7)
 * 3 -> PB3 (pin 2)
 * 4 -> PB4 (pin 3)
 * 5 -> PB5 (pin 1)
 *
 * GND (pin 4)
 * VCC (pin 8)
 */

#define GPIO_ON_BUTTON (2)  // A tact switch to turn on (GND)
#define GPIO_LOAD (3)       // Load (max 40mA)
#define GPIO_BUZZER (4)     // Piezo speaker (+)
#define GPIO_OFF_BUTTON (5) // A tact switch to turn off (GND)

#define INTERVAL_MS (10 * 1000)
#define BEEP_TIME_MS (2 * 1000)
#define BEEP_DURATION_MS (200)
#define BEEP_IN_PERCENT (20)

#define TONE_FREQ (523 * 7)

bool beeped;
bool waiting;
int gpio_switch_state;
uint32_t started;

void
setup()
{
    pinMode(GPIO_LOAD, OUTPUT);
    digitalWrite(GPIO_LOAD, HIGH);

    pinMode(GPIO_BUZZER, OUTPUT);
    pinMode(GPIO_ON_BUTTON, INPUT_PULLUP);
    pinMode(GPIO_OFF_BUTTON, INPUT_PULLUP);
    started = millis();
    beeped = false;
    waiting = true;
}

void
beep()
{
    tone(GPIO_BUZZER, TONE_FREQ, BEEP_DURATION_MS);
}

void
beep_longer()
{
    tone(GPIO_BUZZER, TONE_FREQ, BEEP_DURATION_MS * 3);
    delay(BEEP_DURATION_MS * 3);
}

void
beep_twice()
{
    beep();
    delay(BEEP_DURATION_MS + BEEP_DURATION_MS / 2);
    beep();
    beeped = true;
}

bool
time_to_shutoff()
{
    return millis() - started >= INTERVAL_MS;
}

bool
time_to_beep()
{
    return millis() - started >= INTERVAL_MS - (INTERVAL_MS / 100) * BEEP_IN_PERCENT;
}

void
loop()
{
    if (digitalRead(GPIO_ON_BUTTON) == LOW) {
        started = millis();
        beeped = false;
        waiting = true;
        digitalWrite(GPIO_LOAD, HIGH);
    }
    if (waiting && digitalRead(GPIO_OFF_BUTTON) == LOW) {
        waiting = false;
        digitalWrite(GPIO_LOAD, LOW);
        beep_longer();
    }
    if (!waiting) {
        return;
    }
    if (time_to_shutoff()) {
        waiting = false;
        beep_longer();
        digitalWrite(GPIO_LOAD, LOW);
    }
    if (time_to_beep() && !beeped) {
        beep_twice();
    }
}
