#ifndef _PREDICTOR_H_
#define _PREDICTOR_H_

#include "utils.h"
#include "tracer.h"

#define UINT16      unsigned short int
#define GLOBAL_HISTORY_LENGTH 8//8 bits
#define LOCAL_HISTORY_LENGTH 8 //8 bits
#define HIST_LEN 15 //history length



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

class PREDICTOR{

  // The state is defined for Gshare, change for your design

 private:
  UINT32  *pht;          // pattern history table
  UINT32  historyLength; // history length
  UINT32  numPhtEntries; // entries in pht


  //add for local predictor
  //local pattern history table
  //UINT32 pht_local_bit_size;
  UINT32 *pht_local;
  UINT32 numPhtLocalEntries;
  UINT32 W[1 << GLOBAL_HISTORY_LENGTH][1 << LOCAL_HISTORY_LENGTH][HIST_LEN + 1];
  UINT32 GHR[HIST_LEN]; // global history register
  UINT32 GA[HIST_LEN];
  int MIN_VAL;
  int MAX_VAL;
  int theta;


  //branch history table for local branch predictor
  UINT32 bht_history_length;
  UINT32 numBhtEntries;
  UINT32 bht_bit_size;
  UINT16 *bht;

  //for tournament counter
  UINT32 *predictorChooseCounter;
  UINT32 numTournamentCounter;


 public:

  // The interface to the four functions below CAN NOT be changed

  PREDICTOR(void);
  bool    GetPrediction(UINT32 PC);

  //add for tournament predictor
  bool    GetLocalPrediction(UINT32 PC);
  bool    GetGlobalPrediction(UINT32 PC);

  void    UpdatePredictor(UINT32 PC, bool resolveDir, bool predDir, UINT32 branchTarget);
  void    TrackOtherInst(UINT32 PC, OpType opType, UINT32 branchTarget);
  void    trainPredictor(UINT32 PC, bool prediction);
  int     output;

  // Helper functions
  void initWeights();
  void initGlobalHistoryRegister();
  void initGlobalAddress();


  // Contestants can define their own functions below

};


/***********************************************************/
#endif

