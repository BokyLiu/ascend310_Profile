#ifndef COMMON_DATA_TYPE_H_
#define COMMON_DATA_TYPE_H_

#include "dvpp/dvpp_config.h"
#include "hiaiengine/ai_memory.h"
#include "hiaiengine/data_type.h"
#include "hiaiengine/data_type_reg.h"
#include <vector>
using namespace hiai;

/*
* define 
*/
// main函数中从graph外传入graph内的数据结构
struct Main2GraphInfo {
    std::string strFileName; /* inference image name */
    uint32_t isEOS;     /* flag of video stream reception */
};
template<class Archive>
void serialize(Archive &ar, Main2GraphInfo &data)
{
    ar(data.strFileName, data.isEOS);
}

struct ModelInput {
    int isEOS  = 0;
};

template<class Archive>
void serialize(Archive &ar, ModelInput &data)
{
    ar(data.isEOS);
}
struct ModelOutput {
    uint32_t isEOS  = 0;
    uint32_t width  = 0;       // 输出宽
    uint32_t height = 0;      // 输出高
    uint32_t channel = 0;     // 输出通道数
    uint32_t size    = 0;     // 数据大小
    uint32_t frameIdx  = 0;
    std::shared_ptr<float> data;   // 数据指针
    std::string strNodeName;
};

template<class Archive>
void serialize(Archive &ar, ModelOutput &data)
{
    ar(data.isEOS, data.width, data.height, data.channel, data.size, data.frameIdx, data.strNodeName);
    if (data.size > 0 && data.data.get() == nullptr)
    {
        float *allocBuffer = nullptr;
        hiai::HIAIMemory::HIAI_DMalloc(data.size, (void *&)allocBuffer);
        data.data.reset(allocBuffer, [](float *pdata) { hiai::HIAIMemory::HIAI_DFree(pdata); });
    }
    ar(cereal::binary_data(data.data.get(), data.size));
}



#endif
