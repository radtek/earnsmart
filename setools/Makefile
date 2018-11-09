#
#  There exist several targets which are by default empty and which can be 
#  used for execution of your targets. These targets are usually executed 
#  before and after some main targets. They are: 
#
#     .build-pre:              called before 'build' target
#     .build-post:             called after 'build' target
#     .clean-pre:              called before 'clean' target
#     .clean-post:             called after 'clean' target
#     .clobber-pre:            called before 'clobber' target
#     .clobber-post:           called after 'clobber' target
#     .all-pre:                called before 'all' target
#     .all-post:               called after 'all' target
#     .help-pre:               called before 'help' target
#     .help-post:              called after 'help' target
#
#  Targets beginning with '.' are not intended to be called on their own.
#
#  Main targets can be executed directly, and they are:
#  
#     build                    build a specific configuration
#     clean                    remove built files from a configuration
#     clobber                  remove all built files
#     all                      build all configurations
#     help                     print help mesage
#  
#  Targets .build-impl, .clean-impl, .clobber-impl, .all-impl, and
#  .help-impl are implemented in nbproject/makefile-impl.mk.
#
#  Available make variables:
#
#     CND_BASEDIR                base directory for relative paths
#     CND_DISTDIR                default top distribution directory (build artifacts)
#     CND_BUILDDIR               default top build directory (object files, ...)
#     CONF                       name of current configuration
#     CND_PLATFORM_${CONF}       platform name (current configuration)
#     CND_ARTIFACT_DIR_${CONF}   directory of build artifact (current configuration)
#     CND_ARTIFACT_NAME_${CONF}  name of build artifact (current configuration)
#     CND_ARTIFACT_PATH_${CONF}  path to build artifact (current configuration)
#     CND_PACKAGE_DIR_${CONF}    directory of package (current configuration)
#     CND_PACKAGE_NAME_${CONF}   name of package (current configuration)
#     CND_PACKAGE_PATH_${CONF}   path to package (current configuration)
#
# NOCDDL


# Environment 
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
CND_LIBDIR=${CND_BASEDIR}/../../lib
CND_DLIB_EXT = so

# build
build: .build-post

.build-pre:
# Add your pre 'build' code here...

.build-post: .build-impl
# Add your post 'build' code here...
	mkdir -p "${CND_LIBDIR}"
	cp ${CND_BASEDIR}/../external/mariadbpp/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libmariadbpp.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../seglobal/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libseglobal.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../sedata/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsedata.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../semysql/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsemysql.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../semath/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsemath.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../seadmin/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libseadmin.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../seann/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libseann.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../secubist/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsecubist.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../seeconomy/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libseeconomy.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../seeoddata/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libseeoddata.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../senet/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsenet.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../setraining/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsetraining.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../sepredict/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsepredict.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../sesecurities/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsesecurities.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../sexbrl/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsexbrl.${CND_DLIB_EXT} ${CND_LIBDIR}
	cp ${CND_BASEDIR}/../semain/${CND_DISTDIR}/${CONF}/${CND_PLATFORM_${CONF}}/libsemain.${CND_DLIB_EXT} ${CND_LIBDIR}

# clean
clean: .clean-post

.clean-pre:
# Add your pre 'clean' code here...

.clean-post: .clean-impl
# Add your post 'clean' code here...


# clobber
clobber: .clobber-post

.clobber-pre:
# Add your pre 'clobber' code here...

.clobber-post: .clobber-impl
# Add your post 'clobber' code here...


# all
all: .all-post

.all-pre:
# Add your pre 'all' code here...

.all-post: .all-impl
# Add your post 'all' code here...


# build tests
build-tests: .build-tests-post

.build-tests-pre:
# Add your pre 'build-tests' code here...

.build-tests-post: .build-tests-impl
# Add your post 'build-tests' code here...


# run tests
test: .test-post

.test-pre: build-tests
# Add your pre 'test' code here...

.test-post: .test-impl
# Add your post 'test' code here...


# help
help: .help-post

.help-pre:
# Add your pre 'help' code here...

.help-post: .help-impl
# Add your post 'help' code here...



# include project implementation makefile
include nbproject/Makefile-impl.mk

# include project make variables
include nbproject/Makefile-variables.mk
