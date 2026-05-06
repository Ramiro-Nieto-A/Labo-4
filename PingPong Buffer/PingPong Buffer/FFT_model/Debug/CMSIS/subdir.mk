################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MK64F12.c 

C_DEPS += \
./CMSIS/system_MK64F12.d 

OBJS += \
./CMSIS/system_MK64F12.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCR_INTEGER_PRINTF -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DCPU_MK64FN1M0VDC12 -DCPU_MK64FN1M0VDC12_cm4 -D__NEWLIB__ -I"/home/jacoby/Documents/MCUXpresso_11.10.0_3148/DSP_workspace/DSP3/source" -I"/home/jacoby/Documents/MCUXpresso_11.10.0_3148/DSP_workspace/DSP3" -I"/home/jacoby/Documents/MCUXpresso_11.10.0_3148/DSP_workspace/DSP3/startup" -I"/home/jacoby/Documents/MCUXpresso_11.10.0_3148/DSP_workspace/DSP3/CMSIS" -O3 -fno-common -g3 -gdwarf-4 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wa,-adhlns="$@.lst" -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS

clean-CMSIS:
	-$(RM) ./CMSIS/system_MK64F12.d ./CMSIS/system_MK64F12.o

.PHONY: clean-CMSIS

