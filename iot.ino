#include <LiquidCrystal_I2C.h> //library LCD
#include <AntaresESP8266HTTP.h> //library Antares
#include <DHT.h> //library sensor DHT

#define ACCESSKEY "6388160f97f65e86:814c491f********" //acces key yang didapatkan dari web antares
#define WIFISSID "MACOPAD" //nama ssid wifi yang digunakan
#define PASSWORD "macopad12" //password wifi yang digunakan

#define projectName "MACOPAD" //untuk menyambungkan ke aplikasi yang telah dibuat di antares
#define deviceName "MACOPAD01" //untuk menyambungkan ke device yang telah dibuat di antares

AntaresESP8266HTTP antares(ACCESSKEY); //komunikasi protokol untuk mengkoneksikan ESP8266 ke Antares menggunakan HTTP 

LiquidCrystal_I2C lcd(0x27, 16, 2); // alamat dari LCD dan ukuran dari LCD

#define DHTPIN 14 //mendefinisikan pin dht ke port 14
#define DHTTYPE DHT11 //mendefinisikan tipe sensor yang digunakan

#define Relay_1 12 //mendefinisikan relay 1 di port 12
//#define Relay_2 13 //mendefinisikan relay 2 di port 13

DHT dht(DHTPIN, DHTTYPE); //menginisiasi DHT Sensor

void setup() {
  lcd.init(); //menginisialisasi lcd
  lcd.backlight(); //menyalakan backlight dari LCD
  Serial.begin(9600); //kecepatan serial monitor
  dht.begin(); //memulai dht sensor bekerja

  lcd.setCursor(0,0); //mesetting tata letak tulisan di layar lcd
  lcd.print("Menghubungkan"); //mencetak tulisan di layar lcd
  lcd.setCursor(0,1); //mesetting tata letak tulisan di layar lcd
  lcd.print("ke Wifi...");//mencetak tulisan di layar lcd

  pinMode(Relay_1, OUTPUT); //menjadikan relay 1 ke mode output
  //pinMode(Relay_2, OUTPUT); //menjadikan relay 1 ke mode output

  antares.setDebug(false); //untuk mengatur mode debug off
  antares.wifiConnection(WIFISSID,PASSWORD); //menyambungkan wifi ke anatares supaya data masuk ke antares

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Berhasil");
  lcd.setCursor(0,1);
  lcd.print("terhubung!");

  delay(5000);

}


void loop() {
  float temp = dht.readTemperature(); //mendefinisikan angka temprature dalam bentuk float
  int hum = dht.readHumidity(); //mendefinisikan angka kelembapan dalam bentuk integer
  String fan = "OFF"; // mendefinisikan kondisi kipas
  
  if (isnan(temp) || isnan(hum)){ //mendefinisikan jika bukan angka maka tidak terdeteksi
    Serial.println("Sensor DHT11 tidak ditemukan!"); //mencetak hasil jika bukan angka
    lcd.clear(); // untuk membersihkan lacar lcd
    lcd.setCursor(0,0); // mengatur letak baris dan kolom 
    lcd.print("Sensor DHT11"); 
    lcd.setCursor(0,1); // mengatur letak baris dan kolom
    lcd.print("tidak ditemukan!"); 
    delay(1000); // mengatur lamanya delay (1000milisekon)
    return;
  }

  Serial.println("Temperatur : "+ String(temp)); //mencetak hasil temperatur yang dideteksi

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.setCursor(5,0);
  lcd.print(temp);
  lcd.print((char)223); // menampilkan karakter dari satuan derajat 
  lcd.print("C");

  lcd.setCursor(0,1);
  lcd.print("Fan :");
  //lcd.setCursor(5,1);
  //lcd.print(fan);
  //lcd.print("%");

  if (temp<=40.00) { //membuat kondisi jika temp kurang dari sama dengan 40 maka...
    digitalWrite(Relay_1, LOW); //relay mati
    Serial.println("FAN OFF"); //mencetak keadaan fan off di serial print
    lcd.setCursor(5,1); //tata letak tampilan di LCD
    lcd.print("OFF"); //mencetak String "OFF" pada lcd
    fan= "OFF"; //memberikan data pada variabel fan adalah string "OFF"
    //digitalWrite(Relay_2, HIGH);
   }

  else if (temp>=41.00) { //membuat kondisi jika temp lebih dari sama dengan 41 maka...
    digitalWrite(Relay_1, HIGH);  // relay hidup
    Serial.println("FAN ON"); // mencetak keadaan fan on di serial print
    lcd.setCursor(5,1); //tata letak tampilan di LCD
    lcd.print("ON"); //mencetak String "ON" pada lcd
    fan= "ON";//memberikan data pada variabel fan adalah string "ON"
    //digitalWrite(Relay_2, LOW);
    }
  antares.add("Suhu",temp); //syntax untuk menambahkan data sensor temp yang dideteksi
  antares.add("Kipas",fan); //syntax untuk menambahkan data kondisi kipas
  //antares.add("Kelembaban", hum);
  antares.send(projectName, deviceName); //syntax untuk mengirim data dari suatu device ke Antares
  delay(1000);

}

