#include <WiFi.h>
#include <IRCClient.h>

#define IRC_SERVER "irc.chat.twitch.tv"
#define IRC_PORT 6667

char ssid[] = "trenz";// This is for your Wifi network and password (it worked better when I used my own hotspot)
char password[] = "12345678";

const String twitchChannelName = "vansirect";//Put your twitch channel name here 
#define TWITCH_BOT_NAME "Vansirectt"// Put your twitch bot here 
#define TWITCH_OAUTH_TOKEN "oauth:pfqlvr3hgyu20ibe6py4sugo0lqvew"//Follow this tutorial to get your token 
https://dev.twitch.tv/docs/authentication/getting-tokens-oauth/

String ircChannel = "";

WiFiClient wiFiClient;
IRCClient client(IRC_SERVER, IRC_PORT, wiFiClient);

int ENA = 12;  
int IN1 = 13;  
int IN2 = 15;  

int ENB = 4;   
int IN3 = 14;  
int IN4 = 2;   

String input = "";
int timestart = 0;

int TURN_DELAY = 375;
int MOVE_DELAY = 500;
int TALLY_DELAY = 3000;
int tallytimer = 0;

enum STATE { FORWARD, BACKWARD, LEFT, RIGHT, DANCE } state;
String statename[] = { "Forward", "Backward", "Left", "Right", "Dance" };

uint8_t countTally[] = { 0, 0, 0, 0, 0 };
uint8_t maxTally = countTally[0];

int ind = 0;
uint8_t n = sizeof(countTally);
bool tieVar = false;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  ircChannel = "#" + twitchChannelName;
  client.setCallback(callback);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  tallytimer = millis();
}

void loop() {
  if (!client.connected()) {
    Serial.println("Attempting to connect to " + ircChannel);
    if (client.connect(TWITCH_BOT_NAME, "", TWITCH_OAUTH_TOKEN)) {
      client.sendRaw("JOIN " + ircChannel);
      Serial.println("Connected and ready");
      sendTwitchMessage("Vansirect is ready!");
    } else {
      Serial.println("Failed, retrying in 5 seconds");
      delay(5000);
    }
    return;
  }
  sm_01();
  client.loop();
}

void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}

void callback(IRCMessage ircMessage) {
  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') {
    input = ircMessage.text;
    Serial.print("Received command: ");
    Serial.println(input);

    if (input.startsWith("forward")) {
      countTally[0]++;
      Serial.println("Forward command detected");
    } else if (input.startsWith("backward")) {
      countTally[1]++;
      Serial.println("Backward command detected");
    } else if (input.startsWith("turn left")) {
      countTally[2]++;
      Serial.println("Turn left command detected");
    } else if (input.startsWith("turn right")) {
      countTally[3]++;
      Serial.println("Turn right command detected");
    } else if (input.startsWith("dance")) {
      countTally[4]++;
      Serial.println("Dance command detected");
    }
  }
}

void stopper() {
  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(100);
}

void executeCommand(int duration) {
  Serial.print("Executing command: ");
  Serial.println(statename[state]);

  switch (state) {
    case FORWARD:
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(duration);
      break;
    case BACKWARD:
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(duration);
      break;
    case LEFT:
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(duration);
      break;
    case RIGHT:
      digitalWrite(ENA, HIGH);
      digitalWrite(ENB, HIGH);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(duration);
      break;
    case DANCE:
      for (int i = 0; i < 3; i++) {
        digitalWrite(ENA, HIGH);
        digitalWrite(ENB, HIGH);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        delay(250);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        delay(250);
      }
      break;
  }
  stopper();
}

void sm_01() {
  if (millis() - tallytimer > TALLY_DELAY) {
    maxTally = *std::max_element(countTally, countTally + n);

    ind = 0;
    while (ind < n && countTally[ind] != maxTally) ind++;

    tieVar = std::count(countTally, countTally + n, maxTally) > 1;

    if (!tieVar) {
      state = (STATE)ind;
      sendTwitchMessage("Votes collected!");
      sendTwitchMessage(statename[ind]);
      Serial.print("Votes collected for: ");
      Serial.println(statename[ind]);
      tallytimer = millis();
      timestart = millis();
      memset(countTally, 0, sizeof(countTally));

      int duration = MOVE_DELAY; // Default duration

      if (state == FORWARD || state == BACKWARD) {
        if (input.length() > 7) {
          String durationStr = input.substring(input.indexOf(' ') + 1);
          if (durationStr == "0.5") {
            duration = 500;
          } else if (durationStr == "1") {
            duration = 1000;
          } else if (durationStr == "2") {
            duration = 2000;
          }
        }
      } else if (state == LEFT || state == RIGHT) {
        if (input.length() > 10) {  // Adjusted for "turn left" and "turn right" command lengths
          String durationStr = input.substring(input.indexOf(' ') + 1);
          if (durationStr == "0.1") {
            duration = 100;
          } else if (durationStr == "0.3") {
            duration = 300;
          } else if (durationStr == "0.5") {
            duration = 500;
          }
        }
      }

      executeCommand(duration);
    } else {
      tallytimer = millis();
    }
  }
}

