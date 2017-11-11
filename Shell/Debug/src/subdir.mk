################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/command_parser.c \
../src/commands.c \
../src/environment.c \
../src/file_processing.c \
../src/main.c \
../src/variables.c 

OBJS += \
./src/command_parser.o \
./src/commands.o \
./src/environment.o \
./src/file_processing.o \
./src/main.o \
./src/variables.o 

C_DEPS += \
./src/command_parser.d \
./src/commands.d \
./src/environment.d \
./src/file_processing.d \
./src/main.d \
./src/variables.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


