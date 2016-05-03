//butterworth koeficijenti
float a[]={1.0,
           -2.692610987017434,
           2.867399109111388,
           -1.403484671368140,
           0.264454816443504};
float b[]={0.002234891698082,
           0.008939566792329,
           0.013409350188494,
           0.008939566792329,
           0.002234891698082};
//float a[]={1.0,
//           -3.586239808116908,
//           4.846289804288022,
//           -2.930427216820136,
//           0.670457905953174};
//float b[]={0.016581931669307,
//           0.0,
//           -0.033163863338614,
//           0.0,
//           0.016581931669307};

//butterworth memorija
float x[][4]={
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
};
float y[][4]={
  {0,0,0,0},
  {0,0,0,0},
  {0,0,0,0}
};

//DCdrift
float dc[]={0,0,0};

//prosjeÄŤna devijacija od DC drifta - za detekciju QRS kompleksa
float avg_delta[]={0,0,0};

//mjerenje pulsa
short firstPulseFlag=1;
unsigned long lastPulse;
float T=1000;

//timer za uzorkovanje
unsigned long lastCall;

void pushRegister(float* reg, float data,int len){
  //funkcija pomicanja proĹˇlih vrijednosti filtra, najnovija vrijednost je u [0]!
  for(int i=len-1;i>0;i--){reg[i]=reg[i-1];}
  reg[0]=data;
}

float butter4d(float* b, float* a, float* y, float* x, float u){

  //raÄŤunanje DTF filtra
  float sum_B=b[0]*u;
  float sum_A=0;
  for(int i=0;i<4;i++){
    sum_B+=b[i+1]*x[i];
    sum_A+=a[i+1]*y[i];
  }
  return (sum_B-sum_A)/a[0];

}

void setup() {
  Serial.begin(57600);
  for(int i=0;i<3;i++){
    dc[i]=analogRead(i+1);
  }
  lastCall=micros();
}

void loop() {
  
  //mjerenje vremena
  unsigned long call=micros();                     //vrijeme poziva
  if (call<lastCall){                              //ispravak overflowa? (~70min za arduino uno, 16-bit ulong)
    call+=65536-lastCall;
    lastCall=0;
  }
  if(call-lastCall>4000){                          //okidam uzorkovanje ako je proĹˇlo 4ms
    lastCall=call;                                 //biljeĹľim vrijeme
    
    int u[3];
    float p[3];
    int pulse[3];
    for(int k=0;k<3;k++){
      u[k]=analogRead(k+1);    
      p[k]=butter4d(b,a,y[k],x[k],u[k]);
      dc[k]=0.995*dc[k]+0.005*u[k];
      avg_delta[k]=0.99*abs(avg_delta[k])+0.01*abs(p[k]-dc[k]);
      
      if ((p[k]-dc[k])>avg_delta[k]*3&&(y[k][0]-dc[k])<avg_delta[k]*3){
        if (firstPulseFlag){
          firstPulseFlag=0;
        }else{
          T=0.6*T+0.4*(millis()-lastPulse);
        }
        pulse[k]=1;
        lastPulse=millis();
      }else{
        pulse[k]=0;
      }
      
      pushRegister(x[k],(float)u[k],4);
      pushRegister(y[k],p[k],4);
    }
    
    unsigned long calctime=micros()-call;
  
    //serial stuff
    String message=String(millis())+" ";
    for(int k=0;k<3;k++){
      message+=String(p[k]-dc[k])+" ";
    }
    message+=String(pulse[0])+", ";
    Serial.print(message);
  }
  
}

