/**
 * MIT License

Copyright (c) 2020 BokyLiu

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 **/

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
