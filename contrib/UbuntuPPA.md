Add any ppa:
sudo add-apt-repository 'deb uri distribution [component1] [component2] [...]' 
or
sudo add-apt-repository ppa:<ppa_name>


Example For 17.04 and xenial ppa
sudo add-apt-repository 'deb http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu xenial main' 

# add your gpg key to any keyserver
sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com D46F45428842CE5E


sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com SHA256:nThbg6kXUpJWGl7E1IGOCspRomTxdCARLviKw6E5SY8

Zesty (17.04)
deb http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu zesty main 
deb-src http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu zesty main 

Yakkety (16.10)
deb http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu yakkety main 
deb-src http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu yakkety main 

Xenial (16.04)
deb http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu xenial main 
deb-src http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu xenial main 

Trusty (14.04)
deb http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu trusty main 
deb-src http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu trusty main 

Precise (12.04)
deb http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu precise main 
deb-src http://ppa.launchpad.net/bitcoin/bitcoin/ubuntu precise main 


sudo add-apt-repository ppa:bitcoin/bitcoin

