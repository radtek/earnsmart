#!/bin/bash
 
echo WANdisco Subversion Installer v0.1 for Ubuntu 12.04
echo Please report problems and bugs to opensource@wandisco.com
echo 
echo Gathering some information about your system...

MINVERSION='1'
SVNVER=1.8.0

#functions
gather_info () {
        ARCH=`uname -m`
        SVNSTATUS=`dpkg -l|grep " libsvn1 " | awk '{print $1}'`
}
check_tools () {
        COMMANDS="dpkg wget apt-get"
        for C in $COMMANDS; do
                if [ -z "$(which $C)" ] ; then
                        echo "This installer uses the $C command which was not found in \$PATH."
                        exit 1
                fi
        done
}

check_is_root ()
{
	if [[ $EUID -ne 0 ]]; then
   		echo "This script must be run as root" 1>&2
   		exit 1
	fi	
}
svn_remove_old ()
{
	echo Removing old packages...
	dpkg -P --force-depends libsvn1 subversion libapache2-svn libsvn-dev libsvn-doc libsvn-perl subversion-tools 
}
add_repo_config ()
{
        echo Adding repository configuration to /etc/apt/sources.list.d/
	if [ -f /etc/apt/sources.list.d/WANdisco.list ]; then
		rm /etc/apt/sources.list.d/WANdisco.list
	fi
	echo "Installing Apt repo...."
	echo "# WANdisco Open Source Repo" > /etc/apt/sources.list.d/WANdisco.list
	echo "deb http://opensource.wandisco.com/ubuntu precise svn18" >> /etc/apt/sources.list.d/WANdisco.list
        echo "Importing GPG key"
        wget http://opensource.wandisco.com/wandisco-debian.gpg -O /tmp/wandisco-debian.gpg &>/dev/null
        apt-key add /tmp/wandisco-debian.gpg
        rm -rf /tmp/wandisco-debian.gpg
        apt-get update
}
install_svn ()
{
        echo Checking to see if you already have Subversion installed via dpkg...
        if [ "$SVNSTATUS" == "ii" ]; then
		echo
        	echo Subversion is already installed on the system.
        	echo Do you wish to replace the version of subversion currently installed with the WANdisco version? 
		echo This action will remove the previous version from your system 
		echo \[y/n\]:
		read svn_install_confirm
		if [ "$svn_install_confirm" == "y" -o "$svn_install_confirm" == "Y" ]; then
			svn_remove_old
			add_repo_config
			echo		
			echo Installing Subversion $SVNVER-$MINVERSION
			echo
			apt-get -y --force-yes install libsvn1 libserf1 subversion libsvn-perl subversion-tools
 			echo Would you like to install apache and the apache SVN modules? \[y/n\]
			read dav_svn_confirm
			if [ "$dav_svn_confirm" == "y" -o "$dav_svn_confirm" == "Y" ]; then
				echo Installing apache and subversion modules
				apt-get -y --force-yes install apache2 libapache2-svn
				echo Installation complete. Restart apache? \[y/n\]
				read apache_restart_confirm
				if [ $apache_restart_confirm == "y" -o $apache_restart_confirm == "Y" ]; then
					/etc/init.d/apache2 restart	
				fi
			fi
			
	       	else
			echo "Install Cancelled"
			exit 1
		fi

	else
		# Install SVN
		echo "Subversion is not currently installed"
		echo "Starting installation, are you sure you wish to continue? [y/n]"
		read svn_install_confirm
                if [ "$svn_install_confirm" == "y" -o "$svn_install_confirm" == "Y" ]; then
			add_repo_config
                        echo
                        echo Installing Subversion $SVNVER-$MINVERSION
                        echo
                        apt-get -y --force-yes install libsvn1 libserf1 subversion libsvn-perl subversion-tools
                        echo Would you like to install apache and the apache SVN modules? \[y/n\]
                        read dav_svn_confirm
                        if [ "$dav_svn_confirm" == "y" -o "$dav_svn_confirm" == "Y" ]; then
                                echo Installing apache and subversion modules
                                apt-get -y --force-yes install apache2 libapache2-svn libsvn-dev
                                echo Installation complete. Restart apache? \[y/n\]
                                read apache_restart_confirm
                                if [ $apache_restart_confirm == "y" -o $apache_restart_confirm == "Y" ]; then
                                        /etc/init.d/apache2 restart
                                fi
                        fi

                else
                        echo "Install Cancelled"
                        exit 1
                fi
		
        fi
	
}

install_32 ()
{
        echo Installing for $ARCH
	install_svn
}
install_64 ()
{
        echo Installing for $ARCH
	install_svn
}

#Main
check_is_root
check_tools
gather_info

echo Checking your system arch
if [ "$ARCH" == "i686" -o "$ARCH" == "i386" ]; then
	install_32
elif [ "$ARCH" == "x86_64" ];
then
	install_64
else 
	echo Unsupported platform: $ARCH
	exit 1
fi
