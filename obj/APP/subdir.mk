################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/broadcaster.c \
../APP/broadcaster_main.c 

OBJS += \
./APP/broadcaster.o \
./APP/broadcaster_main.o 

C_DEPS += \
./APP/broadcaster.d \
./APP/broadcaster_main.d 


# Each subdirectory must supply rules for building sources it contributes
APP/%.o: ../APP/%.c
	@	@	riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DCLK_OSC32K=0 -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Startup" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\APP\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Profile\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\StdPeriphDriver\inc" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\HAL\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Ld" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\LIB" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

