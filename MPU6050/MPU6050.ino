
#include <Wire.h>

int MPU = 0x68; 
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;

void setup() {
  Serial.begin(9600);
  Wire.begin();               

  // Reset The Register 0x6B
  Wire.beginTransmission(MPU);       
  Wire.write(0x6B);                  
  Wire.write(0);                  
  Wire.endTransmission();        
  
  //  Accelerometer Sensitivity:
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);                 
  Wire.write(0x10);                  
  Wire.endTransmission();


  
  //  Gyro Sensitivity :
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);                   
  Wire.write(0x10);                   
  Wire.endTransmission();
  
}

void loop() {

  // Requesting the reading of The Accelerometer 
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); 
  Wire.endTransmission();
  Wire.requestFrom(MPU, 6); 


  

  AccX = (Wire.read() * 256 + Wire.read()) / 4096.0; // X-axis value
  AccY = (Wire.read() * 256 + Wire.read()) / 4096.0; // Y-axis value
  AccZ = (Wire.read() * 256 + Wire.read()) / 4096.0; // Z-axis value
  
  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + 1.58; // AccErrorY ~(-1.58)


  // Calculating The elapsed Time
  previousTime = currentTime;        
  currentTime = millis();           
  elapsedTime = (currentTime - previousTime) / 1000; // Convert From ms to seconds


  // Requesting the reading of The Gyroscope
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(MPU, 6); 
  GyroX = (Wire.read() * 256 + Wire.read()) / 32.8;
  GyroY = (Wire.read() * 256 + Wire.read()) / 32.8;
  GyroZ = (Wire.read() * 256 + Wire.read()) / 32.8;


  // Handling Errors
  GyroX = GyroX + 0.56; 
  GyroY = GyroY - 2;
  GyroZ = GyroZ + 0.79; 



  gyroAngleX = gyroAngleX + GyroX * elapsedTime;
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  yaw =  yaw + GyroZ * elapsedTime;


  // Adding Most of the value of the Gyroscope with small amount of the Accelerometer 
  roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;



  
  // Print the values 
  Serial.print(roll);
  Serial.print("/");
  Serial.print(pitch);
  Serial.print("/");
  Serial.println(yaw);
}
