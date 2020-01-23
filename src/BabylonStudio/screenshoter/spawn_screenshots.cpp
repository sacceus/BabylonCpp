#include <babylon/samples/samples_index.h>
#include <babylon/core/logging.h>
#include <iostream>
#include <string>
#include <babylon/core/filesystem.h>
#include <babylon/babylon_imgui/babylon_studio.h>
#include <babylon/samples/sample_spawn.h>
#include <babylon/samples/samples_info.h>
#include <nlohmann/json.hpp>

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

namespace BABYLON {
namespace impl {
using namespace BABYLON::SamplesInfo;

SampleRunInfo runOneSample(const std::string & exeName, const std::string &sampleName)
{
  SampleRunInfo sampleRunInfo;

  std::vector<std::string> command = {exeName, "-s", sampleName, "-p"};
    if (flagAsync)
      command.push_back("-A");

      
    SpawnOptions spawnOptions;
  spawnOptions.MaxExecutionTimeSeconds       = 15.;
  spawnOptions.CopyOutputToMainProgramOutput = false;
  auto spawnResult                           = SpawnWaitSubProcess(command, spawnOptions);

  if (spawnResult.ExitStatus != 0) {
    BABYLON_LOG_WARN("ScreenshotAllSamples", "Subprocess has failed for sample ", sampleName);
    sampleRunInfo.unhandledExceptionStackTrace = spawnResult.StdOutErr;
    sampleRunInfo.sampleRunStatus              = SampleRunStatus::unhandledException;
  }

  if (spawnResult.MaxExecutionTimePassed) {
    BABYLON_LOG_WARN("ScreenshotAllSamples", "Subprocess MaxExecutionTimePassed for sample ",
                     sampleName);
    sampleRunInfo.sampleRunStatus              = SampleRunStatus::tooSlowOrHung;
  }

  if ((spawnResult.ExitStatus == 0) && (!spawnResult.MaxExecutionTimePassed)) {
    bool isRenderingEmpty = BABYLON::Samples::ReadScreenshot_IsImageEmpty(sampleName);
    if (isRenderingEmpty) {
      BABYLON_LOG_WARN("ScreenshotAllSamples", "Empty 3D rendering for sample ", sampleName);
      sampleRunInfo.sampleRunStatus              = SampleRunStatus::empty3d;
    }
    else {
      sampleRunInfo.sampleRunStatus = SampleRunStatus::success;
    }
  }
  return sampleRunInfo;
}


void spawnScreenshots(const std::string & exeName)
{

#ifdef _WIN32
  BABYLON_LOG_ERROR("spawnScreenshots", " needs fix under windows...");
  return;
#endif

  auto &samplesCollection = SamplesCollection::Instance();
  for (const auto& [categoryName, samplesInCategory] : samplesCollection.AllSamples())
  {
    BABYLON_LOG_INFO("spawnScreenshots", "Category", categoryName);
    BABYLON_LOG_INFO("spawnScreenshots", "*******************************************************");
    for (const auto& [sampleName, sampleData] : samplesInCategory)
    {
      BABYLON_LOG_INFO("spawnScreenshots", "Sample", sampleName);
      auto sampleRunInfo = runOneSample(exeName, sampleName);
      samplesCollection.SaveSampleRunInfo(sampleName, sampleRunInfo);
    }
  }


//  // Write Stats file
//  {
//    nlohmann::json jsonData;
//    jsonData = spawnScreenshotsStats;
//    std::string outputFilename = screenshotsDirectory() + "/_stats.txt";
//    FILE * f = fopen(outputFilename.c_str(), "w");
//    std::string jsonString = jsonData.dump(4);
//    fprintf(f, "%s", jsonString.c_str());
//    fclose(f);
//    BABYLON_LOG_INFO("spawnScreenshots", "stats: ", jsonData.dump());
//  }
}


} // namespace impl
} // namespace BABYLON
