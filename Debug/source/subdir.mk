################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/PES_Project.c \
../source/mtb.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/PES_Project.d \
./source/mtb.d \
./source/semihost_hardfault.d 

OBJS += \
./source/PES_Project.o \
./source/mtb.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/home/parth/Work/anotherworkspace/PES_Project/board" -I"/home/parth/Work/anotherworkspace/PES_Project/source" -I"/home/parth/Work/anotherworkspace/PES_Project" -I"/home/parth/Work/anotherworkspace/PES_Project/drivers" -I"/home/parth/Work/anotherworkspace/PES_Project/CMSIS" -I"/home/parth/Work/anotherworkspace/PES_Project/utilities" -I"/home/parth/Work/anotherworkspace/PES_Project/startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/PES_Project.d ./source/PES_Project.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

