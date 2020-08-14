#include <Bounce2.h>

#define GPIO_SWITCH (2)
#define GPIO_LOAD (3)
#define GPIO_BUZZER (4)

#define INTERVAL_MS (10 * 1000)
#define BEEP_TIME_MS (2 * 1000)
#define BEEP_DURATION_MS (200)
#define BEEP_IN_PERCENT (20)

/* C (three octave) */
#define TONE_FREQ (523 * 3)

bool beeped;
bool waiting;
int gpio_switch_state;

uint32_t started;
Bounce debouncer = Bounce();

void
setup()
{
    pinMode(GPIO_LOAD, OUTPUT);
    pinMode(GPIO_SWITCH, INPUT_PULLUP);
    pinMode(GPIO_BUZZER, OUTPUT);
    digitalWrite(GPIO_LOAD, HIGH);

    debouncer.attach(GPIO_SWITCH);
    debouncer.interval(5);
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
    debouncer.update();
    if (debouncer.fell()) {
        started = millis();
        beeped = false;
        waiting = true;
        digitalWrite(GPIO_LOAD, HIGH);
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
