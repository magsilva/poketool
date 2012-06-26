Input:
------

- Program:

- list of functions;

- Desired coverage: DesiredCover;

- Number of different sets: DiffTestSets;

- Number maximum of test cases: MaxTestCases;

Output:
-------

- Set of test cases with specified coverage;
  (sequence of test cases)

Algorithm:
----------

- Initialize the data structures;

-- Create DS to store the maximum number of features exercised by each 
function;
----- Create a bitvector for each function where each feature executed is set;

------ MaxCoverageDS;
------ Function Name;
------ Bitvector of features;

------- Visit every directory and look for criterium.his;
------- Set bit for every feature;

-- Create DS that accumulates the current number of features exercised for 
each function by the selected test set;

------ CurCoverageDS;
------ Function Name;
------ Bitvector of features;

-- Create DS containing the sets of selected test cases (1..DiffTestSets);

------ SelectDiffSets (1..DiffTestSets);
------ Bitvector Seltests;

i=0; 
while(i <= DiffTestSets-1)
--- Select a new seed;
--- Refresh CurCoverageDS;
--- while(!IsDesiredCoverage(MaxCoverageDS,CurCoverageDS,DesiredCover))
--- Select randomly a test case(SelectDiffSets[i]) (1..MaxTestCases):testN;
--- if(IsCoverageImproved(CurCoverageDS,GetTestCoverage(testN)))
      Add TestN features to CurCoverageDS;
      Set TestN SelectDiffSets[i];
--- end if
--- end while
--- if (IsEqual(SelectDiffSets, i))
      Reset SelectDiffSets[i];
    else
      ++i;
end while;









