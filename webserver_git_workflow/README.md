# Git Workflow - Web Server

We will utilize `git subtree` over here

First, add the remote branch and name it to **external**
```
git remote add -f external git@github.com:DevanshRathi1/ECE477WebServer.git
```
Then we will add a subtree
```
git subtree add -P WebServer external main --squash
```
If we want to pull anything from the `main` we can do
```
git subtree pull -P WebServer external main --squash
```
Likewise, if we want to pull anything from the `main` we can do
```
git subtree push -P WebServer external main --squash
```
To switch to another branch, let's say **feature**, we should do,
```
git rm WebServer
git commit -m "removing sub-repo"
git subtree add -P WebServer external feature --squash
```

### GOTCHAS
* `git subtree push` and `git subtree pull` commands has to be performed inside the web server repo 
* Don't use `git pull external main` it will pull everything from the main tree!