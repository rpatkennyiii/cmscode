#! /bin/bash

echo "Executable      = $1" > condorJob.condor
echo 'Universe        = vanilla
Error           = logs/err.$(Process)
Output          = logs/out.$(Process)
Log             = logs/log.$(Process)
Rank            = kflops
Requirements    = Arch == "X86_64"
Getenv          = True
+AccountingGroup = "group_cmshi.mojoe137"
' >> condorJob.condor

shift
i=0

while true; do
	if [ $2 ]; then
		echo "Arguments = $1 ${@: -1}$i.root" >> condorJob.condor
		echo -e "Queue\n" >> condorJob.condor
		shift
	else
		break 
	fi
	(( i++ ))
done
