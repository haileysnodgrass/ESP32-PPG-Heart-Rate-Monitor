/*********************** Heart Rate Constants ********************************/
#define BUFFER_LENGTH           300   // buffer size
#define BUFFER_MIN_PEAK_WIDTH   300   // milliseconds
#define BUFFER_PPG_SAMPLE_RATE  30    // milliseconds

#define HR_CALC_RATE            300   // samples

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println(F("A Better Heart Rate Monitor"));
}

int hr_count = 0;
int const PULSE_SENSOR_PIN = 36;
int vec[BUFFER_LENGTH] = {0};
unsigned long sampleTime[BUFFER_LENGTH] = {0};
int pos = 0;
float heartRate=100.0;
float lastHeartRate = 100.0;

void loop() {

  pos = (pos + 1) % BUFFER_LENGTH;
  vec[pos] = analogRead(PULSE_SENSOR_PIN);  
  sampleTime[pos] = millis();


  hr_count = (hr_count + 1) % HR_CALC_RATE;
  if (hr_count == 0) {
    Calculate_Heart_Rate();

    if ((heartRate >= 30.0) && (heartRate <= 200.0)){
      Serial.println(heartRate);
    } else {
      Serial.println("Acquiring Signal");
    }
    
  }

  delay(10);
}



// Function to calculate heart Rate (based on measured values)
uint32_t Calculate_Heart_Rate(){

  // find all potential peaks
  int numDiff = 0;
  unsigned long lastTime = 0;
  int peakTimeDiff[BUFFER_LENGTH] = {0};
  bool flag = true;
  for (int i=0; i<BUFFER_LENGTH; i++){
    int curIdx = (pos+i) % BUFFER_LENGTH;
    flag = true;
    for (int j=0; j<BUFFER_LENGTH; j++){
      int diff = sampleTime[curIdx] - sampleTime[j];
      if ((vec[curIdx] < vec[j]) && (abs(diff) < BUFFER_MIN_PEAK_WIDTH)) {
        flag = false;
      }
    }

    if (flag) {
      if (lastTime > 0){
        peakTimeDiff[numDiff] = sampleTime[curIdx] - lastTime;
        numDiff++;
      } 
      lastTime = sampleTime[curIdx];
    }
  }

  // Calculate median of peak difference
  int medianIdx = round(numDiff/2);
  unsigned long maxDiff = 0;
  for (int i=0; i<medianIdx; i++){
    maxDiff = 0;
    int maxDiffIdx = -1;
    for (int j=0; j<numDiff; j++){
      if (maxDiff < peakTimeDiff[j]){
        maxDiff = peakTimeDiff[j];
        maxDiffIdx = j;
      }
    }
    peakTimeDiff[maxDiffIdx] = 0;
  }

  if (maxDiff == 0) return 0;
  heartRate = 60000.0/(float(maxDiff));

  return heartRate;
}




