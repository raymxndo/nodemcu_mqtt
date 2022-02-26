#include <ESP8266WiFi.h> 
#include <PubSubClient.h>                                              
#include <SoftwareSerial.h>

#include <Wire.h>
#include <DS3231.h>
 
#define WIFI_SSID "DE LA VICTORIA NETWORK-2.4GHZ"                                          
#define WIFI_PASSWORD "Isuzutrooper_grv208" 

const char* mqtt_server = "test.mosquitto.org";

SoftwareSerial mySerial (D6, D5); //RX, TX pins

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient espclient(espClient);

DS3231 clock1;
RTCDateTime dt;

int period = 500;
unsigned long time_now = 0;

String timeStatus;

int ultrasonicResult;
String ultrasonicRead;
int ultrasonicInt  = 0;
int percentage = 0;

int ultrasonicResultY;
String ultrasonicReadY;
int ultrasonicIntY = 0; 
int percentageY  = 0;

// This functions is executed when some device publishes a message to a topic that your NodeMCU is subscribed to

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageInfo;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageInfo += (char)message[i];
  }
  Serial.println();

  // If a message is received on the topic cage_1/feed_1, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if(topic=="cage_1/feed_1"){
    if(messageInfo == "ON0.5 KG"){
        feed_1_on_0p5kg ();
        Serial.print("Feed 1 On");
      }
  }
  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!espclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (espclient.connect("ESP8266Client2")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      espclient.subscribe("cage_1/feed_1");
      espclient.subscribe("cage_1/weight_1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(espclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// feed 1 voids
void feed_1_on_0p5kg () {
  mySerial.write(111); // Write integer 111 to PIC
  if(millis() >= time_now + period){
    time_now += period;
  espclient.publish("cage_1/feed_1", "OFF"); 
  }
}
/*
void feed_1_on_1kg () {
  mySerial.write(112); // Write integer 112 to PIC
  if(millis() >= time_now + period){
    time_now += period;
    Firebase.setBool(firebaseData, "cage_1/feed_1", false); // OFF the feed based on the delay
  }
}

void feed_1_on_1p5kg () {
  mySerial.write(113); // Write integer 113 to PIC
  if(millis() >= time_now + period){
    time_now += period;
    Firebase.setBool(firebaseData, "cage_1/feed_1", false); // OFF the feed based on the delay
  }
}

//feed 2 voids

void feed_2_on_0p5kg () {
  mySerial.write(211); // Write integer 211 to PIC
  if(millis() >= time_now + period){
    time_now += period;
    Firebase.setBool(firebaseData, "cage_2/feed_2", false); // OFF the feed based on the delay
  }
}

void feed_2_on_1kg () {
  mySerial.write(212); // Write integer 212 to PIC
  if(millis() >= time_now + period){
    time_now += period;
    Firebase.setBool(firebaseData, "cage_2/feed_2", false); // OFF the feed based on the delay
  }
}

void feed_2_on_1p5kg () {
  mySerial.write(213); // Write integer 213 to PIC
  if(millis() >= time_now + period){
    time_now += period;
    Firebase.setBool(firebaseData, "cage_2/feed_2", false); // OFF the feed based on the delay
  }
}*/
                                                              
void setup() 
{
  Serial.begin(115200);
  mySerial.begin(9600);
  
  delay(1000);                   
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                               
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  
  // Initialize DS3231
  Serial.println("Initialize DS3231");;
  clock1.begin();

  // Set sketch compiling time
  clock1.setDateTime(__DATE__, __TIME__);

  pinMode(LED_BUILTIN, OUTPUT);
  espclient.setServer(mqtt_server, 1883);
  espclient.setCallback(callback);

}
 
void loop() 
{

/*  //RTC codes

 dt = clock1.getDateTime();

 timeStatus = clock1.dateFormat("g:i A", dt);*/

  //Serial.println(timeStatus);

 if (!espclient.connected()) {
    reconnect();
  }
  if(!espclient.loop())
    espclient.connect("ESP8266Client2");
    
}
