DATAS SEGMENT   
    ;此处输入数据段代码 
    string_main db 'plase input the function number (1~5)$'
	tip DB 'please input string:','$'
    string_task2_result2 db 'The maximum is :$'
    string_error db 'Wrong number, please input again. $'
    string_task3_info db 'please input the DECimAL numbers:',0DH,0AH,'$'  
    again_or_return db 'What do you want to do next" Main Menu or Redo"[ESC/any other key]: $'
    task1_info db 'Now, we are doing fuction 1: $' 
    task2_info db 'Now, we are doing fuction 2: $' 
    task3_info db 'Now, we are doing fuction 3: $' 
    ; emu8086不支持dup(?) 因此用0来初始化
    string_task1 db 100 dup(0)
    string_task1_result db 100 dup(0)
    string_task2 db 100 dup(0)  
    string_task2_result db 10 dup(0)	 
    string_task3 db 100 dup(0) 
    string_task3_count db 10 dup(0)
    string_task3_result db 100 dup(0)	        
    hour db 0
    minute db 0
    second db 0
    time db "00:00:00$"  
    len equ $-time
    num db 0   

DATAS ENDS

STACKS SEGMENT
	TOP LABEL WORD
    ;此处输入堆栈段代码 
    DW      100H    DUP(0)
STACKS ENDS         

;换行 
print_line macro
    MOV    AH,2      ;调用2号功能系统，将DL的内容显示到屏幕
    MOV    DL,0DH    ;0DH是回车符的ASCII码
    INT    21h
    MOV    AH,2
    MOV    DL,0AH    ;0AH是换行符的ASCII码
    INT    21h
endm       

;调用DOS功能，该功能为显示DS：DX地址处的字符   
print_string macro string
    LEA    DX,string
    MOV    AH,09h
    INT    21h
endm  

;用来获取单个输入字符   
get_char macro
    MOV    AH,1
    INT    21h
endm
          
CODES SEGMENT
    ASSUME CS:CODES,DS:DATAS,SS:STACKS
START:
    MOV AX,DATAS
    MOV DS,AX     
    
menu:
    print_string string_main
    get_char   
    ;只能输入1-5
    CMP    AL,31h
    JB     main_input_error
    CMP    AL,35h
    JA     main_input_error
    JMP    select
    
main_input_error:
    print_line
    print_string string_error    
    print_line
    JMP    menu    
    
select:    
     CMP   AL,31h
     JE    Task1
     CMP   AL,32h
     JE    Task2
     CMP   AL,33h
     JE    Task3
     CMP   AL,34h
     JE    Task4
     CMP   AL,35h
     JE    Task5
 
return_menu:
     print_line
     JMP    menu     

;任务一：实现小写转化成大写
Task1 PROC
    print_line 
    print_line 
    print_string task1_info
    print_line 
    print_string tip
    LEA    SI,string_task1
    MOV    CL,00H;CL用来存储需要处理字符的数量，初始化置0           
    
restore_input:    
    get_char
    MOV    BL,AL  ;BL用来存储输入的字符信息，保护数据
    ;假如当前字符输入是回车(0DH)，则不进行存储，直接跳出
    CMP    AL,0DH
    JE     deal_first 
    ;对空格字符进行存储，单独判断
    CMP    AL,20H ;20H为"空格"的ASCII码
    JE     real_store  
    ;设置输入字符的范围，只能存储数字、大小写字母，否则报错     
    CMP    AL,30H ;30H为"0"的ASCII码
    JB     input_error
    CMP    AL,39H ;39H为"9"的ASCII码
    JBE    real_store
    CMP    AL,41H ;41H为"A"的ASCII码
    JB     input_error
    CMP    AL,5AH ;5AH为"Z"的ASCII码
    JBE    real_store
    CMP    AL,61H ;61H为"a"的ASCII码
    JB     input_error    
    CMP    AL,7AH ;7AH为"z"的ASCII码
    JBE    real_store    
    
;错误跳转信息，并重新提示输入  
input_error:
    print_line
    print_string string_error
    print_line
    print_string tip
    LEA    SI,string_task1
    MOV    CL,00H;CL用来存储需要处理字符的数量，初始化置0  
    JMP    restore_input

;存储当前字符   
real_store:     
    MOV    AL,BL ;将BL存储的字符重新赋值给AL
    MOV    [SI],AL ;将AL存储的字符传输到[SI]进行存储
    INC    SI
    INC    CL
    INC    DI 
    ;未检测到结束标志“回车”，返回上一步继续存储下一个字符
    JMP    restore_input

;重新回到存储的首地址位置，对每一个字符进行处理    
deal_first:    
    LEA    SI,string_task1
    LEA    DI,string_task1_result
    
cmp_data: 
    ;CL用来记录剩余的处理字符数量，若CL无空值，直接跳出   
    CMP    CL,00h    
    JE     task1_finish
    MOV    AL,[SI]
    ;只有小写字母才进行转换，若不是小写字母直接输出
    CMP    AL,61h
    JB     cmp_data_finish
    SUB    AL,20h    ;小写转大写：小写字母-20H
    
cmp_data_finish:
    ;[DI]用来存储处理后的字符数据
    MOV    [DI],AL
    INC    SI
    DEC    CL ;处理完一个字符，CL-1 
    INC    DI 
    ;CL不为0，代表没处理完
    JMP    cmp_data
    
task1_finish:
    print_line 
    ;为输入和输出字符串添加结束标志
    MOV    [SI],'$'    
    MOV    [DI],'$' 
    ;这里显示输入
    print_string string_task1
    print_line           
    ;这里显示输出结果
    print_string string_task1_result
    print_line
    
    ;判断返回菜单或者重试
    print_string again_or_return    
    get_char
    CMP    AL,1BH  	;判断是否按下esc 
    JE     return_menu  

    JMP    Task1   

Task1 ENDP

;任务二：在字符中找最大值
Task2 PROC   
    print_line 
    print_line 
    print_string task2_info
    print_line 
    print_string tip
    LEA    SI,string_task2
    MOV    CL,00H;CL用来存储需要处理字符的数量，初始化置0 
    
task_2_restore_input:    
    get_char
    MOV    BL,AL  ;BL用来存储输入的字符信息，保护数据
    ;假如当前字符输入是回车(0DH)，则不进行存储，直接跳出
    CMP    AL,0DH
    JE     task2_dealfirst 
    
     ;设置输入字符的范围，只能存储数字、大小写字母，否则报错     
    CMP    AL,30H ;30H为"0"的ASCII码
    JB     task2_input_error
    CMP    AL,39H ;39H为"9"的ASCII码
    JBE    task2_store
    CMP    AL,41H ;41H为"A"的ASCII码
    JB     task2_input_error
    CMP    AL,5AH ;5AH为"Z"的ASCII码
    JBE    task2_store
    CMP    AL,61H ;61H为"a"的ASCII码
    JB     task2_input_error    
    CMP    AL,7AH ;7AH为"z"的ASCII码
    JBE    task2_store   
            
task2_store:
    MOV    AL,BL
    MOV    [SI],AL
    INC    SI
    INC    CL
    JMP    task_2_restore_input 
 
;错误跳转信息，并重新提示输入  
task2_input_error:
    print_line
    print_string string_error
    print_line
    print_string tip 
    JMP    Task2
    
task2_dealfirst:    
    LEA    SI,string_task2
    LEA    DI,string_task2_result    
    MOV    AL,[SI]
    MOV    [DI],AL   ;[DI]用来存储输入的数据
    INC    SI
    DEC    CL
    
;遍历比较    
func_2_cmp_data:
    ;假如已经遍历完毕则进行跳出    
    CMP    CL,00h    
    JE     func2_finish
    MOV    AL,[SI]
    MOV    AH,[DI]
    ;假如AL>AH，进行跳转，将AL的值存入[DI]，即最大值
    CMP    AL,AH
    JA     func_2_cmp_data_finish     ;JA:无符号大于则跳转
    INC    SI
    DEC    CL
    JMP    func_2_cmp_data
    
func_2_cmp_data_finish:
    MOV    [DI],AL
    INC    SI
    DEC    CL
    JMP    func_2_cmp_data
    
func2_finish:
    print_line
    MOV    [SI],'$'
    INC    DI    
    MOV    [DI],'$'
    ;先输出原字符串   
    print_string string_task2
    ;补一个空格进行分隔
    MOV    DL,20h
    MOV    AH,2
    INT    21H 
    print_string string_task2_result2 ;the maximum is :$
    print_string string_task2_result
    print_line
    
    ;判断返回菜单或者重试
    print_string again_or_return    
    get_char
    CMP    AL,1BH  	;判断是否按下esc 
    JE     return_menu 

    JMP    Task2  

Task2 ENDP
 
;任务三：输入数据组的排序
Task3 PROC 
    print_line 
    print_line 
    print_string task3_info
    print_line     
    print_string string_task3_info   
    LEA    SI,string_task3
    MOV    CL,0
    MOV    BH,0    
    MOV    DI,0  
      
task3_store:    
    MOV    DL,0 
    MOV    DH,10  
    
task_3_real_store:
    get_char
    CMP    AL,0DH  	;判断是否按下回车键 
    JE     task3_next_step
    CMP    AL,20H  	;判断是否按下空格键 
    JE     task3_save_num  
    ;小于0或大于9跳转报错
    CMP    AL,30H ;30H为"0"的ASCII码
    JB     task3_error_input
    CMP    AL,39H ;39H为"9"的ASCII码
    JA     task3_error_input 
    MOV    BH,0      
    ;这里首先需要将ASCII码转换成十进制数，0的ASCII码为30H，因此可以用ASCII码-30H或者将高位清零
    AND    AL,0FH ;高4位清零 
    MOV    BL,AL
    MOV    AL,DL
    MUL    DH     ;每一个高位*10
    ADD    AX,BX
    MOV    DL,AL 
    JMP    task_3_real_store 
    
task3_error_input: 
    print_line
    print_string string_error   
    JMP    Task3

;检测到空格则进行一次数值保存 
task3_save_num:    
    CMP    BH,0
    JNZ    task3_before_store
    MOV    [SI],DL
    INC    SI
    INC    CL  
    
task3_before_store:    
    MOV    BH,1
    JMP    task3_store     

;输出回车之后    
task3_next_step:
    ;注意，换行会改变DL数值，因此必须用堆栈保护DL，栈仅能对字节进行操作
    PUSH   DX 
    print_line
    POP    DX
    MOV    [SI],DL 
    INC    SI
    INC    CL
    LEA    DI,string_task3_count
    MOV    [DI],CL

task3_finish:
    LEA    DI,string_task3_count
    MOV    CL,[DI]    
        
    LEA    DI,string_task3
    PUSH   DX 
    print_line
    POP    DX

;将十六进制数转换成ASCII码显示输出 
task3_write:   
    MOV    BH,[DI]
    MOV    DL,BH
    AND    DL,0F0H ;低位清零
    SHR    DL,4    ;逻辑右移，将DL右移4位    
    CMP    DL,0AH
    JGE    task3_writehighchar
    ADD    DL,30H  ;数值0-9要进行显示需转换成ASCII码，需+30H
    JMP    task3_writehigh  

;用来写当前十六进制数的高位    
task3_writehighchar:
    ADD    DL,37H  ;数值A-F要进行显示需转换成ASCII码，需+37H   
    
task3_writehigh:
    MOV    AH,2
    INT    21H
    MOV    DL,BH
    AND    DL,00FH ;高位清零  
    CMP    DL,0AH
    JGE    task3_writelowchar    ;JGE大于等于
    ADD    DL,30H
    JMP    task3_writelow 
    
;写当前十六进制数的低位    
task3_writelowchar:
    ADD    DL,37H     
      
task3_writelow: 
     
    MOV    AH,2
    INT    21H   
    ;每输出一个十六进制数在后面添加H标识
    MOV    DL,48H
    MOV    AH,2
    INT    21H   
    ;每输出一个十六进制数显示空格进行分隔
    MOV    DL,20h
    MOV    AH,2
    INT    21H    
     
    INC    DI
    DEC    CL
    CMP    CL,0   ;CL记录剩余处理的字符数量，假如不为0，继续跳转运行
    JNE    task3_write
 
;采用选择排序实现十六进制从小到大输出
;BL用来存储第i位，BH用来存储第i+1,i+2,i+3...位，每次外循环确定第i位最小值
sort: 
    LEA    SI,string_task3_count
    MOV    CL,[SI]
    LEA    SI,string_task3   
    
;外循环
fori:
    LEA    DI,string_task3_count 
    MOV    CH,CL
    MOV    DI,SI
    INC    DI  
;内循环 
forj:
    MOV    BL,[SI]    
    MOV    BH,[DI]
    CMP    BL,BH                                   
    JBE    afterswap
    MOV    [SI],BH
    MOV    [DI],BL    
    
afterswap:
    DEC    CH
    INC    DI
    CMP    CH,1
    JA     forj
    
    INC    SI
    DEC    CL
    CMP    CL,1                               
    JA     fori
             
task3_finish2:
    LEA    DI,string_task3_count
    MOV    CL,[DI]    
    LEA    DI,string_task3
    PUSH   DX 
    print_line
    POP    DX
    
;输出排序后的结果    
task3_write2:   
    MOV    BH,[DI]
    MOV    DL,BH
    AND    DL,0F0H ;低位清零
    SHR    DL,4    ;逻辑右移，将DL右移4位    
    CMP    DL,0AH
    JGE    task3_writehighchar2
    ADD    DL,30H  ;数值0-9要进行显示需转换成ASCII码，需+30H
    JMP    task3_writehigh2  

task3_writehighchar2:
    ADD    DL,37H  ;数值A-F要进行显示需转换成ASCII码，需+37H   
    
task3_writehigh2:
    MOV    AH,2
    INT    21H
    MOV    DL,BH
    AND    DL,00FH ;高位清零  
    CMP    DL,0AH
    JGE    task3_writelowchar2    ;JGE大于等于
    ADD    DL,30H
    JMP    task3_writelow2 
    
;写当前十六进制数的低位    
task3_writelowchar2:
    ADD    DL,37H     
      
task3_writelow2:  
    MOV    AH,2
    INT    21H    
    ;每输出一个十六进制数在后面添加H标识
    MOV    DL,48H
    MOV    AH,2
    INT    21H   
    ;每输出一个十六进制数显示空格进行分隔
    MOV    DL,20h
    MOV    AH,2
    INT    21H        
    INC    DI
    DEC    CL
    CMP    CL,0             ;CL记录剩余处理的字符数量，假如不为0，继续跳转运行
    JNE    task3_write2   
     
    PUSH   DX 
    print_line
    POP    DX 
    ;判断返回菜单或者重试
    print_string again_or_return    
    get_char
    CMP    AL,1BH  	;判断是否按下esc 
    JE     return_menu 

    JMP    Task3 
             
Task3 ENDP


;任务四：输出系统时间到屏幕右上角
Task4 PROC   
;首先对窗口清屏    
clear:    
    MOV AX,CS
    MOV SS,AX
    MOV SP,0x7c00  ;set stack and SP

    MOV AH,0x06
    MOV AL,0

    MOV CH,0  ;(0,0)
    MOV CL,0
    MOV DH,24  ;(24,79)
    MOV DL,79
    MOV BH,0x07 ;黑底白字
    INT 0x10

;设置光标位置 
setshow:    
    MOV DH,0  ;行号    
    MOV DL,72  ;列号  
    MOV BH,0
    ;置光标位置
    MOV AH,2   
    INT 10h   

;获取系统时间
get_system_time: 
    ;获取时间
    MOV AH,2Ch     ;CH=时,CL=分,DH=秒
    INT 21h         
    MOV hour,CH
    MOV minute,CL
    MOV second,DH    
    ;时间显示到字符串
    MOV AX,0      
    MOV AL,hour     ;商:AL,余数:AH
    MOV num,10
    DIV num
    ADD AL,30h
    MOV time[0],AL
    ADD AH,30h
    MOV time[1],AH 
    
    MOV AX,0
    MOV AL,minute
    MOV num,10
    DIV num
    ADD AL,30h
    MOV time[3],AL
    ADD AH,30h
    MOV time[4],AH
    
    MOV AX,0
    MOV AL,second
    MOV num,10
    DIV num
    ADD AL,30h
    MOV time[6],AL
    ADD AH,30h
    MOV time[7],AH
    
    ;输出系统时间
    LEA DX, time
    MOV AH, 9   
    INT 21h  

    ;判断返回菜单或者重试
    print_string again_or_return    
    get_char
    CMP    AL,1BH  	;判断是否按下esc 
    JE     return_menu 

    JMP    setshow 

Task4 ENDP
 
 
;任务五：退出 
Task5 PROC
    MOV AH, 4CH
    INT 21h
Task5 ENDP    
   	
CODES ENDS
    END START



















