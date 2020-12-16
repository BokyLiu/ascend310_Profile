#ifndef ATLASFACEDEMO_ENGINE_TOOLS_H
#define ATLASFACEDEMO_ENGINE_TOOLS_H

#include "error_code.h"
#include "hiaiengine/ai_model_manager.h"
#include "hiaiengine/c_graph.h"
#include <map>
#include <string>
#include <vector>

using hiai::TensorDimension;
using std::map;
using std::shared_ptr;
using std::string;

inline map<string, string> kvmap(const hiai::AIConfig &config)
{
    map<string, string> kv;
    for (int index = 0; index < config.items_size(); ++index) {
        const ::hiai::AIConfigItem &item = config.items(index);
        kv.insert(std::make_pair(item.name(), item.value()));
    }
    return std::move(kv);
}

inline HIAI_StatusT checkEmpty(const string &value)
{
    if (value.empty()) {
        HIAI_ENGINE_LOG(APP_ERROR, "Check empty value!");
        return HIAI_ERROR;
    }
    return HIAI_OK;
}

static std::vector<std::string> splitpath(const std::string &str, const std::set<char> delimiters)
{
    std::vector<std::string> result;
    char const *pch = str.c_str();
    char const *start = pch;
    for (; *pch; ++pch) {
        if (delimiters.find(*pch) != delimiters.end()) {
            if (start != pch) {
                std::string str(start, pch);
                result.push_back(str);
            } else {
                result.push_back("");
            }
            start = pch + 1;
        }
    }
    result.push_back(start);
    return result;
}

inline void logDumpDims(const hiai::TensorDimension &dims)
{
    HIAI_ENGINE_LOG(APP_INFO, "tensor name %s, dim N: %d, C: %d, H: %d, W: %d", dims.name.c_str(), dims.n, dims.c,
                    dims.h, dims.w);
}

#endif  // ATLASFACEDEMO_ENGINE_TOOLS_H
