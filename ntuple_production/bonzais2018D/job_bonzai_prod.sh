#!/bin/bash
set -x

mybasename="`basename "$0"`"
myfullpath="`readlink -f "$0"`"
mydir="`dirname "$myfullpath"`"

function set_exit_code(){
# At the end of the script modify the FJR
    exitCode="90001"
    errorType="$1"
    exitMessage="$2"
    if [ -e FrameworkJobReport.xml ]
    then
	cat <<EOF >FrameworkJobReport.xml.tmp
<FrameworkJobReport>
<FrameworkError ExitStatus="$exitCode" Type="$errorType" >
$exitMessage
</FrameworkError>
EOF
	tail -n+2 FrameworkJobReport.xml >> FrameworkJobReport.xml.tmp
	mv FrameworkJobReport.xml.tmp FrameworkJobReport.xml
    else
	cat <<EOF > FrameworkJobReport.xml
<FrameworkJobReport>
<FrameworkError ExitStatus="$exitCode" Type="$errorType" >
$exitMessage
</FrameworkError>
</FrameworkJobReport>
EOF
    fi
}

die(){
    echo "$@" 1>&2
    set_exit_code 1 scriptError "$*"
    exit 1
}


date;
t1=`date +%s`

#echo Arguments:
#echo "$@"
#echo "----------------------------------------------------------------------"
#echo
#
#echo Environment:
#env
#echo "----------------------------------------------------------------------"
#echo
#
#echo 'Working directory content (xdev, max-depth 3):'
#find . -xdev -maxdepth 3
#echo "----------------------------------------------------------------------"
#echo



#produces FramworkJobReport.xml
cmsRun -j FrameworkJobReport.xml -p PSet.py

NJob="$1"
shift

while [ $# -gt 0 ]; do
    echo "$1" | grep -q '^[[:alpha:]_][[:alnum:]_]*=\([^[:space:]]*\|"[^"]*"\)$'
    [ $? = 0 ] && eval "$1"
    shift
done

[ -n $NJob ] || die "Missing job ID"

mess=""
[ -n "$sel" ] || mess="$mess sel parameter missing from scriptArgs."
[ -n "$subsel" ] || mess="$mess subsel parameter missing from scriptArgs."
[ -n "$output_file" ] || mess="$mess output_file parameter missing from scriptArgs."
[ -n "$input_catalog" ] || mess="$mess input_catalog parameter missing from scriptArgs."
[ -n "$files_per_job" ] || mess="$mess files_per_job parameter missing from scriptArgs."
[ -n "$mess" ] && die "$mess"

echo "Job id: $NJob"

[ -f "$input_catalog" ] || die "Input file catalog, $input_catalog, was not found! Was it included in files to stage in listed in crab configuration?"

[ -f ./pruner ] || die "Executable pruner was not found! It must be included in files to stage in. Was it included in files to stage in listed in crab configuration?"

#unset catalog
#tmp="`cat $job_list | grep -v '[[:space:]]*#' | awk '{if($1=='$NJob') {printf "catalog=%s; sel=%s; subsel=%s;", $2, $3, $4; exit 0} }'`"
#eval "$tmp"

#[ -n "$catalog" ] || die "No record found for Job $NJob in job list $job_list file!"


#ds="`echo "$catalog" | awk -F - '{print $2}'`"

skip_files=$(((NJob - 1) * files_per_job))
./pruner -v --selection "$sel" --subselection "$subsel" --catalog "$input_catalog" --skip-files "$skip_files" --max-files="$files_per_job" -o "$output_file" 2>&1
prunerExit=$?
if [ $prunerExit -gt 0 ]
then
    die "Executable pruner failed!"
fi 

date 
t2=`date +%s`
echo "Duration:  $(((t2*10-t1*10+300)/600)) mn"

