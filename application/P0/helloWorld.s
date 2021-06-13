N			EQU 10; 定义常量

;堆栈声明
Stack_Size 	EQU 0x00000400
			AREA STACK,NOINIT,READWRITE, ALIGN=3;  声明.Align=3表示8字节对齐 
Stack_Mem	SPACE Stack_Size;   分配空间,长度Stack_Size; 标记堆栈开始
__initial_sp;   标记堆栈末尾

;中断向量区
			AREA Reset,DATA,READONLY
__Vectors	DCD __initial_sp;   分配一个字节; 此处为MSP初始值=栈顶
			DCD Reset_Handler;  复位中断处理

;代码区
			THUMB;  声明下面为Thumb指令
			PRESERVE8;  保存8字节对齐
			AREA Init,CODE,READONLY
			ENTRY;  声明汇编入口
Reset_Handler; 下面为一个计算R1=1+2+..+N的例程
			LDR r0, =N
			MOV r1, #0
loop		ADD r1, r0
			SUBS r0, #1
			BNE loop
deadloop	B	deadloop;   程序结束，保持死循环
			NOP
			END;	标志汇编文件结束