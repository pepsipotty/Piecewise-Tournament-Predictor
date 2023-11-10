#include "predictor.h"
#include <cmath>
#define ADDR_LEN 6// 6 bits
#define GA_LEN 6 // 6 bits
#define HIST_LEN 15 // history length
#define WEIGHT_WIDTH 8 

/////////////// STORAGE BUDGET JUSTIFICATION ////////////////


// Number of entries for PC (ADDR_LEN): 2^6  --- A
// Number of entries for Global Address (GA_LEN): 2^6 --- B
// Number of weights for each combination of PC and Global Address(HIST_LEN + 1): 15 + 1 = 16 --- C
// Each Weight size(WEIGHT_WIDTH): 8 bits. --- D
// Total size for W: A x B x C x D bits = 524288 bits --- E

// Total Global History Register size(HIST_LEN x bool): 15 x 1 bits --- F
// Total Global Address size(HIST_LEN x GA_LEN): 15 x 6 bits --- G
// Total size for GHR and GA: F + G bits = 135 bits --- H

// Total Storage Budget in bits: E + H bits = 524393 bits
// Total Storage Budget in bytes: 524393 / 8192 = 64.01 kilobytes


/////////////////////////////////////////////////////////////

PREDICTOR::PREDICTOR(void){
  // This implementation requires the following variables to be declared and initialized:
  // Global Address: GA
  // Global History Register: GHR
  // Weights: W
  MIN_VAL = ((1 << WEIGHT_WIDTH)- 1) * -1;
  MAX_VAL = ((1 << WEIGHT_WIDTH) - 1) * 1;
  theta = static_cast<UINT32>(floor(2.14 * (HIST_LEN + 1)) + 20.58);

  initWeights(); // Initialize W
  initGlobalHistoryRegister(); // Initialize GHR
  initGlobalAddress(); // Initialize GA
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

bool   PREDICTOR::GetPrediction(UINT32 PC){
    int k = PC % (1 << ADDR_LEN);

    output = W[k][0][0];

    for (UINT32 i = 1; i < HIST_LEN; i++) {
        
        int j = GA[i] % (1 << GA_LEN);

        if (GHR[i] == true) {

          output += W[k][j][i+1];

        } else {

          output -= W[k][j][i+1];

        }

    }

    bool prediction = (output >= 0) ? TAKEN : NOT_TAKEN;
    return prediction;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void  PREDICTOR::UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget){
   UINT32 k = PC % (1 << ADDR_LEN); 

   if (abs(output) < theta || predDir != resolveDir) {

      if (resolveDir == TAKEN) {

          if (W[k][0][0] < MAX_VAL) {

            W[k][0][0] += 1;

          }

      } else {

          if (W[k][0][0] > MIN_VAL) {

            W[k][0][0] -= 1;

          }

      }

      for (UINT32 i = 0 ; i < HIST_LEN ; i++)
        {

          UINT32 j = GA[i] % (1 << ADDR_LEN);

            if (GHR[i] == resolveDir)
            { 
              if( W[k][j][i+1] < MAX_VAL) {

                    W[k][j][i+1] += 1; 
                }
            }
            else
            {
              if( W[k][j][i+1] > MIN_VAL) {

                    W[k][j][i+1] -= 1; 

                }

            }
        }

    } 

    for (UINT32 i = HIST_LEN - 1 ; i >= 1 ; i--){

        GA[i] = GA[i-1];
        GHR[i] = GHR[i-1];

    }

    GA[0] = PC;
    GHR[0] = resolveDir;
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

void PREDICTOR::initWeights() {
  for (UINT32 i = 0; i < (1 << ADDR_LEN); i++) {
      for (UINT32 j = 0; j < (1 << GA_LEN); j++) {
          for (UINT32 k = 0; k < (HIST_LEN + 1); k++) {

              W[i][j][k] = 0;

          }
      }
  }
}

void PREDICTOR::initGlobalHistoryRegister() {
    for (UINT32 i = 0; i < HIST_LEN; i++) {

      GHR[i] = false;

    } 
  }

void PREDICTOR::initGlobalAddress() {
    for (UINT32 i = 0; i < HIST_LEN; i++) {

      GA[i] = 0;

    }
}

