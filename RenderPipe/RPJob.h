#ifndef    __RPJOB_HEADER__
#define    __RPJOB_HEADER__


class RPJob {
public:
	//  engine enumeration
	//
	enum JobEngine {
		eUndetermined = 0,
		eMaya, 
		e3dsmax, 
		eNuke, 
		eshake
	};  // end enum <JobEngine>

	//  constructor
	//
	RPJob ( char *jobName = 0,
			float jobStartFrame = 0, float jobEndFrame = 0, 
			char *sourceFile = 0, 
			char *jobProjDir = 0, 
			char *jonOutputName = 0, 
			char **jobSlaves = 0, 
			char **jobLockesSlaves = 0, 
			char **jobFreeSlaves = 0, 
			RPJob::JobEngine jobEngine = eUndetermined );
	//  destructor
	//
	~RPJob();

	//  state enumeration
	//
	enum JobState {
		sUndetermined = 0, 
		sRunning,
		sQueued, 
		sFinished, 
		sStopped, 
		sWaited4Pack, 
		sLastPack
	};  // end enum <JobState>


private:
	//  init Data
	//
	char *_jobName;
	int _jobStartFrame;
	int _jobEndFrame; 
	char *_jobSourceFile; 
	char *_jobProjDir; 
	char *_jobOutputName; 
	char **_jobSlaves; 
	char **_jobLockedSlaves; 
	char **_jobFreeSlaves;
	int _jobPackSize;
	int _jobPriority;
	RPJob::JobEngine _jobEngine;

	//  state Data
	//
	float _jobPackStart;
	float _jobPackEnd;
	RPJob::JobState _jobState;

};	//  end RPJob




#endif    /*__RPJOB_HEADER__*/