# 工程结构

---
  |
  +-- application: 用户代码目录
  |
  +-- periph_config: CubeMx 工程目录 (芯片初始化代码生成目录)
  |
   -- build: 构建生成目录

# 注意

 - 使用 CubeMX 生成代码后，记得查看 Makefile 中**宏定义(C_DEFS)**的变化，并及时更新到工程中