#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/IO_Buffer.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/IO_Buffer.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED="../Generic Libraries/Generic Buffer/GenericBuffer.c" "../Generic Libraries/Double Linked List/DoubleLinkedList.c" ContextSwitch.S CriticalSection.c Interrupt.S InterruptHandler.c Port.c IOBufferPort.c ../NexOS/Kernel/Kernel.c ../NexOS/Kernel/KernelTasks.c ../NexOS/Kernel/Memory.c ../NexOS/Kernel/Task.c ../NexOS/Semaphore/OS_BinarySemaphore.c ../NexOS/Semaphore/Mutex/Mutex.c ../NexOS/IOBuffer/IOBuffer.c main.c HardwareProfile.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/935638537/GenericBuffer.o ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o ${OBJECTDIR}/ContextSwitch.o ${OBJECTDIR}/CriticalSection.o ${OBJECTDIR}/Interrupt.o ${OBJECTDIR}/InterruptHandler.o ${OBJECTDIR}/Port.o ${OBJECTDIR}/IOBufferPort.o ${OBJECTDIR}/_ext/918398346/Kernel.o ${OBJECTDIR}/_ext/918398346/KernelTasks.o ${OBJECTDIR}/_ext/918398346/Memory.o ${OBJECTDIR}/_ext/918398346/Task.o ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o ${OBJECTDIR}/_ext/50398139/Mutex.o ${OBJECTDIR}/_ext/721120033/IOBuffer.o ${OBJECTDIR}/main.o ${OBJECTDIR}/HardwareProfile.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/935638537/GenericBuffer.o.d ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d ${OBJECTDIR}/ContextSwitch.o.d ${OBJECTDIR}/CriticalSection.o.d ${OBJECTDIR}/Interrupt.o.d ${OBJECTDIR}/InterruptHandler.o.d ${OBJECTDIR}/Port.o.d ${OBJECTDIR}/IOBufferPort.o.d ${OBJECTDIR}/_ext/918398346/Kernel.o.d ${OBJECTDIR}/_ext/918398346/KernelTasks.o.d ${OBJECTDIR}/_ext/918398346/Memory.o.d ${OBJECTDIR}/_ext/918398346/Task.o.d ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d ${OBJECTDIR}/_ext/50398139/Mutex.o.d ${OBJECTDIR}/_ext/721120033/IOBuffer.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/HardwareProfile.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/935638537/GenericBuffer.o ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o ${OBJECTDIR}/ContextSwitch.o ${OBJECTDIR}/CriticalSection.o ${OBJECTDIR}/Interrupt.o ${OBJECTDIR}/InterruptHandler.o ${OBJECTDIR}/Port.o ${OBJECTDIR}/IOBufferPort.o ${OBJECTDIR}/_ext/918398346/Kernel.o ${OBJECTDIR}/_ext/918398346/KernelTasks.o ${OBJECTDIR}/_ext/918398346/Memory.o ${OBJECTDIR}/_ext/918398346/Task.o ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o ${OBJECTDIR}/_ext/50398139/Mutex.o ${OBJECTDIR}/_ext/721120033/IOBuffer.o ${OBJECTDIR}/main.o ${OBJECTDIR}/HardwareProfile.o

# Source Files
SOURCEFILES=../Generic Libraries/Generic Buffer/GenericBuffer.c ../Generic Libraries/Double Linked List/DoubleLinkedList.c ContextSwitch.S CriticalSection.c Interrupt.S InterruptHandler.c Port.c IOBufferPort.c ../NexOS/Kernel/Kernel.c ../NexOS/Kernel/KernelTasks.c ../NexOS/Kernel/Memory.c ../NexOS/Kernel/Task.c ../NexOS/Semaphore/OS_BinarySemaphore.c ../NexOS/Semaphore/Mutex/Mutex.c ../NexOS/IOBuffer/IOBuffer.c main.c HardwareProfile.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/IO_Buffer.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/ContextSwitch.o: ContextSwitch.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ContextSwitch.o.d 
	@${RM} ${OBJECTDIR}/ContextSwitch.o 
	@${RM} ${OBJECTDIR}/ContextSwitch.o.ok ${OBJECTDIR}/ContextSwitch.o.err 
	@${FIXDEPS} "${OBJECTDIR}/ContextSwitch.o.d" "${OBJECTDIR}/ContextSwitch.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../IO Buffer" -MMD -MF "${OBJECTDIR}/ContextSwitch.o.d"  -o ${OBJECTDIR}/ContextSwitch.o ContextSwitch.S  -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/ContextSwitch.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1 -mdfp=${DFP_DIR}
	
${OBJECTDIR}/Interrupt.o: Interrupt.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Interrupt.o.d 
	@${RM} ${OBJECTDIR}/Interrupt.o 
	@${RM} ${OBJECTDIR}/Interrupt.o.ok ${OBJECTDIR}/Interrupt.o.err 
	@${FIXDEPS} "${OBJECTDIR}/Interrupt.o.d" "${OBJECTDIR}/Interrupt.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../IO Buffer" -MMD -MF "${OBJECTDIR}/Interrupt.o.d"  -o ${OBJECTDIR}/Interrupt.o Interrupt.S  -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/Interrupt.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1 -mdfp=${DFP_DIR}
	
else
${OBJECTDIR}/ContextSwitch.o: ContextSwitch.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ContextSwitch.o.d 
	@${RM} ${OBJECTDIR}/ContextSwitch.o 
	@${RM} ${OBJECTDIR}/ContextSwitch.o.ok ${OBJECTDIR}/ContextSwitch.o.err 
	@${FIXDEPS} "${OBJECTDIR}/ContextSwitch.o.d" "${OBJECTDIR}/ContextSwitch.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../IO Buffer" -MMD -MF "${OBJECTDIR}/ContextSwitch.o.d"  -o ${OBJECTDIR}/ContextSwitch.o ContextSwitch.S  -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/ContextSwitch.o.asm.d",--gdwarf-2 -mdfp=${DFP_DIR}
	
${OBJECTDIR}/Interrupt.o: Interrupt.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Interrupt.o.d 
	@${RM} ${OBJECTDIR}/Interrupt.o 
	@${RM} ${OBJECTDIR}/Interrupt.o.ok ${OBJECTDIR}/Interrupt.o.err 
	@${FIXDEPS} "${OBJECTDIR}/Interrupt.o.d" "${OBJECTDIR}/Interrupt.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../IO Buffer" -MMD -MF "${OBJECTDIR}/Interrupt.o.d"  -o ${OBJECTDIR}/Interrupt.o Interrupt.S  -DXPRJ_default=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/Interrupt.o.asm.d",--gdwarf-2 -mdfp=${DFP_DIR}
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/935638537/GenericBuffer.o: ../Generic\ Libraries/Generic\ Buffer/GenericBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/935638537" 
	@${RM} ${OBJECTDIR}/_ext/935638537/GenericBuffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/935638537/GenericBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/935638537/GenericBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/935638537/GenericBuffer.o.d" -o ${OBJECTDIR}/_ext/935638537/GenericBuffer.o "../Generic Libraries/Generic Buffer/GenericBuffer.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o: ../Generic\ Libraries/Double\ Linked\ List/DoubleLinkedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1079146332" 
	@${RM} ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d" -o ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o "../Generic Libraries/Double Linked List/DoubleLinkedList.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/CriticalSection.o: CriticalSection.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CriticalSection.o.d 
	@${RM} ${OBJECTDIR}/CriticalSection.o 
	@${FIXDEPS} "${OBJECTDIR}/CriticalSection.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/CriticalSection.o.d" -o ${OBJECTDIR}/CriticalSection.o CriticalSection.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/InterruptHandler.o: InterruptHandler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/InterruptHandler.o.d 
	@${RM} ${OBJECTDIR}/InterruptHandler.o 
	@${FIXDEPS} "${OBJECTDIR}/InterruptHandler.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/InterruptHandler.o.d" -o ${OBJECTDIR}/InterruptHandler.o InterruptHandler.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/Port.o: Port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Port.o.d 
	@${RM} ${OBJECTDIR}/Port.o 
	@${FIXDEPS} "${OBJECTDIR}/Port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/Port.o.d" -o ${OBJECTDIR}/Port.o Port.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/IOBufferPort.o: IOBufferPort.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IOBufferPort.o.d 
	@${RM} ${OBJECTDIR}/IOBufferPort.o 
	@${FIXDEPS} "${OBJECTDIR}/IOBufferPort.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/IOBufferPort.o.d" -o ${OBJECTDIR}/IOBufferPort.o IOBufferPort.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Kernel.o: ../NexOS/Kernel/Kernel.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Kernel.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Kernel.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/918398346/Kernel.o.d" -o ${OBJECTDIR}/_ext/918398346/Kernel.o ../NexOS/Kernel/Kernel.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/KernelTasks.o: ../NexOS/Kernel/KernelTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/KernelTasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/KernelTasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/KernelTasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/918398346/KernelTasks.o.d" -o ${OBJECTDIR}/_ext/918398346/KernelTasks.o ../NexOS/Kernel/KernelTasks.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Memory.o: ../NexOS/Kernel/Memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Memory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/918398346/Memory.o.d" -o ${OBJECTDIR}/_ext/918398346/Memory.o ../NexOS/Kernel/Memory.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Task.o: ../NexOS/Kernel/Task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Task.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Task.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/918398346/Task.o.d" -o ${OBJECTDIR}/_ext/918398346/Task.o ../NexOS/Kernel/Task.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o: ../NexOS/Semaphore/OS_BinarySemaphore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1152203115" 
	@${RM} ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d 
	@${RM} ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d" -o ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o ../NexOS/Semaphore/OS_BinarySemaphore.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/50398139/Mutex.o: ../NexOS/Semaphore/Mutex/Mutex.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/50398139" 
	@${RM} ${OBJECTDIR}/_ext/50398139/Mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/50398139/Mutex.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/50398139/Mutex.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/50398139/Mutex.o.d" -o ${OBJECTDIR}/_ext/50398139/Mutex.o ../NexOS/Semaphore/Mutex/Mutex.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/721120033/IOBuffer.o: ../NexOS/IOBuffer/IOBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/721120033" 
	@${RM} ${OBJECTDIR}/_ext/721120033/IOBuffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/721120033/IOBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/721120033/IOBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/721120033/IOBuffer.o.d" -o ${OBJECTDIR}/_ext/721120033/IOBuffer.o ../NexOS/IOBuffer/IOBuffer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/HardwareProfile.o: HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/HardwareProfile.o.d" -o ${OBJECTDIR}/HardwareProfile.o HardwareProfile.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
else
${OBJECTDIR}/_ext/935638537/GenericBuffer.o: ../Generic\ Libraries/Generic\ Buffer/GenericBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/935638537" 
	@${RM} ${OBJECTDIR}/_ext/935638537/GenericBuffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/935638537/GenericBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/935638537/GenericBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/935638537/GenericBuffer.o.d" -o ${OBJECTDIR}/_ext/935638537/GenericBuffer.o "../Generic Libraries/Generic Buffer/GenericBuffer.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o: ../Generic\ Libraries/Double\ Linked\ List/DoubleLinkedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1079146332" 
	@${RM} ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d" -o ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o "../Generic Libraries/Double Linked List/DoubleLinkedList.c"    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/CriticalSection.o: CriticalSection.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CriticalSection.o.d 
	@${RM} ${OBJECTDIR}/CriticalSection.o 
	@${FIXDEPS} "${OBJECTDIR}/CriticalSection.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/CriticalSection.o.d" -o ${OBJECTDIR}/CriticalSection.o CriticalSection.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/InterruptHandler.o: InterruptHandler.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/InterruptHandler.o.d 
	@${RM} ${OBJECTDIR}/InterruptHandler.o 
	@${FIXDEPS} "${OBJECTDIR}/InterruptHandler.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/InterruptHandler.o.d" -o ${OBJECTDIR}/InterruptHandler.o InterruptHandler.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/Port.o: Port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Port.o.d 
	@${RM} ${OBJECTDIR}/Port.o 
	@${FIXDEPS} "${OBJECTDIR}/Port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/Port.o.d" -o ${OBJECTDIR}/Port.o Port.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/IOBufferPort.o: IOBufferPort.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/IOBufferPort.o.d 
	@${RM} ${OBJECTDIR}/IOBufferPort.o 
	@${FIXDEPS} "${OBJECTDIR}/IOBufferPort.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/IOBufferPort.o.d" -o ${OBJECTDIR}/IOBufferPort.o IOBufferPort.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Kernel.o: ../NexOS/Kernel/Kernel.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Kernel.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Kernel.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/918398346/Kernel.o.d" -o ${OBJECTDIR}/_ext/918398346/Kernel.o ../NexOS/Kernel/Kernel.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/KernelTasks.o: ../NexOS/Kernel/KernelTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/KernelTasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/KernelTasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/KernelTasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/918398346/KernelTasks.o.d" -o ${OBJECTDIR}/_ext/918398346/KernelTasks.o ../NexOS/Kernel/KernelTasks.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Memory.o: ../NexOS/Kernel/Memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Memory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/918398346/Memory.o.d" -o ${OBJECTDIR}/_ext/918398346/Memory.o ../NexOS/Kernel/Memory.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Task.o: ../NexOS/Kernel/Task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Task.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Task.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/918398346/Task.o.d" -o ${OBJECTDIR}/_ext/918398346/Task.o ../NexOS/Kernel/Task.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o: ../NexOS/Semaphore/OS_BinarySemaphore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1152203115" 
	@${RM} ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d 
	@${RM} ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d" -o ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o ../NexOS/Semaphore/OS_BinarySemaphore.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/50398139/Mutex.o: ../NexOS/Semaphore/Mutex/Mutex.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/50398139" 
	@${RM} ${OBJECTDIR}/_ext/50398139/Mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/50398139/Mutex.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/50398139/Mutex.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/50398139/Mutex.o.d" -o ${OBJECTDIR}/_ext/50398139/Mutex.o ../NexOS/Semaphore/Mutex/Mutex.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/721120033/IOBuffer.o: ../NexOS/IOBuffer/IOBuffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/721120033" 
	@${RM} ${OBJECTDIR}/_ext/721120033/IOBuffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/721120033/IOBuffer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/721120033/IOBuffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/_ext/721120033/IOBuffer.o.d" -o ${OBJECTDIR}/_ext/721120033/IOBuffer.o ../NexOS/IOBuffer/IOBuffer.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/HardwareProfile.o: HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -I"../IO Buffer" -Wall -MMD -MF "${OBJECTDIR}/HardwareProfile.o.d" -o ${OBJECTDIR}/HardwareProfile.o HardwareProfile.c    -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/IO_Buffer.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_REAL_ICE=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/IO_Buffer.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp=${DFP_DIR}
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/IO_Buffer.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/IO_Buffer.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_default=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp=${DFP_DIR}
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/IO_Buffer.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
