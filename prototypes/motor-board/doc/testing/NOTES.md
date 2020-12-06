# Initial tests driving motor prototype board from AD2

Setup:

 - Connect 5V power supply to motor board [DONE]

 - Connect 3.3V supply from AD2 to motor board [DONE]

 - Configure two AD2 digital outputs for `MOTOR_IN1` and `MOTOR_IN2` [DONE]

 - Configure AD2 digital input for `PULSE` [DONE]

 - Configure AD2 analog input for `MOTOR_SENSE` [DONE]

Protocol:
  1. Remove 5V power from motor board
  2. Disconnect `MOTOR_IN1` and `MOTOR_IN2` from motor board
  3. Configure AD2 digital outputs in "Patterns" panel and check
  4. Connect `MOTOR_IN1` and `MOTOR_IN2` to motor board
  5. Apply 5V power to motor board
  6. Run AD2 digital output waveform generator
  7. Record results: motor motion, `PULSE` and `MOTOR_SENSE` inputs

Experiments:

    STATE          MOTOR_IN1      MOTOR_IN2

    Motor halted   HIGH           HIGH
    Forward 10%    HIGH           PWM: 10% LOW
    Forward 50%    HIGH           PWM: 50% LOW
    Forward 100%   HIGH           LOW
    Backward 10%   PWM: 10% LOW   HIGH
    Backward 50%   PWM: 50% LOW   HIGH
    Backward 100%  LOW            HIGH

=> ALL WORKS! DIDN'T BOTHER WITH PROTOCOL ABOVE, JUST SWITCHING PWM
   FREQUENCIES IN PATTERNS OUTPUT. PERFECT RESULTS, AS FAR AS I CAN
   TELL. ONLY THING THAT WILL NEED A LITTLE WORK IS SMOOTHING OF PULSE
   TIMES FROM ENCODER, BECAUSE THE ENCODER DISK ISN'T QUITE STRAIGHT.
   BUT: PWM SPEED CONTROL WORKS, FORWARDS AND BACKWARDS; PULSE OUTPUT
   FROM ENCODER WORKS; TORQUE MEASUREMENT WORKS!
