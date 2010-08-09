QT += xml
TARGET = qrrepo
TEMPLATE = lib
OBJECTS_DIR = .obj
MOC_DIR = .moc
win32 {
	CONFIG(debug, debug|release):DESTDIR = ../qrgui/debug
	else:CONFIG(release, debug|release):DESTDIR = ../qrgui/release
	else:error(Windows build definitely needs to be fixed)
}
else:DESTDIR = ../qrgui
DEFINES += QRREPO_LIBRARY
HEADERS += private/client.h \
	private/classes/logicObject.h \
	private/qrRepoGlobal.h \
	private/serializer.h \
	svnClient.h \
	include/wc.hpp \
	include/version.hpp \
	include/vector_wrapper.hpp \
	include/utility_wrapper.hpp \
	include/url.hpp \
	include/targets.hpp \
	include/svn_xml.h \
	include/svn_wc.h \
	include/svn_version.h \
	include/svn_utf.h \
	include/svn_user.h \
	include/svn_types.h \
	include/svn_time.h \
	include/svn_subst.h \
	include/svn_string.h \
	include/svn_sorts.h \
	include/svn_repos.h \
	include/svn_ra.h \
	include/svn_ra_svn.h \
	include/svn_quoprint.h \
	include/svn_props.h \
	include/svn_pools.h \
	include/svn_path.h \
	include/svn_opt.h \
	include/svn_nls.h \
	include/svn_mergeinfo.h \
	include/svn_md5.h \
	include/svn_iter.h \
	include/svn_io.h \
	include/svn_hash.h \
	include/svn_fs.h \
	include/svn_error.h \
	include/svn_error_codes.h \
	include/svn_dso.h \
	include/svn_dirent_uri.h \
	include/svn_diff.h \
	include/svn_delta.h \
	include/svn_dav.h \
	include/svn_ctype.h \
	include/svn_config.h \
	include/svn_compat.h \
	include/svn_cmdline.h \
	include/svn_client.h \
	include/svn_checksum.h \
	include/svn_base64.h \
	include/svn_auth.h \
	include/string_wrapper.hpp \
	include/status.hpp \
	include/status_selection.hpp \
	include/revision.hpp \
	include/property.hpp \
	include/pool.hpp \
	include/path.hpp \
	include/mod_dav_svn.h \
	include/mod_authz_svn.h \
	include/map_wrapper.hpp \
	include/log_entry.hpp \
	include/info.hpp \
	include/exception.hpp \
	include/entry.hpp \
	include/dirent.hpp \
	include/datetime.hpp \
	include/context.hpp \
	include/context_listener.hpp \
	include/client.hpp \
	include/apu.h \
	include/apu_want.h \
	include/apu_version.h \
	include/apr.hpp \
	include/apr.h \
	include/apr_xml.h \
	include/apr_xlate.h \
	include/apr_want.h \
	include/apr_version.h \
	include/apr_uuid.h \
	include/apr_user.h \
	include/apr_uri.h \
	include/apr_time.h \
	include/apr_thread_rwlock.h \
	include/apr_thread_proc.h \
	include/apr_thread_pool.h \
	include/apr_thread_mutex.h \
	include/apr_thread_cond.h \
	include/apr_tables.h \
	include/apr_support.h \
	include/apr_strmatch.h \
	include/apr_strings.h \
	include/apr_signal.h \
	include/apr_shm.h \
	include/apr_sha1.h \
	include/apr_sdbm.h \
	include/apr_rmm.h \
	include/apr_ring.h \
	include/apr_reslist.h \
	include/apr_random.h \
	include/apr_queue.h \
	include/apr_proc_mutex.h \
	include/apr_portable.h \
	include/apr_pools.h \
	include/apr_poll.h \
	include/apr_optional.h \
	include/apr_optional_hooks.h \
	include/apr_network_io.h \
	include/apr_mmap.h \
	include/apr_memcache.h \
	include/apr_md5.h \
	include/apr_md4.h \
	include/apr_lib.h \
	include/apr_ldap.h \
	include/apr_ldap_url.h \
	include/apr_ldap_rebind.h \
	include/apr_ldap_option.h \
	include/apr_ldap_init.h \
	include/apr_inherit.h \
	include/apr_hooks.h \
	include/apr_hash.h \
	include/apr_global_mutex.h \
	include/apr_getopt.h \
	include/apr_general.h \
	include/apr_fnmatch.h \
	include/apr_file_io.h \
	include/apr_file_info.h \
	include/apr_errno.h \
	include/apr_env.h \
	include/apr_dso.h \
	include/apr_dbm.h \
	include/apr_dbd.h \
	include/apr_date.h \
	include/apr_buckets.h \
	include/apr_base64.h \
	include/apr_atomic.h \
	include/apr_anylock.h \
	include/apr_allocator.h \
	include/annotate_line.hpp
SOURCES += private/client.cpp \
	private/classes/logicObject.cpp \
	private/serializer.cpp \
	SvnClient.cpp

# API репозитория
HEADERS += repoApi.h
SOURCES += private/repoApi.cpp

# headers for apr, svn and svncpp
INCLUDEPATH += include

# svn and libraries for it
#unix:INCLUDEPATH += /usr/include/apr-1.0 \
#    /usr/include/subversion-1 \
#    /usr/include/svncpp \
#    /usr/include
unix:LIBS += -L. \
	-lsvncpp \
#    -L/usr/lib \
	-lapr-1 \
	-lsvn_client-1

# Код из qrgui/kernel, который тоже неплохо бы куда-нибудь вынести
include(qrguiCode.pri)

# .pri заведён, чтобы структура папок более круто показывалась в креаторе.
# Содержит код, общий для генератора редакторов и основной части.
include (../utils/utils.pri)
