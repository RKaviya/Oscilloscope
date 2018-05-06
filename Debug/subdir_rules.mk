################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/kaviya/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -DTARGET_IS_BLIZZARD_RB1 -I"/home/kaviya/workspace_v7/Copy of Copy of LCD_Display" -I"/home/kaviya/ti/TivaWare_C_Series-2.1.4.178" -I"../ti/TivaWare_C_Series-2.1.4.178/examples/boards/ek-tm4c123gxl-boostxl-kentec-s1/drivers" -I"../ti/TivaWare_C_Series-2.1.4.178/grlib" -I"/home/kaviya/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


