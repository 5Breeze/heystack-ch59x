################################################################################
# MRS Version: 1.9.2
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../HAL/MCU.c \
../HAL/RTC.c \
../HAL/SLEEP.c 

OBJS += \
./HAL/MCU.o \
./HAL/RTC.o \
./HAL/SLEEP.o 

C_DEPS += \
./HAL/MCU.d \
./HAL/RTC.d \
./HAL/SLEEP.d 


# Each subdirectory must supply rules for building sources it contributes
HAL/MCU.o: C:/Users/bitshen/Desktop/git/heystack-ch59x/HAL/MCU.c
	@	@	riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DCLK_OSC32K=0 -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Startup" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\APP\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Profile\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\StdPeriphDriver\inc" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\HAL\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Ld" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\LIB" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/RTC.o: C:/Users/bitshen/Desktop/git/heystack-ch59x/HAL/RTC.c
	@	@	riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DCLK_OSC32K=0 -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Startup" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\APP\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Profile\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\StdPeriphDriver\inc" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\HAL\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Ld" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\LIB" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
HAL/SLEEP.o: C:/Users/bitshen/Desktop/git/heystack-ch59x/HAL/SLEEP.c
	@	@	riscv-none-elf-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DCLK_OSC32K=0 -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Startup" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\APP\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Profile\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\StdPeriphDriver\inc" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\HAL\include" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\Ld" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\LIB" -I"C:\Users\bitshen\Desktop\git\heystack-ch59x\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

