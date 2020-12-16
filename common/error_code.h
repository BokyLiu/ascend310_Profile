#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include "hiaiengine/status.h"
#include <string>

using namespace std;

/*
* define error code for HIAI_ENGINE_LOG
 */
#define MODID_APP_DEFINE 0x6001

const int ERROR_CODE_NUM = 26;

enum {
    APP_OK_CODE = 0,        // �ɹ�
    APP_FAIL_CODE,          // һ���Դ���
    APP_INNER_CODE,         // �ڲ�����һ����ͬһ��ģ����ʹ�ã������⹫��
    APP_POINTER_CODE,       // �Ƿ�ָ��
    APP_INVALARG_CODE,      // �Ƿ�����
    APP_NOTIMPL_CODE = 5,   // ����δʵ��
    APP_OUTOFMEM_CODE,      // �ڴ�����ʧ��/�ڴ治��
    APP_BUFERROR_CODE,      // ���������󣨲��㣬���ң�
    APP_PERM_CODE,          // Ȩ�޲��㣬����δ��Ȩ�Ķ���
    APP_TIMEOUT_CODE,       // ��ʱ
    APP_NOTINIT_CODE = 10,  // δ��ʼ��
    APP_INITFAIL_CODE,      // ��ʼ��ʧ��
    APP_ALREADY_CODE,       // �ѳ�ʼ�����Ѿ�������
    APP_INPROGRESS_CODE,    // �������С�����״̬
    APP_EXIST_CODE,         // ������Դ����(���ļ���Ŀ¼)�Ѵ���
    APP_NOTEXIST_CODE,      // ��Դ����(���ļ���Ŀ¼)������豸�Ȳ�����
    APP_BUSY_CODE,          // �豸����Դæ����Դ�����ã�
    APP_FULL_CODE,          // �豸/��Դ����
    APP_EMPTY_CODE,         // ����/�ڴ�/����Ϊ��
    APP_OPENFAIL_CODE,      // ��Դ����(���ļ���Ŀ¼��socket)��ʧ��
    APP_READFAIL_CODE,      // ��Դ����(���ļ���Ŀ¼��socket)��ȡ������ʧ��
    APP_WRITEFAIL_CODE,     // ��Դ����(���ļ���Ŀ¼��socket)д�롢����ʧ��
    APP_DELFAIL_CODE,       // ��Դ����(���ļ���Ŀ¼��socket)ɾ�����ر�ʧ��

    /* just for call HIAI_ENGINE_LOG() to output log */
    APP_INFO_CODE,
    APP_WARNING_CODE,
    APP_ERROR_CODE,
};

const string APP_LOG_STRING[ERROR_CODE_NUM] = {
    "Success",
    "General Failed",
    "Internal error",
    "Invalid Pointer",
    "Invalid argument",
    "Not implemented",
    "Out of memory",
    "Buffer error",
    "Permission denied",
    "Timed out",
    "Object not init",
    "Object init failed",
    "Operation already in progress",
    "Operation now in progress",
    "Object exist",
    "Object not exist",
    "Device or resource busy",
    "Device or resource full",
    "Device or resource empty",
    "Device or resource open failed",
    "Device or resource read failed",
    "Device or resource write failed",
    "Device or resource delete failed",

    /* just for call HIAI_ENGINE_LOG() to output log */
    "app log level: info",
    "app log level: warning",
    "app log level: error",
};

HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_OK, APP_LOG_STRING[APP_OK_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_FAIL, APP_LOG_STRING[APP_FAIL_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_INNER, APP_LOG_STRING[APP_INNER_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_POINTER, APP_LOG_STRING[APP_POINTER_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_INVALARG, APP_LOG_STRING[APP_INVALARG_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_NOTIMPL, APP_LOG_STRING[APP_NOTIMPL_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_OUTOFMEM, APP_LOG_STRING[APP_OUTOFMEM_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_BUFERROR, APP_LOG_STRING[APP_BUFERROR_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_PERM, APP_LOG_STRING[APP_PERM_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_TIMEOUT, APP_LOG_STRING[APP_TIMEOUT_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_NOTINIT, APP_LOG_STRING[APP_NOTINIT_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_INITFAIL, APP_LOG_STRING[APP_INITFAIL_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_ALREADY, APP_LOG_STRING[APP_ALREADY_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_INPROGRESS, APP_LOG_STRING[APP_INPROGRESS_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_EXIST, APP_LOG_STRING[APP_EXIST_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_NOTEXIST, APP_LOG_STRING[APP_NOTEXIST_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_BUSY, APP_LOG_STRING[APP_BUSY_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_FULL, APP_LOG_STRING[APP_FULL_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_EMPTY, APP_LOG_STRING[APP_EMPTY_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_OPENFAIL, APP_LOG_STRING[APP_OPENFAIL_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_READFAIL, APP_LOG_STRING[APP_READFAIL_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_WRITEFAIL, APP_LOG_STRING[APP_WRITEFAIL_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_DELFAIL, APP_LOG_STRING[APP_DELFAIL_CODE]);

HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_INFO, APP_INFO, APP_LOG_STRING[APP_INFO_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_WARNING, APP_WARNING, APP_LOG_STRING[APP_WARNING_CODE]);
HIAI_DEF_ERROR_CODE(MODID_APP_DEFINE, HIAI_ERROR, APP_ERROR, APP_LOG_STRING[APP_ERROR_CODE]);

static string GetAPPErrCodeInfo(uint32_t err)
{
    uint32_t errId = err & 0x0FFF;

    if (errId >= ERROR_CODE_NUM) {
        return "Error code unknown";
    } else {
        return APP_LOG_STRING[errId];
    }
}

#endif  // ERROR_CODE_H_
