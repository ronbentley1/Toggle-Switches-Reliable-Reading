// Ron D Bentley, Stafford, UK
// Jan 2021
//
//  This example and code is in the public domain and may be used without restriction and
//  without warranty.
/*
  Reading a simple toggle switch, with debounce
  '''''''''''''''''''''''''''''''''''''''''''''
  This sketch presents a method for reading a simple toggle switch 'cleanly' such
  that any electrical noise created during switch transition is removed.  Without
  this, then simply relying on digitalRead() could provide flaky results.
  
  The sketch has been designed to support two commonly wired switch circuits, as below, but note:

    1.  The digital pin chosen as the input is initialised according to the 'circuit_type' macro
        parameter, either INPUT_PULLUP or INPUT using a call to pinMode(button_switch,circuit_type)
        in the setup() function.
        
        As the conditions for detecting switch on/off are different for each 'circuit_type' (they are
        reversed) this is automatically accommodated for in the reading of the toggle switch.
        The only requirement is to define the 'circuit_type', everything else is
        automatically taken care of.

    2.  The wiring designs for each type of switch circuit are:

        circuit_C1 - With switch pull down resister configured and 'circuit_type' of INPUT

               10K ohm     button      (onboard)
              resister    switch         LED
          ____|<><><>|_______ \___        O
         |              |         |       |
         0v         pin = 2      +5v    pin =
                                    LED_BUILTIN
         ___________________________________
         |            ARDUINO              |

        circuit_C2 - With NO switch pull down resister configured and 'circuit_type' of INPUT_PULLUP

             button     (onboard)
             switch        LED
            ___ \___        O
           |        |       |
         pin 2     0v     pin =
                       LED_BUILTIN
         _______________________
         |       ARDUINO       |
 */
 
#define circuit_C1    INPUT         // switch configured with a 10k ohm pull down resistor
#define circuit_C2    INPUT_PULLUP  // switch simply connected to ground, no resistor
#define circuit_type  circuit_C2    // defines which of the two common circuits types is configured

bool    current_toggle_status =           LOW; // start assuming switch switch is off
bool    previous_toggle_switch_status =  HIGH; // ditto

int     toggle_switch_pin =   2;
#define debounce              10 // debounce period in milli seconds

#define LED     LED_BUILTIN      // normally pin 13, but this ensures it is set correctly by the compiler

//
// test the toggle switch to see if its status has changed since last look.
// note that, although, switch status is a returned value from the function,
// the current status of the switch 'current_toggle_status' is always maintained
// and can be tested outside of the function at any point/time.
//
bool read_toggle_switch() {
  static long int elapse_time = 0;
  static bool transition_started = false;
  int pin_value;
  pin_value = digitalRead(toggle_switch_pin);  // test current state of toggle pin
  if (circuit_type == circuit_C2) {
    // need to invert HIGH/LOW if circuit design sets pin HIGH representing switch in off state
    // ie inititialised as INPUT_PULLUP
    pin_value = !pin_value;
  }
  if (pin_value != current_toggle_status && !transition_started) {
    // switch change detected so start debounce cycle
    transition_started = true;
    elapse_time = millis();  // set start of debounce timer
  } else {
    if (transition_started) {
      // we are in the switch transition cycle so check if debounce period has elapsed
      if (millis() - elapse_time >= debounce) {
        // debounce period elapse sp assume switch has settled down after transition
        previous_toggle_switch_status = current_toggle_status;
        current_toggle_status = !current_toggle_status;  // flip status
        transition_started = false;  // cease transition cycle
      }
    }
  }
  return current_toggle_status;
} // end of read_toggle_switch

void setup() {
  pinMode(toggle_switch_pin, circuit_type);  // setup switch read pin in accordance with circuit design
  pinMode(LED, OUTPUT);                      // setup LED pin
  digitalWrite(LED, LOW);                    // ensure LED off
} // end of setup

void loop() {
  digitalWrite(LED_BUILTIN, read_toggle_switch());
}
