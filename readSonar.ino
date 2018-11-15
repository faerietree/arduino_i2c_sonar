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



int getDistance ()
{
  digitalWrite (SONAR_TRIGGER_PIN, LOW);
  delayMicroseconds (2);

  digitalWrite (SONAR_TRIGGER_PIN, HIGH);
  delayMicroseconds (10);
  digitalWrite (SONAR_TRIGGER_PIN, LOW);

  int duration = pulseIn (SONAR_ECHO_PIN, HIGH);
  // cm
  int distance = duration * 0.034 / 2;

  //Serial.println (distance);
/* working optical distance indicator
  int led_on_time = distance;
  digitalWrite(13, LOW);
  delay (led_on_time);

  digitalWrite(13, HIGH);
  delay(led_on_time);

  digitalWrite(13, LOW);
  delay(led_on_time);

  digitalWrite(13, HIGH);
  delay(led_on_time);

  digitalWrite(13, LOW);
 */
  return distance;
}
