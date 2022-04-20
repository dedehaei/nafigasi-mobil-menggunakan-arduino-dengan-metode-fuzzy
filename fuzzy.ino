// Motor kanan 
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor kiri 
int enB = 3;
int in3 = 4;
int in4 = 5;

const int trigPin1 = A2;//depan
const int echoPin1 = A3;
const int trigPin2 = A0; //kiri
const int echoPin2 = A1;
const int trigPin3 = A4;//kanan
const int echoPin3 = A5;

float kiri,kanan;
float pwmkanan;
float pwmkiri;
float temp;
float defuzkiri,defuzkanan;

float jarakdepan[3];
float jarakkiri[3];
float jarakkanan[3];
float rule[3][3][3];
  
float rule000, rule001, rule002; 
float rule010, rule011, rule012;
float rule020, rule021, rule022;
float rule100, rule101, rule102;
float rule110, rule111, rule112;
float rule120, rule121, rule122;
float rule200, rule201, rule202;
float rule210, rule211, rule212;
float rule220, rule221, rule222;

void setup() {
  Serial.begin(9600);
	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  
    pinMode(trigPin1, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin2, INPUT);
    pinMode(trigPin3, OUTPUT);
    pinMode(echoPin3, INPUT);
}

float Sensordepan(){
float jarakdepan;
long dur;
digitalWrite(trigPin1, LOW);
delayMicroseconds(2); 
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin1, LOW);
dur = pulseIn(echoPin1, HIGH);
jarakdepan = dur * 0.034 / 2;
  Serial.print("depan\t = ");
 Serial.println(jarakdepan);
return jarakdepan;
}

float Sensorkanan (){
float jarakkanan;

long dur;
digitalWrite(trigPin1, LOW);
delayMicroseconds(2); 
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin1, LOW);
dur = pulseIn(echoPin1, HIGH);
jarakkanan = dur * 0.034 / 2;
  Serial.print("kanan\t = ");
  Serial.println(jarakkanan);
return jarakkanan;
}

float Sensorkiri ()
{
float jarakkiri;
long dur;
digitalWrite(trigPin1, LOW);
delayMicroseconds(2); 
digitalWrite(trigPin1, HIGH);
delayMicroseconds(10); 
digitalWrite(trigPin1, LOW);
dur = pulseIn(echoPin1, HIGH);
jarakkiri = dur * 0.034 / 2;
  Serial.print("kiri\t = ");
  Serial.println(jarakkiri);
return jarakkiri;
}

void fuzzydepan(){
   Sensordepan();
    if (Sensordepan() >= 5&&Sensordepan() <= 10){jarakdepan[0] =1;}
    else if (Sensordepan() >=10 && Sensordepan() <=19){jarakdepan[0]=(19-Sensordepan())/9;}
  
    if (Sensordepan() >=10 && Sensordepan() <=19){jarakdepan[1]=(Sensordepan()-10)/9;}
    else if (Sensordepan() >=19 && Sensordepan() <=28){jarakdepan[1]=(28-Sensordepan())/9;}
   

    if (Sensordepan()>=19 && Sensordepan() <=28){jarakdepan[2]=(Sensordepan()-19)/9;}
    else if (Sensordepan() >= 28){jarakdepan[2] =1;}
}

void fuzzykiri(){
   Sensorkiri();
    if (Sensorkiri() >= 5&&Sensorkiri() <= 10){jarakkiri[0] =1;}
    else if (Sensorkiri() >=10 && Sensorkiri() <=19){jarakkiri[0]=(19-Sensorkiri())/9;}
  
    if (Sensorkiri() >=10 && Sensorkiri() <=19){jarakkiri[1]=(Sensorkiri()-10)/9;}
    else if (Sensorkiri() >=19 && Sensorkiri() <=28){jarakkiri[1]=(28-Sensorkiri())/9;}
   
    if (Sensorkiri()>=19 && Sensorkiri() <=28){jarakkiri[2]=(Sensorkiri()-19)/9;}
    else if (Sensorkiri() >= 28){jarakkiri[2] =1;}
}

void fuzzykanan(){
   Sensorkanan();
    if (Sensorkanan() >= 5&&Sensorkanan() <= 10){jarakkanan[0] =1;}
    else if (Sensorkanan() >=10 && Sensorkanan() <=19){jarakkanan[0]=(19-Sensorkanan())/9;}
  
    if (Sensorkanan() >=10 && Sensorkanan() <=19){jarakkanan[1]=(Sensorkanan()-10)/9;}
    else if (Sensordepan() >=19 && Sensorkanan() <=28){jarakkanan[1]=(28-Sensorkanan())/9;}
  
    if (Sensorkanan()>=19 && Sensorkanan() <=28){jarakkanan[2]=(Sensorkanan()-19)/9;}
  	else if (Sensorkanan() >= 28){jarakkanan[2] =1;}
}
  
void fuzzifikasi(){
  fuzzykiri();
  fuzzydepan();
  fuzzykanan();
}

void RuleEva (){
  fuzzifikasi();
 int i,j,k,bantu;
 for ( i=0; i<=2; i=i+1)
 {
   for ( j=0; j<=2; j=j+1)
   {
     for ( k = 0; k<=2;k=k+1){
       if((jarakdepan[i]<jarakkiri[j])&&(jarakdepan[i]<jarakkanan[k])){
         temp = jarakdepan[i];}
       else if ((jarakkiri[i]<jarakdepan[j])&&(jarakkiri[j]<jarakkanan[k])){
         temp = jarakkiri[j];}
       else {
         temp = jarakkanan[k];}
     rule [i][j][k] = temp;
       //Serial.print(" rule");
       //Serial.print(i);Serial.print(j);Serial.print(k);
       //Serial.print("\t = ");
       //Serial.println(temp);
     }
   } 
 } 

 rule000 = rule [0][0][0]; // (dekat,dekat,dekat = Lambat,cepat)
 rule001 = rule [0][0][1]; // (dekat,dekat,sedang = cepat,lambat)
 rule002 = rule [0][0][2]; // (dekat,dekat,jauh = cepat,Lambat)
 
 rule010 = rule [0][1][0]; // (dekat,sedang,dekat = sedang,sedang)
 rule011 = rule [0][1][1]; // (dekat,sedang,sedang = Sedang,lambat)
 rule012 = rule [0][1][2]; // (dekat,sedang,jauh = sedang,lambat)
 
 rule020 = rule [0][2][0]; // (dekat,jauh,dekat = Cepat,cepat)
 rule021 = rule [0][2][1]; // (dekat,jauh,sedang = sedang,lambat)
 rule022 = rule [0][2][2]; // (dekat,jauh,jauh= sedang,lambat)

 rule100 = rule [1][0][0]; // (sedang,dekat,dekat = Lambat,cepat)
 rule101 = rule [1][0][1]; // (sedang,dekat,sedang = Lambat,lambat)
 rule102 = rule [1][0][2]; // (sedang,dekat,jauh = cepat,lambat)
 
 rule110 = rule [1][1][0]; // (sedang,sedang,dekat = lambat,sedang)
 rule111 = rule [1][1][1]; // (sedang,sedang,sedang = lambat,lambat)
 rule112 = rule [1][1][2]; // (sedang,sedang,jauh = sedang,lambat)
 
 rule120 = rule [1][2][0]; // (sedang,jauh,dekat = lambat,sedang)
 rule121 = rule [1][2][1]; // (sedang,jauh,sedang = sedang,sedang)
 rule122 = rule [1][2][2]; // (sedag,jauh,jauh= lambat,sedang)
  
 rule200 = rule [2][0][0]; // (jauh,dekat,dekat = Lambat,cepat)
 rule201 = rule [2][0][1]; // (jauh,dekat,sedang = Lambat,ceapt)
 rule202 = rule [2][0][2]; // (jauh,dekat,jauh = sedang,sedang)
 
 rule210 = rule [2][1][0]; // (jauh,sedang,dekat = lambat,sedang)
 rule211 = rule [2][1][1]; // (jauh,sedang,sedang = cepat,lambat)
 rule212 = rule [2][1][2]; // (jauh,sedang,jauh = lambat,lambat)
 
 rule220 = rule [2][2][0]; // (jauh,jauh,dekat = lambat,sedang)
 rule221 = rule [2][2][1]; // (jauh,jauh,sedang = sedang,lambat)
 rule222 = rule [2][2][2]; // (jauh,jauh,jauh= sedang,sedang)

}

void Defuzzykiri () {
  // metode sugeno (weighted average)
  float lambat = 100;
  float sedang = 200;
  float cepat = 250;
  RuleEva();
  kiri =  (rule000 * lambat) + 
          (rule001 * cepat)+ 
          (rule002 * cepat)+ 
    
          (rule010 * sedang)+ 
          (rule011 * sedang)+ 
          (rule012 * sedang) + 
    
          (rule020 * cepat)+ 
          (rule021 * sedang)+ 
          (rule022 * sedang)+
    
          (rule100 * lambat) + 
          (rule101 * lambat)+ 
          (rule102 * cepat)+ 
    
          (rule110 * lambat)+ 
          (rule111 * lambat)+ 
          (rule112 * sedang) + 
    
          (rule120 * lambat)+ 
          (rule121 * sedang)+ 
          (rule122 * lambat)+
    
          (rule200 * lambat) + 
          (rule201 * lambat)+ 
          (rule202 * sedang)+ 
    
          (rule210 * lambat)+
          (rule211 * cepat)+ 
          (rule212 * lambat) + 
    
          (rule220 * lambat)+ 
          (rule221 * sedang)+ 
          (rule222 * sedang);
  

  defuzkiri = 0;
 
  int i, j,k;
  for ( i=0; i<=2; i=i+1)
  {
    for ( j=0; j<=2; j=j+1)
    {
      for(k=0;k<=2;k=k+1){
        defuzkiri = defuzkiri + rule [i][j][k];
        }
    } 
  } 
//   Serial.print("kiri1 = ");
  //Serial.println(kiri);

  //
 // Serial.print("defuzkiri = ");
  //Serial.println(defuzkiri);
 

  pwmkiri = kiri / defuzkiri;
  Serial.print("pwm kiri\t=");
  Serial.println(pwmkiri);
  
  
}

void Defuzzykanan () {
  float lambat = 100;
  float sedang = 200;
  float cepat = 250;
  RuleEva();

     kanan =  (rule000 * lambat) + 
              (rule001 * lambat)+ 
              (rule002 *lambat)+ 

              (rule010 * sedang)+ 
              (rule011 * lambat)+ 
              (rule012 *lambat) + 

              (rule020 * cepat)+ 
              (rule021 * lambat)+ 
              (rule022 * lambat)+

              (rule100 * cepat) + 
              (rule101 * lambat)+ 
              (rule102 * lambat)+ 

              (rule110 * sedang)+ 
              (rule111 * lambat)+ 
              (rule112 * lambat)+ 

              (rule120 * sedang)+ 
              (rule121 * sedang)+ 
              (rule122 * sedang)+

              (rule200 * cepat) + 
              (rule201 * cepat)+ 
              (rule202 * sedang)+ 

              (rule210 * sedang)+
              (rule211 * lambat)+ 
              (rule212 * lambat)+ 

              (rule220 * sedang)+ 
              (rule221 * lambat)+ 
              (rule222 * sedang);
  
   defuzkanan = 0;
   int i, j,k;
  for ( i=0; i<=2; i=i+1)
  {
    for ( j=0; j<=2; j=j+1)
    {
      for(k=0;k<=2;k=k+1){
 
        defuzkanan = defuzkanan + rule [i][j][k];}
    } 
  } 
 // Serial.print("kanan = ");
  //Serial.println(kanan);
 //Serial.println(defuzkanan);
  pwmkanan = kanan / defuzkanan;
  Serial.print("pwm kanan\t=");
  Serial.println(pwmkanan);
  
}
void loop(){
 

  	// Turn on motors
	digitalWrite(in1, HIGH);
  	digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);	

  
  	Defuzzykiri ();
  	Defuzzykanan ();
  	analogWrite(enB,pwmkiri);
  	analogWrite(enA,pwmkanan);
  

}