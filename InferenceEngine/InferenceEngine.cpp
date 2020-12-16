#include "InferenceEngine.h"
#include "engine_tools.h"
#include "error_code.h"
#include "hiaiengine/ai_memory.h"
#include "hiaiengine/c_graph.h"
#include "hiaiengine/data_type.h"
#include "hiaiengine/log.h"
#include <memory>

using std::map;
using std::shared_ptr;
using std::string;
using std::vector;

HIAI_REGISTER_DATA_TYPE("ModelOutput", ModelOutput);
HIAI_REGISTER_DATA_TYPE("ModelInput", ModelInput);

HIAI_StatusT InferenceEngine::Init(const hiai::AIConfig &config,
                                   const std::vector<hiai::AIModelDescription> &model_desc)
{
    HIAI_ENGINE_LOG(APP_INFO, "[AICoreInferenceEngine] start init!");
    HIAI_StatusT ret;
    /*
    * AI model maneger
    */
    if (modelManager == nullptr) {
        modelManager = std::make_shared<hiai::AIModelManager>();
    }
    hiai::AIModelDescription modelDesc;
    map<string, string> kvcfg = kvmap(config);
    if (HIAI_OK != checkEmpty(kvcfg["model"])) {
        return HIAI_ERROR;
    }
    std::string modelPath = kvcfg["model"];
    std::set<char> delims { '\\', '/' };
    std::vector<std::string> path = splitpath(modelPath, delims);
    modelName = path.back();
    #ifdef OUTPUTLOG
    HIAI_ENGINE_LOG(APP_ERROR, "model path %s", modelPath.c_str());
    HIAI_ENGINE_LOG(APP_ERROR, "model name %s", modelName.c_str());
    #endif
    modelDesc.set_path(modelPath);
    modelDesc.set_name(modelName);
    modelDesc.set_key(kvcfg["passcode"]);
    // init ai model manager
    ret = modelManager->Init(config, { modelDesc });
    if (hiai::SUCCESS != ret) {
        HIAI_ENGINE_LOG(APP_ERROR, "ai model manager init failed!");
        return HIAI_ERROR;
    }

    /*
    *  input/output buffer allocation
    */
    // get info of input and output
    ret = modelManager->GetModelIOTensorDim(modelName, inputTensorDims, outputTensorDims);
    if (ret != hiai::SUCCESS) {
        HIAI_ENGINE_LOG(APP_ERROR, "[AICoreInferenceEngine][%s] GetModelIOTensorDim() failed.", modelName);
        return HIAI_ERROR;
    }
    // Get input image size form om file
    kHeight = inputTensorDims[0].h;
    kWidth = inputTensorDims[0].w;
    kChannel = inputTensorDims[0].c;
    kBatchSize = inputTensorDims[0].n;
    #ifdef OUTPUTLOG
    HIAI_ENGINE_LOG(APP_ERROR, "[AICoreInferenceEngine][%s] inputShape NCWH: %d x %d x %d x %d",
                        modelName.c_str(), kBatchSize, kChannel, kWidth, kHeight);
    #endif
    outputSize.clear();
    for(int i=0; i<outputTensorDims.size(); i++)
    {
        outputSize.push_back(outputTensorDims[i].n*outputTensorDims[i].c*outputTensorDims[i].w*outputTensorDims[i].h*sizeof(float));
        #ifdef OUTPUTLOG
        HIAI_ENGINE_LOG(APP_ERROR, "[AICoreInferenceEngine][%s] node%d[%s], outputShape NCWH: %d x %d x %d x %d",
                        modelName.c_str(), i, outputTensorDims[i].name.c_str(), outputTensorDims[i].n, outputTensorDims[i].c, outputTensorDims[i].w, outputTensorDims[i].h);
        #endif
    }
    

    for (auto &dims : inputTensorDims) {
        logDumpDims(dims);
    }
    for (auto &dims : outputTensorDims) {
        logDumpDims(dims);
    }

    if (inputTensorDims.size() != 1) {
        HIAI_ENGINE_LOG(APP_ERROR, "[AICoreInferenceEngine][%s] inputTensorDims.size() != 1", modelName.c_str());
        return HIAI_ERROR;
    }
    kInputSize = kBatchSize * kHeight * kWidth * kChannel * sizeof(float);
    if (kInputSize != inputTensorDims[0].size) {
        HIAI_ENGINE_LOG(APP_ERROR, "[AICoreInferenceEngine][%s] inputSize != inputTensorDims[0].size (%d vs. %d)",
                        modelName.c_str(), kInputSize, inputTensorDims[0].size);
        return HIAI_ERROR;
    }
    frame_num = 0;
    HIAI_ENGINE_LOG(APP_INFO, "InferenceEngine initial successfully!");
    return HIAI_OK;
    
}

HIAI_IMPL_ENGINE_PROCESS("InferenceEngine", InferenceEngine, INFERENCE_INPUT_SIZE)
{
    HIAI_ENGINE_LOG(APP_ERROR, "InferenceEngine Process start");

    inputTensorVec.clear();
    outputTensorVec.clear();
    std::shared_ptr<ModelInput> modelInput;
    if (nullptr == arg0)
    {
        HIAI_ENGINE_LOG(APP_ERROR, "InferenceEngine arg0 is nullptr!");
        return HIAI_ERROR;
    }
    else
    {
        HIAI_ENGINE_LOG(APP_INFO, "InferenceEngine Process one frame");
        modelInput = std::static_pointer_cast<ModelInput>(arg0);
        if (modelInput == nullptr)
        {
            HIAI_ENGINE_LOG(APP_ERROR, "InferenceEngine modelInput is nullptr!");
            return HIAI_ERROR;
        }

        float *pInput = nullptr;
        HIAI_StatusT ret = hiai::HIAIMemory::HIAI_DMalloc(kInputSize, (void *&)pInput);
        if (ret != HIAI_OK)
        {
            HIAI_ENGINE_LOG(APP_ERROR, "size: %d, HIAI_DMalloc() error\n", kInputSize);
            return HIAI_ERROR;
        }
        // 输入数据满足了要求，才能初始化
        std::shared_ptr<float> inPtr = std::shared_ptr<float>(pInput, [](float *data) { hiai::HIAIMemory::HIAI_DFree(data); });
        if (inPtr == nullptr)
        {
            HIAI_ENGINE_LOG(APP_ERROR, "[InferenceEngine] HIAI_DMalloc inPtr failed.");
            return HIAI_ERROR;
        }
        float *inputdata = (float *)malloc(kInputSize);
        for (int i = 0; i < kInputSize / sizeof(float); i++)
        {
            inputdata[i] = modelInput->isEOS;
        }
        errno_t m_ret = memcpy_s(inPtr.get(), kInputSize, inputdata,
                                 kInputSize);
        if (m_ret != EOK)
        {
            HIAI_ENGINE_LOG(APP_ERROR, "memcpy_s of InferenceEngine modelInput is wrong");
            return HIAI_ERROR;
        }
        hiai::AITensorDescription inputTensorDesc = hiai::AINeuralNetworkBuffer::GetDescription();
        std::shared_ptr<hiai::IAITensor> inputTensor = hiai::AITensorFactory::GetInstance()->CreateTensor(inputTensorDesc,
                                                                                                          static_cast<void *>(inPtr.get()), kInputSize);
        inputTensorVec.push_back(inputTensor);
        // 输出初始化
        ret = modelManager->CreateOutputTensor(inputTensorVec,
                                                outputTensorVec);
        HIAI_ENGINE_LOG(APP_ERROR, "outputTensorVec.size() = %d", outputTensorVec.size());
        // inference
        HIAI_ENGINE_LOG(APP_INFO, "AI Model Manager Process Start!");
        HIAI_ENGINE_LOG(APP_ERROR, "start");
        hiai::AIContext aiContext;
        ret = modelManager->Process(aiContext, inputTensorVec, outputTensorVec, 0);
        HIAI_ENGINE_LOG(APP_ERROR, "end");
        if (hiai::SUCCESS != ret)
        {
            HIAI_ENGINE_LOG(APP_ERROR, "AI Model Manager Process failed, ret:%d", ret);
            return HIAI_ERROR;
        }
        // send data to next engine
        for (int i = 0; i < outputTensorVec.size(); i++) // 偷懒写法
        {
            std::shared_ptr<ModelOutput> modelOutput = std::make_shared<ModelOutput>();
            modelOutput->frameIdx = frame_num;
            // send data to next engine
            modelOutput->height = outputTensorDims[i].h;
            modelOutput->width = outputTensorDims[i].w;
            modelOutput->channel = outputTensorDims[i].c;
            modelOutput->strNodeName = outputTensorDims[i].name;
            shared_ptr<hiai::AINeuralNetworkBuffer> tensorResults =
                std::static_pointer_cast<hiai::AINeuralNetworkBuffer>(outputTensorVec[i]);

            HIAI_ENGINE_LOG(APP_ERROR, "AICoreInferenceEngine[%s] node_%d[%s] output shape[c*h*w]: %d x %d x %d",
                                modelName.c_str(), i, outputTensorDims[i].name.c_str(),
                                modelOutput->channel, modelOutput->height, modelOutput->width);

            if (tensorResults->GetSize() != outputSize[i])
            {
                HIAI_ENGINE_LOG(APP_ERROR, "outputsize is error,right:%d, get:%d", outputSize[i], tensorResults->GetSize());
                return HIAI_ERROR;
            }
            float *resStartPtr = static_cast<float *>(tensorResults->GetBuffer());
            modelOutput->size = tensorResults->GetSize();
            modelOutput->data = std::shared_ptr<float>(new float[modelOutput->size / sizeof(float)], std::default_delete<float[]>());
            errno_t mem_ret = memcpy_s(modelOutput->data.get(), modelOutput->size, resStartPtr, modelOutput->size);
            if (mem_ret != EOK)
            {
                HIAI_ENGINE_LOG(APP_ERROR, "AICoreInferenceEngine memcpy_s error: %d!", mem_ret);
            }

            if (HIAI_OK != hiai::Engine::SendData(0, "ModelOutput", std::static_pointer_cast<void>(modelOutput)))
            {
                return HIAI_ERROR;
            }
        }
        frame_num++;
        free((void *)inputdata);
    }
    HIAI_ENGINE_LOG(APP_INFO, "InferenceEngine senddata successfully!");
    return HIAI_OK;
}
