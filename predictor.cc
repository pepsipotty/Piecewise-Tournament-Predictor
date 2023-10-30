#include "predictor.h"

//sengwei: Idealized Piecewise Linear Branch Prediction
#define GLOBAL_HISTORY_LENGTH 8//8 bits
#define LOCAL_HISTORY_LENGTH 8 //8 bits
#define HIST_LEN 15 //history length


// #define PHT_CTR_MAX  3
// //chuan: for tournament predictor
// #define TOURNAMENT_CTR_MAX 3
// #define PHT_CTR_INIT 2

// #define TOUR_LEN   15
// #define BHT_BIT_SIZE 12
// #define BHT_HIST_LENGTH 15
// #define PHT_LOCAL_CTR_INIT 2
// #define PHT_LOCAL_CTR_MAX  3
// #define UINT16      unsigned short int

/////////////// STORAGE BUDGET JUSTIFICATION ////////////////
// Total storage budget: 32KB + 32 bits

// Total PHT counters for Global predictor: 2^15
// Total PHT size for global predictor = 2^15 * 2 bits/counter = 2^16 bits = 8KB
// GHR size for global predictor: 32 bits

// Total PHT counters for local predictor: 2^15
// Total PHT size for local predictor = 2^15 * 2 bits/counter = 2^16 bits = 8KB
// Total BHT size for local predictor = 2^12 * 16 bits/counter = 2^16 bits = 8KB
// Total Size for local predictor = 8KB + 8KB = 16KB

// Total Tournament counters is: 2^15
// Total Tournament counter's size = 2^15 * 2 bits/counter = 2^16 bits = 8KB
/////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PREDICTOR::PREDICTOR(void){
  
  initWeights();
  initGlobalHistoryRegister();


}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool   PREDICTOR::GetPrediction(UINT32 PC){
    int output = W[PC][0][0];
    for (int i = 1; i <= h; i++) {
        if (GHR[i]) {
            output += W[PC][GA[i]][i];
        } else {
            output -= W[PC][GA[i]][i];
        }
    }
    bool prediction = (output >= 0) ? TAKEN : NOT_TAKEN;
    return prediction;
}


//for global predictor
bool   PREDICTOR::GetGlobalPrediction(UINT32 PC){
    UINT32 phtIndex   = (PC^ghr) % (numPhtEntries);
    UINT32 phtCounter = pht[phtIndex];
    if(phtCounter > PHT_CTR_MAX/2){
        return TAKEN;
    }else{
        return NOT_TAKEN;
    }
}

//for local predictor
bool   PREDICTOR::GetLocalPrediction(UINT32 PC){
    UINT32 bhtIndex   = (PC >> (32-bht_bit_size));
    UINT16 bht_result = bht[bhtIndex];
    UINT32 pht_local_index = (PC^(UINT32)(bht_result))% (numPhtLocalEntries);

    if(pht_local[pht_local_index] > PHT_LOCAL_CTR_MAX/2){
        return TAKEN;
    }else{
        return NOT_TAKEN;
    }
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void  PREDICTOR::UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget){

 


}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void    PREDICTOR::TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget){

  // This function is called for instructions which are not
  // conditional branches, just in case someone decides to design
  // a predictor that uses information from such instructions.
  // We expect most contestants to leave this function untouched.

  return;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// My beloved helper functions to keep the code above clean

void PREDICTOR::initWeights() {
  UINT16 n = (1 << GLOBAL_HISTORY_LENGTH);
  UINT16 m = (1 << LOCAL_HISTORY_LENGTH);
  UINT32 W[n][m][HIST_LEN + 1]; 
  GHR = 0; // Global history register.

  for (UINT32 i = 0; i < (1 << GLOBAL_HISTORY_LENGTH); i++) {
      for (UINT32 j = 0; j < (1 << LOCAL_HISTORY_LENGTH); j++) {
          for (UINT32 k = 0; k < (HIST_LEN + 1); k++) {
              W[i][j][k] = 0;
          }
      }
  }
}

void PREDICTOR::initGlobalHistoryRegister() {
  GHR = 0;
}
