#include "HostPreProcessEngine.h"
#include "error_code.h"
#include <hiaiengine/data_type.h>
#include "hiaiengine/log.h"
#include "hiaiengine/data_type_reg.h"
#include "hiaiengine/api.h"
#include "hiaiengine/ai_model_manager.h"
#include <string.h>
#include <vector>
#include <stdio.h>
#include <map>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <queue>
#include <malloc.h>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <time.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <memory>
const int FRAME_SKIP_INTERVAL = 1;
extern std::string g_image_dir;

HIAI_REGISTER_DATA_TYPE("ModelInput", ModelInput);
HIAI_REGISTER_DATA_TYPE("ModelOutput", ModelOutput);
using namespace std;
using namespace cv;

HIAI_StatusT HostPreProcessEngine::Init(const AIConfig &config, const std::vector<AIModelDescription> &model_desc)
{
    HIAI_StatusT ret = HIAI_OK;
    HIAI_ENGINE_LOG(APP_INFO, "[HostPreProcessEngine] init");

    return HIAI_OK;
}

/**
* @ingroup hiaiengine
*
**/
HIAI_IMPL_ENGINE_PROCESS("HostPreProcessEngine", HostPreProcessEngine, HOST_PRE_PROCESS_INPUT_SIZE)
{
    if(arg0 != nullptr)
    {
        std::shared_ptr<Main2GraphInfo> main2GraphInfo = std::static_pointer_cast<Main2GraphInfo>(arg0);
        if (main2GraphInfo == nullptr)
        {
            HIAI_ENGINE_LOG(APP_ERROR, "[HostPreProcessEngine] Invalid input parameter of engine!");
            return HIAI_ERROR;
        }

        if(main2GraphInfo->strFileName == "true")
        {
            saveXML = true;
        }
        else{
            saveXML = false;
        }
        /* start decode data */
        if (HIAI_OK != PreProcess(main2GraphInfo->strFileName, main2GraphInfo->isEOS))
        {
            HIAI_ENGINE_LOG(APP_ERROR, "[HostPreProcessEngine] PreProcess() error!");
            return HIAI_ERROR;
        }
        start = clock();
        HIAI_ENGINE_LOG(APP_INFO, "[HostPreProcessEngine] end");
    }
    if(arg1 != nullptr)
    {
        end = clock();
        printf("*the running time is : %fs\n", double(end -start)/CLOCKS_PER_SEC);
        std::shared_ptr<ModelOutput> args = std::static_pointer_cast<ModelOutput>(arg1);
        /**
         * 第一帧，保存结果
         * */

        if(args->frameIdx == 0 && saveXML)
        {
            std::string save_name = args->strNodeName + ".xml";
            std::cout<<"* Saving model output tensor as file: "<<save_name<<std::endl;
            FileStorage fs(save_name, FileStorage::WRITE);
            for (size_t c = 0; c < args->channel; c++)
            {
                int output_height = args->height;
                int output_width = args->width;
                cv::Mat mat = cv::Mat(output_height, output_width, CV_32FC1, args->data.get() + output_height * output_width * c).clone();
                std::string channel = "channel_" + to_string(c);
                fs<<channel<<mat;
            }
            fs.release();
        }
    }

    return HIAI_OK;
}

HIAI_StatusT HostPreProcessEngine::PreProcess(std::string &fileName, int32_t isEOS)
{
    /* construct data transmission struct */
    std::shared_ptr<ModelInput> modelInputInfo = std::make_shared<ModelInput>();
    modelInputInfo->isEOS = isEOS;

    // 发送每一帧数据
    if (HIAI_OK != hiai::Engine::SendData(0, "ModelInput",
                                          std::static_pointer_cast<void>(modelInputInfo)))
    {
        std::cout<<"[HostPreProcessEngine] SendData ModelInput error!"<<std::endl;
        return HIAI_ERROR;
    }
    return HIAI_OK;
}
