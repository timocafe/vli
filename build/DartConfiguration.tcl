# This file is configured by CMake automatically as DartConfiguration.tcl
# If you choose not to use CMake, this file may be hand configured, by
# filling in the required variables.


# Configuration directories and files
SourceDirectory: /Users/ewart/Desktop/vli
BuildDirectory: /Users/ewart/Desktop/vli/build

# Where to place the cost data store
CostDataFile: 

# Site is something like machine.domain, i.e. pragmatic.crd
Site: cbdhcp-2-030.epfl.ch

# Build name is osname-revision-compiler, i.e. Linux-2.4.2-2smp-c++
BuildName: Darwin-c++

# Submission information
IsCDash: TRUE
CDashVersion: 
QueryCDashVersion: 
DropSite: maquis.ch
DropLocation: /cdash/submit.php?project=Series+Expansion
DropSiteUser: 
DropSitePassword: 
DropSiteMode: 
DropMethod: http
TriggerSite: 
ScpCommand: /usr/bin/scp

# Dashboard start time
NightlyStartTime: 00:00:00 EST

# Commands for the build/test/submit cycle
ConfigureCommand: "/usr/local/cmake/2.8.12/bin/cmake" "/Users/ewart/Desktop/vli"
MakeCommand: /usr/bin/make -i
DefaultCTestConfigurationType: Release

# CVS options
# Default is "-d -P -A"
CVSCommand: CVSCOMMAND-NOTFOUND
CVSUpdateOptions: -d -A -P

# Subversion options
SVNCommand: /usr/bin/svn
SVNOptions: 
SVNUpdateOptions: 

# Git options
GITCommand: /usr/bin/git
GITUpdateOptions: 
GITUpdateCustom: 

# Generic update command
UpdateCommand: /usr/bin/svn
UpdateOptions: 
UpdateType: svn

# Compiler info
Compiler: /usr/bin/c++

# Dynamic analysis (MemCheck)
PurifyCommand: 
ValgrindCommand: 
ValgrindCommandOptions: 
MemoryCheckCommand: MEMORYCHECK_COMMAND-NOTFOUND
MemoryCheckCommandOptions: 
MemoryCheckSuppressionFile: 

# Coverage
CoverageCommand: /usr/bin/gcov
CoverageExtraFlags: -l

# Cluster commands
SlurmBatchCommand: SLURM_SBATCH_COMMAND-NOTFOUND
SlurmRunCommand: SLURM_SRUN_COMMAND-NOTFOUND

# Testing options
# TimeOut is the amount of time in seconds to wait for processes
# to complete during testing.  After TimeOut seconds, the
# process will be summarily terminated.
# Currently set to 25 minutes
TimeOut: 1500

UseLaunchers: 
CurlOptions: 
# warning, if you add new options here that have to do with submit,
# you have to update cmCTestSubmitCommand.cxx

# For CTest submissions that timeout, these options
# specify behavior for retrying the submission
CTestSubmitRetryDelay: 5
CTestSubmitRetryCount: 3
