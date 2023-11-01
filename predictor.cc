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
  // This implementation requires the following variables to be declared and initialized:
  // Global Address: GA
  // Global History Register: GHR
  // Weights: W
  MIN_VAL = ((1 << HIST_LEN) - 1) * -1;
  MAX_VAL = ((1 << HIST_LEN) - 1) * 1;

  theta = static_cast<UINT32>(2.14 * (HIST_LEN + 1)) + 20.58;

  initWeights(); // Initialize W
  initGlobalHistoryRegister(); // Initialize GHR
  initGlobalAddress(); // Initialize GA
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool   PREDICTOR::GetPrediction(UINT32 PC){
    int k = PC % (1 << GLOBAL_HISTORY_LENGTH);

    output = W[k][0][0];

    for (UINT32 i = 1; i <= HIST_LEN; i++) {

        int j = GA[i] % (1 << LOCAL_HISTORY_LENGTH);

        if (GHR[i]) {


          output += W[k][j][i+1];


        } else {

          output += W[k][j][i+1];

        }

    }
    bool prediction = (output >= 0) ? TAKEN : NOT_TAKEN;
    return prediction;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void  PREDICTOR::UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget){
   UINT32 k = PC % (1 << GLOBAL_HISTORY_LENGTH); 

   if (abs(output) < theta || predDir != resolveDir) {

      if (resolveDir == true) {

          if (W[k][0][0] < MAX_VAL) {

            W[k][0][0] += 1;

          }

      } else {

          if (W[k][0][0] > MIN_VAL) {

            W[k][0][0] -= 1;

          }

      }

/////////////////////////////////////////////////////////////

      for (UINT32 i = 0 ; i < HIST_LEN ; i++)
        {

          UINT32 j = GA[i] % GLOBAL_HISTORY_LENGTH;

            if (GHR[i] == resolveDir)
            { 
              if( W[k][j][i+1] < MAX_VAL) 
                {

                    W[k][j][i+1] += 1 ; 
                }
            }
            else
            {
              if( W[k][j][i+1] > MIN_VAL) 
                {

                    W[k][j][i+1] -= 1 ; 
                }

            }
        }

    } 

    for (UINT32 i = HIST_LEN - 1 ; i >= 1 ; i--){

        GA[i] = GA[i-1];
        GHR[i] = GHR[i-1];

    }

    GA[0] = PC;
    GHR[0] = predDir;
}



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void PREDICTOR::TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget) {
  // This function is called for instructions which are not
  // conditional branches, just in case someone decides to design
  // a predictor that uses information from such instructions.
  // We expect most contestants to leave this function untouched.
  return;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// My beloved helper functions to keep the code above clean



/*
 
  #     # ####### #       ######  ####### ######  
  #     # #       #       #     # #       #     # 
  #     # #       #       #     # #       #     # 
  ####### #####   #       ######  #####   ######  
  #     # #       #       #       #       #   #   
  #     # #       #       #       #       #    #  
  #     # ####### ####### #       ####### #     # 
                                                  
 
*/

void PREDICTOR::initWeights() {
  UINT16 n = (1 << GLOBAL_HISTORY_LENGTH);
  UINT16 m = (1 << LOCAL_HISTORY_LENGTH);
  UINT32 W[n][m][HIST_LEN + 1]; 

  for (UINT32 i = 0; i < (1 << GLOBAL_HISTORY_LENGTH); i++) {
      for (UINT32 j = 0; j < (1 << LOCAL_HISTORY_LENGTH); j++) {
          for (UINT32 k = 0; k < (HIST_LEN + 1); k++) {

              W[i][j][k] = 0;

          }
      }
  }
}

void PREDICTOR::initGlobalHistoryRegister() {
    for (UINT32 i = 0; i < HIST_LEN; i++) {

      GHR[i] = 0;

    } 
  }

void PREDICTOR::initGlobalAddress() {
    for (UINT32 i = 0; i < HIST_LEN; i++) {

      GA[i] = 0;

    }
}

