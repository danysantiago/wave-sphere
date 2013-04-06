################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
pff2a/src/diskio.obj: ../pff2a/src/diskio.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"/opt/ti/ccsv5/tools/compiler/msp430_4.1.2/bin/cl430" -vmsp --abi=eabi -g --include_path="/opt/ti/ccsv5/ccs_base/msp430/include" --include_path="/opt/ti/ccsv5/tools/compiler/msp430_4.1.2/include" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="pff2a/src/diskio.pp" --obj_directory="pff2a/src" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

pff2a/src/pff.obj: ../pff2a/src/pff.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"/opt/ti/ccsv5/tools/compiler/msp430_4.1.2/bin/cl430" -vmsp --abi=eabi -g --include_path="/opt/ti/ccsv5/ccs_base/msp430/include" --include_path="/opt/ti/ccsv5/tools/compiler/msp430_4.1.2/include" --advice:power=all --define=__MSP430G2553__ --diag_warning=225 --display_error_number --diag_wrap=off --printf_support=minimal --preproc_with_compile --preproc_dependency="pff2a/src/pff.pp" --obj_directory="pff2a/src" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


