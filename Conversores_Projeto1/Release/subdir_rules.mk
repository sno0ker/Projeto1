################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.3.LTS/bin/cl2000" -v28 -ml -mt -O2 --include_path="C:/Users/nunin/workspace_v10/Conversores_Projeto1" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.3.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.3.LTS/bin/cl2000" -v28 -ml -mt -O2 --include_path="C:/Users/nunin/workspace_v10/Conversores_Projeto1" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-c2000_20.2.3.LTS/include" --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


