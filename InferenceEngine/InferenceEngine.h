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
