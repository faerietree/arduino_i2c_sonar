int ReadSonarPulse()
{
long val = 0;

  val = pulseIn(SONAR_PULSE_IN_PIN, HIGH,(US_PER_CM*MAX_RANGE)+1000);          // US_PER_CM*MAX_RANGE is calculated time out for sonar PWM data
  val = val / US_PER_CM;                                                       // Convert pulse width to cm
  return((int) val);
}

int ReadSonarAnalog()
{
  int val;
  val = analogRead(SONAR_ANALOG_IN_PIN);
  // ?? Convert val to cm here if required                  
  return(val);
}

