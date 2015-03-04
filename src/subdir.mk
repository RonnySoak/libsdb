OBJS += \
./src/libsdb.o \
./src/database.o \
./src/util.o \
./src/libssa_extern_db.o \
./src/sdb_error.o

USER_OBJS += \
./src/util.h \
./src/libsdb.h \
./src/database.h \
./src/sdb_error.h

TO_CLEAN += \
./src/libsdb_example.o