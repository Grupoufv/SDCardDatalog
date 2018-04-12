 #include <dht.h> 
 #include <SPI.h>
 #include <SD.h>
 #include <Ultrasonic.h>
 dht DHT; 
 File myFile;
 File myFile2;
 int DHTPino;
 int ULTRAPinoT=9;//pinos base para o sensor ultrasonico
 int ULTRAPinoE=8;
 int LDRPino;
 int HOUR;
 int MIN;
 int YEAR;
 int MONTH;
 int DAY;
 int INTERVAL;
 float luz;
 float distancia; 
Ultrasonic ultrasonic(ULTRAPinoT,ULTRAPinoE);
 
 void setup()
 {
 Serial.begin(9600);

 Serial.print("Iniciando o cartão SD ...");
 
 if (!SD.begin(10)) {
 Serial.println("falha ao iniciar cartão SD!");
 return;
 }
 Serial.println(" Cartão SD iniciado corretamente.");
 
 readSDSettings();

 
 if(!SD.exists("Datalog.csv"))
  {
      
      myFile = SD.open("Datalog.csv", FILE_WRITE);
      if (myFile) {
        Serial.println("Arquivo novo");
        myFile.println("DATA;HORA;TEMPERATURA;DISTANCIA;LUZ");
        myFile.close();
      } else {

        Serial.print("Erro criando o arquivo ");
       
      }
    
  }

 }
 
 void loop()
 {
   //Faz a leitura a cada 2 segundos
  long microsec = ultrasonic.timing();
  DHT.read11(DHTPino); //Lê as informações do sensor
  
  Serial.print("Temperatura = ");
  Serial.print(DHT.temperature); 
  Serial.println("");
  luz=analogRead(LDRPino);
  Serial.println(luz);
  
  
  distancia = ultrasonic.convert(microsec, Ultrasonic::CM);
 int Hatual=HOUR;
  int Matual=MIN;
  int Satual=millis()/1000;
  Matual+=Satual/60;
  Satual=Satual%60;
  Hatual+=Matual/60;
  Matual=Matual%60;

  myFile = SD.open("Datalog.csv", FILE_WRITE);//abrimos  o arquivo
  if (myFile) { 
        Serial.print("Escrevendo no  SD: ");
        myFile.println("DATA;HORA;TEMPERATURA;DISTANCIA;LUZ");
        myFile.print(YEAR);
        myFile.print("/");
        myFile.print(MONTH);
        myFile.print("/");
        myFile.print(DAY);
        myFile.print(";");
        myFile.print(Hatual);
        myFile.print(":");
        myFile.print(Matual);
        myFile.print(":");
        myFile.print(Satual);
        myFile.print(";");
        myFile.print(DHT.temperature);
        myFile.print(";");
        myFile.print(distancia);
        myFile.print(";");
        myFile.print(luz);
        myFile.println("");
        
        myFile.close(); 
       
  } else {
    
    Serial.println("Erro ao abrir arquivo");
  }

  
  
  delay(INTERVAL*60000);  
  
 }
 
void readSDSettings(){
  char character;
  String settingName;
  String settingValue;
  myFile = SD.open("comandos.txt");
  if (myFile) {
    while (myFile.available()) {
      character = myFile.read();
      while((myFile.available()) && (character != ' ')){
        settingName = settingName + character;
        character = myFile.read();
      }
      character = myFile.read();
      while((myFile.available()) && (character != '\n')){
        settingValue = settingValue + character;
        character = myFile.read();
      }
      if(character == '\n'){
        // Apply the value to the parameter
        applySetting(settingName,settingValue);
        // Reset Strings
        settingName = "";
        settingValue = "";
      }
    }
    // close the file:
    myFile.close();
  } else {
    
    Serial.println("Erro ao abrir  Comandos.txt");
  }
}
 
void applySetting(String settingName, String settingValue) {
  if(settingName == "DHT") {
    DHTPino=settingValue.toInt();
  } else if (settingName == "ULTRA"){
    ULTRAPinoT = separaValor(settingValue,' ').toInt();
    ULTRAPinoE = settingValue.toInt();
  } else if (settingName == "LDR"){
    separaValor(settingValue, 'A');
    LDRPino = settingValue.toInt();
  } else if (settingName == "HOUR"){
    HOUR = separaValor(settingValue,':').toInt();
    MIN = settingValue.toInt();
  } else if (settingName == "DATE"){
    YEAR = separaValor(settingValue,'/').toInt();
    MONTH = separaValor(settingValue,'/').toInt();
    DAY = settingValue.toInt();
  } else if (settingName == "INTERVAL"){
    INTERVAL = settingValue.toInt();
  }
}

String separaValor(String& valor, char sep){
  String aux = "";
  String aux2 = "";
  int i;
  for(i = 0; i < valor.length(); i++){
    if(valor[i] == sep) break;
    aux = aux + valor[i];
  }
  for(i++; i < valor.length(); i++){
    aux2 = aux2 + valor[i];
  }
  valor = aux2;
  return aux;
}
 
 // converting string to Float
 float toFloat(String settingValue){
 char floatbuf[settingValue.length()+1];
 settingValue.toCharArray(floatbuf, sizeof(floatbuf));
 float f = atof(floatbuf);
 return f;
 }
 
 long toLong(String settingValue){
 char longbuf[settingValue.length()+1];
 settingValue.toCharArray(longbuf, sizeof(longbuf));
 long l = atol(longbuf);
 return l;
 }
 
 // Converting String to integer and then to boolean
 // 1 = true
 // 0 = false
 boolean toBoolean(String settingValue) {
 if(settingValue.toInt()==1){
 return true;
 } else {
 return false;
 }
 }
 
