const uint8_t IGNITION_START_PIN  = 1; // Digital pin connected to the first relay on the ignition
const uint8_t IGNITION_RUN_PIN    = 2; // Digital pin connected to the first relay on the ignition
const uint8_t CAR_STOPPED         = 0;
const uint8_t CAR_STARTED         = 1;

class IgnitionController
{
  public:
    IgnitionController(uint8_t debug);

    void start();
    void stop();
    void loop(uint8_t rate);

    uint16_t  getCurrentStatus();

  private:
    void      _checkStarterMotorStatus();

    uint8_t   debug;
    uint8_t   current_status; // CAR_STOPPED vs CAR_STARTED
    
    const char* CLASS_NAME = "IgnitionController";
};

// Initialise the IgnitionController
// pass true for debug to get Serial replies
IgnitionController::IgnitionController(uint8_t debug)
{
  this->debug = debug;

  if (debug) {
    Serial.print(CLASS_NAME);
    Serial.println(": initialised");
  }

  pinMode(IGNITION_START_PIN, OUTPUT);
  pinMode(IGNITION_RUN_PIN, OUTPUT);

  digitalWrite(IGNITION_START_PIN, LOW);
  digitalWrite(IGNITION_RUN_PIN, LOW);
}

// Return whether car has started
uint16_t IgnitionController::getCurrentStatus()
{
  return current_status;
}

void IgnitionController::_checkStarterMotorStatus()
{
  // insert code to check that the starter moter is running and update current_status
  if(true) { 
    current_status = CAR_STARTED;
  } else {
    current_status = CAR_STOPPED;
  }
}

// Turn relays IGNITION_START_PIN then IGNITION_RUN_PIN
void IgnitionController::start()
{
  if (current_status != CAR_STARTED) {
    digitalWrite(IGNITION_START_PIN, HIGH);
    digitalWrite(IGNITION_RUN_PIN, HIGH);
  }
}

// loop is expected to be called from the main loop with a value passed for how frequently it must execute in the timer wheel
void IgnitionController::loop(uint8_t rate)
{
  if (millis() % rate == 0) {
    // Execute code
    if (current_status == CAR_STARTED) {
      digitalWrite(IGNITION_START_PIN, LOW);
      digitalWrite(IGNITION_RUN_PIN, LOW);
    }
  }
}