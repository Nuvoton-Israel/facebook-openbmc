#pragma once

typedef enum HMCPhase {
  HMC_FW_UNKNOWN = -1,
  HMC_FW_EVT = 0,
  HMC_FW_DVT = 1,
  BMC_FW_DVT = 2,
} HMCPhase;

typedef enum GPUConfig {
  GPU_CONFIG_HGX,
  GPU_CONFIG_UBB,
  GPU_CONFIG_UNKNOWN
} GPUConfig;

#define MAX_NUM_GPUs     (8)

#ifdef __cplusplus
#include <stdexcept>
#include <string>
#include <vector>

namespace hgx {

enum class DiagnosticDataType { MANAGER, OEM_EROT, OEM_SELF_TEST, OEM_FPGA, OEM_RETIMER };

// Standard HTTP Exception
struct HTTPException : std::runtime_error {
  int errorCode = 0;
  HTTPException(int code)
      : std::runtime_error("HTTP Error: " + std::to_string(code)),
        errorCode(code) {}
};

struct GPUNotReady : std::runtime_error {
  GPUNotReady() : std::runtime_error("GPU Is not ready") {}
};

struct TaskStatus {
  std::string resp{};
  // one of Starting, Stopping, Completed, Exception, and Running
  std::string state{};
  std::string status{};
  std::vector<std::string> messages{};
};

// Get a subpath after /redfish/v1
// Example, get on 192.168.31.1/redfish/v1/blah, the subpath = blah
std::string redfishGet(const std::string& subpath);
// Post a subpath after /redfish/v1
// Example, get on 192.168.31.1/redfish/v1/blah, the subpath = blah
std::string redfishPost(const std::string& subpath, std::string&& args);

// Perform Factory reset to default
void factoryReset();

// Graceful-reset the HMC
void reset();

// Patch a subpath after /redfish/v1
std::string redfishPatch(const std::string& subpath, std::string&& args);

// Return the version of the component
std::string version(const std::string& comp, bool returnJson = false);

// Initiate an update and return the task ID.
// User can call taskStatus to get the current
// status.
std::string updateNonBlocking(const std::string& comp, const std::string& path, bool returnJson = false);

// Patch EroT and HMC before updating FW
void patch_bf_update();

// Initiate an update and wait till the task
// completes.
int update(const std::string& comp, const std::string& path);

// Get HGX's phase
HMCPhase getHMCPhase(void);

// Get Configuration
GPUConfig getConfig(void);

// Get HGX's sensors from Telemetry Service
// status.
void getMetricReports();

// Get task status (JSON output)
TaskStatus getTaskStatus(const std::string& id);

// Get the current value of a sensor.
float sensor(const std::string& component, const std::string& name);

// Get dump
void dump(DiagnosticDataType type, const std::string& path);

// Trigger Dump Collection
std::string dumpNonBlocking(DiagnosticDataType type);

// Retrieve data
void retrieveDump(const std::string& taskID, const std::string& path);

// Print event log
void printEventLog(std::ostream& os, bool jsonFmt = false);

// clear event log
void clearEventLog();

// Sync current BMC Time
void syncTime();

// Set HGX FW version in KV store
int setFWVersion();

// Get components supporting measurements
std::vector<std::string> integrityComponents();

// Get component measurement
std::string getMeasurement(const std::string& comp);

// Set power limit
std::string setPowerLimit(int gpuID, std::string pwrLimit);

// Get power limit
std::string getPowerLimit(int gpuID);

// Request for the value of the sensor, but return all the JSON output
std::string sensorRaw(const std::string& component, const std::string& name);
} // namespace hgx

// These are APIs to be used by C based libraries.
extern "C" {
#endif

int hgx_get_metric_reports();
int get_hgx_sensor(const char* component, const char* snr_name, float* value);
int get_hgx_ver(const char* component, char *version);
HMCPhase get_hgx_phase();
GPUConfig get_gpu_config();

#ifdef __cplusplus
}
#endif
