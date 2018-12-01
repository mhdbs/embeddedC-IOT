
#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
int buzzer = D1;
int red = D4;
int buttonState = 0;  
const char* ssid = ""; //Your ssid 
const char* password = ""; //Your password
char server[] = ""; //eg: 192.168.0.222
//eg tags aa,ab,ac
char taa[] ="0000A6C00365"; // Replace with your own Tag ID
char tab[] ="4A007BD4A045";
char tac[] ="4A007B8AD66D";
char input[3]; // A variable to store the Tag ID being presented
int count = 0; // A counter variable to navigate through the input[] character array
//character array
boolean flag1,flag2,flag3  = 0; 
int temp;
 WiFiClient client;
//WiFiServer server(80);
     
void setup()
{
    Serial.begin(9600);
    delay(10);
    SPI.begin(); // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        }
        Serial.println("");
        Serial.println("WiFi connected");
        // Start the server
        // server.begin();
        Serial.println("Server started");
        Serial.print(WiFi.localIP());
        delay(1000);
        Serial.println("connecting...");
        pinMode(buzzer, OUTPUT);
        pinMode(D7, OUTPUT);
        pinMode(red, OUTPUT);
        pinMode(A0, OUTPUT);
        }

void loop()
    {  
        if(Serial.available()>0)// Check if there is incoming data in the RFID Reader Serial Buffer.
            {
                count = 0; // Reset the counter to zero
                /* Keep reading Byte by Byte from the Buffer till the RFID Reader Buffer is empty 
                or till 12 Bytes (the ID size of our Tag) is read */
                while(Serial.available() && count < 12) 
                    {
                        input[count] = Serial.read(); // Read 1 Byte of data and store it in the input[] variable
                        count++; // increment counter
                        delay(50);
                        }
                        /* When the counter reaches 12 (the size of the ID) we stop and compare each value 
                        of the input[] to the corresponding stored value */
                        if(count == 12) // 
                            {
                                count =0; // reset counter varibale to 0
                                flag1 = 1;
                                flag2 = 1;
                                flag3 = 1;
                                /* Iterate through each value and compare till either the 12 values are 
                                all matching or till the first mistmatch occurs */
                                while(count<12 && flag1 !=0)  
                                    {
                                        if(input[count]==taa[count])
                                            flag1 = 1; // everytime the values match, we set the flag variable to 1
                                        else
                                            flag1 = 0; 
                                            /* if the ID values don't match, set flag variable to 0 and 
                                            stop comparing by exiting the while loop */
                                            count++; // increment i
                                        }
                                while(count<12 && flag2 !=0)  
                                    {
                                        if(input[count]==tab[count])
                                        flag2 = 1; // everytime the values match, we set the flag variable to 1
                                        else
                                        flag2 = 0; 
                                        /* if the ID values don't match, set flag variable to 0 and 
                                        stop comparing by exiting the while loop */
                                        count++; // increment i
                                        }
                                while(count<12 && flag3 !=0)  
                                    {
                                        if(input[count]==tac[count])
                                        flag3 = 1; // everytime the values match, we set the flag variable to 1
                                        else
                                        flag3 = 0; 
                                                            /* if the ID values don't match, set flag variable to 0 and 
                                                                stop comparing by exiting the while loop */
                                        count++; // increment i
                                        }
                                }
                        if(flag1 == 1) // If flag variable is 1, then it means the tags match
                            {
                                Serial.println("card 1");
                                //  analogWrite(A0, HIGH);
                                digitalWrite(D7, HIGH);
                                delay(5000);
                                digitalWrite(D7, LOW);
                                delay(2000);
                                temp=1;
                                Sending_To_DB();
                                delay(2000);
                                }
                        else if(flag2 == 1) // If flag variable is 1, then it means the tags match
                            {
                                Serial.println("card 2");
                                analogWrite(A0, HIGH);
                                digitalWrite(D7, HIGH);
                                delay(5000);
                                digitalWrite(D7, LOW);
                                delay(2000);
                                temp=2;
                                Sending_To_DB();
                                delay(5000);
                                }
                        else if(flag3 == 1) // If flag variable is 1, then it means the tags match
                            {
                                Serial.println("card 3");
                                analogWrite(A0, HIGH);
                                digitalWrite(D7, HIGH);
                                delay(5000);
                                digitalWrite(D7, LOW);
                                delay(2000);
                                temp=3;
                                Sending_To_DB();
                                delay(2000);
                                }
                        else
                            {
                                Serial.println("Unauthorised card");
                                digitalWrite(buzzer, HIGH);
                                digitalWrite(red, HIGH);
                                delay(1000);
                                digitalWrite(buzzer, LOW);
                                digitalWrite(red, LOW);
                                digitalWrite(D7, LOW);
                                delay(2000);
                                }
    /* Fill the input variable array with a fixed value 'F' to overwrite 
    all values getting it empty for the next read cycle */
        for(count=0; count<12; count++) 
            {
                input[count]= 'F';
                }
            count = 0; // Reset counter variable
            }
    

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
}


 void Sending_To_DB()   //CONNECTING WITH MYSQL
 {
   if (client.connect(server, 80)) {
    Serial.println("connected");
   String url = ""; //URL 
      url += temp;
  
  // This will send the request to the server
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" +
               "Connection: close\r\n\r\n");
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  client.stop();
 }  
