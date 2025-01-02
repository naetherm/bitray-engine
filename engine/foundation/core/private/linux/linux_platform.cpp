////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2025 RacoonStudios
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
////////////////////////////////////////////////////////////////////////////////////////////////////


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "core/linux/linux_platform.h"
#include "core/io/file.h"
#include "core/memory/memory.h"
#include "core/std/math.h"
#include "core/string/regex.h"
#include <pwd.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <locale.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <sys/stat.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace core {


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
bool LinuxPlatform::get_memory_information(MemoryInformation &sMemoryInformation) const
{
  // Initialize memory information
  sMemoryInformation.nTotalPhysicalMemory	= 0;
  sMemoryInformation.nFreePhysicalMemory	= 0;
  sMemoryInformation.nTotalSwapMemory		= 0;
  sMemoryInformation.nFreeSwapMemory		= 0;

  // Error!
  return false;
}

LinuxPlatform::LinuxPlatform()
: mSysInfoInit(!uname(&mName)) {
  initialize_cpu_info();
}

LinuxPlatform::~LinuxPlatform() {

}

String LinuxPlatform::get_platform() const {
  static String SPlatform = "Linux";
  return SPlatform;
}

String LinuxPlatform::get_os() const {
  // get system info
  if (mSysInfoInit) {
    String sVersion = mName.sysname;
    sVersion += ' ';
    sVersion += mName.machine;
    sVersion += ' ';
    sVersion += mName.release;
    return sVersion;
  } else {
    return get_platform() + " unknown";
  }
}

char LinuxPlatform::get_separator() const {
  return '/';
}

String LinuxPlatform::get_shared_library_prefix() const {
  static String String = "lib";
  return String;
}

String LinuxPlatform::get_shared_library_extension() const {
  static String String = "so";
  return String;
}

uint32 LinuxPlatform::get_cpu_mhz() const {
  uint32 mhz = 0;

  File* file = new File();

  // Parse kernel information file
  if (file->open("/proc/cpuinfo", EFileMode::Read)) {
    static CRegEx cRegEx("^\\s*cpu MHz\\s*:\\s*(\\d+(\\.\\d+)).*$");

    while (!file->is_eof()) {
      const String line = file->get_str();
      if (cRegEx.match(line)) {
        const String sResult = cRegEx.get_result(0);
        mhz = sResult.get_int();
      }
    }
  }

  file->close();

  delete file;

  return mhz;
}

CPUInfo LinuxPlatform::get_cpu_info() const {
  return mCpuInfo;
}

String LinuxPlatform::get_executable_filename() const {
  // get PID of current process
  const pid_t nPID = getpid();
  char szLinkName[512];
  if (snprintf(szLinkName, 512, "/proc/%d/exe", nPID) >= 0) {
    // Read symbolic link
    char szProgram[512];
    const int nRet = readlink(szLinkName, szProgram, 512);
    if (nRet < 512) {
      szProgram[nRet] = 0;
      return String(szProgram);
    }
  }

  // Error: Could not determine path to application
  return "";
}

String LinuxPlatform::get_current_dir() const {
  // Get current directory and allocate the buffer
  char* temp = getcwd(nullptr, 0);

  String currentDir(temp);

  free(temp);

  return currentDir;
}

void LinuxPlatform::urgent_message(const String &sMessage) const {
  // Do also feed the console, safe is safe
  fputs(sMessage, stdout);
  fputs("\n", stdout);
  fflush(stdout);

  // There's no such thing as "MessageBox()" from MS Windows and using a GUI system
  // like Qt would be a total overkill in here, so, go the easiest possible way...
  char szCommand[1024];
  sprintf(szCommand, "xmessage -center \"%s\"", sMessage.c_str());
  system(szCommand);
}


uint64 LinuxPlatform::get_milliseconds() const {
  struct timeval now;
  gettimeofday(&now, nullptr);
  return static_cast<uint64>(now.tv_sec*1000 + now.tv_usec/1000);
}

uint64 LinuxPlatform::get_microseconds() const {
  struct timeval now;
  gettimeofday(&now, nullptr);
  return static_cast<uint64>(now.tv_sec*1000000 + now.tv_usec);
}

void LinuxPlatform::sleep(uint64 nMilliseconds) const {
  // We have to split up the given number of milliseconds to sleep into seconds and milliseconds

  // Calculate the number of seconds to sleep
  const time_t nSeconds = static_cast<time_t>(nMilliseconds/1000);

  // Overwrite the given number of milliseconds with the remaining calculated number of milliseconds to sleep
  nMilliseconds = nMilliseconds - (nSeconds*1000);

  // Now sleep well my friend...
  struct timespec sTimespec;
  sTimespec.tv_sec  = nSeconds;
  sTimespec.tv_nsec = nMilliseconds*1000000L;
  nanosleep(&sTimespec, 0);
}

void LinuxPlatform::yield() const {
  sched_yield();
}

void LinuxPlatform::initialize_cpu_info() {
  cpu_set_t cpus;
  CPU_ZERO(&cpus);
  if (sched_getaffinity(0, sizeof(cpus), &cpus) == 0) {
    int32 numberOfCores = 0;
    struct CpuInfo
    {
      int32 Core;
      int32 Package;
    } cpusInfo[CPU_SETSIZE];
    Memory::set(cpusInfo, 0, sizeof(cpusInfo));
    int32 maxCoreId = 0;
    int32 maxPackageId = 0;
    int32 cpuCountAvailable = 0;

    for (int32 cpuIdx = 0; cpuIdx < CPU_SETSIZE; cpuIdx++)
    {
      if (CPU_ISSET(cpuIdx, &cpus))
      {
        cpuCountAvailable++;

        {
          File cFile;
          if (cFile.open(String().append_format("sys/devices/system/cpu/cpu%d/topology/core_id", cpuIdx), EFileMode::Read)) {
            String content = cFile.get_content_as_string();
            cpusInfo[cpuIdx].Core = content.get_int();
            cFile.close();
          }
        }

        {
          File cFile;
          if (cFile.open(String().append_format("sys/devices/system/cpu/cpu%d/topology/physical_package_id", cpuIdx), EFileMode::Read)) {
            String content = cFile.get_content_as_string();

            cpusInfo[cpuIdx].Package = content.get_int();
            if (cpusInfo[cpuIdx].Package < 0) {
              cpusInfo[cpuIdx].Package = cpusInfo[cpuIdx].Core;
            }
            cFile.close();
          }

          maxCoreId = core::max(maxCoreId, cpusInfo[cpuIdx].Core);
          maxPackageId = core::max(maxPackageId, cpusInfo[cpuIdx].Package);
        }
      }
    }

    int32 coresCount = maxCoreId + 1;
    int32 packagesCount = maxPackageId + 1;
    int32 pairsCount = packagesCount * coresCount;

    if (coresCount * 2 < cpuCountAvailable) {
      numberOfCores = cpuCountAvailable;
    } else {
      uint8* pairs = new uint8[pairsCount];
      Memory::set(pairs, 0, pairsCount * sizeof(unsigned char));

      for (int32 cpuIdx = 0; cpuIdx < CPU_SETSIZE; cpuIdx++) {
        if (CPU_ISSET(cpuIdx, &cpus)) {
          pairs[cpusInfo[cpuIdx].Package * coresCount + cpusInfo[cpuIdx].Core] = 1;
        }
      }

      for (int32 i = 0; i < pairsCount; i++) {
        numberOfCores += pairs[i];
      }

      delete[] pairs;
    }

    mCpuInfo.ProcessorPackageCount = packagesCount;
    mCpuInfo.ProcessorCoreCount = core::max(numberOfCores, 1);
    mCpuInfo.LogicalProcessorCount = CPU_COUNT(&cpus);
  } else {
    mCpuInfo.ProcessorPackageCount = 1;
    mCpuInfo.ProcessorCoreCount = 1;
    mCpuInfo.LogicalProcessorCount = 1;
  }

  // Get cache sizes
  mCpuInfo.L1CacheSize = 0;
  mCpuInfo.L2CacheSize = 0;
  mCpuInfo.L3CacheSize = 0;
  for (int32 cacheLevel = 1; cacheLevel <= 3; cacheLevel++) {
    File cFile;
    if (cFile.open(String().append_format("sys/devices/system/cpu/cpu0/cache/index%d/size", cacheLevel), EFileMode::Read)) {
      String content = cFile.get_content_as_string();

      char cType = content[content.length()-1];
      uint32 nMultiplier = 1;
      switch (cType) {
        case 'K':
          nMultiplier = 1024;
        case 'M':
          nMultiplier = 1024*1024;
        case 'G':
          nMultiplier = 1024*1024*1024;
        default:
          nMultiplier = 1;
      }
      uint32 nResult = content.substr(0, content.length()-1).get_int()*nMultiplier;

      switch (cacheLevel) {
        case 1:
          mCpuInfo.L1CacheSize = nResult;
          break;
        case 2:
          mCpuInfo.L2CacheSize = nResult;
          break;
        case 3:
          mCpuInfo.L3CacheSize = nResult;
          break;
      }
      cFile.close();
    }
  }

  // Get page size
  mCpuInfo.PageSize = sysconf(_SC_PAGESIZE);

  // Get clock speed
  mCpuInfo.ClockSpeed = get_cpu_mhz();

  // Get cache line size
  mCpuInfo.CacheLineSize = sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // core
