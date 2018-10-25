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
# ��   ��   ��: libbitcoin_common.mk
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
LOCAL_TARGET_NAME := libbitcoin_common.so

#*********************************************************************************************************
# Source list
#*********************************************************************************************************
LOCAL_SRCS := \
./src/base58.cpp  \
./src/chainparams.cpp  \
./src/coins.cpp  \
./src/compressor.cpp  \
./src/core_read.cpp  \
./src/core_write.cpp  \
./src/key.cpp  \
./src/keystore.cpp  \
./src/netaddress.cpp  \
./src/netbase.cpp  \
./src/policy/feerate.cpp  \
./src/protocol.cpp  \
./src/scheduler.cpp  \
./src/script/sign.cpp  \
./src/script/standard.cpp  \
./src/warnings.cpp  \
./src/cpp-ethereum/libdevcore/Base64.cpp  \
./src/cpp-ethereum/libdevcore/Common.cpp  \
./src/cpp-ethereum/libdevcore/CommonData.cpp  \
./src/cpp-ethereum/libdevcore/CommonIO.cpp  \
./src/cpp-ethereum/libdevcore/CommonJS.cpp  \
./src/cpp-ethereum/libdevcore/FileSystem.cpp  \
./src/cpp-ethereum/libdevcore/FixedHash.cpp  \
./src/cpp-ethereum/libdevcore/Guards.cpp  \
./src/cpp-ethereum/libdevcore/Hash.cpp  \
./src/cpp-ethereum/libdevcore/Log.cpp  \
./src/cpp-ethereum/libdevcore/MemoryDB.cpp  \
./src/cpp-ethereum/libdevcore/OverlayDB.cpp  \
./src/cpp-ethereum/libdevcore/RLP.cpp  \
./src/cpp-ethereum/libdevcore/SHA3.cpp  \
./src/cpp-ethereum/libdevcore/TransientDirectory.cpp  \
./src/cpp-ethereum/libdevcore/TrieCommon.cpp  \
./src/cpp-ethereum/libdevcore/Worker.cpp  \
./src/cpp-ethereum/libevm/ExtVMFace.cpp  \
./src/cpp-ethereum/libevm/VM.cpp  \
./src/cpp-ethereum/libevm/VMOpt.cpp  \
./src/cpp-ethereum/libevm/VMCalls.cpp  \
./src/cpp-ethereum/libevm/VMFactory.cpp  \
./src/cpp-ethereum/libevmcore/Instruction.cpp  \
./src/cpp-ethereum/libethereum/Account.cpp  \
./src/cpp-ethereum/libethereum/Defaults.cpp  \
./src/cpp-ethereum/libethereum/GasPricer.cpp  \
./src/cpp-ethereum/libethereum/State.cpp  \
./src/cpp-ethereum/libethcore/ABI.cpp  \
./src/cpp-ethereum/libethcore/ChainOperationParams.cpp  \
./src/cpp-ethereum/libethcore/Common.cpp  \
./src/cpp-ethereum/libethcore/ICAP.cpp  \
./src/cpp-ethereum/libethcore/Precompiled.cpp  \
./src/cpp-ethereum/libdevcore/TrieDB.cpp  \
./src/cpp-ethereum/libdevcrypto/Common.cpp  \
./src/cpp-ethereum/libdevcrypto/CryptoPP.cpp  \
./src/cpp-ethereum/libdevcrypto/AES.cpp  \
./src/cpp-ethereum/libdevcrypto/ECDHE.cpp  \
./src/cpp-ethereum/libethashseal/GenesisInfo.cpp  \
./src/cpp-ethereum/libethereum/ChainParams.cpp  \
./src/cpp-ethereum/libethcore/Transaction.cpp  \
./src/cpp-ethereum/libethereum/Transaction.cpp  \
./src/cpp-ethereum/libethereum/Executive.cpp  \
./src/cpp-ethereum/libethereum/ExtVM.cpp  \
./src/cpp-ethereum/libethereum/Block.cpp  \
./src/cpp-ethereum/libethereum/BlockChain.cpp  \
./src/cpp-ethereum/libethereum/BlockDetails.cpp  \
./src/cpp-ethereum/libethereum/TransactionQueue.cpp  \
./src/cpp-ethereum/libethereum/BlockQueue.cpp  \
./src/cpp-ethereum/libethcore/BlockHeader.cpp  \
./src/cpp-ethereum/libethereum/TransactionReceipt.cpp  \
./src/cpp-ethereum/libethcore/SealEngine.cpp  \
./src/cpp-ethereum/libdevcore/TrieHash.cpp  \
./src/cpp-ethereum/libethereum/GenesisInfo.cpp  \
./src/cpp-ethereum/libethashseal/Ethash.cpp  \
./src/cpp-ethereum/libethashseal/EthashCPUMiner.cpp  \
./src/cpp-ethereum/libethashseal/EthashAux.cpp  \
./src/cpp-ethereum/libethashseal/EthashProofOfWork.cpp  \
./src/cpp-ethereum/libethash/internal.c  \
./src/cpp-ethereum/libethash/io.c  \
./src/cpp-ethereum/libethash/sha3.c  \
./src/cpp-ethereum/libethashseal/genesis/mainNetwork.cpp  \
./src/cpp-ethereum/libethashseal/genesis/eip150Test.cpp  \
./src/cpp-ethereum/libethashseal/genesis/eip158Test.cpp  \
./src/cpp-ethereum/libethashseal/genesis/frontierTest.cpp  \
./src/cpp-ethereum/libethashseal/genesis/homesteadTest.cpp  \
./src/cpp-ethereum/libethashseal/genesis/mainNetworkTest.cpp  \
./src/cpp-ethereum/libethashseal/genesis/metropolisTest.cpp  \
./src/cpp-ethereum/libethashseal/genesis/qtumMainNetwork.cpp  \
./src/cpp-ethereum/libethashseal/genesis/qtumTestNetwork.cpp  \
./src/cpp-ethereum/libethashseal/genesis/ropsten.cpp  \
./src/cpp-ethereum/libethashseal/genesis/transitionnetTest.cpp  \
./src/cpp-ethereum/libethash/mmap_win32.c  \
./src/cpp-ethereum/libethash/io_posix.c

#*********************************************************************************************************
# Header file search path (eg. LOCAL_INC_PATH := -I"Your header files search path")
#*********************************************************************************************************
LOCAL_INC_PATH := \
-I"./src" \
-I"./include" \
-I"./src/leveldb/include" \
-I"./src/leveldb/helpers/memenv" \
-I"./src/cpp-ethereum/utils" \
-I"./src/secp256k1/include" \
-I"./src/univalue/include" \
-I"./src/cpp-ethereum" \
-I"./src/config" \
-I"../libboost/src" \
-I"$(SYLIXOS_BASE_PATH)/openssl/openssl/include"

#*********************************************************************************************************
# Pre-defined macro (eg. -DYOUR_MARCO=1)
#*********************************************************************************************************
LOCAL_DSYMBOL := -DHAVE_CONFIG_H -DQTUM_BUILD -DHAVE_BUILD_INFO -D__STDC_FORMAT_MACROS \
				   -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE \
				   -D_FORTIFY_SOURCE=2 -DQTUM_BUILD -DBOOST_SP_USE_STD_ATOMIC -DBOOST_AC_USE_STD_ATOMIC \
				   -DETH_FATDB=1 -DOS_RNG_AVAILABLE

#*********************************************************************************************************
# Compiler flags
#*********************************************************************************************************
LOCAL_CFLAGS   := -std=c99 #-pipe -O2 
LOCAL_CXXFLAGS := -std=c++11 -lpthread #-fvisibility=hidden #-pipe -O2 -fPIE

#*********************************************************************************************************
# Depend library (eg. LOCAL_DEPEND_LIB := -la LOCAL_DEPEND_LIB_PATH := -L"Your library search path")
#*********************************************************************************************************
LOCAL_DEPEND_LIB      := \
-lleveldb \
-lcryptopp \
-lboost_thread-mt \
-lboost_chrono-mt \
-lboost_random-mt \
-lbitcoin_util \
-lcextern \
-lsecp256k1

LOCAL_DEPEND_LIB_PATH :=  \
-L"./$(Output)" \
-L"../libboost/$(Output)"

#*********************************************************************************************************
# Depend target
#*********************************************************************************************************
LOCAL_DEPEND_TARGET := ../libboost/$(Output)/libboost_thread-mt-sd.so
LOCAL_DEPEND_TARGET += ../libboost/$(Output)/libboost_chrono-mt-s.so
LOCAL_DEPEND_TARGET += ../libboost/$(Output)/libboost_random-mt-s.so
LOCAL_DEPEND_TARGET += ./$(Output)/libbitcoin_util.so
LOCAL_DEPEND_TARGET += ./$(Output)/libsecp256k1.so

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
