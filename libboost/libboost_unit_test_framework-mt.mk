#*********************************************************************************************************
#
#                                    �й������Դ��֯
#
#                                   Ƕ��ʽʵʱ����ϵͳ
#
#                                SylixOS(TM)  LW : long wing
#
#                               Copyright All Rights Reserved
#
#--------------�ļ���Ϣ--------------------------------------------------------------------------------
#
# ��   ��   ��: libboost_unit_test_framework.mk
#
# ��   ��   ��: RealEvo-IDE
#
# �ļ���������: 2017 �� 12 �� 08 ��
#
# ��        ��: ���ļ��� RealEvo-IDE ���ɣ��������� Makefile ���ܣ������ֶ��޸�
#*********************************************************************************************************

#*********************************************************************************************************
# Clear setting
#*********************************************************************************************************
include $(CLEAR_VARS_MK)

#*********************************************************************************************************
# Target
#*********************************************************************************************************
LOCAL_TARGET_NAME := libboost_unit_test_framework-mt.so

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS := \
libboost_unit_test_framework-mt/src/compiler_log_formatter.cpp \
libboost_unit_test_framework-mt/src/decorator.cpp \
libboost_unit_test_framework-mt/src/framework.cpp \
libboost_unit_test_framework-mt/src/plain_report_formatter.cpp \
libboost_unit_test_framework-mt/src/progress_monitor.cpp \
libboost_unit_test_framework-mt/src/results_collector.cpp \
libboost_unit_test_framework-mt/src/results_reporter.cpp \
libboost_unit_test_framework-mt/src/test_framework_init_observer.cpp \
libboost_unit_test_framework-mt/src/test_tools.cpp \
libboost_unit_test_framework-mt/src/test_tree.cpp \
libboost_unit_test_framework-mt/src/unit_test_log.cpp \
libboost_unit_test_framework-mt/src/unit_test_main.cpp \
libboost_unit_test_framework-mt/src/unit_test_monitor.cpp \
libboost_unit_test_framework-mt/src/unit_test_parameters.cpp \
libboost_unit_test_framework-mt/src/xml_log_formatter.cpp \
libboost_unit_test_framework-mt/src/xml_report_formatter.cpp

#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your header files search path")
#*********************************************************************************************************
LOCAL_INC_PATH := \
-I"./src"

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL := 

#*********************************************************************************************************
# Compiler flags
#*********************************************************************************************************
LOCAL_CFLAGS   := 
LOCAL_CXXFLAGS := 

#*********************************************************************************************************
# Depend library (eg. LOCAL_DEPEND_LIB := -la LOCAL_DEPEND_LIB_PATH := -L"Your library search path")
#*********************************************************************************************************
LOCAL_DEPEND_LIB      := 
LOCAL_DEPEND_LIB_PATH := 

#*********************************************************************************************************
# C++ config
#*********************************************************************************************************
LOCAL_USE_CXX := yes
LOCAL_USE_CXX_EXCEPT := yes

#*********************************************************************************************************
# Code coverage config
#*********************************************************************************************************
LOCAL_USE_GCOV := no

#*********************************************************************************************************
# OpenMP config
#*********************************************************************************************************
LOCAL_USE_OMP := no

#*********************************************************************************************************
# User link command
#*********************************************************************************************************
LOCAL_PRE_LINK_CMD   := 
LOCAL_POST_LINK_CMD  := 
LOCAL_PRE_STRIP_CMD  := 
LOCAL_POST_STRIP_CMD := 

include $(LIBRARY_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************
