#include <Arduino.h>
//#include <queue>

#define hall0 2 //interrupt pins for hall effect sensors

using namespace std;

// int count[] = {0}; //Actuator
//                    //int maxCounts = 1150;//number of counts when fully extended

// float speedM1 = 0;
// int time1 = 0;
// int time2 = 0;
// float previousPositionM1 = 0;
// float previousTime = 0;
// float motor1Speed = 0;
// float smoothedMotor1Speed = 0;
long debounceArray[3] {0, 0, 0};
long timerHSOne[5] = {0};
int timerHSOneCounter = 0;
bool firstTime = true;
long previousTime = 0;
long newTime = 0;
long HS1Array[10] = {0};
int HS1Counter = 0;
long timeDelta = 0;

// float 5avg = 0;
float avgTen = 0;
float avgTwentyFive = 0;
float avgFifty = 0;
float avgOneHundred = 0;
String motorState = "stop";

bool initialStabilization = false;

void setup()
{

  pinMode(hall0, INPUT);
  digitalWrite(hall0, HIGH);          //set hall, set low to start for rising edge
  attachInterrupt(0, speed0, RISING); //enable the hall effect interupts

  Serial.begin(9600);
  Serial1.begin(9600);
  // time1 = millis();
  Serial.println("Start ");
} //end setup

void loop()
{
  if (!firstTime) {
    if (checkDebounceArray(0) >= 200)
    {
      firstTime = true;
      HS1Counter = 0;
      for (int i = 0; i < 10; i++)
      {
        HS1Array[i] = 0;
      }
      Serial1.write(0);
      Serial.println("motorOff");
    }
  }

  if (timerHSOneCounter >= 1)
  {
    setDebounceArray(0);
    checkSpeed();
    // calculateAverages();
    checkJam();
  }
} //end loop

void checkSpeed()
{
  // could add counter here to check positions
  HS1Counter++;
  if (firstTime) // check to see if it is first hit since stop
  {
    previousTime = timerHSOne[0];
    timerHSOneCounter--;
    for (int i = 0; i < (timerHSOneCounter); i++)
    {
      timerHSOne[i] = timerHSOne[i + 1];
    }
    Serial1.write(1); 
    Serial.println("motorOn");
    firstTime = false;
  }
  else // if not first run since stop then compare the difference in time and store it into all arrays to recalculate the average
  {
    newTime = timerHSOne[0]; // store in the time of next interupt
    timerHSOneCounter--;
    for (int i = 0; i < (timerHSOneCounter); i++)
    {
      timerHSOne[i] = timerHSOne[i + 1];
    }
    timeDelta = newTime - previousTime; // calculate the time difference
    previousTime = newTime;             // store the new time as previous time for next use
    HS1Array[HS1Counter % 10] = timeDelta;
    // HS1Array[(HS1Counter - 1) % 101] = timeDelta;
    //    Serial.println(timerHSOneCounter);
    if (checkDebounceArray(2) > 1000)
    {
      Serial.println(timeDelta);
      setDebounceArray(2);
    }
  }
}
void checkJam()
{
  int temp = 0;
  for (int i = 0; i < 10; i++)
  {
    if (timeDelta >= 22000)
//    if (HS1Array[i] >= 23000)
    {
      temp++;
    }
  }
  if (temp >= 2)
  {
    Serial1.write(2);
    Serial.println("jam");
  }
}

// void calculateAverages()
// {

// //   avgOneHundred = (((HS1Array[(HS1Counter - 1) % 101] / 100) - (HS1Array[(HS1Counter + 99) % 101] / 100)) + avgOneHundred);
// //   avgFifty = (((HS1Array[(HS1Counter - 1) % 101] / 50) - (HS1Array[(HS1Counter + 49) % 101] / 50)) + avgFifty);
// //   avgTwentyFive = (((HS1Array[(HS1Counter - 1) % 101] / 25) - (HS1Array[(HS1Counter + 24) % 101] / 25)) + avgTwentyFive);
// //   avgTen = (((HS1Array[(HS1Counter - 1) % 101] / 10) - (HS1Array[(HS1Counter + 90) % 101] / 10)) + avgTen);
// // //  Serial.print("100 avg = ");
// // //  Serial.println(avgOneHundred);
// // //  Serial.print("50 avg = ");
// // //  Serial.println(avgFifty);
// // //  Serial.print("25 avg = ");
// // //  Serial.println(avgTwentyFive);
// //   Serial.print("10 avg = ");
// //   Serial.println(avgTen);
// }

// void checkJam()
// {
//   float jamPercent = .05;
//   float jamDelta = avgOneHundred * jamPercent;
//   if ((avgFifty - avgOneHundred) >= jamDelta)
//   {
//     jamPercent = .05;
//     jamDelta = avgFifty * jamPercent;
//     if ((avgTwentyFive - avgFifty) >= jamDelta)
//     {
//       jamPercent = .05;
//       jamDelta = avgTwentyFive * jamPercent;
//       if ((avgTen - avgTwentyFive) >= jamDelta)
//       {
// //        Serial.println("Jammed");
//       }
//     }
//   }
// }

void speed0()
{
  timerHSOne[timerHSOneCounter] = micros(); // store the milisecond time of the interupt into queue
  timerHSOneCounter++;

} //end speed0

void setDebounceArray(int index)
{
  debounceArray[index] = millis();
}

int checkDebounceArray(int index)
{
  return (millis() - debounceArray[index]);
}

// #include <Arduino.h>
// //#include <queue>

// #define hall0 2 //interrupt pins for hall effect sensors

// using namespace std;

// // int count[] = {0}; //Actuator
// //                    //int maxCounts = 1150;//number of counts when fully extended

// // float speedM1 = 0;
// // int time1 = 0;
// // int time2 = 0;
// // float previousPositionM1 = 0;
// // float previousTime = 0;
// // float motor1Speed = 0;
// // float smoothedMotor1Speed = 0;
// long debounceArray[3]{0, 0, 0};
// long timerHSOne[5] = {0};
// int timerHSOneCounter = 0;
// bool firstTime = true;
// long previousTime = 0;
// long newTime = 0;
// long HS1Array[101] = {0};
// int HS1Counter = 0;

// // float 5avg = 0;
// float avgTen = 0;
// float avgTwentyFive = 0;
// float avgFifty = 0;
// float avgOneHundred = 0;
// String motorState = "stop";

// bool initialStabilization = false;

// void setup()
// {

//   pinMode(hall0, INPUT);
//   digitalWrite(hall0, HIGH);          //set hall, set low to start for rising edge
//   attachInterrupt(0, speed0, RISING); //enable the hall effect interupts

//   Serial.begin(9600);
//   // time1 = millis();
//   Serial.println("Start ");
// } //end setup

// void loop()
// {
//   while (motorState == "stop") // waiting for movement
//   {
//     if (timerHSOneCounter != 0) // if its not empty
//     {
//       motorState = "moving";
//       checkSpeed();
//     }
//   }
//   while (motorState != "stop") // while motor is moving
//   {
//     checkSpeed();
//     if ((millis() - previousTime) >= 300) // check to see if it stops
//     {                                     //motor has stopped moving change states
//       motorState = "stop";
//       //      HS1Array = {0};
//       //set all averages to 0?
//     }

//     checkJam();
//     //check to see if it reaches first stabilization // skip after it reaches stable state if it never reaches throw error

//     // check to see if it meets first resistance

//     // check for jams
//   }
// } //end loop

// void checkSpeed()
// {
//   // could add counter here to check positions
//   HS1Counter++;
//   if (firstTime) // check to see if it is first hit since stop
//   {
//     previousTime = timerHSOne[0];
//     timerHSOneCounter--;
//     for (int i = 0; i < (timerHSOneCounter); i++)
//     {
//       timerHSOne[i] = timerHSOne[i + 1];
//     }
//     firstTime = false;
//   }
//   else // if not first run since stop then compare the difference in time and store it into all arrays to recalculate the average
//   {
//     newTime = timerHSOne[0]; // store in the time of next interupt
//     timerHSOneCounter--;
//     for (int i = 0; i < (timerHSOneCounter); i++)
//     {
//       timerHSOne[i] = timerHSOne[i + 1];
//     }
//     long timeDelta = newTime - previousTime; // calculate the time difference
//     previousTime = newTime;                  // store the new time as previous time for next use
//     HS1Array[(HS1Counter - 1) % 101] = timeDelta;
//     Serial.println("time Delta = " + timeDelta);
//   }
// }

// void calculateAverages(int size)
// {

//   avgOneHundred = (((HS1Array[(HS1Counter - 1) % 101] / 100) - (HS1Array[(HS1Counter) % 101] / 100)) + avgOneHundred);
//   avgFifty = (((HS1Array[(HS1Counter - 1) % 101] / 50) - (HS1Array[(HS1Counter + 49) % 101] / 50)) + avgFifty);
//   avgTwentyFive = (((HS1Array[(HS1Counter - 1) % 101] / 25) - (HS1Array[(HS1Counter + 24) % 101] / 25)) + avgTwentyFive);
//   avgTen = (((HS1Array[(HS1Counter - 1) % 101] / 10) - (HS1Array[(HS1Counter + 9) % 101] / 10)) + avgTen);
// }

// void checkInitialStabilization()
// {

//   //@ 150 count stabalized?
//   // float stabilizationPercent = .05;
//   // float stabilizationDelta = 24avg * stabilizationPercent;
//   // if ((96avg - 24avg) <= stabilizationDelta)
//   // {
//   //   initialStabilization = true;
//   // }
// }

// void checkJam()
// {
//   float jamPercent = .05;
//   float jamDelta = avgOneHundred * jamPercent;
//   if ((avgFifty - avgOneHundred) >= jamDelta)
//   {
//     jamPercent = .05;
//     jamDelta = avgFifty * jamPercent;
//     if ((avgTwentyFive - avgFifty) >= jamDelta)
//     {
//       jamPercent = .05;
//       jamDelta = avgTwentyFive * jamPercent;
//       if ((avgTen - avgTwentyFive) >= jamDelta)
//       {
//         Serial.println("Jammed");
//       }
//     }
//   }
// }
// void speed0()
// {
//   timerHSOne[timerHSOneCounter] = millis(); // store the milisecond time of the interupt into queue
//   timerHSOneCounter++;

// } //end speed0

// void setDebounceArray(int index)
// {
//   debounceArray[index] = millis();
// }

// int checkDebounceArray(int index)
// {
//   return (millis() - debounceArray[index]);
// }

// #include <Arduino.h>
// // #include <queue>

// #define hall0 2 //interrupt pins for hall effect sensors

// using namespace std;

// // int count[] = {0}; //Actuator
// //                    //int maxCounts = 1150;//number of counts when fully extended

// // float speedM1 = 0;
// // int time1 = 0;
// // int time2 = 0;
// // float previousPositionM1 = 0;
// // float previousTime = 0;
// // float motor1Speed = 0;
// // float smoothedMotor1Speed = 0;
// long debounceArray[3]{0, 0, 0};
// long timerHSOne[5] = {0};
// int timerHSOneCounter = 0;
// bool firstTime = true;
// long previousTime = 0;
// long newTime = 0;
// long HS1Array[101] = {0};
// int HS1Counter = 0;

// // float 5avg = 0;
// float 10avg = 0;
// float 25avg = 0;
// float 50avg = 0;
// float 100avg = 0;
// string motorState = "stop";

// bool initialStabilization = false;

// void setup()
// {

//   pinMode(hall0, INPUT);
//   digitalWrite(hall0, HIGH);          //set hall, set low to start for rising edge
//   attachInterrupt(0, speed0, RISING); //enable the hall effect interupts

//   Serial.begin(9600);
//   // time1 = millis();

// } //end setup

// void loop()
// {
//   while (motorState == "stop") // waiting for movement
//   {
//     if (timerHSOneCounter != 0) // if its not empty
//     {
//       motorState = "moving";
//       checkSpeed();
//     }
//   }
//   while (motorState != "stop") // while motor is moving
//   {
//     if ((millis() - previousTime) >= 300) // check to see if it stops
//     {                                     //motor has stopped moving change states
//       motorState = "stop";
//       HS1Array = {0};
//       //set all averages to 0?
//     }

//     checkJam();
//     //check to see if it reaches first stabilization // skip after it reaches stable state if it never reaches throw error

//     // check to see if it meets first resistance

//     // check for jams
//   }
// } //end loop

// void checkSpeed()
// {
//   // could add counter here to check positions
//   HS1Counter++;
//   if (firstTime) // check to see if it is first hit since stop
//   {
//     previousTime = timerHSOne[0];
//     timerHSOneCounter--;
//     for (int i = 0; i < (timerHSOneCounter); i++)
//     {
//       timerHSOne[i] = timerHSOne[i + 1];
//     }
//     firstTime = false;
//   }
//   else // if not first run since stop then compare the difference in time and store it into all arrays to recalculate the average
//   {
//     newTime = timerHSOne[0]; // store in the time of next interupt
//     timerHSOneCounter--;
//     for (int i = 0; i < (timerHSOneCounter); i++)
//     {
//       timerHSOne[i] = timerHSOne[i + 1];
//     }
//     long timeDelta = newTime - previousTime; // calculate the time difference
//     previousTime = newTime;                  // store the new time as previous time for next use
//     HS1Array[(HS1Counter - 1) % 101] = timeDelta;
//     Serial.println("time Delta = " + timeDelta);
//   }
// }

// void calculateAverages(int size)
// {

//   100avg = (((HS1Array[(HS1Counter - 1) % 101] / 100) - (HS1Array[(HS1Counter) % 101] / 100)) + 100avg);
//   50avg = (((HS1Array[(HS1Counter - 1) % 101] / 50) - (HS1Array[(HS1Counter + 49) % 101] / 50)) + 50avg);
//   25avg = (((HS1Array[(HS1Counter - 1) % 101] / 25) - (HS1Array[(HS1Counter + 24) % 101] / 25)) + 25avg);
//   10avg = (((HS1Array[(HS1Counter - 1) % 101] / 10) - (HS1Array[(HS1Counter + 9) % 101] / 10)) + 10avg);
// }

// void checkInitialStabilization()
// {

//   //@ 150 count stabalized?
//   // float stabilizationPercent = .05;
//   // float stabilizationDelta = 24avg * stabilizationPercent;
//   // if ((96avg - 24avg) <= stabilizationDelta)
//   // {
//   //   initialStabilization = true;
//   // }
// }

// void checkJam()
// {
//   float jamPercent = .05;
//   float jamDelta = 100avg * jamPercent;
//   if ((50avg - 100avg) >= jamDelta)
//   {
//     jamPercent = .05;
//     jamDelta = 50avg * jamPercent;
//     if ((25avg - 50avg) >= jamDelta)
//     {
//       jamPercent = .05;
//       jamDelta = 25avg * jamPercent;
//       if ((10avg - 25avg) >= jamDelta)
//       {
//         Serial.println("Jammed");
//       }
//     }
//   }
// }
// void speed0()
// {
//   timerHSOne[timerHSOneCounter] = millis(); // store the milisecond time of the interupt into queue
//   timerHSOneCounter++;
// } //end speed0

// void setDebounceArray(int index)
// {
//   debounceArray[index] = millis();
// }

// int checkDebounceArray(int index)
// {
//   return (millis() - debounceArray[index]);
// }
