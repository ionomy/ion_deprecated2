# Copyright (c) 2013-2016 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.cpppensource.org/licenses/mit-license.php.

DIST_SUBDIRS = secp256k1

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(HARDENED_CXXFLAGS) $(ERROR_CXXFLAGS)
AM_CPPFLAGS = $(HARDENED_CPPFLAGS)
EXTRA_LIBRARIES = 

ION_CONFIG_INCLUDES=-I$(builddir)/config
ION_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

ION_INCLUDES += -I$(srcdir)/secp256k1/include

LIBIONQT=qt/libionqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la

if ENABLE_WALLET
LIBION_WALLET=libion_wallet.a
endif

$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

EXTRA_LIBRARIES += \
  $(LIBION_WALLET) 

lib_LTLIBRARIES =

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =
BENCHMARKS =

# TODO: check if it's a problem that this could be build after iond.o
obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	/bin/sh ../share/genbuild.sh obj/build.h
#	@$(top_srcdir)/share/genbuild.sh "(abs_top_builddir)/src/obj/build.h" 
version.cpp: obj/build.h

# wallet: shared between iond and ion-qt, but only linked
# when wallet enabled
libion_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(ION_INCLUDES)
libion_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libion_wallet_a_SOURCES = \
  db.cpp \
  miner.cpp \
  rpcdump.cpp \
  rpcmining.cpp \
  rpcwallet.cpp \
  wallet.cpp \
  walletdb.cpp \
  $(ION_CORE_H)


if BUILD_IOND
  bin_PROGRAMS += iond
endif

.PHONY: FORCE check-symbols check-security
# ion core #
ION_CORE_H = \
activemasternode.h  chainparams.h       core_io.h         ecwrapper.h    limitedmap.h           miner.h      pubkey.h       spork.h         torcontrol.h    utilstrencodings.h  \
addrman.h           chainparamsseeds.h  core_memusage.h   hash.h         main.h                 mruset.h     reverselock.h  stake.h         txdb.h          version.h		\
alert.h             checkpoints.h       crypter.h         indirectmap.h  masternodeconfig.h     netbase.h    rpcclient.h    stealth.h       txdb-leveldb.h  walletdb.h		\
allocators.h        clientversion.h     cuckoocache.h     init.h         masternode.h           net.h        rpcprotocol.h  strlcpy.h       txmempool.h     wallet.h		\
amount.h            coincontrol.h       stashedsend.h        instantx.h     masternodeman.h        prevector.h  rpcserver.h    sync.h          ui_interface.h			\
arith_uint256.h     compat.h            stashedsend-relay.h  key.h          masternode-payments.h  proofs.h     script.h       threadsafety.h  uint256.h				\
base58.h            core.h              db.h              keystore.h     masternode-pos.h       protocol.h   serialize.h    tinyformat.h    util.h				

iond_SOURCES = iond.cpp \
	alert.cpp \
	allocators.cpp \
	version.cpp \
	support/cleanse.cpp \
	checkpoints.cpp \
	netbase.cpp \
	addrman.cpp \
	base58.cpp \
	crypter.cpp \
	key.cpp \
	pubkey.cpp \
	ecwrapper.cpp \
	init.cpp \
	keystore.cpp \
	core.cpp \
	main.cpp \
	net.cpp \
	protocol.cpp \
	rpcclient.cpp \
	rpcprotocol.cpp \
	rpcserver.cpp \
	rpcmisc.cpp \
	rpcnet.cpp \
	rpcblockchain.cpp \
	rpcrawtransaction.cpp \
	script.cpp \
	sync.cpp \
	txdb-leveldb.cpp \
	txmempool.cpp \
	util.cpp \
	hash.cpp \
	noui.cpp \
	stake.cpp \
	chainparams.cpp \
	stealth.cpp \
	activemasternode.cpp \
	stashedsend.cpp \
	stashedsend-relay.cpp \
	instantx.cpp \
	masternodeconfig.cpp \
	masternodeman.cpp \
	masternode.cpp \
	masternode-payments.cpp \
	rpcstashedsend.cpp \
	spork.cpp \
	crypto/hmac_sha256.cpp \
	crypto/hmac_sha512.cpp \
	crypto/ripemd160.cpp \
	crypto/sha1.cpp \
	crypto/sha256.cpp \
	crypto/sha512.cpp \
	proofs.cpp \
	uint256.cpp \
	amount.cpp




iond_CPPFLAGS = $(AM_CPPFLAGS) $(ION_INCLUDES) $(MINIUPNPC_CPPFLAGS) 
iond_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
iond_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

iond_LDADD = \
  $(LIBION_WALLET) \
  $(LIBLEVELDB) \
  $(LIBMEMENV) \
  $(LIBSECP256K1)
# iond_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)
# TODO: CHECK IF THESE LIBS ARE PRESENT AND NECESSARY
iond_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(MINIUPNPC_LIBS) $(CRYPTO_LIBS) 


config/ion-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/ion-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/ion-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/ion-config.h.in

clean-local:
	-$(MAKE) -C secp256k1 clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

.mm.o:
	$(AM_V_CXX) $(OBJCXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) \
	  $(CPPFLAGS) $(AM_CXXFLAGS) $(QT_INCLUDES) $(AM_CXXFLAGS) $(PIE_FLAGS) $(CXXFLAGS) -c -o $@ $<

check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<



if EMBEDDED_LEVELDB
include Makefile.leveldb.include
endif

if ENABLE_QT
include Makefile.qt.include
endif

