/*
               =.            This file is part of the OPIE Project
             .=l.            Copyright (c)  2002 Robert Griebl <sandman@handhelds.org>
           .>+-=
 _;:,     .>    :=|.         This file is free software; you can
.> <`_,   >  .   <=          redistribute it and/or modify it under
:`=1 )Y*s>-.--   :           the terms of the GNU General Public
.="- .-=="i,     .._         License as published by the Free Software
 - .   .-<_>     .<>         Foundation; either version 2 of the License,
     ._= =}       :          or (at your option) any later version.
    .%`+i>       _;_.
    .i_,=:_.      -<s.       This file is distributed in the hope that
     +  .  -:.       =       it will be useful, but WITHOUT ANY WARRANTY;
    : ..    .:,     . . .    without even the implied warranty of
    =_        +     =;=|`    MERCHANTABILITY or FITNESS FOR A
  _.=:.       :    :=>`:     PARTICULAR PURPOSE. See the GNU General
..}^=.=       =       ;      Public License for more details.
++=   -.     .`     .:
 :     =  ...= . :.=-        You should have received a copy of the GNU
 -.   .:....=;==+<;          General Public License along with this file;
  -_. . .   )=.  =           see the file COPYING. If not, write to the
    --        :-=`           Free Software Foundation, Inc.,
                             59 Temple Place - Suite 330,
                             Boston, MA 02111-1307, USA.

*/

#include "loginapplication.h"

/* OPIE */
#include <opie2/odebug.h>

/* QT */

/* STD */
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>

#ifdef USEPAM
extern "C" {
#include <security/pam_appl.h>
}
#else
#include <crypt.h>
#include <shadow.h>
#endif


LoginApplication *lApp;

LoginApplication::LoginApplication ( int &argc, char **argv, pid_t parentpid )
    : QPEApplication ( argc, argv, GuiServer )
{
    lApp = this;
    m_parentpid = parentpid;
}

const char *LoginApplication::s_username = 0;

#ifdef USEPAM

const char *LoginApplication::s_pam_password = 0;

int LoginApplication::pam_helper ( int num_msg, const struct pam_message **msg, struct pam_response **resp, void * )
{
    int replies = 0;
    struct pam_response *reply = 0;
    int size = sizeof( struct pam_response );

    for ( int i = 0; i < num_msg; i++ ) {
        switch ( msg [i]-> msg_style ) {
            case PAM_PROMPT_ECHO_ON: // user name given to PAM already
                return PAM_CONV_ERR;

            case PAM_PROMPT_ECHO_OFF: // wants password
                reply = (struct pam_response *) ::realloc ( reply, size );
                if ( !reply )
                    return PAM_CONV_ERR;
                size += sizeof( struct pam_response );

                reply [replies]. resp_retcode = PAM_SUCCESS;
                reply [replies]. resp = ::strdup ( s_pam_password );
                replies++; // PAM frees resp
                break;

            case PAM_TEXT_INFO:
                break;

            default:
                /* unknown or PAM_ERROR_MSG */
                if ( reply )
                    ::free ( reply );
                return PAM_CONV_ERR;
        }
    }
    if ( reply )
        *resp = reply;
    return PAM_SUCCESS;
}


bool LoginApplication::checkPassword ( const char *user, const char *pass )
{
    static struct pam_conv conv = { &LoginApplication::pam_helper, 0 };

    int pam_error;
    pam_handle_t *pamh = 0;

    pam_error = ::pam_start( "xdm", user, &conv, &pamh );
    if ( pam_error == PAM_SUCCESS ) {
        s_pam_password = pass;
        pam_error = ::pam_authenticate ( pamh, 0 );
        s_pam_password = 0;
    }
    ::pam_end ( pamh, pam_error );
    return ( pam_error == PAM_SUCCESS );
}

#else

bool LoginApplication::checkPassword ( const char *user, const char *pass )
{
    char *encrypted, *correct;
    struct passwd *pw;

    if ( !user || !pass )
        return false;

    pw = ::getpwnam ( user );

    if ( !pw )
        return false;

    if (( ::strcmp ( pw-> pw_passwd, "x" ) == 0 ) || ( ::strcmp ( pw-> pw_passwd, "*" ) == 0 )) {
        struct spwd *sp = ::getspnam ( pw-> pw_name );

        if ( !sp )
            return false;

        correct = sp-> sp_pwdp;
    }
    else
        correct = pw-> pw_passwd;

    if ( correct == 0 || correct[0] == '\0' )
        return true;

    encrypted = ::crypt ( pass, correct );
    return ( ::strcmp ( encrypted, correct ) == 0 );
}

#endif

bool LoginApplication::changeIdentity ( )
{
    const char *DEFAULT_LOGIN_PATH      = "/bin:/usr/bin";
    const char *DEFAULT_ROOT_LOGIN_PATH = "/usr/sbin:/bin:/usr/bin:/sbin";

    if ( !s_username )
        return false;
    struct passwd *pw = ::getpwnam ( s_username );
    if ( !pw )
        return false;

    // we are still root at this point - try to run the pre-session script
    if ( !runRootScript ( "OPIEDIR", "share/opie-login/pre-session", s_username ))
        owarn << "failed to run $OPIEDIR/share/opie-login/pre-session" << oendl;

    bool fail = false;
    fail |= ( ::initgroups ( pw-> pw_name, pw-> pw_gid ));
    ::endgrent ( );
    fail |= ( ::setgid ( pw-> pw_gid ));
    fail |= ( ::setuid ( pw-> pw_uid ));

    fail |= ( ::chdir ( pw-> pw_dir ) && ::chdir ( "/" ));

    fail |= ( ::setenv ( "HOME",    pw-> pw_dir, 1 ));
    fail |= ( ::setenv ( "SHELL",   pw-> pw_shell, 1 ));
    fail |= ( ::setenv ( "USER",    pw-> pw_name, 1 ));
    fail |= ( ::setenv ( "LOGNAME", pw-> pw_name, 1 ));
    fail |= ( ::setenv ( "PATH",    ( pw-> pw_uid ? DEFAULT_LOGIN_PATH : DEFAULT_ROOT_LOGIN_PATH ), 1 ));

    return !fail;
}

bool LoginApplication::login ( )
{
    execUserScript ( "HOME", ".opie-session" );
    execUserScript ( "OPIEDIR", "share/opie-login/opie-session" );
    execUserScript ( "OPIEDIR", "bin/qpe" );

    owarn << "failed to start an Opie session" << oendl;
    return false;
}

void LoginApplication::logout ( )
{
    // we are now root again - try to run the post-session script
    if ( !runRootScript ( "OPIEDIR", "share/opie-login/post-session" ))
        owarn << "failed to run $OPIEDIR/scripts/post-session" << oendl;
}


static char *buildarg ( const char *base, const char *script )
{
    const char *dir = base ? ::getenv ( base ) : "/";
    char *arg = new char [::strlen ( dir ) + ::strlen ( script ) + 2];

    ::strcpy ( arg, dir );
    ::strcat ( arg, "/" );
    ::strcat ( arg, script );

    return arg;
}

bool LoginApplication::runRootScript ( const char *base, const char *script, const char *param )
{
    bool res = false;
    char *arg = buildarg ( base, script );

    struct stat st;
    if (( ::stat ( arg, &st ) == 0 ) && ( st. st_uid == 0 )) {
        pid_t child = ::fork ( );

        if ( child == 0 ) {
            ::execl ( "/bin/sh", "-sh", arg, param, 0 );
            ::_exit ( -1 );
        }
        else if ( child > 0 ) {
            int status = 0;

            while ( ::waitpid ( child, &status, 0 ) < 0 ) { }
            res = ( WIFEXITED( status )) && ( WEXITSTATUS( status ) == 0 );
        }
    }

    delete [] arg;
    return res;
}

void LoginApplication::execUserScript ( const char *base, const char *script )
{
    char *arg = buildarg ( base, script );

    struct stat st;
    if ( ::stat ( arg, &st ) == 0 ) {
        if ( st. st_mode & S_IXUSR )
            ::execl ( "/bin/sh", "-sh", "-c", arg, 0 );
        else
            ::execl ( "/bin/sh", "-sh", arg, 0 );
    }
    delete [] arg;
}

const char *LoginApplication::loginAs ( )
{
    return s_username;
}

void LoginApplication::setLoginAs ( const char *name )
{
    s_username = name;
}

QStringList LoginApplication::allUsers ( )
{
    struct passwd *pwd;
    QStringList sl;

    while (( pwd = ::getpwent ( ))) {
        if (( pwd-> pw_uid == 0 ) || ( pwd-> pw_uid >= 500 && pwd-> pw_uid < 65534 ))
            sl << QString ( pwd-> pw_name );
    }

    ::endpwent ( );

    return sl;
}

void LoginApplication::quitToConsole ( )
{
    QPEApplication::quit ( );
    ::kill ( m_parentpid, SIGTERM );
}
