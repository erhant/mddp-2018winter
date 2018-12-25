#define DELAY_MS 1000

// Bluetooth is connected to Serial1 of Arduino MEGA
char btChar;

/* BLUETOOTH Commands
 * Ileri gitme: ileri (^)
 * Saga donme: sag (>)
 * Sola donme: sol (<)
 * Geri gitme: geri (v)
 * Durma: dur (CARPI)
 * 
 * Ultrasound toggle: toggle ultrasound (DAIRE)
 * Line follow toggle: toggle line (UCGEN)
 * 
 * 
 */
// Setup
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
}

// Loop
void loop()
{
  if (Serial1.available()) {
    String serialChar = Serial1.readString();
    Serial.print("I GOT A STRING: ");
    Serial.println(serialChar);
    Serial1.print("I GOT A STRING: ");
    Serial1.println(serialChar);
  } else {
    Serial.println("Awaiting command.");
    Serial1.println("Awaiting command.");
  }
  delay(DELAY_MS);
}
