echo "******************************"
echo "          UPDATING            "
echo "******************************"
sudo apt-get update

echo "******************************"
echo "          GETTING VIM         "
echo "******************************"
sudo apt-get install vim

echo "******************************"
echo "          GETTING GIT         "
echo "******************************"
sudo apt-get install git

echo "******************************"
echo "          GETTING RAILS       "
echo "******************************"
sudo apt-get install curl
gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
\curl -sSL https://get.rvm.io | bash -s stable --rails


echo "PS1=\"\\[\\033[1;30m\\][\\T]\\[\\033[1;37m\\] \\[\\033[1;32m\\]\\u\\[\\033[1;33m\\]:\\[\\033[1;31m\\]\\w\\[\\033[1;37m\\] \\[\\033[1;32m\\]$\\[\\033[1;37m\\] \"" > ~/.bashrc

echo >> ~/.bashrc

echo "alias ls='ls --color=always'" >> ~/.bashrc
echo "alias l='ls'" >> ~/.bashrc
echo "alias ll='ls -l'" >> ~/.bashrc
echo "alias la='ls -a'" >> ~/.bashrc
echo "alias lr='ls -R'" >> ~/.bashrc

echo >> ~/.bashrc

echo "alias c='clear'" >> ~/.bashrc
echo "alias e='exit'" >> ~/.bashrc
echo "alias m='make'" >> ~/.bashrc
echo "alias mc='make clean'" >> ~/.bashrc

echo >> ~/.bashrc

echo "cd" >> ~/.bashrc

rm -rf Desktop/ Documents/ Public/ Templates/ Videos/
