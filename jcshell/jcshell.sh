#!/bin/sh
# ==========================================================================
#     jcshell.sh
#     This file launches the NXP internal version of the JCShell for command line usage

# ==========================================================================

# --------------------------------------------------------------------------
# Check for java
# --------------------------------------------------------------------------
if [ "$JAVA_HOME" != "" ]; then
    if [ -x "$JAVA_HOME/bin/java" ] ; then
        JAVA="$JAVA_HOME/bin/java"
    else
        echo "ERROR : JAVA_HOME not properly defined. Could not find java."
        exit
    fi
else
    JAVA="$(which java)"
    if [ "$JAVA" = "" ]; then
        echo "ERROR : Could not find java in PATH. Please define JAVA_HOME."
        exit
    fi
fi

# --------------------------------------------------------------------------
# Extract jcshell absolute path no matter from where it's called
# --------------------------------------------------------------------------
OSNAME=`uname`
#JAVA32=""
if [ $OSNAME = "Darwin" ]; then # MacOS 
    JCSHELL_PATH=$(cd "$(dirname "$0")"; pwd)
    #JAVA32="-d32"
    #if [ "$isOSX" != "" -a "$HOSTTYPE" = "x86_64" -a "$check64" != "" ]; then
    #    JAVA32='-d32'
    #fi
else
	if [ $OSNAME = "Linux" ]; then # Linux
	    JCSHELL_PATH=$(cd "$(dirname "$0")"; pwd)
	else
        JCSHELL_PATH="$(dirname "$(readlink -f "$0")")"
    #JAVA32=""
	fi
fi
# --------------------------------------------------------------------------
# Include the JAR files from lib folder in CLASSPATH for JVM
# --------------------------------------------------------------------------
CLASSPATH_JCSHELL="$JCSHELL_PATH/lib/jcshell.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/JNASCRWrapper.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/JNAMP65Wrapper.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/JNAMPReaderWrapper.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/jna.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/jline.jar"
# replace IBM crypto jar with nxp-cli-lib.jar
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/nxp-cli-lib.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/nxp-common.jar"         
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/bcprov-jdk15on.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/bcpkix-jdk15on.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/nxp-crypto-lib.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/bcprov-ext-jdk15on.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/groovy-all.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/nxp-jrcp-lib.jar"
CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$JCSHELL_PATH/lib/commons-io.jar"

if [ "$NUT_HOME" != "" ]; then
	CLASSPATH_JCSHELL="$CLASSPATH_JCSHELL:$NUT_HOME/nut.jar"
fi

export PATH=$PATH:$JCSHELL_PATH

chmod 777 $JCSHELL_PATH/tcpserver/*

# --------------------------------------------------------------------------
# Launch the JVM with jcshell as main application and pass in the command
# line from the call to this script file.
# --------------------------------------------------------------------------
echo
echo Welcome to NXP INTERNAL JCShell!
echo "(c) 2016 NXP Semiconductors Netherlands BV"
echo ------------------------------------------------------------------------------
echo "FOR NXP INTERNAL USE ONLY! "
echo ------------------------------------------------------------------------------
echo
cd "$JCSHELL_PATH"

#"$JAVA" $JAVA32 -cp "$CLASSPATH_JCSHELL" com.nxp.id.jc.tools.JCShell $@
"$JAVA" -cp "$CLASSPATH_JCSHELL" com.nxp.id.jc.tools.JCShell "$@"
