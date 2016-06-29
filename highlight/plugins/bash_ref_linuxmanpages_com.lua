--[[
Sample plugin file for highlight 3.9
]]

Description="Add linuxmanpages.net reference links to HTML, LaTeX, RTF and ODT output of Bash scripts"

-- optional parameter: syntax description
function syntaxUpdate(desc)

  if desc~="Bash" then
     return
  end
  
  function Set (list)
    local set = {}
    for _, l in ipairs(list) do set[l] = true end
      return set
  end

   man1_items = Set {"a2p","a2ps","aafire","ab","abcde","abxtest","ac","access","achfile","aconnect",
"acyclic","addftinfo","addr2line","addresses","addwords","aecho","afile",
"afm2tfm","afmtodit","afppasswd","afslog","aimk","alias","allcm","allec",
"allneeded","alsactl","alsamixer","amidi","amixer","amstex","gawk","anemone",
"anemotaxis","animate","ansi2knr","ant","antinspect","antlr","antspotlight",
"aplay","aplaymidi","apm","apmsleep","apollonian","apple2","apple_cp","apple_mv"
,"apple_rm","appletviewer","appres","apropos","ar","arch","arecord",
"arecordmidi","as","as86","as86_encap","ascii-xfr","aseqnet","ash","asn1parse",
"at","atlantis","atobm","atq","atrm","attr","attraction","atunnel","aumix",
"autoconf","autoexpect","autoheader","autom4te","autopoint","autopvf",
"autoreconf","autorun","autoscan","autoupdate","autovbox","awk","backend",
"balsa","barcode","basename","bash","bashbug","basictopvf","batch","bc","bcc",
"bcomps","bdftopcf","bdftruncate","beep-media-player","beforelight","bg",
"bibtex","biff","bind","binhex","bison","bitmap","blaster","blinkbox","blitspin"
,"blocktube","bmtoa","bonobo-activation-server","bouboule","bouncingcow","boxed"
,"braid","break","brltty","bsetbg","bsetroot","bsh","bsod","bubble3d","bubbles",
"bug-buddy","buildhash","builtin","builtins","bumps","bunzip2","busybox","byacc"
,"bzcat","bzcmp","bzdiff","bzegrep","bzfgrep","bzgrep","bzip2","bzip2recover",
"bzless","bzmore","c2ph","c3270","ca","cabextract","cadaver","cage","cal",
"cancel","cancel-cups","cannacheck","cannakill","cannaserver","cannastat",
"capinfo","CA.pl","captoinfo","card","cat","catdic","catdoc","ccmakedep",
"ccomps","ccurve","cd","cdda2ogg","cdda2wav","cd-discid","c++decl","cdecl",
"cdlabelgen","cdp","cdparanoia","cdrdao","cdrecord","cert2ldap","certfind",
"certtool","certwatch","cervisia","cfgmaker","c++filt","chacl","chage","chattr",
"chcon","chfn","chgrp","chkdupexe","chmod","chmoddic","chown","chroot","chrt",
"chsh","chvt","ci","ciphers","ciptool","circo","circuit","cjpeg","ckpasswd",
"cksum","cleanlinks","clear","cloudlife","clusterdb","cmp","co","col","colcrt",
"colrm","column","combinediff","comm","command","compare","compass","compgen",
"compile_et","complete","composite","compress","config.guess","config.sub",
"conjure","consoletype","cont-de","cont-en","continue","cont-nl","convdate",
"convert","convfont","coral","couriertcpd","coverpg","cp","cpan","cpdic","cpio",
"cplay","cpp","cproto","createdb","createlang","createuser","critical","crl",
"crl2pkcs7","crontab","crystal","cscope","csh","cshost","cspctl","csplit",
"ctags","ctow","cu","cubenetic","cubestorm","cue2toc","cups-config",
"cupstestppd","curl","curl-config","cut","cvs","cvsblame","cvscheck",
"cvsversion","cweb","cxpm","cynosure","cyradm","dangerball","dasher","date",
"db2dvi","db2html","db2pdf","db2ps","db2rtf","dbiprof","dbiproxy","dbmmanage2",
"dbus-cleanup-sockets","dbus-daemon-1","dbus-launch","dbus-monitor","dbus-send",
"dc","dc_client","dc_server","dc_snoop","dctc","dctc_cmd","dctc_gdl123",
"dc_test","dd","ddate","ddd","deallocvt","decayscreen","declare","deco",
"dehtmldiff","deluxe","delwords","demon","dep-clean","df","dga","dgst","dhparam"
,"dialog","dicar","dict","dictfmt","dictfmt_index2suffix","dictfmt_index2word",
"dictunformat","dictzip","diet","diff","diff3","diffpp","diffstat","dig",
"dijkstra","dir","dircolors","dirname","dirs","discrete","dislocate","disown",
"dispatch-conf","display","distort","djpeg","dlg","dlpsh","dmp","dmxtodmx",
"dnsdomainname","dns-helper","docbook2dvi","docbook2html","docbook2man",
"docbook2man-spec.pl","docbook2pdf","docbook2ps","docbook2rtf","docbook2tex",
"docbook2texi","docbook2texi-spec.pl","doexec","domainname","dos2unix","dot",
"dot2gxl","dotlockfile","dotneato-config","dotty","dpbindic","dprofpp",
"dpromdic","dpsexec","dpsinfo","drift","dropdb","droplang","dropuser","dsa",
"dsaparam","dtach","du","dubdv","dumpiso","dumpkeys","dumpreg","dund",
"dvconnect","dvcont","dvdrecord","dvgrab","dvi2fax","dvicopy","dvihp","dvipdf",
"dvipdfm","dvips","dvired","dvitomp","dvitype","e2pall","easytag","ebuild",
"echo","ecpg","ed","edit","editcap","editdiff","editreg","editres","efax","efix"
,"egrep","einitex","eject","elatex","elinks","elks","elksemu","emacs","emerge",
"enable","enc","enc2xs","enchant","encodedv","endgame","engine","enscript","env"
,"envsubst","env-update","envy24control","epic","epicycle","eplain","eps2eps",
"epsffit","epstopdf","eqn","eqn2graph","equery","eruby","eruption","escputil",
"esd","esdcat","esd-config","esdctl","esddsp","esdfilt","esdloop","esdmon",
"esdplay","esdrec","esdsample","espdiff","etags","etcat","etc-update","Eterm",
"etex","ethereal","euler2d","euse","eval","evim","evirtex","ewhich","ex","exec",
"exit","expand","expect","expectk","expiry","export","expr","extcheck",
"extractres","extrusion","exuberant-ctags","factor","fadeplot","false","fastjar"
,"fastrm","fax","fax2ps","fax2tiff","faxq","faxrm","faxrunq","faxspool","fbrun",
"fbsetbg","fc","fc-cache","fc-list","fdp","festival","festival_client",
"fetchmail","fetchmailconf","fg","fgconsole","fgr","fgrep","fig2dev",
"fig2ps2tex","file","filter","filterdiff","find","find2perl","findaffix",
"findsmb","finger","firefox","fix132x43","fixcvsdiff","fixdlsrps","fixfmps",
"fixmacps","fixps","fixpsditps","fixpspps","fixscribeps","fixtpps","fixwfwps",
"fixwpps","fixwwps","flac","flag","flame","flea","flex","flex++","flipdiff",
"flipflop","flipscreen3d","flow","fltk-config","fluid","fluidballs","flurry",
"fluxbox","fluxstyle","flyingtoasters","fmt","fold","font2c","fontexport",
"fontglide","fontimport","fontinst","foomatic-combo-xml","foomatic-compiledb",
"foomatic-configure","foomatic-gswrapper","foomatic-perl-data","foomatic-ppdfile",
"foomatic-ppd-options","foomatic-printjob","foomatic-rip","forest",
"formail","free","frommac","fslsfonts","fstobdf","ftp","ftpcopy","ftpcount",
"ftpcp","ftpls","ftptop","ftpwho","funzip","fuser","fuzzyflakes","g++","g32pbm",
"g3cat","g77","gaim","gaim-remote","galaxy","gamma4scanimage","gawk","gc",
"gcalctool","gcc","gccmakedep","gcj","gcjh","gconf-editor","gconfigger",
"gconftool-2","gcov","g-cpan.pl","gctags","gda-config-tool","gdb","gdbserver",
"gdbtui","gdk-pixbuf-csource","gdk-pixbuf-query-loaders","gdm","gears","gedit",
"gendiff","gendsa","genhostid","genrsa","gentoo","geqn","GET","getafm",
"getconfig","getent","getfacl","getfattr","getlist","getopt","getopts","gettext"
,"gettextize","getzones","gfdl","gfloppy","gflux","gfortran","gftodvi","gftopk",
"gftp","gftype","ghostscript","ghostview","gif2tiff","giftrans","gij","gimp-1.2"
,"gimp","gimp-2.0","gimpprint-config","gimp-remote-1.2","gimp-remote","gimp-remote-2.0",
    "gimptool-1.2","gimptool","gimptool-2.0","gindxbib","gkrellm",
"gkrellm2","gkrellmd","glblur","gleidescope","glforestfire","glib-config","glib-genmarshal",
"glib-gettextize","glib-mkenums","glknots","glmatrix","glookbib",
"glplanet","glslideshow","glsnake","gltext","glxgears","glxinfo","gnc-prices",
"gnome","gnome-about","gnome-bug","gnome-config","gnome-dictionary","gnome-doc",
"gnome-dump-metadata","gnome-gen-mimedb","gnomemeeting","gnome-mkstub","gnome-moz-remote",
"gnome-name-service","gnome-panel","gnome-pty-helper","gnome-search-tool","gnome_segv",
"gnome-session","gnome-session-save","gnome-smproxy","gnome-system-log","gnome-wm",
"gnroff","gnuattach","gnucash","gnuclient","gnudoit",
"gnumeric","gnuplot","gnuserv","gnutls-cli","gnutls-cli-debug","gnutls-serv",
"goad-browser","gob2","gobject-query","goop","gpasswd","gperf","gpg","gpgv",
"gphoto2","gpic","gpilot-install-file","gpm-root","gprof","grav","grefer","grep"
,"grepdiff","grephistory","grepjar","greynetic","grmic","grmiregistry","grn",
"grodvi","groff","groffer","grog","grohtml","grolbp","grolj4","grops","grotty",
"groups","growisofs","gs","gsbj","gsdj","gsdj500","gsftopk","gslj","gslp","gsnd"
,"gsoelim","gs-pcl3","gst-complete-0.6","gst-complete-0.8","gst-compprep-0.6",
"gst-compprep-0.8","gst-feedback-0.6","gst-feedback-0.8","gst-inspect-0.6","gst-inspect-0.8",
"gst-launch-0.6","gst-launch-0.8","gst-launch-ext-0.6","gst-launch-ext-0.8","gst-md5sum-0.6",
"gst-md5sum-0.8","gst-register-0.6","gst-register-0.8"
,"gst-typefind-0.6","gst-typefind-0.8","gst-visualise-0.6","gst-visualise-0.8",
"gst-xmlinspect-0.8","gst-xmllaunch-0.6","gst-xmllaunch-0.8","gtbl","gtf",
"gthumb","gtk-config","gtk-query-immodules-2.0","gtroff","gtv","gunzip","gv",
"gvcolor","gvim","gvimdiff","gvpack","gvpr","gxditview","gxl2dot","gzcat",
"gzexe","gzip","h2ph","h2xs","halftone","halo","hash","hattrib","hboot","hcc",
"hcd","hcitool","hcopy","hcp","hdel","hdir","head","HEAD","helix","help",
"hesinfo","hexbin","hexdump","hexedit","hf77","hformat","hfs","hfssh","hfsutils"
,"hinotes","history","hls","hmkdir","hmount","hopalong","host","hostid",
"hostname","hostx","hpftodit","hpwd","hqx2bin","hrename","hrmdir","htdig",
"htdigest","htdigest2","htdig-pdfparser","htdump","htfuzzy","htload","htmerge",
"htnotify","htpasswd","htpasswd2","htpurge","htsearch","htstat","hublist",
"humount","hvol","hyperball","hypercube","hypertorus","ibm_hosts","ibod",
"iceauth","ico","iconv","icotool","id","id3","id3v2","ident","identify","idesk",
"idl2eth","idlj","idn","iecset","ietf2datebook","ifnames","ifs","igawk",
"ijsgimpprint","ImageMagick","imake","imlib_config","imlib-config","import",
"imsmap","imtest","includemocs","includeres","incm","indent","indexmaker",
"indxbib","inews","info","infocmp","infokey","infotocap","inimf","inimpost",
"iniomega","initdb","initex","initlocation","initlog","innconfval","innfeed",
"innmail","install","install-datebook","install-expenses","install-hinote",
"install-info","install-memo","install-netsync","installsieve","install-todo",
"install-user","interdiff","interference","intro","introu","iostat","ipcalc",
"ipcclean","irb","irssi","isamchk","isamlog","isdnconf","isdnrate","isdnrep",
"ispell","jade","jar","jarsigner","java","javac","java-config","javadoc","javah"
,"javap","javaws","jcf-dump","jdb","jed","jigglypuff","jigsaw","jmacs","jobs",
"joe","join","jpegtran","jpico","jpilot","jpilot-dial","jpilot-dump","jpilot-sync",
"jpilot-upgrade-99","jsattach","jscal","jstar","jstest","juggle","julia",
"jv-convert","jv-scan","jw","jwhois","kahakai","kaleidescope","kbd_mode","kcc",
"kde-build","kermit","kernelversion","keygen","keytool","kibitz","kill",
"killall","kinit","kinput2","kjscmd","klein","klist","kon","kpsepath","kpsestat"
,"kpsetool","kpsewhich","kpsexpand","ksh","ktab","kumppa","kwordtrans","l2ping",
"lambda","lamboot","lamclean","lamd","lame","lament","lamexec","lamgrow",
"laminfo","lamshrink","lamtrace","laser","last","lastb","lastcomm","latex",
"lavalite","lbxproxy","lchfn","lchsh","ld","ld86","ldapadd","ldapcompare",
"ldapdelete","ldapmodify","ldapmodrdn","ldappasswd","ldapsearch","ldapwhoami",
"ldd","lefty","less","lesskey","lesstif","let","lex","lftp","libart-config",
"libmikmod-config","libnetcfg","libxrx","lightning","line","link","links",
"lintopvf","linuxdoc","lisa","lissie","listres","ljlatest","lkbib","lmorph",
"lmtptest","ln","lndir","lneato","loaderinfo","loadkeys","loadshlib","local",
"locale","localedef","locate","lockfile","log2pcap","logger","login","logname",
"logout","logresolve","longrun","look","lookbib","loop","lp","lp5250d","lp-cups"
,"lpoptions","lppasswd","lpq","lpq-cups","lpr","lpr-cups","lprm","lprm-cups",
"lpstat","lpstat-cups","lrelease","ls","lsattr","lsb_release","lsdic","lsdiff",
"ltrace","ltversion","luit","lupdate","lv","lwp-download","lwp-mirror","lwp-request",
"lwp-rget","lynx","mac2unix","macbinary","macsave","macstream",
"macunpack","macutil","madplay","mag","Magick-config","Magick++-config","mail",
"maildiracl","maildirkw","maildirmake","mailq","mailq.postfix","mailq.sendmail",
"make","makedepend","makeg","makeindex","makeinfo","makempx","makepsres",
"make_smbcodepage","makestrs","MakeTeXPK","make_unicodemap","man","man2html",
"manpath","manweb","mattrib","maze","mbadblocks","mbchk","mc","mcat","mcd",
"mcedit","mcookie","mcopy","mcview","md2","md4","md5","md5sum","mdc2","mdel",
"mdeltree","mdir","mdu","megatron","memos","memscroller","mencoder","menger",
"merge","mergecap","mergelib","mesg","metaballs","metaflac","mev","mewcat",
"mewdecode","mewencode","mewls","mf","mformat","mft","mgetty_fax","mgp","mgp2ps"
,"mgpembed","mgpnet","mib2c","mikmod","minfo","minicom","mirrorblob","mismunch",
"mkbindic","mkcfm","mk_cmds","mkdep","mkdic","mkdir","mkdirhier","mkebuild",
"mkfifo","mkfontdir","mkfontscale","mkhtmlindex","mkindex","mkmanifest","mknmz",
"mknod","mkoctfile","mkpasswd","mktemp","mktexlsr","mktexmf","mktexpk",
"mktextfm","mkxauth","mkzftree","mlabel","mmd","mmount","mmove","mmroff","moc",
"moebius","mogrify","moire","moire2","molecule","montage","more","morph3d",
"mountain","mouse-test","mozilla","mp3burn","mp3info","mpage","mpartition",
"mpg123","mpic++","mpicc","mpiCC","mpiexec","mpif77","mpimsg","mpirun","mpitask"
,"mplayer","mpost","mpstat","mpto","mrd","mren","mrtg","mrtg-contrib","mrtg-faq"
,"mrtg-forum","mrtg-ipv6","mrtglib","mrtg-logfile","mrtg-mibhelp","mrtg-nt-guide",
    "mrtg-reference","mrtg-rrd","mrtg-squid","mrtg-unix-guide","mrtg-webserver",
    "msgattrib","msgcat","msgcmp","msgcomm","msgconv","msgen","msgexec",
"msgfilter","msgfmt","msggrep","msginit","msgmerge","msgunfmt","msguniq",
"mshowfat","mt","mtools","mtoolstest","mtx","mtype","multixterm","munch",
"munchlist","mupdatetest","mutt","mv","mvdic","mwm","mysql","mysqlaccess",
"mysqladmin","mysqld","mysqld_multi","mysqld_safe","mysqldump",
"mysql_fix_privilege_tables","mysqlshow","mysql_zap","mzip","namazu","namei",
"nano","nasm","native2ascii","nbp","nbplkup","nbprgstr","nbpunrgstr","nc",
"ncftp","ncftpbatch","ncftpget","ncftpls","ncftpput","ncftpspooler","ncopy",
"ndisasm","neato","nedit","neon-config","neqn","nerverot","netatalk-config",
"netreport","newaliases","newaliases.postfix","newaliases.sendmail","newer",
"newgrp","new-object","newrole","nex","nex","ngettext","nice","nisdomainname",
"nkf","nl","nm","nmap","nmapfe","nmblookup","nntpget","nntptest","nohup",
"noncvslist","noof","nop","normalize","normalize-mp3","noseguy","nprint","nroff"
,"nsend","nseq","nsgmls","nslookup","ntlm_auth","ntpd","ntpdate","ntpdc",
"ntpdsim","ntpq","ntpstat","ntptime","ntptrace","nvi","nvi","nview","nview",
"nwauth","nwbols","nwboprops","nwbpset","nwbpvalues","nwdir","nwfsinfo",
"nwfstime","nwpasswd","nwpjmv","nwpqjob","nwpurge","nwrights","nwsfind",
"nwtrustee","nwuserlist","nwvolinfo","oafd","obex_push","objcopy","objdump",
"oclock","ocsp","octave","octave-bug","octave-config","od","ogg123","oggdec",
"oggenc","ogginfo","ogonkify","omega","omshell","onsgmls","opannotate",
"oparchive","opcontrol","open","openjade","openssl","openssl-passwd","openvt",
"opgprof","op_help","opreport","oprofile","opstack","orbd","osgmlnorm","ospam",
"ospent","osx","pacman","pagsh","pal2rgb","pand","pango-querymodules","pap",
"papstatus","passmass","passwd","paste","patch","patgen","pathchk","pawd","pax",
"pbm2g3","pbm2ppa","pbmtomatrixorbital","pcitweak","pcl3opts","pcregrep",
"pcretest","pdf2dsc","pdf2ps","pdfetex","pdffonts","pdfimages","pdfinfo",
"pdfinitex","pdflatex","pdfopt","pdftex","pdftoppm","pdftops","pdftotext",
"pdfvirtex","pdiff","pdksh","pdl","PDL::API","PDL::BadValues","PDL::Dataflow",
"PDL::Delta","pdldoc","PDL::FAQ","PDL::Impatient","PDL::Indexing","pedal",
"penetrate","penrose","perl","perl5004delta","perl5005delta",
"perl561delta","perl56delta","perl570delta","perl571delta","perl572delta",
"perl573delta","perl581delta","perl582delta","perl583delta","perl584delta",
"perl585delta","perl58delta","perlaix","perlamiga","perlapi","perlapio",
"perlapollo","perlartistic","perlbeos","perlbook","perlboot","perlbot",
"perlbs2000","perlbug","perlcall","perlcc","perlce","perlcheat","perlclib",
"perlcn","perlcompile","perlcygwin","perldata","perldbmfilter","perldebguts",
"perldebtut","perldebug","perldelta","perldgux","perldiag","perldl","perldoc",
"perldos","perldsc","perlebcdic","perlembed","perlepoc","perlfaq1","perlfaq",
"perlfaq2","perlfaq3","perlfaq4","perlfaq5","perlfaq6","perlfaq7","perlfaq8",
"perlfaq9","perlfilter","perlfork","perlform","perlfreebsd","perlfunc","perlgpl"
,"perlguts","perlhack","perlhist","perlhpux","perlhurd","perlintern","perlintro"
,"perliol","perlipc","perlirix","perlivp","perljp","perlko","perllexwarn",
"perllocale","perllol","perlmachten","perlmacos","perlmacosx","perlmint",
"perlmod","perlmodinstall","perlmodlib","perlmodstyle","perlmpeix","perlnetware"
,"perlnewmod","perlnumber","perlobj","perlop","perlopentut","perlos2",
"perlos390","perlos400","perlothrtut","perlpacktut","perlplan9","perlpod",
"perlpodspec","perlport","perlqnx","perlre","perlref","perlreftut","perlrequick"
,"perlreref","perlretut","perlrun","perlsec","perlsolaris","perlstyle","perlsub"
,"perlsyn","perlthrtut","perltie","perltoc","perltodo","perltooc","perltoot",
"perltrap","perltru64","perltw","perlunicode","perluniintro","perlutil",
"perluts","perlvar","perlvmesa","perlvms","perlvos","perlwin32","perlxs",
"perlxstut","perror","petri","pf2afm","pfb2pfa","pfbtopfa","pfbtops","pflogsumm"
,"pftp","pg","pgawk","pg_config","pg_controldata","pg_ctl","pg_dump",
"pg_dumpall","pgrep","pg_resetxlog","pg_restore","pgtclsh","phosphor","php",
"pic","pic2graph","pic2tpic","pico","piconv","piecewise","pi-getram","pi-getrom"
,"pilot","pilot-addresses","pilot-foto","pilot-xfer","pine","pinfo","pinky",
"pipes","pk2bm","pkcs12","pkcs7","pkcs8","pkg-clean","pkg-config","pkg-size",
"pkill","pktogf","pktype","pl2pm","play","playdv","plaympeg","pltotf","pmake",
"pman","pmap","pnm2ppa","pnmflip","pod2html","pod2latex","pod2man","pod2text",
"pod2usage","podchecker","podselect","policytool","polyhedra","polyominoes",
"polytopes","pong","pooltype","pop3test","popd","POST","postalias","postcat",
"postconf","postdrop","postfix","postgres","postkick","postlock","postlog",
"postmap","postmaster","postqueue","postsuper","ppm2tiff","ppmcolors","ppmtomap"
,"pptemplate","pqlist","pqrm","pqstat","pr","pr3287","printafm","printenv",
"printf","privoxy","procmail","profiles","prove","providence","proxymngr",
"prune","ps","ps2ascii","ps2epsi","ps2frag","ps2pdf12","ps2pdf13","ps2pdf",
"ps2pdfwr","ps2pk","ps2ps","psbook","psed","pserver","psfaddtable","psfgettable"
,"psfstriptable","psfxtable","pslatex","psmandup","psmerge","psnup","psorder",
"psql","psresize","psselect","psset","pstack","pstops","pstree","pstruct",
"pswrap","ptx","pulsar","pushd","pvf","pvfamp","pvfcut","pvfecho","pvffft",
"pvffile","pvfmix","pvfreverse","pvfsine","pvfspeed","pvftoau","pvftobasic",
"pvftolin","pvftormd","pvftovoc","pvftowav","pvm","PVM","pvmd","pvmd3",
"pvm_intro","pvm_shmd","pwd","pyro","python","qix","qpkg","qshape","queens",
"quickpkg","quota","radclient","radeapclient","radlast","radtest","radwho",
"radzap","rand","ranlib","ras2tiff","raw2tiff","rbash","rcp","rcs","rcsclean",
"rcsdiff","rcsintro","rcsmerge","rdate","rd-bomb","rdesktop","rdist","rdjpgcom",
"read","readcd","readelf","read-expenses","read-ical","readlink","read-notepad",
"readonly","read-palmpix","readprofile","read-todos","rec","recode","recon",
"recountdiff","red","rediff","refer","rename","replace","repoman","req","reset",
"resize","restorefont","restorepalette","restoretextmode","return","rev","revdep-rebuild",
"revpath","rexec","rfcomm","rgb2ycbcr","ripemd160","ripples","rjoe",
"rlog","rlogin","rm","rman","rmdfile","rmdic","rmdir","rmdtopvf","rmdtovbox",
"rmic","rmid","rmiregistry","rnews","rngtest","rocks","rorschach","rotor",
"rotzoomer","rpcclient","rpdump","rpload","rsa","rsautl","rsh","rstart",
"rstartd","rsvg","rsync","rtf2rtf","rubibtex","rubik","ruby18","ruby",
"rumakeindex","runcon","rundig","runscript","runtest","runuser","runx","rup",
"ruptime","rusers","rvi","rview","rvim","rwall","rwho","rz","s2p","sabcmd",
"safe_mysqld","sa-learn","sane-config","saned","sane-find-scanner","sar","sash",
"savetextmode","sballs","scanadf","scanimage","scanpci","sccmap","scgcheck",
"s_client","scm2scm","scp","screen","script","scrollkeeper-config","scrollkeeper-gen-seriesid",
"scs2ascii","scs2pdf","scs2ps","scsitape","sdiff","sdptool","sed"
,"sendiso","sendmail.postfix","sensors","seq","serialver","servertool","sess_id"
,"session-properties","sessreg","set","setfacl","setfattr","setleds","setmclk",
"setmetamode","setterm","setxkbmap","sff2g3","sftp","sfxload","sg","sgi2tiff",
"sgml2html","sgml2info","sgml2latex","sgml2lyx","sgml2rtf","sgml2txt","sgml2xml"
,"sgmlcheck","sgmldiff","sgmlnorm","sgmlpre","sgmlsasp","sh","sha1","sha",
"sha1sum","shadebobs","shar","shift","shlock","shopt","showfont","showkey",
"showrgb","shred","shrinkfile","sierpinski","sierpinski3d","sieveshell",
"simpleftp","single2bin","sivtest","size","skill","skkinput","slabtop","sleep",
"sliceprint","slidescreen","slip","slist","slocate","slogin","slrn","smbcacls",
"smbclient","smbcontrol","smbcquotas","smbget","smbsh","smbstatus","smbtar",
"smbtree","smime","smproxy","smtp-sink","smtp-source","smtptest","sndfile-convert",
"sndfile-info","sndfile-play","snice","snmpbulkget","snmpbulkwalk",
"snmpcmd","snmpdelta","snmpdf","snmpget","snmpgetnext","snmpinform",
"snmpnetstat","snmpset","snmpstatus","snmptable","snmptest","snmptranslate",
"snmptrap","snmpusm","snmpvacm","snmpwalk","sodipodi","soelim","sonar","sort",
"source","sox","soxexam","soxmix","spam","spamassassin","spamc","spamd","speed",
"speedmine","speexdec","speexenc","spent","sphere","spheremonics","spiral",
"spkac","splain","splint","split","splitdiff","splitword","spotlight","sprof",
"sproingies","sq","squiral","srptool","ssconvert","s_server","ssh","ssh-add",
"ssh-agent","ssh-keygen","ssh-keyscan","ssl-passwd","sslpasswd","sslrand",
"sslswamp","stairs","star","starfish","startfluxbox","startinnfeed","startx",
"starwars","stat","states","statserial","s_time","stonerview","strace","strange"
,"strings","strip","stty","su","sum","superquadrics","suspend","svgakeymap",
"svn","svnadmin","svndumpfilter","svnlook","svnversion","swirl","switch2",
"switchdesk","switchto","sxpm","sync","synclient","syndaemon","system-config-httpd",
"systool","sz","t1mapper","t3d","tac","tack","tail","tailf","talk",
"tangle","tapeinfo","tar","taskset","tbl","tclsh","tcsh","tee","telnet",
"tempfile","test","testlibraw","testparm","testprns","tethereal","tex",
"texconfig","texdoc","texdoctk","texexec","texhash","texi2dvi","texi2dvi4a2ps",
"texi2html","texi2pdf","texindex","texshow","text2pcap","texteroids","textmode",
"texutil","tfmtodit","tftopl","tftp","thornbird","thumbnail","thumbpdf","tic",
"tickadj","tie","tiff2bw","tiff2pdf","tiff2ps","tiff2rgba","tiffcmp","tiffcp",
"tiffdither","tiffdump","tiffgt","tiffinfo","tiffmedian","tiffsplit","tiffsv",
"time","timeout","times","timidity","tkill","tknewsbiff","tload","tn5250",
"tnameserv","toe","tomac","top","totem","touch","tping","tput","tr","transfig",
"trap","tred","tree","triangle","troff","truchet","true","tryaffix","tsclient",
"tset","tsort","ttcp","tty","tvtime","tvtime-command","tvtime-configure","tvtime-scanner",
"twang","twm","twopi","type","typeset","ucs2any","uic","uil","ul",
"ulimit","umask","umb-scheme","unalias","uname","unbin","unbuffer","uncompress",
"unexpand","unflatten","unhex","unicode_start","unicode_stop","uniq","units",
"unix2dos","unlink","unset","unshar","unsingle","unsq","unwrapdiff","unzip",
"unzipsfx","updatedb","update-mime-database","updmap","uptime","urlview",
"userinfo","usermount","userpasswd","users","usleep","uucp","uudecode",
"uuencode","uufilter","uuidgen","uustat","uux","vacation","vacuumdb","vbox",
"vboxbeep","vboxconvert","vboxctrl","vboxmode","vboxplay","vboxtoau","vcut",
"vdir","vdltodmx","vedit","verify","vermiculate","version","vfstest","vftovp",
"vi","vidwhacker","view","viewfax","viewres","vim","vimdiff","vimtutor","vines",
"virmf","virmpost","viromega","virtex","vlock","vncconfig","vncpasswd",
"vncserver","vncviewer","voctopvf","volname","vorbiscomment","vptovf","vtysh",
"w","w3m","w3mman","waimea","wait","wall","Wand-config","wander","watch",
"wavtopvf","wbinfo","wc","weave","webalizer","webcollage","wftopfa","wget",
"whatis","whereis","which","whirlwindwarp","whirlygig","who","whoami","whois",
"wine","wipe","wish","wmclockmon","wmcpuload","wmxmms","wordtrans","wordview",
"worm","wormhole","wrestool","write","wrjpgcom","wtoc","wvdial","wvdialconf",
"x0vncserver","x11perf","x11perfcomp","x3270","x3270if","x3270-script","x509",
"x86info","xanalogtv","xanim","xargs","xaumix","xauth","xbiff","xcalc","xcam",
"xcdroast","xcftopnm","xclipboard","xclock","xcmsdb","xconsole","xcscope",
"xcursorgen","xcutsel","xdelta","xditview","xdm","Xdmx","xdmxconfig","xdpyinfo",
"xdriinfo","xdvi","xdvizilla","xedit","xemacs","xepsf","xev","xeyes","xf86cfg",
"xf86config","xfd","xfig","xfindproxy","xflame","xfontsel","XFree86","xfs",
"xfsinfo","xfwp","xgamma","xgc","xgettext","xhfs","xhost","xine-config","xinit",
"xisdnload","xjack","xkbcomp","xkbevd","xkbprint","xkibitz","xkill","xload",
"xloadimage","xlogo","xls2csv","xlsatoms","xlsclients","xlsfonts","xlyap","xmag"
,"xman","Xmark","xmatrix","xmbind","xmessage","xmh","xmkmf","xml2-config",
"xmlcatalog","xml_grep","xmlif","xmllint","xml_pp","xmlsec1","xmlsec1-config",
"xml_spellcheck","xmlto","xmlwf","xmms","xmodmap","xmonisdn","Xnest","xon",
"Xorg","xorgcfg","xorgconfig","xosview","xpdf","xprop","xrandr","xrayswarm",
"xrdb","xrefresh","xrestop","xsane","xscreensaver","xscreensaver-command",
"xscreensaver-demo","xscreensaver-getimage","xscreensaver-getimage-file",
"xscreensaver-getimage-video","xscreensaver-gl-helper","Xserver","xset","xsetbg"
,"xsetmode","xsetpointer","xsetroot","xsltproc","xsm","xsnow","xspirograph",
"xstdcmap","xsublim","xsubpp","xteevee","xterm","xtrap","xtrapchar","xtrapin",
"xtrapinfo","xtrapout","xtrapproto","xtrapreset","xtrapstats","Xvfb","xvidtune",
"xview","xvinfo","Xvnc","xwd","xwininfo","xwintoppm","xwud","xxd","yacc","yapp",
"yes","ypcat","ypchfn","ypchsh","ypdomainname","ypmatch","yppasswd","ypwhich",
"ytalk","zcat","zcmp","zdiff","zenity","zforce","zgrep","zip","zipgrep",
"zipinfo","zless","zmore","znew","zoom","zplay","zsh","zshall","zshbuiltins",
"zshcompctl","zshcompsys","zshcompwid","zshcontrib","zshexpn","zshmisc",
"zshmodules","zshoptions","zshparam","zshtcpsys","zshzftpsys","zshzle","zsoelim"
   }

man2_items = Set {
 
"accept","access","acct","adjtimex","afs_syscall","alarm","alloc_hugepages",
"arch_prctl","bdflush","bind","break","brk","cacheflush","capget","capset",
"chdir","chmod","chown","chroot","clone","close","connect","creat",
"create_module","DC_CTX_new","DC_PLUG_new","DC_PLUG_read","DC_SERVER_new",
"delete_module","dup2","dup","epoll_create","epoll_ctl","epoll_wait","execve",
"_exit","fchdir","fchmod","fchown","fcntl","fdatasync","fgetxattr","flistxattr",
"flock","fork","free_hugepages","fremovexattr","fsetxattr","fstat","fstatfs",
"fstatvfs","fsync","ftruncate","futex","getcontext","getdents","getdomainname",
"getdtablesize","getegid","geteuid","getgid","getgroups","gethostid",
"gethostname","getitimer","get_kernel_syms","getpagesize","getpeername",
"getpgid","getpgrp","getpid","getpmsg","getppid","getpriority","getresgid",
"getresuid","getrlimit","getrusage","getsid","getsockname","getsockopt",
"get_thread_area","gettid","gettimeofday","getuid","getxattr","gtty","idle",
"inb","inb_p","init_module","inl","inl_p","insb","insl","insw","intro","inw",
"inw_p","io_cancel","ioctl","ioctl_list","io_destroy","io_getevents","ioperm",
"iopl","io_setup","io_submit","ipc","kill","killpg","lchown","lgetxattr","link",
"listen","listxattr","llistxattr","_llseek","llseek","lock","lookup_dcookie",
"lremovexattr","lseek","lsetxattr","lstat","madvise","mbind","mincore","mkdir",
"mknod","mlock.2","mlock","mlockall.2","mlockall","mmap2","mmap","modify_ldt",
"mount","mprotect","mpx","mremap","msgctl","msgget","msgop","msgrcv","msgsnd",
"msync","munlock","munlockall","munmap","NAL_ADDRESS_new","NAL_BUFFER_new",
"NAL_CONNECTION_new","NAL_decode_uint32","NAL_LISTENER_new","NAL_SELECTOR_new",
"nanosleep","_newselect","nfsservctl","nice","obsolete","oldfstat","oldlstat",
"oldolduname","oldstat","olduname","open","outb","outb_p","outl","outl_p",
"outsb","outsl","outsw","outw","outw_p","pause","pciconfig_iobase",
"pciconfig_read","pciconfig_write","personality","pipe","pivot_root","poll",
"posix_fadvise","prctl","pread","prof","pselect","ptrace","putpmsg","pwrite",
"query_module","quotactl","read","readahead","readdir","readlink","readv",
"reboot","recv","recvfrom","recvmsg","remap_file_pages","removexattr","rename",
"rmdir","sbrk","sched_getaffinity","sched_getparam","sched_get_priority_max",
"sched_get_priority_min","sched_getscheduler","sched_rr_get_interval",
"sched_setaffinity","sched_setparam","sched_setscheduler","sched_yield",
"security","select","select_tut","semctl","semget","semop","semtimedop","send",
"sendfile","sendmsg","sendto","setcontext","setdomainname","setegid","seteuid",
"setfsgid","setfsuid","setgid","setgroups","sethostid","sethostname","setitimer"
,"set_mempolicy","setpgid","setpgrp","setpriority","setregid","setresgid",
"setresuid","setreuid","setrlimit","setsid","setsockopt","set_thread_area",
"settimeofday","setuid","setup","setxattr","sgetmask","shmat","shmctl","shmdt",
"shmget","shmop","shutdown","sigaction","sigaltstack","sigblock","siggetmask",
"sigmask","signal","sigpause","sigpending","sigprocmask","sigqueue","sigreturn",
"sigsetmask","sigsuspend","sigtimedwait","sigvec","sigwaitinfo","socket",
"socketcall","socketpair","ssetmask","sstk","stat","statfs","statvfs","stime",
"stty","swapoff","swapon","symlink","sync","syscall","syscalls","_sysctl",
"sysctl","sysfs","sysinfo","syslog","time","times","tkill","truncate","tux",
"umask","umount2","umount","uname","undocumented","unimplemented","unlink",
"uselib","ustat","utime","utimes","vfork","vhangup","vm86","wait","wait3",
"wait4","waitpid","write","writev"
}
   

   function getURL(token, manId)
     url='http://linuxmanpages.net/manpages/fedora21/man'..manId .. '/' .. token .. '.'..manId..'.php'
     
     if (HL_OUTPUT== HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
        return '<a class="hl" target="new" href="' .. url .. '">'.. token .. '</a>'
     elseif (HL_OUTPUT == HL_FORMAT_LATEX) then
	return '\\href{'..url..'}{'..token..'}'
      elseif (HL_OUTPUT == HL_FORMAT_RTF) then
	return '{{\\field{\\*\\fldinst HYPERLINK "'..url..'" }{\\fldrslt\\ul\\ulc0 '..token..'}}}'
      elseif (HL_OUTPUT == HL_FORMAT_ODT) then
	return '<text:a xlink:type="simple" xlink:href="'..url..'">'..token..'</text:a>'
     end
   end

  function Decorate(token, state)

    if state~=HL_KEYWORD and state ~=HL_STANDARD then return end

    if man1_items[token] then
	return getURL(token, 1)
    elseif man2_items[token] then
       return getURL(token, 2)
    end

  end
end

function themeUpdate(desc)
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    Injections[#Injections+1]="a.hl, a.hl:visited {color:inherit;font-weight:inherit;}"
  elseif (HL_OUTPUT==HL_FORMAT_LATEX) then
    Injections[#Injections+1]="\\usepackage[colorlinks=false, pdfborderstyle={/S/U/W 1}]{hyperref}"
  end
end

--The Plugins array assigns code chunks to themes or language definitions.
--The chunks are interpreted after the theme or lang file were parsed,
--so you can refer to elements of these files

Plugins={

  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },

}
