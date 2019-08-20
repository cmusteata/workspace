# generate ssh key id_rsa.pub
cd ~/.ssh
ssh-keygen -t rsa -C "email@address.com"

# copy key to github
profile -> edit profile -> SSH & GPG Keys

# check ssh connection to github
ssh -T git@github.com

# init remote git repository
profile -> edit profile -> add repository -> workspace

# config local git
git config --global user.name "First Last"
git config --global user.email "email@address.com"
git config --global color.ui true
git config --global core.editor vi
git config --global push.default simple

# init local git repository
cd ~/workspace
git init

# set origin
git remote add origin git@github.com:username/workspace.git
git push [-u | --set-upstream] origin master
