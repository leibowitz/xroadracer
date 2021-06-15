AC_INIT([JolaRacer], [0.0.2], [gianni@cmsoft.net], [jola])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER(config.h)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE

#Do not forget to delete that for release version
AM_MAINTAINER_MODE

#basic compiler settings
AC_GNU_SOURCE
AC_PROG_CC
AC_HEADER_STDC

#to handle future localisation
#AH_TEMPLATE([GETTEXT_PACKAGE], [Package name for gettext])
#GETTEXT_PACKAGE=jola
#AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE")
#AC_SUBST(GETTEXT_PACKAGE)
#ALL_LINGUAS="fr"
#AM_GLIB_GNU_GETTEXT
#AC_PROG_INTLTOOL

if test "x${prefix}" = "xNONE"; then
AC_DEFINE_UNQUOTED(DATA_DIR, 
	"${ac_default_prefix}/games/${PACKAGE}", 
	[path to pix files and maps files])
else
AC_DEFINE_UNQUOTED(DATA_DIR, 
	"${prefix}/${PACKAGE}/games/${PACKAGE}", 
	[path to pix files and maps files])
fi

#compiler/linker options
AC_CHECK_LIB([m],[round],[AC_DEFINE([HAVE_ROUND],[1],["c99 function"])])
AC_CHECK_LIB([m],[roundf],[AC_DEFINE([HAVE_ROUNDF],[1],["c99 function"])])

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

#look for SDL
SDL_VERSION=1.2.0
dnl Check for SDL
AM_PATH_SDL($SDL_VERSION,:,AC_MSG_ERROR([*** SDL version $SDL_VERSION not found.]))

CFLAGS="$CFLAGS $SDL_CFLAGS -D_GNU_SOURCE"
CPPFLAGS="$CPPFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

AC_CHECK_LIB(SDL_image, IMG_Load,,check_sdl_image="no")
AC_CHECK_LIB([SDL_ttf], [TTF_OpenFont],,check_sdl_ttf="no")
AC_CHECK_LIB(SGE, sge_transform, check_sdl_sge="no")
AC_CHECK_LIB(SDL_gfx, pixelColor, check_sdl_gfx="no")

LIBS="$LIBS -lSDL_image -lSDL_ttf -lSDL_gfx -lSGE -lm"


#INTI_REQUIRED_VERSION=1.0.7
#PKG_CHECK_MODULES(INTI, inti-1.0 >= $INTI_REQUIRED_VERSION)
#AC_SUBST(INTI_CFLAGS)
#AC_SUBST(INTI_LIBS)

#generate files

AC_CONFIG_FILES([
Makefile
src/Makefile
data/Makefile
data/construct/Makefile
data/cars/Makefile
data/fonts/Makefile
data/images/Makefile
data/images/hud/Makefile
data/images/lights/Makefile
data/maps/Makefile
data/maps/custom/Makefile
data/maps/static/Makefile
])
AC_OUTPUT

