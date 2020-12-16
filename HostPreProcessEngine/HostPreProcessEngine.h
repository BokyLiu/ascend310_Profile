#ifndef HOST_PRE_PROCESS_ENGINE_H
#define HOST_PRE_PROCESS_ENGINE_H
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <map>
#include <queue>
#include <hiaiengine/ai_model_manager.h>
#include "hiaiengine/engine.h"
#include "hiaiengine/data_type_reg.h"
#include "hiaiengine/api.h"

#include "common_data_type.h"
#include "error_code.h"

#define HOST_PRE_PROCESS_INPUT_SIZE  2
#define HOST_PRE_PROCESS_OUTPUT_SIZE 1

#define CHECK_ODD(NUM)  ((((NUM) % (2)) != (0)) ? (NUM) : ((NUM) - (1)))
#define CHECK_EVEN(NUM) ((((NUM) % (2)) == (0)) ? (NUM) : ((NUM) - (1)))

// define an engine
class HostPreProcessEngine : public Engine {
public:
    HostPreProcessEngine(){}

    ~HostPreProcessEngine(){}
    HIAI_StatusT Init(const AIConfig &config, const std::vector<AIModelDescription> &model_desc);

    HIAI_StatusT PreProcess(std::string &fileName, int32_t isEOS);

    /**
    * @ingroup hiaiengine
    */
    HIAI_DEFINE_PROCESS(HOST_PRE_PROCESS_INPUT_SIZE, HOST_PRE_PROCESS_OUTPUT_SIZE);

private:
    int32_t frame_idx = 0;
    int32_t yoloInputSize = 256;
    time_t start, end;
    bool firstpic=true;
    bool saveXML=false;
};
#endif
