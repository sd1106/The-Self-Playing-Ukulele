# The-Self-Playing-Ukulele
An electromechanical ukulele that reads sheet music and plays it autonomously. Four stepper motors slide fretting carriages up and down the neck, four press servos fret the strings, and four strum servos pluck them. All electronics are coordinated by an ESP32-S3 microcontroller running firmware we wrote from scratch.

## How it works: 

PDF Sheet Music -> 
Optical Music Recognition (oemer) -> 
MusicXML mapped to music21 (Python) -> 
Note/Chord mapped to String + Fret mapping ->
Serial commands over USB -> 
ESP32-S3 Firmware ->
Stepper Motors (position)  +  Press Servos (fret)  +  Strum Servos (pluck)




## Mechanical System Description: 
Each of the four strings has its own independent carriage that rides along the neck on a timing belt driven by a 28BYJ-48 stepper motor. The carriage holds a press servo that swings up to 90 degrees to push the string down onto the fret, then returns to 0 degrees once the note has been strummed. A separate strum servo sits near the soundhole and alternates between two angles each time it fires. It tracks whether it's currently "in" or "out" and always moves to the opposite position, so it never tries to strum from the same direction twice.

Each string has its own lookup table of 14 timing values (milliseconds) representing how long the stepper needs to run to reach each fret from the home position (open/nut end). On startup, all carriages home to position zero. When a note is requested, the firmware:

1. Looks up the target fret's position in that string's table
2. Subtracts the current stored position to get the delta
3. Runs the motor forward (positive delta) or backward (negative delta) for that many milliseconds
4. Updates the stored current position

This means "string 3, fret 7" and "string 4, fret 7" will move different physical distances, because fret spacing and belt geometry differ per string, since each has its own calibrated table.




## Hardware Components:

- ESP32-S3-WROOM-1: Main microcontroller
- 28BYJ-48 stepper motor (x4): Move fretting carriage along the neck of the ukulele
- ULN2003 driver board (x4): Drive the steppers (4 GPIO pins each)
- 180° servo motor (x4): Strum strings (alternates direction each strike)
- 180° servo motor (x4): Press strings down onto frets (0°→90°→0°)
- 5V power supply: Powers steppers and servos

*Note: The ESP32-S3-WROOM-1 is used specifically. Pin assignments in the firmware reflect this. Pins 0, 3, 45, and 46 are avoided as they are strapping/reserved pins on this module.




## Software Components: 

The Arduino firmware: ukulele_riptide.ino 
The main firmware runs on the ESP32-S3 and handles all real-time motor control. 
Key behaviours:

- strumAll() — fires all four strum servos. Uses a global strumState boolean to alternate direction every call, so the servo always moves to the opposite of its current position
- pressG/C/E/A — each press servo goes to 60°–90° to fret, then returns to 0° after the chord is strummed
- Chord functions (Aminor(), G(), C()) — move the correct stepper carriages to position, set press angles, then call strummingRhythm()
- strummingRhythm() — plays the Riptide strumming pattern (down down, down-up-down with timed delays)
riptide() — sequences the three chords of Riptide (Am → G → C) and loops


Python Pipeline: ukulele_player.py
Sits on the host PC and handles the sheet music side:

1. Opens a PDF and rasterises it to a 300 DPI PNG
2. Runs oemer (optical music recognition) to produce MusicXML
3. Parses the MusicXML with music21, extracting notes, chords, rests, and tempo
4. Maps each pitch to a (string, fret) pair using standard high-G ukulele tuning (G4 · C4 · E4 · A4), choosing whichever assignment minimises carriage travel
5. Voices chords across all four strings with a greedy least-travel algorithm
6. Sends MOVE, PLUCK, and HOME commands over USB serial in time with the score's tempo




## Libraries:

- ESP32Servo: Servo control via LEDC PWM on ESP32-S3 ✅ compatible
- Stepper (Arduino built-in): 28BYJ-48 stepper 
- controlmusic21 (Python): MusicXML parsing and score analysis
- pymupdf (Python): PDF rasterisation
- pyserial (Python): USB serial communication to ESP32




## Calibration:
Each string's fret timing table needs to be measured once on your physical build:

1. Upload firmware and open Serial Monitor
2. Send HOME — all carriages drive to the nut end limit switches
3. For each string, manually time (in milliseconds) how long the motor must run to reach each of the 14 frets from home
4. Update the lookup tables in the firmware constants at the top of ukulele_riptide.ino
5. Send DUMP over serial to verify stored positions, CAL to re-run calibration mode

Because fret spacing is not uniform (it compresses toward the body) and belt geometry differs per string, each of the four tables will have different values even for the same fret number.

### Built as a Computer Science capstone project, June 2026.
      
