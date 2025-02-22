# irisHelper

适用于搭载Pixelworks X7的设备
插帧、HDR、超分参数自定义工具

功能: 
* 读取包名，根据配置文件参数对/odm/bin/irisConfig进行设置
* 可对feas相关参数进行配置（必须设备本身支持）
* inotifyd监控配置文件，监控到修改即重载

食用方法：刷入模块，对/data/adb/modules/irisHelper/config/iriscfgcustomize.conf进行配置，也可以自己再添加包名
本模块与Hydro_BrÛleur模块iris_helper的配置文件通用，可以把其配置文件复制到本模块的对应位置

配置文件说明:
app: 被监控的包名
params_{a..d}: 传递给irisConfig的参数，最大支持四组
perfmgr_enable：是否开启feas
fixed_target_fps：游戏内目标帧率
perfmgr_powersave：Yuni/Pandora内核专属节点。在高帧率(90/120/144)游戏如需更大程度降低功耗，可以填写Y。此状态下游戏体验可能有一定劣化，还原可填写N或者将整行删除
至于参数写法，可以执行su -c /odm/bin/irisConfig -help指令查询，也可以查看模块内ab大佬写的iris_helper_user_guide.md

配置文件块写法：
app: "com.miHoYo.hkrpg"
params_a: 258 4 40 -1 2 -1 3 0
params_b: 47 1 13
params_c: 273 2 1 3
params_d: 267 2 1 3
perfmgr_enable: 1
fixed_target_fps: 60
perfmgr_powersave: N

本模块与ab大佬的读文件逻辑不大相同，不写冒号，引号也可以，如下：

app com.miHoYo.hkrpg
params_a 258 4 40 -1 2 -1 3 0
params_b 47 1 13
params_c 273 2 1 3
params_d 267 2 1 3
perfmgr_enable 1
fixed_target_fps 60
perfmgr_powersave Y

甚至是，第一列内容不规范的写法也行，例如:

$(&+#appa#)+' com.miHoYo.hkrpg
_("?'!params_axj 258 4 40 -1 2 -1 3 0
!'!$(2_params_bfhf 47 1 13
$('!!'params_cxjzj_&-+(@_" 273 2 1 3
_('39!params_dxhzj 267 2 1 3
ssdfccperfmgr_enablensns 1
jsjsbsfixed_target_fps 60
_\_=perfmgr_powersavehshs Y

当然，也只有第一列内容可以乱写
逻辑是只要第一列的字符串包含指定的子串就可以被识别
（第一列: 一行文本从左向右读，出现的第一个空格左边的全部内容为第一列的内容，不包含空格本身)
如果写了多个相同包名的配置块，则在运行时只会以先写的为准
如果连续写了多个相同的参数，则以最后的写的参数为准
必须对配置文件内的app进行至少一项的参数配置，不配置则不会被添加到容器
