# 2018年邀请赛水位控制代码
使用FDC2214采集水位数据，驱动水泵控制水位恒定<br>
现场给的水泵调速性能极差，实际驱动电路使用pwm后用运放滤波恒压控制，并且输出pwm值会快速来回抖动保持水泵运行。<br>
FDC2214的config寄存器有两个reserved位需要设置为1，这段代码没有注意到，导致fdc的输出幅度会自己调节，但调节的滞回电压时候会导致采集数据突变。所以开始之前需要先排空水槽，保证起始时候挡位确定。<br>