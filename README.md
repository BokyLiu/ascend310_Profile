# Ascend310模型测试工具


为了方便AI1模型的测试开发此工具，提供om模型inference耗时测试、结果保存的功能

## 测试环境


应该是C32版本的AI1 --_--!!  （那个npu-info工具挂了，所以我也不知道当前版本）

## 用法


```shell
ascend310_Profile [OPTION]
Options:

    -m                    Must. 输入模型的路径
    -h                    Print a usage message.
    -n                    Optional. 计算次数， 默认值: 10
    -i                    Optional. 输入数值，若-i=1那么输入元素全部是1, 默认值: 0
    -o                    Optional. 若输入-o则保存输出结果为xml文件

Eg: ./ascend310_Profile -m model.om -n 10 -i 0 -o

```



## 结果


```shell

* Model path:/opt/ws/app/y1/model/siammask_127_custom.om
./graph.config
DEVICE_ID = 0, GRAPH_ID = 10000 init success
***Init over device 0***
* input data fill with 2
* input data fill with 2
* input data fill with 2
* input data fill with 2
* input data fill with 2
* input data fill with 2
* input data fill with 2
* input data fill with 2
* input data fill with 2
* input data fill with 2
press Enter to exit
*the running time is : 0.000782s
* Saving model output tensor as file: output_0_bn_scale44_0.xml
*the running time is : 0.034734s
*the running time is : 0.034760s
*the running time is : 0.034776s
*the running time is : 0.034789s
*the running time is : 0.034802s
*the running time is : 0.035021s
*the running time is : 0.035210s
*the running time is : 0.035394s
*the running time is : 0.035586s
```

