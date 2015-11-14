################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../uart_32m1.c \
../adc.c \
../ata6844.c \
../main.c \
../mc_drv.c \
../mc_sensor.c \
../sw_timer.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
uart_32m1.o \
adc.o \
ata6844.o \
main.o \
mc_drv.o \
mc_sensor.o \
sw_timer.o

OBJS_AS_ARGS +=  \
uart_32m1.o \
adc.o \
ata6844.o \
main.o \
mc_drv.o \
mc_sensor.o \
sw_timer.o

C_DEPS +=  \
uart_32m1.d \
adc.d \
ata6844.d \
main.d \
mc_drv.d \
mc_sensor.d \
sw_timer.d

C_DEPS_AS_ARGS +=  \
uart_32m1.d \
adc.d \
ata6844.d \
main.d \
mc_drv.d \
mc_sensor.d \
sw_timer.d

OUTPUT_FILE_PATH +=mega32m1_bemf_bldc.elf

OUTPUT_FILE_PATH_AS_ARGS +=mega32m1_bemf_bldc.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler















./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DF_CPU=16000000UL  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall  -mmcu=atmega32m1  -c -gdwarf-2 -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.3.3
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="mega32m1_bemf_bldc.map" -Wl,--start-group  -Wl,--end-group -Wl,--gc-sections  -mmcu=atmega32m1   
	@echo Finished building target: $@
	"C:\WinAVR\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "mega32m1_bemf_bldc.elf" "mega32m1_bemf_bldc.hex"
	"C:\WinAVR\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "mega32m1_bemf_bldc.elf" "mega32m1_bemf_bldc.eep" || exit 0
	"C:\WinAVR\bin\avr-objdump.exe" -h -S "mega32m1_bemf_bldc.elf" > "mega32m1_bemf_bldc.lss"
	"C:\WinAVR\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "mega32m1_bemf_bldc.elf" "mega32m1_bemf_bldc.srec"
	"C:\WinAVR\bin\avr-size.exe" "mega32m1_bemf_bldc.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "mega32m1_bemf_bldc.elf" "mega32m1_bemf_bldc.a" "mega32m1_bemf_bldc.hex" "mega32m1_bemf_bldc.lss" "mega32m1_bemf_bldc.eep" "mega32m1_bemf_bldc.map" "mega32m1_bemf_bldc.srec" "mega32m1_bemf_bldc.usersignatures"
	