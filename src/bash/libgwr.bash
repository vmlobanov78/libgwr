#!/bin/bash

#   ----------------------------------------------------------------------------
#   fn_libgwr_file_has_changed()
#
#   Brief   Detect if a file differs from its backup file ( .bak ).
#
#   $1      filename
#
#   Return  0 if file is unchanged, 1 if file has changed
#   ----------------------------------------------------------------------------
function    fn_libgwr_file_has_changed
{
    local   fn
    local   fb
    local   r
    #   ........................................................................
    fn="$1"
    fb="${fn}.bak"

    if [[ ! -e "${fb}" ]] ; then
        return 1
    fi

    diff --brief ${fn} ${fb} 2>&1 >/dev/null
    r=$?

    return $((r))
}
#   ----------------------------------------------------------------------------
#   fn_libgwr_file_accel_build()
#
#   Brief   If a file is identical to its backup file ( .bak ), modify
#           file's timestamp for accelerating build.
#
#   $1      filename
#
#   Return  0 if file is unchanged, 1 if file has changed
#   ----------------------------------------------------------------------------
function    fn_libgwr_file_accel_build
{
    local   fn
    local   fb
    local   r
    #   ........................................................................
    fn="$1"
    fb="${fn}.bak"

    if [[ ! -e "${fb}" ]] ; then
        return 1
    fi

    fn_libgwr_file_has_changed "${fn}"
    r=$?

    if [ $((r)) -eq 0 ] ; then
        echo "> ${fn} unchanged, modifying timestamps for accelerating libkconfig compilation"
        touch -r "${fb}" "${fn}"
    else
        echo "> ${fn} changed"
        cp  "${fn}" "${fb}"
    fi


    return $((r))
}
