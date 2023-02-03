#!/bin/sh
if [ "$1" != "configure" ] && [ "$1" != "clean" ] && [ "$1" != "make" ] && [ "$1" != "install" ] && [ "$1" != "all" ]; then
        echo "$0 [ configure | clean | make | install | all ]"
        exit 1
fi
. ../../define/FUNCTION_SCRIPT
. ../../define/PATH
. ../../tools/script_tools.sh

PATH=${CROSS_BIN}:$PATH
tag_time_value=0

function revert_file {
	if [ -f ${APPLICATIONS_DIR}/apform.h ]; then
		DRAW_BOX 80 "revert_file" 33
		cp -r  ${APPLICATIONS_DIR}/apform.h ${BOA_DIR}/src/
		rm -f  ${APPLICATIONS_DIR}/apform.h
	fi

	if [ -f ${APPLICATIONS_DIR}/fmoem_set.c ]; then
		cp -r  ${APPLICATIONS_DIR}/fmoem_set.c ${BOA_DIR}/src/
		rm -f  ${APPLICATIONS_DIR}/fmoem_set.c
	fi
}
function tag_time_value_2_cgi {
	DRAW_BOX 80 "tag_time_value_2_cgi" 33
	#echo "tag_time_value_2_cgi....."

	rm -f  ${APPLICATIONS_DIR}/apform.h
	cp -r ${BOA_DIR}/src/apform.h ${APPLICATIONS_DIR}/

	rm -f  ${APPLICATIONS_DIR}/fmoem_set.c
	cp -r ${BOA_DIR}/src/fmoem_set.c ${APPLICATIONS_DIR}/

	

	pwd=`pwd`
	cd $BOA_DIR/$WEB_DIR

	linkpool=(`ls | grep -v [.]asp | grep -v [.]htm | grep -v file | grep -v graphics | grep -v tmp`)
	echo `pwd`
	if [ -d file ]; then
		linkpool=(${linkpool[@]} `ls file | grep -v [.]asp | grep -v [.]htm | grep -v [.]html`)
	fi
	if [ -d graphics ]; then
		linkpool=(${linkpool[@]} `ls graphics | grep -v [.]asp | grep -v [.]htm | grep -v [.]html`)
	fi
	if [ -d tmp ]; then
		linkpool=(${linkpool[@]} `ls tmp | grep -v [.]asp | grep -v [.]htm | grep -v [.]html`)
	fi
	#echo "====================="
	#echo ${linkpool[@]}
	#echo "====================="

	cd $BOA_DIR/src

	#check path
	dirkpool=(${BOA_DIR}/src/apform.h ${BOA_DIR}/src/fmoem_set.c)

	#start modify all link URL
	for dirloca in ${dirkpool[@]}; do
		if [ -f $dirloca ]; then
			echo $dirloca
			for linkname in ${linkpool[@]}; do
				sed -i 's/'$linkname'?time='$timevalue'/'$linkname'/g' $dirloca
				sed -i 's/'$linkname'/'$linkname'?time='$timevalue'/g' $dirloca
			done
		fi
	done

	#cp -r ${BOA_DIR}/src/apform.h ${APPLICATIONS_DIR}/apform.h.time

	cd $pwd
}
function tag_time_value_2_web {
	DRAW_BOX 80 "tag_time_value_2_web" 33
	#echo "tag_time_value_2_web....."
	pwd=`pwd`
	cd ${ROMFS_DIR}/web
	#check path
	dirkpool=(${ROMFS_DIR}/web ${ROMFS_DIR}/web/file ${ROMFS_DIR}/web/graphics ${ROMFS_DIR}/web/tmp)

	#collecting all link name
	linkpool=(`ls | grep -v [.]asp | grep -v [.]htm | grep -v file | grep -v graphics | grep -v tmp`)
	if [ -d file ]; then
		linkpool=(${linkpool[@]} `ls file | grep -v [.]asp | grep -v [.]htm | grep -v [.]html`)
	fi
	if [ -d graphics ]; then
		linkpool=(${linkpool[@]} `ls graphics | grep -v [.]asp | grep -v [.]htm | grep -v [.]html`)
	fi
	if [ -d tmp ]; then
		linkpool=(${linkpool[@]} `ls tmp | grep -v [.]asp | grep -v [.]htm | grep -v [.]html`)
	fi
	#start modify all link URL
	for dirloca in ${dirkpool[@]}; do
		if [ -d $dirloca ]; then
			cd $dirloca
			echo `pwd`
			for webFile in $(ls *.asp *.css); do
				for linkname in ${linkpool[@]}; do
					sed -i 's/'$linkname'?time='$timevalue'/'$linkname'/g' $webFile
					sed -i 's/'$linkname'/'$linkname'?time='$timevalue'/g' $webFile
				done
			done
		fi
	done

	cd $pwd
}


if [ "$tag_time_value" = "1" ]; then 
	tag_time_value_2_cgi
fi


cd src

if [ "$1" = "make" ] || [ "$1" = "clean" ] || [ "$1" = "all" ]; then
	make clean
	if [ $? != 0 ]; then
		revert_file 
		exit 1
	fi					
	make -f Makefile-tools clean
	if [ $? = 1 ]; then 
		revert_file
		exit 1 
	fi
fi

if [ "$1" = "make" ] || [ "$1" = "all" ]; then
	make -s
	if [ $? != 0 ]; then
		revert_file
		exit 1
	fi
	./tool.mk
	if [ $? != 0 ]; then
		revert_file
		echo "make tool error!!"
		exit 1
	fi
	revert_file
fi

if [ "$1" = "install" ] || [ "$1" = "all" ]; then
  mkdir -p ${ROMFS_DIR}/bin
  mkdir -p ${ROMFS_DIR}/web

  cp -rf ${BOA_DIR}/web/* ${ROMFS_DIR}/web
  
  if [ "$tag_time_value" = "1" ]; then 
	tag_time_value_2_web
  fi
  
  if [ $? = 1 ]; then 
    exit 1 
  fi

	if [ "$_MODEL_" = "BR6278AC" ]; then
		cp -f ${BOA_DIR}/src/flash_BR6278AC ${ROMFS_DIR}/bin/flash
	elif [ "$_EW7478ACR_" = "y" ]; then
		cp -f ${BOA_DIR}/src/flash_EW7478ACR ${ROMFS_DIR}/bin/flash
	else
		cp -f ${BOA_DIR}/src/flash_EW7478AC ${ROMFS_DIR}/bin/flash
	fi

  if [ $? = 1 ]; then 
	exit 1		
  fi
 
  cp -f ${BOA_DIR}/etc/boa/boa.conf.ori ${BOA_DIR}/etc/boa/boa.conf
  if [ "$_IQv2_" = "y" ]; then
	sed -i 's/DirectoryIndex index.asp/DirectoryIndex index1.asp/g' ${BOA_DIR}/etc/boa/boa.conf
  fi	
  cp -rf ${BOA_DIR}/etc/boa ${ROMFS_DIR}/etc

  if [ $? = 1 ]; then 	
	exit 1
  fi

  cp -f ${BOA_DIR}/src/boa ${ROMFS_DIR}/bin/webs
  if [ $? = 1 ]; then 
	exit 1
  fi

  cp -f ${ROOT_DIR}/define/FUNCTION_SCRIPT ${ROMFS_DIR}/web
  if [ $? = 1 ]; then
	exit 1
  fi
fi

revert_file

cd ..

