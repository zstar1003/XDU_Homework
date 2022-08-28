DATAS SEGMENT   
    ;�˴��������ݶδ��� 
    string_main db 'plase input the function number (1~5)$'
	tip DB 'please input string:','$'
    string_task2_result2 db 'The maximum is :$'
    string_error db 'Wrong number, please input again. $'
    string_task3_info db 'please input the DECimAL numbers:',0DH,0AH,'$'  
    again_or_return db 'What do you want to do next" Main Menu or Redo"[ESC/any other key]: $'
    task1_info db 'Now, we are doing fuction 1: $' 
    task2_info db 'Now, we are doing fuction 2: $' 
    task3_info db 'Now, we are doing fuction 3: $' 
    ; emu8086��֧��dup(?) �����0����ʼ��
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
    ;�˴������ջ�δ��� 
    DW      100H    DUP(0)
STACKS ENDS         

;���� 
print_line macro
    MOV    AH,2      ;����2�Ź���ϵͳ����DL��������ʾ����Ļ
    MOV    DL,0DH    ;0DH�ǻس�����ASCII��
    INT    21h
    MOV    AH,2
    MOV    DL,0AH    ;0AH�ǻ��з���ASCII��
    INT    21h
endm       

;����DOS���ܣ��ù���Ϊ��ʾDS��DX��ַ�����ַ�   
print_string macro string
    LEA    DX,string
    MOV    AH,09h
    INT    21h
endm  

;������ȡ���������ַ�   
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
    ;ֻ������1-5
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

;����һ��ʵ��Сдת���ɴ�д
Task1 PROC
    print_line 
    print_line 
    print_string task1_info
    print_line 
    print_string tip
    LEA    SI,string_task1
    MOV    CL,00H;CL�����洢��Ҫ�����ַ�����������ʼ����0           
    
restore_input:    
    get_char
    MOV    BL,AL  ;BL�����洢������ַ���Ϣ����������
    ;���統ǰ�ַ������ǻس�(0DH)���򲻽��д洢��ֱ������
    CMP    AL,0DH
    JE     deal_first 
    ;�Կո��ַ����д洢�������ж�
    CMP    AL,20H ;20HΪ"�ո�"��ASCII��
    JE     real_store  
    ;���������ַ��ķ�Χ��ֻ�ܴ洢���֡���Сд��ĸ�����򱨴�     
    CMP    AL,30H ;30HΪ"0"��ASCII��
    JB     input_error
    CMP    AL,39H ;39HΪ"9"��ASCII��
    JBE    real_store
    CMP    AL,41H ;41HΪ"A"��ASCII��
    JB     input_error
    CMP    AL,5AH ;5AHΪ"Z"��ASCII��
    JBE    real_store
    CMP    AL,61H ;61HΪ"a"��ASCII��
    JB     input_error    
    CMP    AL,7AH ;7AHΪ"z"��ASCII��
    JBE    real_store    
    
;������ת��Ϣ����������ʾ����  
input_error:
    print_line
    print_string string_error
    print_line
    print_string tip
    LEA    SI,string_task1
    MOV    CL,00H;CL�����洢��Ҫ�����ַ�����������ʼ����0  
    JMP    restore_input

;�洢��ǰ�ַ�   
real_store:     
    MOV    AL,BL ;��BL�洢���ַ����¸�ֵ��AL
    MOV    [SI],AL ;��AL�洢���ַ����䵽[SI]���д洢
    INC    SI
    INC    CL
    INC    DI 
    ;δ��⵽������־���س�����������һ�������洢��һ���ַ�
    JMP    restore_input

;���»ص��洢���׵�ַλ�ã���ÿһ���ַ����д���    
deal_first:    
    LEA    SI,string_task1
    LEA    DI,string_task1_result
    
cmp_data: 
    ;CL������¼ʣ��Ĵ����ַ���������CL�޿�ֵ��ֱ������   
    CMP    CL,00h    
    JE     task1_finish
    MOV    AL,[SI]
    ;ֻ��Сд��ĸ�Ž���ת����������Сд��ĸֱ�����
    CMP    AL,61h
    JB     cmp_data_finish
    SUB    AL,20h    ;Сдת��д��Сд��ĸ-20H
    
cmp_data_finish:
    ;[DI]�����洢�������ַ�����
    MOV    [DI],AL
    INC    SI
    DEC    CL ;������һ���ַ���CL-1 
    INC    DI 
    ;CL��Ϊ0������û������
    JMP    cmp_data
    
task1_finish:
    print_line 
    ;Ϊ���������ַ�����ӽ�����־
    MOV    [SI],'$'    
    MOV    [DI],'$' 
    ;������ʾ����
    print_string string_task1
    print_line           
    ;������ʾ������
    print_string string_task1_result
    print_line
    
    ;�жϷ��ز˵���������
    print_string again_or_return    
    get_char
    CMP    AL,1BH  	;�ж��Ƿ���esc 
    JE     return_menu  

    JMP    Task1   

Task1 ENDP

;����������ַ��������ֵ
Task2 PROC   
    print_line 
    print_line 
    print_string task2_info
    print_line 
    print_string tip
    LEA    SI,string_task2
    MOV    CL,00H;CL�����洢��Ҫ�����ַ�����������ʼ����0 
    
task_2_restore_input:    
    get_char
    MOV    BL,AL  ;BL�����洢������ַ���Ϣ����������
    ;���統ǰ�ַ������ǻس�(0DH)���򲻽��д洢��ֱ������
    CMP    AL,0DH
    JE     task2_dealfirst 
    
     ;���������ַ��ķ�Χ��ֻ�ܴ洢���֡���Сд��ĸ�����򱨴�     
    CMP    AL,30H ;30HΪ"0"��ASCII��
    JB     task2_input_error
    CMP    AL,39H ;39HΪ"9"��ASCII��
    JBE    task2_store
    CMP    AL,41H ;41HΪ"A"��ASCII��
    JB     task2_input_error
    CMP    AL,5AH ;5AHΪ"Z"��ASCII��
    JBE    task2_store
    CMP    AL,61H ;61HΪ"a"��ASCII��
    JB     task2_input_error    
    CMP    AL,7AH ;7AHΪ"z"��ASCII��
    JBE    task2_store   
            
task2_store:
    MOV    AL,BL
    MOV    [SI],AL
    INC    SI
    INC    CL
    JMP    task_2_restore_input 
 
;������ת��Ϣ����������ʾ����  
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
    MOV    [DI],AL   ;[DI]�����洢���������
    INC    SI
    DEC    CL
    
;�����Ƚ�    
func_2_cmp_data:
    ;�����Ѿ�����������������    
    CMP    CL,00h    
    JE     func2_finish
    MOV    AL,[SI]
    MOV    AH,[DI]
    ;����AL>AH��������ת����AL��ֵ����[DI]�������ֵ
    CMP    AL,AH
    JA     func_2_cmp_data_finish     ;JA:�޷��Ŵ�������ת
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
    ;�����ԭ�ַ���   
    print_string string_task2
    ;��һ���ո���зָ�
    MOV    DL,20h
    MOV    AH,2
    INT    21H 
    print_string string_task2_result2 ;the maximum is :$
    print_string string_task2_result
    print_line
    
    ;�жϷ��ز˵���������
    print_string again_or_return    
    get_char
    CMP    AL,1BH  	;�ж��Ƿ���esc 
    JE     return_menu 

    JMP    Task2  

Task2 ENDP
 
;�����������������������
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
    CMP    AL,0DH  	;�ж��Ƿ��»س��� 
    JE     task3_next_step
    CMP    AL,20H  	;�ж��Ƿ��¿ո�� 
    JE     task3_save_num  
    ;С��0�����9��ת����
    CMP    AL,30H ;30HΪ"0"��ASCII��
    JB     task3_error_input
    CMP    AL,39H ;39HΪ"9"��ASCII��
    JA     task3_error_input 
    MOV    BH,0      
    ;����������Ҫ��ASCII��ת����ʮ��������0��ASCII��Ϊ30H����˿�����ASCII��-30H���߽���λ����
    AND    AL,0FH ;��4λ���� 
    MOV    BL,AL
    MOV    AL,DL
    MUL    DH     ;ÿһ����λ*10
    ADD    AX,BX
    MOV    DL,AL 
    JMP    task_3_real_store 
    
task3_error_input: 
    print_line
    print_string string_error   
    JMP    Task3

;��⵽�ո������һ����ֵ���� 
task3_save_num:    
    CMP    BH,0
    JNZ    task3_before_store
    MOV    [SI],DL
    INC    SI
    INC    CL  
    
task3_before_store:    
    MOV    BH,1
    JMP    task3_store     

;����س�֮��    
task3_next_step:
    ;ע�⣬���л�ı�DL��ֵ����˱����ö�ջ����DL��ջ���ܶ��ֽڽ��в���
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

;��ʮ��������ת����ASCII����ʾ��� 
task3_write:   
    MOV    BH,[DI]
    MOV    DL,BH
    AND    DL,0F0H ;��λ����
    SHR    DL,4    ;�߼����ƣ���DL����4λ    
    CMP    DL,0AH
    JGE    task3_writehighchar
    ADD    DL,30H  ;��ֵ0-9Ҫ������ʾ��ת����ASCII�룬��+30H
    JMP    task3_writehigh  

;����д��ǰʮ���������ĸ�λ    
task3_writehighchar:
    ADD    DL,37H  ;��ֵA-FҪ������ʾ��ת����ASCII�룬��+37H   
    
task3_writehigh:
    MOV    AH,2
    INT    21H
    MOV    DL,BH
    AND    DL,00FH ;��λ����  
    CMP    DL,0AH
    JGE    task3_writelowchar    ;JGE���ڵ���
    ADD    DL,30H
    JMP    task3_writelow 
    
;д��ǰʮ���������ĵ�λ    
task3_writelowchar:
    ADD    DL,37H     
      
task3_writelow: 
     
    MOV    AH,2
    INT    21H   
    ;ÿ���һ��ʮ���������ں������H��ʶ
    MOV    DL,48H
    MOV    AH,2
    INT    21H   
    ;ÿ���һ��ʮ����������ʾ�ո���зָ�
    MOV    DL,20h
    MOV    AH,2
    INT    21H    
     
    INC    DI
    DEC    CL
    CMP    CL,0   ;CL��¼ʣ�ദ����ַ����������粻Ϊ0��������ת����
    JNE    task3_write
 
;����ѡ������ʵ��ʮ�����ƴ�С�������
;BL�����洢��iλ��BH�����洢��i+1,i+2,i+3...λ��ÿ����ѭ��ȷ����iλ��Сֵ
sort: 
    LEA    SI,string_task3_count
    MOV    CL,[SI]
    LEA    SI,string_task3   
    
;��ѭ��
fori:
    LEA    DI,string_task3_count 
    MOV    CH,CL
    MOV    DI,SI
    INC    DI  
;��ѭ�� 
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
    
;��������Ľ��    
task3_write2:   
    MOV    BH,[DI]
    MOV    DL,BH
    AND    DL,0F0H ;��λ����
    SHR    DL,4    ;�߼����ƣ���DL����4λ    
    CMP    DL,0AH
    JGE    task3_writehighchar2
    ADD    DL,30H  ;��ֵ0-9Ҫ������ʾ��ת����ASCII�룬��+30H
    JMP    task3_writehigh2  

task3_writehighchar2:
    ADD    DL,37H  ;��ֵA-FҪ������ʾ��ת����ASCII�룬��+37H   
    
task3_writehigh2:
    MOV    AH,2
    INT    21H
    MOV    DL,BH
    AND    DL,00FH ;��λ����  
    CMP    DL,0AH
    JGE    task3_writelowchar2    ;JGE���ڵ���
    ADD    DL,30H
    JMP    task3_writelow2 
    
;д��ǰʮ���������ĵ�λ    
task3_writelowchar2:
    ADD    DL,37H     
      
task3_writelow2:  
    MOV    AH,2
    INT    21H    
    ;ÿ���һ��ʮ���������ں������H��ʶ
    MOV    DL,48H
    MOV    AH,2
    INT    21H   
    ;ÿ���һ��ʮ����������ʾ�ո���зָ�
    MOV    DL,20h
    MOV    AH,2
    INT    21H        
    INC    DI
    DEC    CL
    CMP    CL,0             ;CL��¼ʣ�ദ����ַ����������粻Ϊ0��������ת����
    JNE    task3_write2   
     
    PUSH   DX 
    print_line
    POP    DX 
    ;�жϷ��ز˵���������
    print_string again_or_return    
    get_char
    CMP    AL,1BH  	;�ж��Ƿ���esc 
    JE     return_menu 

    JMP    Task3 
             
Task3 ENDP


;�����ģ����ϵͳʱ�䵽��Ļ���Ͻ�
Task4 PROC   
;���ȶԴ�������    
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
    MOV BH,0x07 ;�ڵװ���
    INT 0x10

;���ù��λ�� 
setshow:    
    MOV DH,0  ;�к�    
    MOV DL,72  ;�к�  
    MOV BH,0
    ;�ù��λ��
    MOV AH,2   
    INT 10h   

;��ȡϵͳʱ��
get_system_time: 
    ;��ȡʱ��
    MOV AH,2Ch     ;CH=ʱ,CL=��,DH=��
    INT 21h         
    MOV hour,CH
    MOV minute,CL
    MOV second,DH    
    ;ʱ����ʾ���ַ���
    MOV AX,0      
    MOV AL,hour     ;��:AL,����:AH
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
    
    ;���ϵͳʱ��
    LEA DX, time
    MOV AH, 9   
    INT 21h  

    ;�жϷ��ز˵���������
    print_string again_or_return    
    get_char
    CMP    AL,1BH  	;�ж��Ƿ���esc 
    JE     return_menu 

    JMP    setshow 

Task4 ENDP
 
 
;�����壺�˳� 
Task5 PROC
    MOV AH, 4CH
    INT 21h
Task5 ENDP    
   	
CODES ENDS
    END START



















