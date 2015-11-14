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
SUBDIRS :=  \
../uart


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../ascii.c \
../main.c \
../mc_control.c \
../mc_drv.c \
../mc_interface.c \
../ushell_task.c \
../uart/uart_lib.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
ascii.o \
main.o \
mc_control.o \
mc_drv.o \
mc_interface.o \
ushell_task.o \
uart/uart_lib.o

OBJS_AS_ARGS +=  \
ascii.o \
main.o \
mc_control.o \
mc_drv.o \
mc_interface.o \
ushell_task.o \
uart/uart_lib.o

C_DEPS +=  \
ascii.d \
main.d \
mc_control.d \
mc_drv.d \
mc_interface.d \
ushell_task.d \
uart/uart_lib.d

C_DEPS_AS_ARGS +=  \
ascii.d \
main.d \
mc_control.d \
mc_drv.d \
mc_interface.d \
ushell_task.d \
uart/uart_lib.d

OUTPUT_FILE_PATH +=bldc_sensorless.elf

OUTPUT_FILE_PATH_AS_ARGS +=bldc_sensorless.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler















./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -I"..\..\..\Drivers\adc" -I"..\..\..\Drivers\comparator" -I"..\..\..\Drivers\pll" -I"..\conf" -I"..\." -I"..\uart" -I"..\..\..\Drivers" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall  -mmcu=atmega32m1  -c -gdwarf-2 -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

uart/%.o: ../uart/%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -I"..\..\..\Drivers\adc" -I"..\..\..\Drivers\comparator" -I"..\..\..\Drivers\pll" -I"..\conf" -I"..\." -I"..\uart" -I"..\..\..\Drivers" -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall  -mmcu=atmega32m1  -c -gdwarf-2 -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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
	$(QUOTE)C:\WinAVR\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="bldc_sensorless.map" -Wl,--start-group  -Wl,--end-group -Wl,--gc-sections  -mmcu=atmega32m1   
	@echo Finished building target: $@
	"C:\WinAVR\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "bldc_sensorless.elf" "bldc_sensorless.hex"
	"C:\WinAVR\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "bldc_sensorless.elf" "bldc_sensorless.eep" || exit 0
	"C:\WinAVR\bin\avr-objdump.exe" -h -S "bldc_sensorless.elf" > "bldc_sensorless.lss"
	"C:\WinAVR\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "bldc_sensorless.elf" "bldc_sensorless.srec"
	"C:\WinAVR\bin\avr-size.exe" "bldc_sensorless.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "bldc_sensorless.elf" "bldc_sensorless.a" "bldc_sensorless.hex" "bldc_sensorless.lss" "bldc_sensorless.eep" "bldc_sensorless.map" "bldc_sensorless.srec" "bldc_sensorless.usersignatures"
	