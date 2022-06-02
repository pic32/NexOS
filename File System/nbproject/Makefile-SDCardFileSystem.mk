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
ifeq "$(wildcard nbproject/Makefile-local-SDCardFileSystem.mk)" "nbproject/Makefile-local-SDCardFileSystem.mk"
include nbproject/Makefile-local-SDCardFileSystem.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=SDCardFileSystem
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/File_System.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/File_System.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
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
SOURCEFILES_QUOTED_IF_SPACED=ffunicode.c ffsystem.c ff.c diskio.c "../Generic Libraries/Double Linked List/DoubleLinkedList.c" CriticalSection.c Port.c ContextSwitch.S ../NexOS/Kernel/Kernel.c ../NexOS/Kernel/KernelTasks.c ../NexOS/Kernel/Task.c ../NexOS/Kernel/Memory.c ../NexOS/Semaphore/OS_BinarySemaphore.c ../NexOS/Semaphore/Mutex/Mutex.c HardwareProfile.c SD-SPI.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/ffunicode.o ${OBJECTDIR}/ffsystem.o ${OBJECTDIR}/ff.o ${OBJECTDIR}/diskio.o ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o ${OBJECTDIR}/CriticalSection.o ${OBJECTDIR}/Port.o ${OBJECTDIR}/ContextSwitch.o ${OBJECTDIR}/_ext/918398346/Kernel.o ${OBJECTDIR}/_ext/918398346/KernelTasks.o ${OBJECTDIR}/_ext/918398346/Task.o ${OBJECTDIR}/_ext/918398346/Memory.o ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o ${OBJECTDIR}/_ext/50398139/Mutex.o ${OBJECTDIR}/HardwareProfile.o ${OBJECTDIR}/SD-SPI.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/ffunicode.o.d ${OBJECTDIR}/ffsystem.o.d ${OBJECTDIR}/ff.o.d ${OBJECTDIR}/diskio.o.d ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d ${OBJECTDIR}/CriticalSection.o.d ${OBJECTDIR}/Port.o.d ${OBJECTDIR}/ContextSwitch.o.d ${OBJECTDIR}/_ext/918398346/Kernel.o.d ${OBJECTDIR}/_ext/918398346/KernelTasks.o.d ${OBJECTDIR}/_ext/918398346/Task.o.d ${OBJECTDIR}/_ext/918398346/Memory.o.d ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d ${OBJECTDIR}/_ext/50398139/Mutex.o.d ${OBJECTDIR}/HardwareProfile.o.d ${OBJECTDIR}/SD-SPI.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/ffunicode.o ${OBJECTDIR}/ffsystem.o ${OBJECTDIR}/ff.o ${OBJECTDIR}/diskio.o ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o ${OBJECTDIR}/CriticalSection.o ${OBJECTDIR}/Port.o ${OBJECTDIR}/ContextSwitch.o ${OBJECTDIR}/_ext/918398346/Kernel.o ${OBJECTDIR}/_ext/918398346/KernelTasks.o ${OBJECTDIR}/_ext/918398346/Task.o ${OBJECTDIR}/_ext/918398346/Memory.o ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o ${OBJECTDIR}/_ext/50398139/Mutex.o ${OBJECTDIR}/HardwareProfile.o ${OBJECTDIR}/SD-SPI.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=ffunicode.c ffsystem.c ff.c diskio.c ../Generic Libraries/Double Linked List/DoubleLinkedList.c CriticalSection.c Port.c ContextSwitch.S ../NexOS/Kernel/Kernel.c ../NexOS/Kernel/KernelTasks.c ../NexOS/Kernel/Task.c ../NexOS/Kernel/Memory.c ../NexOS/Semaphore/OS_BinarySemaphore.c ../NexOS/Semaphore/Mutex/Mutex.c HardwareProfile.c SD-SPI.c main.c



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
	${MAKE}  -f nbproject/Makefile-SDCardFileSystem.mk dist/${CND_CONF}/${IMAGE_TYPE}/File_System.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
	@${FIXDEPS} "${OBJECTDIR}/ContextSwitch.o.d" "${OBJECTDIR}/ContextSwitch.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ContextSwitch.o.d"  -o ${OBJECTDIR}/ContextSwitch.o ContextSwitch.S  -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/ContextSwitch.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1 -mdfp=${DFP_DIR}
	
else
${OBJECTDIR}/ContextSwitch.o: ContextSwitch.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ContextSwitch.o.d 
	@${RM} ${OBJECTDIR}/ContextSwitch.o 
	@${RM} ${OBJECTDIR}/ContextSwitch.o.ok ${OBJECTDIR}/ContextSwitch.o.err 
	@${FIXDEPS} "${OBJECTDIR}/ContextSwitch.o.d" "${OBJECTDIR}/ContextSwitch.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ContextSwitch.o.d"  -o ${OBJECTDIR}/ContextSwitch.o ContextSwitch.S  -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/ContextSwitch.o.asm.d",--gdwarf-2 -mdfp=${DFP_DIR}
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/ffunicode.o: ffunicode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/ffunicode.o 
	@${FIXDEPS} "${OBJECTDIR}/ffunicode.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/ffunicode.o.d" -o ${OBJECTDIR}/ffunicode.o ffunicode.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/ffsystem.o: ffsystem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ffsystem.o.d 
	@${RM} ${OBJECTDIR}/ffsystem.o 
	@${FIXDEPS} "${OBJECTDIR}/ffsystem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/ffsystem.o.d" -o ${OBJECTDIR}/ffsystem.o ffsystem.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/ff.o: ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ff.o.d 
	@${RM} ${OBJECTDIR}/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/ff.o.d" -o ${OBJECTDIR}/ff.o ff.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/diskio.o: diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/diskio.o.d 
	@${RM} ${OBJECTDIR}/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/diskio.o.d" -o ${OBJECTDIR}/diskio.o diskio.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o: ../Generic\ Libraries/Double\ Linked\ List/DoubleLinkedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1079146332" 
	@${RM} ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d" -o ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o "../Generic Libraries/Double Linked List/DoubleLinkedList.c"    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/CriticalSection.o: CriticalSection.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CriticalSection.o.d 
	@${RM} ${OBJECTDIR}/CriticalSection.o 
	@${FIXDEPS} "${OBJECTDIR}/CriticalSection.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/CriticalSection.o.d" -o ${OBJECTDIR}/CriticalSection.o CriticalSection.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/Port.o: Port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Port.o.d 
	@${RM} ${OBJECTDIR}/Port.o 
	@${FIXDEPS} "${OBJECTDIR}/Port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/Port.o.d" -o ${OBJECTDIR}/Port.o Port.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Kernel.o: ../NexOS/Kernel/Kernel.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Kernel.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Kernel.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/918398346/Kernel.o.d" -o ${OBJECTDIR}/_ext/918398346/Kernel.o ../NexOS/Kernel/Kernel.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/KernelTasks.o: ../NexOS/Kernel/KernelTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/KernelTasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/KernelTasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/KernelTasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/918398346/KernelTasks.o.d" -o ${OBJECTDIR}/_ext/918398346/KernelTasks.o ../NexOS/Kernel/KernelTasks.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Task.o: ../NexOS/Kernel/Task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Task.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Task.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/918398346/Task.o.d" -o ${OBJECTDIR}/_ext/918398346/Task.o ../NexOS/Kernel/Task.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Memory.o: ../NexOS/Kernel/Memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Memory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/918398346/Memory.o.d" -o ${OBJECTDIR}/_ext/918398346/Memory.o ../NexOS/Kernel/Memory.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o: ../NexOS/Semaphore/OS_BinarySemaphore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1152203115" 
	@${RM} ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d 
	@${RM} ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d" -o ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o ../NexOS/Semaphore/OS_BinarySemaphore.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/50398139/Mutex.o: ../NexOS/Semaphore/Mutex/Mutex.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/50398139" 
	@${RM} ${OBJECTDIR}/_ext/50398139/Mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/50398139/Mutex.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/50398139/Mutex.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/50398139/Mutex.o.d" -o ${OBJECTDIR}/_ext/50398139/Mutex.o ../NexOS/Semaphore/Mutex/Mutex.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/HardwareProfile.o: HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/HardwareProfile.o.d" -o ${OBJECTDIR}/HardwareProfile.o HardwareProfile.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/SD-SPI.o: SD-SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SD-SPI.o.d 
	@${RM} ${OBJECTDIR}/SD-SPI.o 
	@${FIXDEPS} "${OBJECTDIR}/SD-SPI.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/SD-SPI.o.d" -o ${OBJECTDIR}/SD-SPI.o SD-SPI.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
else
${OBJECTDIR}/ffunicode.o: ffunicode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/ffunicode.o 
	@${FIXDEPS} "${OBJECTDIR}/ffunicode.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/ffunicode.o.d" -o ${OBJECTDIR}/ffunicode.o ffunicode.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/ffsystem.o: ffsystem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ffsystem.o.d 
	@${RM} ${OBJECTDIR}/ffsystem.o 
	@${FIXDEPS} "${OBJECTDIR}/ffsystem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/ffsystem.o.d" -o ${OBJECTDIR}/ffsystem.o ffsystem.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/ff.o: ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ff.o.d 
	@${RM} ${OBJECTDIR}/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/ff.o.d" -o ${OBJECTDIR}/ff.o ff.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/diskio.o: diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/diskio.o.d 
	@${RM} ${OBJECTDIR}/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/diskio.o.d" -o ${OBJECTDIR}/diskio.o diskio.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o: ../Generic\ Libraries/Double\ Linked\ List/DoubleLinkedList.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1079146332" 
	@${RM} ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d 
	@${RM} ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o.d" -o ${OBJECTDIR}/_ext/1079146332/DoubleLinkedList.o "../Generic Libraries/Double Linked List/DoubleLinkedList.c"    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/CriticalSection.o: CriticalSection.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CriticalSection.o.d 
	@${RM} ${OBJECTDIR}/CriticalSection.o 
	@${FIXDEPS} "${OBJECTDIR}/CriticalSection.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/CriticalSection.o.d" -o ${OBJECTDIR}/CriticalSection.o CriticalSection.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/Port.o: Port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Port.o.d 
	@${RM} ${OBJECTDIR}/Port.o 
	@${FIXDEPS} "${OBJECTDIR}/Port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/Port.o.d" -o ${OBJECTDIR}/Port.o Port.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Kernel.o: ../NexOS/Kernel/Kernel.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Kernel.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Kernel.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Kernel.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/918398346/Kernel.o.d" -o ${OBJECTDIR}/_ext/918398346/Kernel.o ../NexOS/Kernel/Kernel.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/KernelTasks.o: ../NexOS/Kernel/KernelTasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/KernelTasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/KernelTasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/KernelTasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/918398346/KernelTasks.o.d" -o ${OBJECTDIR}/_ext/918398346/KernelTasks.o ../NexOS/Kernel/KernelTasks.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Task.o: ../NexOS/Kernel/Task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Task.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Task.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/918398346/Task.o.d" -o ${OBJECTDIR}/_ext/918398346/Task.o ../NexOS/Kernel/Task.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/918398346/Memory.o: ../NexOS/Kernel/Memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/918398346" 
	@${RM} ${OBJECTDIR}/_ext/918398346/Memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/918398346/Memory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/918398346/Memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/918398346/Memory.o.d" -o ${OBJECTDIR}/_ext/918398346/Memory.o ../NexOS/Kernel/Memory.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o: ../NexOS/Semaphore/OS_BinarySemaphore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1152203115" 
	@${RM} ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d 
	@${RM} ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o.d" -o ${OBJECTDIR}/_ext/1152203115/OS_BinarySemaphore.o ../NexOS/Semaphore/OS_BinarySemaphore.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/50398139/Mutex.o: ../NexOS/Semaphore/Mutex/Mutex.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/50398139" 
	@${RM} ${OBJECTDIR}/_ext/50398139/Mutex.o.d 
	@${RM} ${OBJECTDIR}/_ext/50398139/Mutex.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/50398139/Mutex.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/_ext/50398139/Mutex.o.d" -o ${OBJECTDIR}/_ext/50398139/Mutex.o ../NexOS/Semaphore/Mutex/Mutex.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/HardwareProfile.o: HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/HardwareProfile.o.d" -o ${OBJECTDIR}/HardwareProfile.o HardwareProfile.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/SD-SPI.o: SD-SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/SD-SPI.o.d 
	@${RM} ${OBJECTDIR}/SD-SPI.o 
	@${FIXDEPS} "${OBJECTDIR}/SD-SPI.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/SD-SPI.o.d" -o ${OBJECTDIR}/SD-SPI.o SD-SPI.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -D_SUPPRESS_PLIB_WARNING -DSD_CARD_FILE_SYSTEM_EXAMPLE_PROJECT -I"../File System" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/File_System.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_REAL_ICE=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/File_System.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_REAL_ICE=1,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp=${DFP_DIR}
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/File_System.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/File_System.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_SDCardFileSystem=$(CND_CONF)  -legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp=${DFP_DIR}
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/File_System.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/SDCardFileSystem
	${RM} -r dist/SDCardFileSystem

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
