## Language

- [English](#english)
- [中文](#中文)

---

# English
### Introduction
This project aims to integrate multiple projects to create an easy-to-use and easy-to-set-up custom FindMy network. This is an extension and improvement of the Macless version. This repository primarily stores the firmware for ch59x. The usage of the firmware is consistent with the tutorial in the original macless repository.
### Firmware Setup
1. Go to the [releases](https://github.com/5Breeze/heystack-ch59x/releases ) section and download the required firmware file.
2. Execute the `generate_keys.py` script to generate your key pair. (Note: The `cryptography` dependency is required. Install it using `pip install cryptography`.)
3. Use the following script to fix your firmware:  
   `xxd -p -c 100000 xxx_keyfile | xxd -r -p | dd of=xxx.bin skip=1 bs=1 seek=$(grep -oba OFFLINEFINDINGPUBLICKEYHERE! xxx.bin | cut -d ':' -f 1) conv=notrunc`  
   (Note: The `xxd` dependency is required. Install it using `apt install xxd`.)
4. Unzip the firmware and flash it to your device using [WCH_ISP_TOOL](https://www.wch.cn/download/WCHISPTool_Setup_exe.html ).

### Other Questions
You can visit [macless](https://github.com/dchristl/macless-haystack ) to find tools and assistance.

### Disclaimer
This repository is intended for research purposes only. The use of this code is your responsibility.  
I assume NO responsibility and/or liability for how you choose to use any of the source code available here. By using any of the files in this repository, you acknowledge that you are AGREEING TO USE AT YOUR OWN RISK. Once again, ALL files available here are for EDUCATIONAL and/or RESEARCH purposes ONLY.


---

# 中文
### 介绍
本项目旨在整合多个项目，以创建一个易于使用和设置的自定义 FindMy 网络。这是macless版本的扩充和改进，本仓库主要存储ch59x的固件，固件使用方式和源仓库[macless](https://github.com/dchristl/macless-haystack )的教程一致。

### 固件设置
1. 前往[发布](https://github.com/5Breeze/heystack-ch59x/releases )页面，下载所需的固件文件。
2. 执行`generate_keys.py`脚本以生成密钥对。（注意：需要`cryptography`依赖项。使用`pip install cryptography`进行安装。）
3. 使用以下脚本修复固件：  
   `xxd -p -c 100000 xxx_keyfile | xxd -r -p | dd of=xxx.bin skip=1 bs=1 seek=$(grep -oba OFFLINEFINDINGPUBLICKEYHERE! xxx.bin | cut -d ':' -f 1) conv=notrunc`  
   （注意：需要`xxd`依赖项。使用`apt install xxd`进行安装。）
4. 解压固件，并使用[WCH_ISP_TOOL](https://www.wch.cn/download/WCHISPTool_Setup_exe.html )将其烧录到设备中。

### 其他问题
您可以访问[macless](https://github.com/dchristl/macless-haystack )以获取工具和帮助。

### 免责声明
本代码库仅用于研究目的。代码的使用由您自行负责。  
对于您选择如何使用此处提供的任何源代码，我不承担任何责任和/或 liability。使用本代码库中的任何文件，即表示您同意自行承担风险。再次强调，此处提供的所有文件仅供教育和/或研究使用。怎么能让这个readme变成中英切换按钮
