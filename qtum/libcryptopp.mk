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
# ��   ��   ��: libcryptopp.mk
#
# ��   ��   ��: RealEvo-IDE
#
# �ļ���������: 2018 �� 06 �� 27 ��
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
LOCAL_TARGET_NAME := libcryptopp.so

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS := \
./src/cryptopp/cryptlib.cpp  \
./src/cryptopp/cpu.cpp  \
./src/cryptopp/integer.cpp  \
./src/cryptopp/algebra.cpp  \
./src/cryptopp/algparam.cpp  \
./src/cryptopp/asn.cpp  \
./src/cryptopp/basecode.cpp  \
./src/cryptopp/dsa.cpp  \
./src/cryptopp/des.cpp  \
./src/cryptopp/dessp.cpp  \
./src/cryptopp/dll.cpp  \
./src/cryptopp/ec2n.cpp  \
./src/cryptopp/eccrypto.cpp  \
./src/cryptopp/ecp.cpp  \
./src/cryptopp/eprecomp.cpp  \
./src/cryptopp/filters.cpp  \
./src/cryptopp/fips140.cpp  \
./src/cryptopp/gf2n.cpp  \
./src/cryptopp/gfpcrypt.cpp  \
./src/cryptopp/hex.cpp  \
./src/cryptopp/hmac.cpp  \
./src/cryptopp/hrtimer.cpp  \
./src/cryptopp/iterhash.cpp  \
./src/cryptopp/keccak.cpp  \
./src/cryptopp/misc.cpp  \
./src/cryptopp/modes.cpp  \
./src/cryptopp/mqueue.cpp  \
./src/cryptopp/nbtheory.cpp  \
./src/cryptopp/oaep.cpp  \
./src/cryptopp/osrng.cpp  \
./src/cryptopp/polynomi.cpp  \
./src/cryptopp/pubkey.cpp  \
./src/cryptopp/queue.cpp  \
./src/cryptopp/randpool.cpp  \
./src/cryptopp/rdtables.cpp  \
./src/cryptopp/rijndael.cpp  \
./src/cryptopp/rng.cpp  \
./src/cryptopp/sha.cpp  \
./src/cryptopp/strciphr.cpp  \
./src/cryptopp/winpipes.cpp  \
./src/cryptopp/sha3.cpp

#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your header files search path")
#*********************************************************************************************************
LOCAL_INC_PATH := \
-I"./src/config"  \
-I"./include" \
-I"./src/cryptopp" \
-I"./src/cryptopp/include" \
-I"../libboost/src"

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL := -DHAVE_CONFIG_H -DQTUM_BUILD -DHAVE_BUILD_INFO -D__STDC_FORMAT_MACROS \
				   -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -DQTUM_BUILD \
				   -DNDEBUG -DETH_FATDB=1

#*********************************************************************************************************
# Compiler flags
#*********************************************************************************************************
LOCAL_CFLAGS   := #-pipe -O2 
LOCAL_CXXFLAGS := -std=c++11 #-fvisibility=hidden #-pipe -O2 -fPIC -O2 -g2

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

#*********************************************************************************************************
# Depend target
#*********************************************************************************************************
LOCAL_DEPEND_TARGET := 

include $(LIBRARY_MK)

#*********************************************************************************************************
# End
#*********************************************************************************************************
