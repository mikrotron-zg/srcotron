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

//prosječne devijacije - za detekciju QRS kompleksa
float avg_delta[]={0,0,0};
float d_avg_delta[]={0,0,0};

//mjerenje pulsa
short firstPulseFlag=1;
unsigned long lastPulse;
float T=1000;

//buffer za PQRST kompleks
float complex_buffer[300];
int buffer_pos=0;

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

int onlineStep(){
  
  static int pulse_counter=0;

  int u[3];
  float p[3];
  int pulse[3];
  for(int k=0;k<3;k++){
    u[k]=analogRead(k+1);    
    p[k]=butter4d(b,a,y[k],x[k],u[k]);
    dc[k]=0.995*dc[k]+0.005*u[k];
    avg_delta[k]=0.99*avg_delta[k]+0.01*abs(p[k]-dc[k]);
    d_avg_delta[k]=0.99*d_avg_delta[k]*0.99+0.01*abs(p[k]-y[k][0]);
    
    if ((p[k]-dc[k])>avg_delta[k]*3&&(y[k][0]-dc[k])<avg_delta[k]*3&&2*d_avg_delta[k]<p[k]-y[k][0]){
      if (firstPulseFlag){
        firstPulseFlag=0;
      }else{
        T=0.6*T+0.4*(millis()-lastPulse);
      }
      pulse[k]=1;
      if(!k){pulse_counter+=1;}
      lastPulse=millis();
    }else{
      pulse[k]=0;
    }
    
    pushRegister(x[k],(float)u[k],4);
    pushRegister(y[k],p[k],4);
  }
  
  //buffer stuff
  
  if(buffer_pos==300){buffer_pos=0;}
  complex_buffer[buffer_pos]=p[0]-dc[0];
  buffer_pos++;

  //serial stuff
  
  String message=String(millis())+" ";
  for(int k=0;k<2;k++){ // izmjena za test, za sva 3 kanala je k<3
    message+=String(p[k]-dc[k])+" ";
  }
  message+=String(u[0])+" ";   //sirov signal za testiranje
  message+=String(pulse[0]);
  Serial.println(message);
  return pulse_counter;
  
}

void meanComplex(){} //ToDo

void loop() {
  
  //privremeni switch
  char mode='a';
  
  //mjerenje vremena
  unsigned long call=micros();                     //vrijeme poziva
  if (call<lastCall){                              //ispravak overflowa? (~70min za arduino uno, 16-bit ulong)
    call+=65536-lastCall;
    lastCall=0;
  }
  if(mode=='a'&&call-lastCall>4000){                          //okidam uzorkovanje ako je proĹˇlo 4ms
    lastCall=call;                                 //biljeĹľim vrijeme
    if(onlineStep()%30==1){mode='b';}
  }else if (mode=='b'){
    
  }    
  
}

