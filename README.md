# `deadman-switch`

A deadman's switch implemented on AVR.

`deadman` is chosen in the project name because it sounds odd, and makes
people curious about why it is called so when teaching. I do understand
opinions against the usage of the term ([Why you should stop using the term ‘
Deadman](https://machinerysafety101.com/2011/03/28/stop-using-the-term-deadman/)),
and the term should not be used in official documents.

## Schematic

![Schematic](kicad/deadman-switch.svg)

## Notes

`D1` and `R1` draw 10mA. The load should not draw more than 30mA from the GPIO
(the max current is 40mA).
