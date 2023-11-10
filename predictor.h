#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include "utils.h"
#include "tracer.h"

#define UINT16      unsigned short int
#define ADDR_LEN 8//8 bits
#define GA_LEN 8 //8 bits
#define HIST_LEN 15 //history length
#define WEIGHT_WIDTH 8

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

class PREDICTOR{

 private:
  UINT32 GHR[HIST_LEN]; // global history register
  intptr_t GA[HIST_LEN];
  int W[1 << ADDR_LEN][1 << GA_LEN][HIST_LEN + 1];
  int MIN_VAL;
  int MAX_VAL;
  int theta;


 public:

  // The interface to the four functions below CAN NOT be changed

  PREDICTOR(void);
  bool    GetPrediction(UINT32 PC);

    //add for tournament predictor
  bool    GetLocalPrediction(UINT32 PC);
  bool    GetGlobalPrediction(UINT32 PC);

  void    UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget);
  void    TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget);

  // Contestants can define their own functions below
  int output;
  void initWeights();
  void initGlobalHistoryRegister();
  void initGlobalAddress();
};


/***********************************************************/
#endif

