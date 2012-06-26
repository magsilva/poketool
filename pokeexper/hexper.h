#include "newpokelib.h"

#ifndef _EXPER_

#define _EXPER_

#define MAXLINE 1200

#define TPC      1
#define TPR      2
#define TPF      3
#define TGC      4
#define TGR      5
#define TGF      6
#define MTC      7
#define MTR      8
#define MTF      9
#define TGDC     10
#define TGDR     11

/* Data Structure to store Coverage Information */

typedef struct coverage {
   char * funcname;
   b_vector features;
   int criterion;
   int maxfeatures;
} Coverage;

/* Data Structure to store Test Case Sets */

typedef struct testset {
   int no_test_set;
   double coverage;
   b_vector testcases;
} Testset;

/* Data Structure for Scenario Information */

typedef struct scen {
  int scenario;
  int difftestsets; 
  int testsetsize;
  int sizetestpool;
  int no_trials;
  int   fault_version;
  double desiredcover; 
  Coverage * maxcover, * curcover, *testncover, ** testscover;
  Testset * selectdiffsets;
  b_vector output_info;
  element * funcs;
  char * criterion;
  char * output_file;
} Scenario;
int IsThereAnFaultRevealingTestCase(Testset *,b_vector *, int, int, int);
char * le_linha_gen(FILE * );
char * le_linha_str(char *,char * ,FILE * );
char * le_linha_str_exper(char *,char *, FILE *);
char * le_no_feature(char *, int *);
int get_crit_number(char *);
int get_scenario(char *);
char * get_des_file(char *,char*);
char * get_freq_file(char *, char *, int);
char * get_cases_file(char *, char *);
Coverage ** InitializeMaxCoverage(element *,char *, int, Coverage **);
Coverage * InitializeCurCoverage(element *, char *);
void RefreshCurCoverage(Coverage *,element *);
Coverage * GetTestCoverage(Coverage *,Coverage **,int,element *);
double CurrentCoverage(Coverage *,element * );
int IsCoverageImproved(Coverage *, Coverage  *, element *);
void AddFeatures(Coverage *, Coverage *, element *);
Testset * InitializeSelectDiffSets(int, int);
void SetTestNSelectDiffSets(Testset *,int,int,int,int);
int IsDesiredCoverage(element *, Coverage *,double);
int SelectRandomTestCase(Testset *,int,int,int);
element * GetFuncsNames(char *[],int *);
void TGStructuralCriterionScenario(Scenario *);
void TGRandomScenario(Scenario *);
void TGDebugStructuralCriterionScenario(Scenario *);
void TGDebugRandomScenario(Scenario *);
void TPStructuralCriterionScenario(Scenario *);
void TPRandomScenario(Scenario *);
void MTStructuralCriterionScenario(Scenario *);
void GetScenFixedPars(Scenario *,int *,char *[]);
void GetScenCritPars(Scenario *,int *,char * argv[]);
void InitDSCrit(Scenario *);
void CheckFaultDetectionAbility(Scenario *);
void PrintOutput(Scenario *);

#endif




