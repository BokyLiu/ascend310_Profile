#include <unistd.h>
#include <sys/stat.h>

#include <thread>
#include <fstream> //ifstream读文件，ofstream写文件，fstream读写文件
#include <algorithm>
#include <libgen.h>
#include <string>
#include <map>
#include "proto/graph_config.pb.h"
#include "hiaiengine/api.h"

#include "CommandLine.h"
#include "common_data_type.h"
#include "CustomDataRecvInterface.h"

#include "opencv2/opencv.hpp" //算时间的

bool g_hExist = false;
bool g_mExist = false;
std::string g_graphFile = "./graph.config";
std::string g_modelFile = "";
string g_iter_num = "10";
string g_input_value = "0";
bool b_savexml = false;

bool g_signalReceived = false;
std::map<int, bool> g_graphFlag;

const int GRAPH_ID = 10000;
const int DEVICE_ID = 0;
const int HOST_PRE_PROCESS_ENGINE_ID = 1001;

void parseErrorCode(HIAI_StatusT code)
{
    uint32_t moduleId = (code >> 16) & 0xFFFF;
    uint32_t logLevel = (code >> 12) & 0xF;
    uint32_t codeName = code & 0x0FFF;
    std::cout << "moduleId: " << hex << moduleId
              << ", logLevel: " << oct << logLevel
              << ", codeName: " << codeName << std::endl;
}

void SigalHandler(int signo)
{
    if (signo == SIGINT) {
        g_signalReceived = true;
    }
}

int sendDataToGraph(const int value)
{
    // printf(std::string(fileName+".jpg").c_str());
    printf("* input data fill with %d\n", value);
    std::shared_ptr<Main2GraphInfo> main2GraphInfo = std::make_shared<Main2GraphInfo>();
    main2GraphInfo->isEOS = value;
    if(b_savexml == true)
        main2GraphInfo->strFileName = "true";
    else
    {
        main2GraphInfo->strFileName = "false";
    }
        
    std::shared_ptr<hiai::Graph> graph = hiai::Graph::GetInstance(GRAPH_ID);
    if (nullptr == graph) {
        return -1;
    }
    hiai::EnginePortID engineId;
    engineId.graph_id = GRAPH_ID;
    engineId.port_id = 0;
    engineId.engine_id = HOST_PRE_PROCESS_ENGINE_ID;

    HIAI_StatusT hiaiRet = graph->SendData(engineId, "Main2GraphInfo",
                                            std::static_pointer_cast<void>(main2GraphInfo));
    if (hiaiRet != HIAI_OK) {
        printf("call memcpy_s fail\n");
        return -1;
    }
    return 0;
}

int parseModelName(const std::string &fileName)
{
    std::ifstream infile(fileName, ios::in);
    if (infile.fail()){
        printf("[ERROR] %s : failed \n", __FUNCTION__);
        infile.close();
        return -1;
    }
    infile.close();
    return 0;
}


void modifyGraph(hiai::GraphConfig *graphcfg, std::string engineName, std::string key, std::string value)
{
    int engines = graphcfg->engines_size();
    for (int i = 0; i < graphcfg->engines_size(); ++i) {
        hiai::EngineConfig *engCfgPtr = graphcfg->mutable_engines(i);
        // 检查engine名是否相同
        std::string std_mutalbe_engine_name = *(engCfgPtr->mutable_engine_name());
        if (*(engCfgPtr->mutable_engine_name()) != engineName)
            continue;
        // 检查是否有可配置参数
        if (!engCfgPtr->has_ai_config())
            continue;
        // 查找是否存在key
        hiai::AIConfig *aiCfgPtr = engCfgPtr->mutable_ai_config();
        for (int j = 0; j < aiCfgPtr->items_size(); ++j) {
            hiai::AIConfigItem *itemCfgPtr = aiCfgPtr->mutable_items(j);
            if (*(itemCfgPtr->mutable_name()) != key)
                continue;
            itemCfgPtr->set_value(value);
        }
    }
}

// Init and create graph
HIAI_StatusT HIAI_InitAndStartGraph(void)
{
    HIAI_StatusT status;
    
    // 解析config
    std::cout<<g_graphFile<<std::endl;
    hiai::GraphConfigList graphConfigList;
    status = hiai::Graph::ParseConfigFile(g_graphFile, graphConfigList);
    if (status != HIAI_OK) {
        std::cout << "MAIN: ParseConfigFile failed: " << g_graphFile << ", status: " << status<<std::endl;
        return status;
    }
    hiai::GraphConfig *graphcfg = graphConfigList.mutable_graphs(0);
    graphcfg->set_device_id("0");
    graphcfg->set_graph_id(GRAPH_ID);
    modifyGraph(graphcfg, "InferenceEngine", "model", g_modelFile);

    status = HIAI_Init(DEVICE_ID);
    if (status != HIAI_OK) {
        parseErrorCode(status);
        printf("Failed to start the graph");
        return status;
    }

    status = hiai::Graph::CreateGraph(graphConfigList);
    // 这里报错的定义是HIAI_GRAPH_ENGINE_INIT_FAILED，
    // 查看log，也发现是device端的推断engine初始化失败，为啥咧
    if (status != HIAI_OK) {
        parseErrorCode(status);
        printf("CreateGraph %d error\n", GRAPH_ID);
        return status;
    }

    std::shared_ptr<hiai::Graph> graph = hiai::Graph::GetInstance(GRAPH_ID);
    if (nullptr == graph) {
        printf("Fail to get the graph-%u", GRAPH_ID);
        return status;
    }

    printf("DEVICE_ID = %d, GRAPH_ID = %d init success\n", DEVICE_ID, GRAPH_ID);

    return HIAI_OK;
}



int main(int argc, char *argv[])
{
    // parseErrorCode(16986191);
    /* register signal of the "ctrl + c" */
    if (signal(SIGINT, SigalHandler) == SIG_ERR) {
        printf("\ncan't catch SIGINT\n");
    }

    // 0.parse the command line
    if (ParseAndCheckCommandLine(argc, argv) != true) {
        return 0;
    }

    std::cout<<"* Model path:"<<g_modelFile<<std::endl;
    if (parseModelName(g_modelFile) != 0){
        return -1;
    }

    // 1.create graph
    HIAI_StatusT ret = HIAI_InitAndStartGraph();
    if (ret != HIAI_OK) {
        return -1;
    }
    printf("***Init over device %d***\n", DEVICE_ID);
    
    for(int i=0; i<atoi(g_iter_num.c_str()); i++)
        sendDataToGraph(atoi(g_input_value.c_str()));
    
    std::cout<<"press Enter to exit"<<std::endl;
    getchar();
    // printf("***g_inputFile:\n %s \nProcess over***\n", g_inputFile_i.c_str());
    // // 3.Exit
    // while (g_signalReceived == false) {
    //     usleep(20000);
    //     bool exitFlag = true;
    //     std::map<int, bool>::iterator iter;
    //     for (iter = g_graphFlag.begin(); iter != g_graphFlag.end(); iter++) {
    //         exitFlag = iter->second && exitFlag;
    //     }
    //     if (exitFlag) {
    //         break;
    //     }
    //     printf("wait process over ");
    // }
    // tm.stop();
    // std::cout << "spent:" << tm.getTimeMilli() << "ms" << std::endl;
    // 4.destroy graph
    // g_graphFlag.clear();
    // hiai::Graph::DestroyGraph(GRAPH_ID);

    return 0;
}
