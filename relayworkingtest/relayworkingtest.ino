// Define the GPIO pin connected to the relay
const int relayPin = 2; // Change this to match the GPIO pin you've connected the relay to

void setup() {
  // Initialize the GPIO pin as an output
  pinMode(relayPin, OUTPUT);
}

void loop() {
  // Turn the relay on (close the circuit)
  digitalWrite(relayPin, HIGH);
  delay(1000); // Wait for 1 second

  // Turn the relay off (open the circuit)
  digitalWrite(relayPin, LOW);
  delay(1000); // Wait for 1 second
}
