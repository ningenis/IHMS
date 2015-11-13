/*
Kode Sistem Pengelolaan Intelligent Home untuk Arduino
*/

// Setting WiFi
#define SSID "WR1043ND"		// SSID
#define PASS ""                // Password
#define HOST "192.168.1.101"  // Webhost
//-------------------------------

// Setting PIR
int calibrationTime = 10; // Waktu Kalibrasi      
int interval = 970; // Interval untuk satu fungsi void loop
int pirPin = 3;    // Pin digital ke sensor PIR
boolean noMotion = true; // Status Pergerakan pada PIR
// Waktu Update Pergerakan Sensor Pada PIR (3 detik)
int minSecsBetweenUpdates = 3; 
long lastSend = -minSecsBetweenUpdates * 1000l;
long now;

// Deklarasi Variabel
float celcius; // Suhu Ruangan
float calibrate; // Penampung nilai dari sensor suhu yang akan dikalibrasi
int lampu; // Status Lampu
int val = 0; // Penampung nilai dari sensor api 
int valu = 0; // Penampung nilai dari sensor cahaya 
int value = 0; // Penampung nilai dari sensor suhu

// Deklarasi Variabel Pin untuk Sensor dan LED 
const int inPin = 0; // Pin untuk sensor suhu
const int pinLDR = 1; // Pin untuk sensor cahaya
const int flame = 2; // Pin untuk sensor api
const int LED = 9; // Pin untuk LED Kuning (Lampu)
const int LED2 = 12; // Pin untuk LED Hijau (AC)
const int LED3 = 13; // Pin untuk LED Merah (Spinkler)
const int LED4 = 11; // Pin untuk LED Biru (Alarm)


//-------------------------------

// Setup
void setup(){
  // Deklarasi Serial
  Serial.begin(9600);
  
  // Deklarasi Pin untuk PIR
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);
  
  // Deklarasi LED sebagai output
  pinMode(LED, OUTPUT); 
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT); 
  pinMode(LED4, OUTPUT); 
  
  // Kalibrasi Sensor
  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
    }
  Serial.println(" done");
  Serial.println("SENSOR ACTIVE");
  delay(50);

  // Tes awal modul ESP8266.
  Serial.println("AT");
  delay(5000);
  connectWiFi();
  //if(Serial.find("OK")){
    //connectWiFi();
  //}
}

// Program Utama (Loop)
void loop(){
  now = millis(); // Inisialisasi waktu saat ini untuk sensor PIR
  ac();
  delay(10);
  lamp();
  delay(10);
  // Kirim data ke server
  sendData(String(celcius), String(lampu));
  sprinkler();
  delay(10);
  alarm();
  delay(interval);
}

// Prosedur memproses data sensor suhu dan mengaktuasi AC (LED Hijau)
void ac() {
  value = analogRead(inPin); // Simpan nilai dari sensor
  float millivolts = (value / 1024.0) * 5000; // Rumus pembagi tegangan
  calibrate = millivolts / 10; // Keluaran sensor 10mV/Celsius
  celcius = (0.94*calibrate) + 1.55; // Rumus Kalibrasi
  Serial.print("Suhu = ");
  Serial.print(celcius); // Print di serial monitor untuk debug
  Serial.println();
  // Logika aktuasi
  if (celcius <= 26.00 && celcius >= 20.00)
  {
    digitalWrite(LED2, LOW);
  }
  else
  {
    digitalWrite(LED2, HIGH);
  }
}

// Prosedur memproses data sensor cahaya dan mengaktuasi Lampu (LED Kuning)
void lamp() {
  valu = analogRead(pinLDR); // Simpan nilai dari sensor
  Serial.print("Nilai LDR = ");
  Serial.print(valu); // Print di serial monitor untuk debug
  Serial.println();
  // Logika aktuasi
  if (valu < 123) {
    digitalWrite(LED, HIGH);
    lampu = 1;
  }
  if (valu >= 881) {
    digitalWrite(LED, LOW);
    lampu = 0;
  }
  if (valu >= 123 && valu < 471) {
    analogWrite(LED, (138));
    lampu = 2;
  }
  if (valu >= 471 && valu < 881) {
    analogWrite(LED, (35));
    lampu = 2;
  }
}

// Prosedur memproses data sensor api dan mengaktuasi sprinkler (LED Merah)
void sprinkler() {
  val = analogRead(flame); // Simpan nilai dari sensor
  Serial.print("Nilai Sensor Api = ");
  Serial.print(val, DEC); // Print di serial monitor untuk debug
  Serial.println();
  // Logika aktuasi
  if (val >= 625) {
    digitalWrite(LED3, HIGH);
  }
  else
  {
    digitalWrite(LED3, LOW);
  } 
}

// Prosedur memproses data PIR dan aktuasi alarm (LED Biru)
void alarm() {
  // Logika aktuasi
  if (digitalRead(pirPin) == HIGH)
    {
      if (now > (lastSend + minSecsBetweenUpdates * 1000l) || noMotion == true)
        {
          lastSend = now;
          noMotion = false;
          Serial.println("Motion");
          digitalWrite(LED4, HIGH); 
        } 
    }
  else 
    {
      if (now > (lastSend + minSecsBetweenUpdates * 2000l))
        {
          lastSend = now;
          noMotion = true;
          Serial.println("No motion");
          digitalWrite(LED4, LOW);
        }         
    }    
}

// Prosedur mengirimkan data ke server
void sendData(String celcius, String lampu){
  // Setup koneksi TCP.
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += HOST;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);

  // Kirim data ke server dengan metode GET.
  cmd = "GET /arduino/add_data.php"; // Link ke skrip PHP                    
  cmd += "?temperature=";  // Selektor Temperatur
  cmd += celcius;          // Nilai Temperatur
  cmd += "&lampu=";        // Selektor Status Lampu    
  cmd += lampu;            // Nilai Lampu
  cmd += " HTTP/1.1\r\nHost: 192.168.1.101\r\n\r\n\r\n"; // Host
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  Serial.print(cmd);
  //if(Serial.find(">")){
    //Serial.print(cmd);
  //}  
}

// Prosedur Koneksi WiFi
void connectWiFi(){
  // Koneksi ke Titik Akses (AP)
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  // Pengecekan Koneksi WiFi
  if(Serial.find("OK")){
    Serial.println("Connection");
    //return true;
  }
  else{
    Serial.println("No Connection");
    //return false;
  }
}
