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

#ifndef INFERENCE_ENGINE_H
#define INFERENCE_ENGINE_H

#include "hiaiengine/engine.h"
#include "hiaiengine/ai_model_manager.h"
#include "hiaiengine/multitype_queue.h"
#include <utility>

#include "common_data_type.h"

#define INFERENCE_INPUT_SIZE  1
#define INFERENCE_OUTPUT_SIZE 1

class InferenceEngine : public hiai::Engine {
public:
    InferenceEngine(){}
    ~InferenceEngine(){}

    std::string modelName;
    HIAI_StatusT Init(const hiai::AIConfig &config,
                      const std::vector<hiai::AIModelDescription> &model_desc);

    HIAI_DEFINE_PROCESS(INFERENCE_INPUT_SIZE, INFERENCE_OUTPUT_SIZE)

private:
    uint32_t kBatchSize = 1;
    uint32_t kChannel = 0;
    uint32_t kWidth = 0;
    uint32_t kHeight = 0;
    uint32_t kInputSize = 0;

    std::vector<uint32_t> outputSize;

    std::shared_ptr<hiai::AIModelManager> modelManager;

    std::vector<hiai::TensorDimension> inputTensorDims;
    std::vector<hiai::TensorDimension> outputTensorDims;

    std::vector<std::shared_ptr<hiai::IAITensor>> inputTensorVec;
    std::vector<std::shared_ptr<hiai::IAITensor>> outputTensorVec;
    int frame_num;
};

#endif
