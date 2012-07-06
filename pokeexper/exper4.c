/*
* Copyright 2012 Marcos Lordello Chaim, José Carlos Maldonado, Mario Jino, 
* CCSL-ICMC <ccsl.icmc.usp.br>
* 
* This file is part of POKE-TOOL.
* 
* POKE-TOOL is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "util.h"
#include "newpokelib.h"
#include "hexper.h"
#include "haval.h"

void TGStructuralCriterionScenario(sc)
Scenario * sc;
{
  int setno, testn, trials;
  unsigned int lastseed=0, seed=0;

  setno = 0;
  trials = 0;
  while((setno <= sc->difftestsets-1) && trials < sc->no_trials)
   {
   /* Select new seed */
   
   seed = time(NULL);

   if(seed == lastseed)
     {
     seed += ((unsigned int) clock()/trials);
     }
   else
     lastseed=seed;
     
   /* printf("\nNew seed: %d\n",(unsigned int) seed); */

   srand((unsigned int) seed);
       
   RefreshCurCoverage(sc->curcover,sc->funcs);
   while(!IsDesiredCoverage(sc->funcs,sc->curcover,sc->desiredcover))
     {
       testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
       sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
       if(IsCoverageImproved(sc->curcover,sc->testncover,sc->funcs))
	 {
	   AddFeatures(sc->curcover,sc->testncover,sc->funcs);
	   SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
	 }
     }

   if(IsEqual(sc->selectdiffsets,setno,sc->difftestsets))
     reset_all(&(sc->selectdiffsets[setno].testcases));
   else
     {
       sc->selectdiffsets[setno].coverage = CurrentCoverage(sc->curcover,sc->funcs);
       ++setno;
     }
   trials++;
   }

  if(trials == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      exit(1);
    }
}

void TGDebugStructuralCriterionScenario(sc)
Scenario * sc;
{
  int setno, testn, trials;
  unsigned int lastseed=0, seed=0;

  setno = 0;
  trials = 0;
  while((setno <= sc->difftestsets-1) && trials < sc->no_trials)
   {
   /* Select new seed */
   
   seed = time(NULL);

   if(seed == lastseed)
     {
     seed += ((unsigned int) clock()/trials);
     }
   else
     lastseed=seed;
     
   printf("\nNew seed: %d\n",(unsigned int) seed); 

   srand((unsigned int) seed);
       
   RefreshCurCoverage(sc->curcover,sc->funcs);
   while(!IsDesiredCoverage(sc->funcs,sc->curcover,sc->desiredcover))
     {
       testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
       sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
       if(IsCoverageImproved(sc->curcover,sc->testncover,sc->funcs))
	 {
	   AddFeatures(sc->curcover,sc->testncover,sc->funcs);
	   SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
	 }
     }

   if(IsEqual(sc->selectdiffsets,setno,sc->difftestsets) || 
      !IsThereAnFaultRevealingTestCase(sc->selectdiffsets,&(sc->output_info),setno,sc->difftestsets,sc->sizetestpool))
     reset_all(&(sc->selectdiffsets[setno].testcases));
   else
     {
       sc->selectdiffsets[setno].coverage = CurrentCoverage(sc->curcover,sc->funcs);
       ++setno;
     }
   trials++;
   }

  if(trials == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      exit(1);
    }
}
void TGRandomScenario(sc)
     Scenario * sc;
{
  int setno, testn, trials;
  int maxrandom = sc->testsetsize;
  int notestcase = 0;
  unsigned int lastseed=0, seed=0;
  
  /* Random selection */
  setno = 0;
  trials = 0;

  while((setno <= sc->difftestsets-1) && trials < sc->no_trials)
   {
   /* Select new seed */
   
   seed = time(NULL);

   if(seed == lastseed)
     {
     seed += ((unsigned int) clock()/trials);
     }
   else
     lastseed=seed;
     
   printf("\nNew seed: %d\n",(unsigned int) seed);

   srand((unsigned int) seed);

   notestcase = 0;
   RefreshCurCoverage(sc->curcover,sc->funcs);
   while(notestcase < maxrandom)
     {
       testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
       SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
       sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
       AddFeatures(sc->curcover,sc->testncover,sc->funcs);
       ++notestcase;
     }
             
   if(IsEqual(sc->selectdiffsets,setno,sc->difftestsets))          
     reset_all(&(sc->selectdiffsets[setno].testcases));       
   else
     {
     sc->selectdiffsets[setno].coverage = CurrentCoverage(sc->curcover,sc->funcs);
     ++setno;
     }
   trials++;
   }

  if(trials == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      exit(1);
    }
}

void TGDebugRandomScenario(sc)
     Scenario * sc;
{
  int setno, testn, trials;
  int maxrandom = sc->testsetsize;
  int notestcase = 0;
  unsigned int lastseed=0, seed=0;
  
  /* Random selection */
  setno = 0;
  trials = 0;

  while((setno <= sc->difftestsets-1) && trials < sc->no_trials)
   {
   /* Select new seed */
   
   seed = time(NULL);

   if(seed == lastseed)
     {
     seed += ((unsigned int) clock()/trials);
     }
   else
     lastseed=seed;
     
   printf("\nNew seed: %d\n",(unsigned int) seed);

   srand((unsigned int) seed);

   notestcase = 0;
   RefreshCurCoverage(sc->curcover,sc->funcs);
   while(notestcase < maxrandom)
     {
       testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
       SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
       sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
       AddFeatures(sc->curcover,sc->testncover,sc->funcs);
       ++notestcase;
     }
             
   if(IsEqual(sc->selectdiffsets,setno,sc->difftestsets) || 
      !IsThereAnFaultRevealingTestCase(sc->selectdiffsets,&(sc->output_info),setno,sc->difftestsets,sc->sizetestpool))          
     reset_all(&(sc->selectdiffsets[setno].testcases));       
   else
     {
     sc->selectdiffsets[setno].coverage = CurrentCoverage(sc->curcover,sc->funcs);
     ++setno;
     }
   trials++;
   }

  if(trials == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      exit(1);
    }
}

void TPStructuralCriterionScenario(sc)
Scenario * sc;
{
  int setno, testn, trials;
  unsigned int lastseed=0, seed=0;

  setno = 0;
  trials = 0;
  while((setno <= sc->difftestsets-1) && trials < sc->no_trials)
   {
   /* Select new seed */
   
   seed = time(NULL);

   if(seed == lastseed)
     {
     seed += ((unsigned int) clock()/trials);
     }
   else
     lastseed=seed;
     
   printf("\nNew seed: %d\n",(unsigned int) seed); 

   srand((unsigned int) seed);
       
   RefreshCurCoverage(sc->curcover,sc->funcs);

   /* Select a non-failure-revealing test case to start */

   do
     {
       testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
     }
   while(test_bit(testn, &(sc->output_info)));
       
   sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
   while(!test_bit(testn, &(sc->output_info)))
     {
       AddFeatures(sc->curcover,sc->testncover,sc->funcs);
       SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);

       do
	 {
	   testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
	   sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
	  }
       while(!IsCoverageImproved(sc->curcover,sc->testncover,sc->funcs)
             && !IsDesiredCoverage(sc->funcs,sc->curcover,sc->desiredcover));
     }

   /* Add features and include the failure-revealing test case in testset */

   sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
   AddFeatures(sc->curcover,sc->testncover,sc->funcs);
   SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);

   if(IsEqual(sc->selectdiffsets,setno,sc->difftestsets) ||
      IsDesiredCoverage(sc->funcs,sc->curcover,sc->desiredcover))
     reset_all(&(sc->selectdiffsets[setno].testcases));
   else
     {
       sc->selectdiffsets[setno].coverage = CurrentCoverage(sc->curcover,sc->funcs);
       ++setno;
     }
   trials++;
   }

  if(trials == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      exit(1);
    }
}

void TPRandomScenario(sc)
     Scenario * sc;
{
  int setno, testn, trials;
  unsigned int lastseed=0, seed=0;
  
  /* Random selection */
  setno = 0;
  trials = 0;

  while((setno <= sc->difftestsets-1) && trials < sc->no_trials)
   {
   /* Select new seed */
   
   seed = time(NULL);

   if(seed == lastseed)
     {
     seed += ((unsigned int) clock()/trials);
     }
   else
     lastseed=seed;
     
   printf("\nNew seed: %d\n",(unsigned int) seed);

   srand((unsigned int) seed);


   RefreshCurCoverage(sc->curcover,sc->funcs);

   /* Select a non-failure-revealing test case to start */

   do
     {
       testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
     }
   while(test_bit(testn, &(sc->output_info)));
       
   while(!test_bit(testn, &(sc->output_info)))
     {
       sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
       AddFeatures(sc->curcover,sc->testncover,sc->funcs);
       SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
       testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
       
     }

   /* Add features and include in testset failure-revealing test case */
       
   sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
   AddFeatures(sc->curcover,sc->testncover,sc->funcs);
   SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
             
   if(IsEqual(sc->selectdiffsets,setno,sc->difftestsets))          
     reset_all(&(sc->selectdiffsets[setno].testcases));       
   else
     {
     sc->selectdiffsets[setno].coverage = CurrentCoverage(sc->curcover,sc->funcs);
     ++setno;
     }
   trials++;
   }

  if(trials == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      exit(1);
    }
}

void MTStructuralCriterionScenario(sc)
Scenario * sc;
{
  int setno, testn, trials;
  unsigned int lastseed=0, seed=0;

  setno = 0;
  trials = 0;
  while((setno <= sc->difftestsets-1) && trials < sc->no_trials)
   {
   /* Select new seed */
     int auxtrials0 = 0;

   seed = time(NULL);

   if(seed == lastseed)
     {
     seed += ((unsigned int) clock()/trials);
     }
   else
     lastseed=seed;
     
   printf("\nNew seed: %d\n",(unsigned int) seed); 

   srand((unsigned int) seed);
       
   RefreshCurCoverage(sc->curcover,sc->funcs);
   while(!IsDesiredCoverage(sc->funcs,sc->curcover,sc->desiredcover)
	 && auxtrials0 < sc->no_trials)
     {
       int auxtrials;
       /* Select a non-failure revealing test case */
       auxtrials=0;
       do
	 {
	     testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
	     auxtrials++;
	 }
       while(test_bit(testn, &(sc->output_info)) && auxtrials < sc->no_trials);

       if(auxtrials == sc->no_trials)
	 {
	   fprintf(stderr,"pokeexper: Exceeded the allowed number of trials for non-failure-revealing test case selection\n");
	   exit(1);
	 }

       sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
       if(IsCoverageImproved(sc->curcover,sc->testncover,sc->funcs))
	 {
	   AddFeatures(sc->curcover,sc->testncover,sc->funcs);
	   SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
	 }
       auxtrials0++;
     }

   if(auxtrials0 == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      fprintf(stderr,"pokeexper: Aproximative set is generated\n");
      /* reset_all(&(sc->selectdiffsets[setno].testcases)); ++trials; continue;       exit(1);  */
    } 

   /* Pick up one failure-revealing test case */
   do
   testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
   while(!test_bit(testn, &(sc->output_info)));

   /* Add features and include in testset failure-revealing test case */

   sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
   AddFeatures(sc->curcover,sc->testncover,sc->funcs);	   
   SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);

   if(IsEqual(sc->selectdiffsets,setno,sc->difftestsets))
     reset_all(&(sc->selectdiffsets[setno].testcases));
   else
     {
       sc->selectdiffsets[setno].coverage = CurrentCoverage(sc->curcover,sc->funcs);
       ++setno;
     }
   trials++;
   }

  if(trials == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      exit(1);
    } 
}

void MTRandomScenario(sc)
     Scenario * sc;
{
  int setno, testn, trials;
  int maxrandom = sc->testsetsize;
  int notestcase = 0;
  unsigned int lastseed=0, seed=0;
  
  /* Random selection */
  setno = 0;
  trials = 0;

  while((setno <= sc->difftestsets-1) && trials < sc->no_trials)
   {
   /* Select new seed */
   
   seed = time(NULL);

   if(seed == lastseed)
     {
     seed += ((unsigned int) clock()/trials);
     }
   else
     lastseed=seed;
     
   printf("\nNew seed: %d\n",(unsigned int) seed);

   srand((unsigned int) seed);

   notestcase = 0;
   RefreshCurCoverage(sc->curcover,sc->funcs);
   while(notestcase < maxrandom)
     {
       int auxtrials;
       auxtrials=0;   
       do
	 {
	  testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
	  auxtrials++;
	 }
       while(test_bit(testn, &(sc->output_info)) && auxtrials < sc->no_trials);

       if(auxtrials == sc->no_trials)
	 {
	   fprintf(stderr,"pokeexper: Exceeded the allowed number of trials for non-failure-revealing test case selection\n");
	   exit(1);
	 }

       sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
       SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
       AddFeatures(sc->curcover,sc->testncover,sc->funcs);

       ++notestcase;
     }

   /* Pick up one failure-revealing test case */     
   do
     {
       testn = SelectRandomTestCase(sc->selectdiffsets,setno,sc->sizetestpool,sc->difftestsets);
     }
   while(!test_bit(testn, &(sc->output_info)));

   /* Add features and include the failure-revealing test case in testset */

   sc->testncover = GetTestCoverage(sc->testncover,sc->testscover,testn,sc->funcs);
   AddFeatures(sc->curcover,sc->testncover,sc->funcs);
   SetTestNSelectDiffSets(sc->selectdiffsets,setno,testn,sc->difftestsets,sc->sizetestpool);
             
   if(IsEqual(sc->selectdiffsets,setno,sc->difftestsets))          
     reset_all(&(sc->selectdiffsets[setno].testcases));       
   else
     {
     sc->selectdiffsets[setno].coverage = CurrentCoverage(sc->curcover,sc->funcs);
     ++setno;
     }
   trials++;
   }

  if(trials == sc->no_trials)
    {
      fprintf(stderr,"pokeexper: Exceeded the allowed number of trials\n");
      exit(1);
    }
}
