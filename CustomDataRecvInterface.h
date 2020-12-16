#ifndef CUSTOM_DATA_RECV_INTERFACE_H
#define CUSTOM_DATA_RECV_INTERFACE_H
#include <hiaiengine/api.h>
#include <string.h>

class CustomDataRecvInterface : public hiai::DataRecvInterface {
public:
    /**
    * @ingroup FasterRcnnDataRecvInterface
    * @brief init
    * @param [in]desc:std::string
    */
    CustomDataRecvInterface(const int graphID, const int chNum = 1):graphID(graphID), videoChNum(chNum)
    {
        if (videoChNum <= 0) {
            videoChNum = 1;
        }

        try {
            chFlag = new bool[videoChNum];
        } catch (bad_alloc &memExp) {
            chFlag = NULL;
        }

        for (int i = 0; i < videoChNum; ++i) {
            *(chFlag + i) = false;
        }
    }

    ~CustomDataRecvInterface()
    {
        if (chFlag != NULL) {
            delete[] chFlag;
        }
    }

    /**
    * @ingroup FasterRcnnDataRecvInterface
    * @brief RecvData RecvData
    * @param [in]
 */
    HIAI_StatusT RecvData(const std::shared_ptr<void> &message);

private:
    int graphID;
    bool *chFlag;
    int videoChNum;

    void SetChFlag(int channel)
    {
        if ((chFlag != NULL) && (channel < videoChNum)) {
            chFlag[channel] = true;
        }
    }

    bool GetGraphFlag(void)
    {
        bool flag = true;
        if (chFlag != NULL) {
            for (int i = 0; i < videoChNum; ++i) {
                flag = flag && chFlag[i];
            }
        } else {
            flag = false;
        }

        return flag;
    }
};

#endif  // MAIN_H
