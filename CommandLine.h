/**
 * ============================================================================
 *
 * Copyright (C) 2019, Huawei Technologies Co., Ltd. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1 Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *   2 Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *   3 Neither the names of the copyright holders nor the names of the
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ============================================================================
 */

#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H
#include <string>
#include <stdio.h>
#include <unistd.h>
#include "CommandParser.h"

using namespace std;

/// @brief Define flag for showing help message <br>
static const string HELP_MESSAGE = "Print a usage message.";
extern bool g_hExist;
extern bool g_mExist;
extern bool b_savexml;
extern string g_modelFile;
const string M_MESSAGE = "Must. 输入模型的路径";
extern string g_modelpath;
const string N_MESSAGE = "Optional. 计算次数， 默认值: 10";
extern string g_iter_num;
const string I_MESSAGE = "Optional. 输入数值，若-i=1那么输入元素全部是1, 默认值: 0";
const string O_MESSAGE = "Optional. 若输入-o则保存输出结果为xml文件";
extern string g_input_value;

static bool ValidateInput(string flagName, const string &value)
{
    /* judge whether folder is existing: !(-1) -- exist */
    if ((access(value.c_str(), F_OK)) == -1) {
        printf("Parameter %s valid value: %s, file or folder does not exist!\n", flagName.c_str(), value.c_str());
        return false;
    } else {
        return true;
    }
}

const int MIN_CH_NUM_PER_GRAPH = 1;
const int MAX_CH_NUM_PER_GRAPH = 4;
static bool ValidateVideoChNumPerGraph(string flagName, const int value)
{
    /*
    * hardware limit: 
    *    h26* decode support 16 channel video maximum
    *    Infer model performance 
    * software limti:
    *    Create DecodeEngine Object Number in graph.config
 */
    if ((value < MIN_CH_NUM_PER_GRAPH) || (value > MAX_CH_NUM_PER_GRAPH)) {
        printf("Parameter %s valid value: %d, beyond [%d, %d]\n", flagName.c_str(), value, MIN_CH_NUM_PER_GRAPH,
               MAX_CH_NUM_PER_GRAPH);
        return false;
    } else {
        return true;
    }
}

const int MIN_DEVICE_ID = 0;
const int MAX_DEVICE_ID = 63;
static bool ValidateDeviceId(string flagName, const int value)
{
    /* Atlas driver request device id must be in [0, 63] */
    if ((value < MIN_DEVICE_ID) || (value > MAX_DEVICE_ID)) {
        printf("Parameter %s valid value: %d, beyond [%d, %d]\n", flagName.c_str(), value,
               MIN_DEVICE_ID, MAX_DEVICE_ID);
        return false;
    } else {
        return true;
    }
}
/**
* @brief This function show a help message
 */
static void ShowUsage(void)
{
    printf("\n");
    printf("ascend310_Profile [OPTION]\n");
    printf("Options:\n");
    printf("\n");
    printf("    -m                    %s\n", M_MESSAGE.c_str());
    printf("    -h                    %s\n", HELP_MESSAGE.c_str());
    printf("    -n                    %s\n", N_MESSAGE.c_str());  
    printf("    -i                    %s\n", I_MESSAGE.c_str());
    printf("    -o                    %s\n", O_MESSAGE.c_str());
    printf("\n");
    printf("Eg:");
    printf(" ./ascend310_Profile -m model.om -n 10 -i 0 -o\n\n");
}

static bool ParseAndCheckCommandLine(int argc, char *argv[])
{
    // ---------------------------Parsing and validation of input args--------------------------------------
    CommandParser options;
    options.addOption("-h").addOption("-i", "0").addOption("-n", "10").addOption("-m").addOption("-o");

    options.parseArgs(argc, argv);
    g_hExist = options.cmdOptionExists("-h");
    g_mExist = options.cmdOptionExists("-m");
    b_savexml = options.cmdOptionExists("-o");
    g_input_value = options.cmdGetOption("-i");
    g_iter_num = options.cmdGetOption("-n");
    g_modelFile = options.cmdGetOption("-m");

    if (g_hExist || g_mExist == false) {
        ShowUsage();
        return false;
    }

    return true;
}

#endif  // COMMAND_LINE_H_
