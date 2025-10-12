//compile time flags
#define PROD 0
#define INFO_PRINT 1
#define WEB_PAGE 0
#define BASE_FEEDBACK_FLOW 1
#define API_V1 0
#define API_V2 1
#if PROD
  #define INFO_PRINT 0
  #define WEB_PAGE 0
  #define BASE_FEEDBACK_FLOW 0
#endif

#if INFO_PRINT
  // 2: flow feedback.
  // 1: [default]print debug info in serial.
  // 0: don't print debug info in serial.
  byte InfoPrint = 0;
#endif


// devices info:

// run new json cmd
bool runNewJsonCmd = false;

// 1: RaspRover
// 2: UGV Rover
// 3: UGV Beast
byte mainType = 2;

// 0: [Base default] without additional modules.
byte moduleType = 0;

#if BASE_FEEDBACK_FLOW
// 0: turn off base info feedback flow.
// 1: [default] turn on base info feedback flow.
bool baseFeedbackFlow = 0;
#endif

String thisMacStr;

// the uart used to control servos.
// GPIO 18 - S_RXD, GPIO 19 - S_TXD, as default.
#define S_RXD 18
#define S_TXD 19

bool newCmdReceived = false;

// --- --- --- Pneumatic Components && Lights --- --- ---
const uint16_t ANALOG_WRITE_BITS = 8;
const uint16_t MAX_PWM = pow(2, ANALOG_WRITE_BITS)-1;
const uint16_t MIN_PWM = MAX_PWM/4;

#define PWMA 25         // Motor A PWM control  
#define AIN2 17         // Motor A input 2     
#define AIN1 21         // Motor A input 1     
#define BIN1 22         // Motor B input 1       
#define BIN2 23         // Motor B input 2       
#define PWMB 26         // Motor B PWM control  

#define AENCA 35        // Encoder A input      
#define AENCB 34

#define BENCB 16        // Encoder B input     
#define BENCA 27

int freq = 100000;
int channel_A = 5;
int channel_B = 6;

// --- --- --- i2c Settings --- --- ---
#define S_SCL   33
#define S_SDA   32

//  --- --- --- web / constant moving --- --- ---
#define MOVE_STOP 0
#define MOVE_INCREASE 1
#define MOVE_DECREASE 2

#define CONST_ANGLE 0
#define CONST_XYZT  1

float const_spd;
byte  const_mode;

unsigned long prev_time = 0;

String jsonFeedbackWeb = "";

//  --- --- --- pid controller --- --- ---
float __kp = 20.0;
float __ki = 2000.0;
float __kd = 0;
float windup_limits = 255;

//  --- --- --- ugv base --- --- ---
#define THRESHOLD_PWM 23

// mainType:01 RaspRover
// #define WHEEL_D 0.0800
// #define ONE_CIRCLE_PLUSES	2100
// #define TRACK_WIDTH	0.125
// #define SET_MOTOR_DIR false

// mainType:02 UGV Rover
// #define WHEEL_D 0.0800
// #define ONE_CIRCLE_PLUSES	660
// #define TRACK_WIDTH	0.172
// #define SET_MOTOR_DIR false

// mainType:03 UGV Beast
// #define WHEEL_D	0.0523
// #define ONE_CIRCLE_PLUSES	1092
// #define TRACK_WIDTH	0.141
// #define SET_MOTOR_DIR true

double WHEEL_D = 0.0800;
int ONE_CIRCLE_PLUSES = 660;
double TRACK_WIDTH = 0.172;
bool SET_MOTOR_DIR = false;

#define IO4_PIN 4
#define IO5_PIN 5

int IO4_CH = 7;
int IO5_CH = 8;

const uint16_t FREQ = 200;

int feedbackFlowExtraDelay = 1000;
bool uartCmdEcho = 0;

#define SERVO_STOP_DELAY 3

int HEART_BEAT_DELAY = 3000;
unsigned long lastCmdRecvTime = millis();

// --- --- --- ugv imu --- --- ---
double icm_pitch = 0;
double icm_roll = 0;
double icm_yaw = 0;

// IMU scale factors
#define IMU_ACCEL_SCALE_FACTOR 16384.0  // Scale factor for ±2g range (16-bit values)

// IMU safety limits
#define IMU_PITCH_LIMIT_RADIANS 0.6  // Maximum allowed pitch angle in radians (≈34.4°)

float icm_temp;
unsigned long last_imu_update = 0;

double ax, ay, az;
double mx, my, mz;
double gx, gy, gz;

double en_odom_l, en_odom_r;

unsigned long imu_last_time = micros();

int sample_count = 240;

double gx_offset = 0;
double gy_offset = 0;
double gz_offset = 0;
double gyro_h = 0.04;

float kf_accel_q = 1.0;
float kf_accel_r = 0.1;
float kf_accel_p = 0.1;

double ax_offset = 0;
double ay_offset = 0;
double az_offset = 0;
double accel_h = 0.01;

double q0, q1, q2, q3, q2sqr, t0, t1, t2, t3, t4;

double qc0 = 1.0;
double qc1 = 0.0;
double qc2 = 0.0;
double qc3 = 0.0;

float max_ax = 0;
float min_ax = 0;

// int32_t biasGyroX, biasGyroY, biasGyroZ;
// int32_t biasAccelX, biasAccelY, biasAccelZ;
// int32_t biasCPassX, biasCPassY, biasCPassZ;

// Define a storage struct for the biases. Include a non-zero header and a simple checksum
struct biasStore
{
  int32_t biasGyroX = 0;
  int32_t biasGyroY = 0;
  int32_t biasGyroZ = 0;
  int32_t biasAccelX = 0;
  int32_t biasAccelY = 0;
  int32_t biasAccelZ = 0;
  int32_t biasCPassX = 0;
  int32_t biasCPassY = 0;
  int32_t biasCPassZ = 0;
};

biasStore store;