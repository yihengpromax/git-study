git-study

First Commit



Second Commit

```sh
# 第一次提交步骤
git add <yourfiles path> or git add .
git commit -m "desc"
git branch -m main
git push -u <remote> <branch> # 这一步需要验证身份，才能推送成功
```



Third Commit

```sh
# 例子
echo "# test" >> README.md
git init
git add README.md
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/yihengpromax/test.git
git push -u origin main

# Exist
git remote add origin https://github.com/yihengpromax/test.git
git branch -M main
git push -u origin main

xxxxx
```

#Four Commit
```sh
# 克隆
git init
git clone

# 拉取代码
git pull origin main

# 切换分支
git checkout <分支名>
git checkout . # 把当前工作目录下所有已追踪（tracked）文件的本地更改全部还原为最近一次 commit 的状态。

#设置用户名和密码
git config --global user.name "Your Name"
git config --global user.email "youremail@example.com"
git config --global --list # 查看设置的用户名和密码
```