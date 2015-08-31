# README

- 使用以下宏定义开关决定应用行为：

```
#define load
```

- 打开开关后应用可将当前所有进程输出到 **`crc32list.txt`**，形如：

```
\Device\HarddiskVolume1\Windows\explorer.exe:1058962686
\Device\HarddiskVolume1\Program Files\Realtek\Audio\HDA\RtkNGUI64.exe:456284698
\Device\HarddiskVolume1\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static\MOM.exe:1334055883
\Device\HarddiskVolume2\Program Files\QQ2012\Bin\QQ.exe:690026472
\Device\HarddiskVolume1\Program Files (x86)\ATI Technologies\ATI.ACE\Core-Static\CCC.exe:-450218975
\Device\HarddiskVolume2\Program Files\QQ2012\Bin\TXPlatform.exe:1333314291
```

- 配置 **`list.txt`** 文件（例如添加 QQ.exe TXPlatform.exe 的 crc32 码入列表）：

```
690026472
1333314291
```

- **关闭开关编译，后台运行程序即可静默监控进程列表，并杀死指定进程**