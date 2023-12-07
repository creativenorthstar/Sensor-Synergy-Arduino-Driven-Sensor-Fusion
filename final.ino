  #include <Wire.h>
  #include <Adafruit_BMP085.h>
  Adafruit_BMP085 bmp;

  #include <OneWire.h>
  #include <DallasTemperature.h>
  #define ONE_WIRE_BUS 6 // Data pin for DS18B20 sensors
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);

  #define echoPin 3  // Echo pin of the ultrasonic sensor
  #define trigPin 2    // Trigger pin of the ultrasonic sensor
  int IRSensor = 5; // Connect IR sensor module to Arduino pin 9
  int buzzer = 4; // Connect the buzzer to Arduino pin 10
  const int sensor_pin = A3;	/* Soil moisture sensor O/P pin */

  const int vibrationPin = 7; // Connect the SW-420 signal pin to Arduino digital pin 7

  void setup() {
    Serial.begin(9600);
    sensors.begin();
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(IRSensor, INPUT); // Set the IR Sensor pin as INPUT
    pinMode(buzzer, OUTPUT); // Set the buzzer pin as OUTPUT
    
    pinMode(vibrationPin, INPUT);
    if (!bmp.begin()) {
      Serial.println("Could not find a valid BMP180 sensor, check wiring!");
      while (1);
    }

  }

  void loop() {
    long duration, distance;
    int sensorStatus = digitalRead(IRSensor); // Read the IR sensor status
    float moisture_percentage;
    int sensor_analog;
    sensors.requestTemperatures();

    int vibrationStatus = digitalRead(vibrationPin);


    // Trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    


    // Ultrasonic sensor
    
    // Read the time taken for the ultrasonic wave to return
    duration = pulseIn(echoPin, HIGH);

    // Calculate the distance in centimeters
    distance = duration * 0.034 / 2;


    // Dallas Temperature sensors

    // Print all temperature data in a single line
    Serial.println("Temperatures from top to bottom: ");
    for (int i = 0; i < sensors.getDeviceCount(); i++) {
      Serial.print("Sensor ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(sensors.getTempCByIndex(i));
      Serial.print("°C  ");
      // Serial.print(sensors.getTempFByIndex(i));
      // Serial.print("°F");

      if (i < sensors.getDeviceCount() - 1) {
        Serial.print(" | ");
      }
    }
    Serial.println();





    // Print the distance to the Serial Monitor
    Serial.print("\nDistance from object: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (sensorStatus == HIGH) {
      //Serial.println("_________________________");
      Serial.println("No Object detected!"); // Print message if the sensor reads an object
      Serial.println("You are safe!");
      
      delay(100); 
      digitalWrite(buzzer, LOW); // Turn off the buzzer when no object is detected
    } else {
      //Serial.println("_________________________");
      Serial.println("Alert!");
      Serial.println("Object detected"); // Print message if an object is detected
      Serial.println("Move Faster!!");
      
      //delay(2000); 
      digitalWrite(buzzer, HIGH); // Turn on the buzzer when an object is detected
      delay(900); // Adjust the delay to control the buzzer sound duration
      digitalWrite(buzzer, LOW); // Turn off the buzzer
    }


    // Moisture sensor

    sensor_analog = analogRead(sensor_pin);
    moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
    
    if (moisture_percentage < 15)
    {
      Serial.println("\nLow Moisture ");
    }
    else
    {
      Serial.println("\nHigh Moisture");

    }
    Serial.print("Moisture Percentage = ");
    Serial.println(moisture_percentage);
    // Serial.print("%\n\n");
    // Serial.print("---------------------------------------------\n");
    
    

    // BMP 180

    // Read temperature and pressure
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure();

    // Calculate altitude based on pressure at sea level (101325 Pa)
    float altitude = bmp.readAltitude(101325);

    // Print the results
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Pressure: ");
    Serial.print(pressure / 100.0);  // Convert pressure to hPa
    Serial.println(" hPa");

    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println(" meters");
    

    

    // Vibratiobal Sensor
    // Check the status of vibrations
    if (vibrationStatus == HIGH) {
      Serial.println("\nVibration detected!");
    } else {
      Serial.println("\nNo vibration.");
    }

    // Add a brief delay to prevent continuous readings
    // delay(100);

    // Wait until the vibration level goes below the threshold
    while (vibrationStatus == HIGH) {
     vibrationStatus = digitalRead(vibrationPin);
    // delay(100);
    }

    

    Serial.println("\================================================================================================\n");
    delay(2000);  // Add a delay to make the readings more readable
  }
