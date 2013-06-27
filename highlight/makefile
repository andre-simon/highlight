# Installation script for highlight.
# See INSTALL for details.

# Installation directories:

# Destination directory for installation (intended for packagers)
DESTDIR =

# Root directory for final installation
PREFIX = /usr

# Location of the highlight data files:
data_dir = ${PREFIX}/share/highlight/

# Location of the highlight binary:
bin_dir = ${PREFIX}/bin/

# Location of the highlight man page:
man_dir = ${PREFIX}/share/man/man1/

# Location of the highlight documentation:
doc_dir = ${PREFIX}/share/doc/highlight/

# Location of the highlight examples:
examples_dir = ${doc_dir}examples/

# Location of the highlight config files:
conf_dir = /etc/highlight/
#conf_dir = ${PREFIX}/etc/highlight/

# Location of additional gui files
desktop_apps = ${PREFIX}/share/applications/
desktop_pixmaps = ${PREFIX}/share/pixmaps/

# Commands:
INSTALL_DATA=install -m644
INSTALL_PROGRAM=install -m755
MKDIR=mkdir -p -m 755
RMDIR=rm -r -f

all cli:
	${MAKE} -C ./src -f ./makefile HL_DATA_DIR=${data_dir} HL_CONFIG_DIR=${conf_dir}

lib lib-static:
	${MAKE} -C ./src -f ./makefile HL_DATA_DIR=${data_dir} HL_CONFIG_DIR=${conf_dir} lib-static

lib-shared:
	${MAKE} -C ./src -f ./makefile HL_DATA_DIR=${data_dir} HL_CONFIG_DIR=${conf_dir} PIC=1 lib-shared

gui:
	${MAKE} -C ./src -f ./makefile HL_DATA_DIR=\"${data_dir}\" HL_CONFIG_DIR=\"${conf_dir}\" HL_DOC_DIR=\"${doc_dir}\" gui-qt
	@echo
	@echo "You need to run 'make install' AND 'make install-gui' now!"

install:
	@echo "This script will install highlight in the following directories:"
	@echo "Data directory:          ${DESTDIR}${data_dir}"
	@echo "Documentation directory: ${DESTDIR}${doc_dir}"
	@echo "Plugin directory:        ${DESTDIR}${data_dir}/plugins"
	@echo "Examples directory:      ${DESTDIR}${examples_dir}"
	@echo "Manual directory:        ${DESTDIR}${man_dir}"
	@echo "Binary directory:        ${DESTDIR}${bin_dir}"
	@echo "Configuration directory: ${DESTDIR}${conf_dir}"
	@echo

	${MKDIR} ${DESTDIR}${doc_dir}
	${MKDIR} ${DESTDIR}${conf_dir}
	${MKDIR} ${DESTDIR}${examples_dir} \
		${DESTDIR}${examples_dir}web_plugins \
		${DESTDIR}${examples_dir}web_plugins/dokuwiki \
		${DESTDIR}${examples_dir}web_plugins/movabletype \
		${DESTDIR}${examples_dir}web_plugins/wordpress \
		${DESTDIR}${examples_dir}swig
	${MKDIR} ${DESTDIR}${data_dir} \
		${DESTDIR}${data_dir}themes \
		${DESTDIR}${data_dir}langDefs \
		${DESTDIR}${data_dir}plugins
	${MKDIR} ${DESTDIR}${man_dir}
	${MKDIR} ${DESTDIR}${bin_dir}

	${INSTALL_DATA} ./langDefs/*.lang ${DESTDIR}${data_dir}langDefs/
	${INSTALL_DATA} ./*.conf ${DESTDIR}${conf_dir}
	${INSTALL_DATA} ./themes/*.theme ${DESTDIR}${data_dir}themes/
	${INSTALL_DATA} ./plugins/*.lua ${DESTDIR}${data_dir}plugins/
	${INSTALL_DATA} ./man/highlight.1.gz ${DESTDIR}${man_dir}
	${INSTALL_DATA} ./AUTHORS ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./README ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./README_DE ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./README_REGEX ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./README_LANGLIST ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./ChangeLog ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./COPYING ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./INSTALL ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./examples/web_plugins/dokuwiki/* ${DESTDIR}${examples_dir}web_plugins/dokuwiki/
	${INSTALL_DATA} ./examples/web_plugins/movabletype/* ${DESTDIR}${examples_dir}web_plugins/movabletype/
	${INSTALL_DATA} ./examples/web_plugins/wordpress/* ${DESTDIR}${examples_dir}web_plugins/wordpress/
	${INSTALL_DATA} ./examples/swig/*.py ./examples/swig/*.pl ${DESTDIR}${examples_dir}swig
	${INSTALL_DATA} ./examples/swig/*.i ./examples/swig/makefile ${DESTDIR}${examples_dir}swig
	${INSTALL_DATA} ./examples/swig/README_SWIG ${DESTDIR}${doc_dir}
	${INSTALL_DATA} ./examples/highlight_pipe.* ${DESTDIR}${examples_dir}
	${INSTALL_DATA} ./examples/*.py ${DESTDIR}${examples_dir}
	${INSTALL_PROGRAM} ./src/highlight ${DESTDIR}${bin_dir}

	@echo
	@echo "Done."
	@echo "Type highlight --help or man highlight for instructions."
	@echo "Take a look at ${DESTDIR}${examples_dir} for plugins and SWIG language bindings."
	@echo "Execute 'make install-gui' to install the highlight GUI ('make gui')."
	@echo "Do not hesitate to report problems. Unknown bugs are hard to fix."

install-gui:
	@echo "Installing files for the GUI..."
	${MKDIR} ${DESTDIR}${data_dir} \
		${DESTDIR}${data_dir}gui_files \
		${DESTDIR}${data_dir}gui_files/ext \
		${DESTDIR}${data_dir}gui_files/l10n \
		${DESTDIR}${desktop_apps} \
		${DESTDIR}${desktop_pixmaps}

	${INSTALL_DATA} ./gui_files/l10n/* ${DESTDIR}${data_dir}gui_files/l10n/
	${INSTALL_DATA} ./gui_files/ext/* ${DESTDIR}${data_dir}gui_files/ext/
	${INSTALL_DATA} ./highlight.desktop ${DESTDIR}${desktop_apps}
	${INSTALL_DATA} ./src/gui-qt/highlight.xpm ${DESTDIR}${desktop_pixmaps}
	${INSTALL_PROGRAM} ./src/highlight-gui ${DESTDIR}${bin_dir}

uninstall:
	@echo "Removing highlight files from system..."
	${RMDIR} ${DESTDIR}${data_dir}
	${RMDIR} ${DESTDIR}${doc_dir}
	${RMDIR} ${DESTDIR}${conf_dir}
	${RMDIR} ${DESTDIR}${examples_dir}
	rm -rf ${DESTDIR}${man_dir}highlight.1.gz
	rm -rf ${DESTDIR}${bin_dir}highlight
	rm -rf ${DESTDIR}${bin_dir}highlight-gui
	rm -rf ${DESTDIR}${desktop_apps}highlight.desktop
	rm -rf ${DESTDIR}${desktop_pixmaps}highlight.xpm
	@echo "Done."

clean cleanall:
	$(MAKE) -C ./src -f ./makefile clean

apidocs:
	doxygen Doxyfile

help:
	@echo "This makefile offers the following options:"
	@echo
	@echo "(all)            Compile the command line interface."
	@echo "lib-static       Compile only the static library."
	@echo "lib-shared       Compile only the shared library."
	@echo "gui              Compile the Qt (4.x/5.x) GUI."
	@echo "install*         Copy all data files to ${data_dir}."
	@echo "install-gui*     Copy GUI data files to ${data_dir}."
	@echo "clean            Remove object files and binaries."
	@echo "apidocs          Generate HTML API documentation using doxygen."
	@echo "uninstall*       Remove highlight files from system."
	@echo
	@echo "* Command needs root privileges."
	@echo "See src/makefile for compilation and linking options."

# Target needed for redhat 9.0 rpmbuild
install-strip:

.PHONY: clean all install apidocs help uninstall install-strip
