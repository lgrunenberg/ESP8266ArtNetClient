#include <ESP8266WiFi.h>
#include <WiFiUDP.h>


#define DEBUG

#define RED 4
#define GREEN 5
#define BLUE 10


// wifi connection variables
const char* ssid = "MYSSID";
const char* password = "MYPASSWORD";
boolean wifiConnected = false;

// UDP variables
unsigned int localPort = 6454;
WiFiUDP UDP;
boolean udpConnected = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged"; // a string to send back

void setColor(uint8_t red, uint8_t green, uint8_t blue) {
  uint16_t actualRed = map(red, 0, 255, 0, 1023);
  uint16_t actualGreen = map(green, 0, 255, 0, 1023);
  uint16_t actualBlue = map(blue, 0, 255, 0, 1023);
  analogWrite(RED, actualRed);
  analogWrite(GREEN, actualGreen);
  analogWrite(BLUE, actualBlue);
}

void initLED() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void setup() {
  // Initialise Serial connection
  Serial.begin(115200);
  initLED();

  // Initialise wifi connection
  wifiConnected = connectWifi();

  // only proceed if wifi connection successful
  if (wifiConnected) {
    udpConnected = connectUDP();
    if (udpConnected) {
      // initialise pins

    }
  }
}

void decodePackage(int packetSize) {
  if (packetSize >= 7) {
    if (strncmp(packetBuffer, "Art-Net", 7) == 0) {
      Serial.println("Artnet packet received");
      uint16_t numChannels = 0;
      numChannels |= packetBuffer[16];
      numChannels <<= 8;
      numChannels |= packetBuffer[17];
      Serial.print("Number of channels:");
      Serial.println(numChannels);
      uint8_t led = 0;
      if (numChannels >= 3) {
        setColor(packetBuffer[18], packetBuffer[19], packetBuffer[20]);

      }
    }
  }
}

void loop() {
  // check if the WiFi and UDP connections were successful
  if (wifiConnected) {
    if (udpConnected) {

      // if there’s data available, read a packet
      int packetSize = UDP.parsePacket();
      if (packetSize)
      {

#ifdef DEBUG
        Serial.println("");
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
#endif
        // read the packet into packetBufffer
        UDP.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
#ifdef DEBUG
        for (int i = 0; i < UDP_TX_PACKET_MAX_SIZE; i++) {
          Serial.write(packetBuffer[i]);
        }
#endif

        decodePackage(packetSize);


        // send a reply, to the IP address and port that sent us the packet we received
        /*UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
        UDP.write(ReplyBuffer);
        UDP.endPacket();*/

        // turn LED on or off depending on value recieved
        // digitalWrite(5, value);
      }
      delay(10);

    }

  }

}

// connect to UDP – returns true if successful or false if not
boolean connectUDP() {
  boolean state = false;

  Serial.println("");
  Serial.println("Connecting to UDP");

  if (UDP.begin(localPort) == 1) {
    Serial.println("Connection successful");
    state = true;
  }
  else {
    Serial.println("Connection failed");
  }

  return state;
}
// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}
