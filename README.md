# git-study


# git的高阶用法请访问下面的链接
https://www.cnblogs.com/jamiechoo/articles/18408791



# 以下是学习日志

# First submission

```sh
# 第一次提交步骤
git add <yourfiles path> or git add .
git commit -m "desc"
git branch -m main
git push -u <remote> <branch> # 这一步需要验证身份，才能推送成功
```





# Second submission

| 命令                           | 作用                         |
| ------------------------------ | ---------------------------- |
| `git branch`                   | 查看本地分支                 |
| `git checkout <branch>`        | 切换分支                     |
| `git checkout <branch>`        | 切换到 master（推荐新语法）  |
| `git pull origin <branch>`     | 拉取远程 master 最新代码     |
| ```git merge  <branch>```      | 合并分支                     |
| `git push origin <branch>`     | 推送本地更改                 |
| `git status`                   | 查看工作区状态               |
| `git log --oneline --graph`    | 查看简洁提交图               |
| `git config`                   | 配置用户信息                 |
| `git init`                     | 初始化一个新的 Git 仓库      |
| `git clone <远程代码仓库link>` | 克隆远程仓库到本地           |
| `git add <文件名>`             | 将文件添加到暂存区           |
| `git commit -m "提交信息"`     | 提交暂存区的文件修改到版本库 |





# Third submission

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





# Fourth submission

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





# Fifth submission

```sh
# 暂存
git add <file>
# 例如 git add . 暂存当前目录下所有修改的文件

# 重置掉对应文件的修改
git restore <file>
```





# Sixth submission

##### Git的工作原理

根据 git 的几个文件存储区域，git 的工作区域可以划分为 4 个：

- 工作区：你在本地编辑器里改动的代码，所见即所得，里面的内容都是最新的
- 暂存区：通过 `git add` 指令，会将你工作区改动的代码提交到暂存区里
- 本地仓库：通过 `git commit` 指令，会将暂存区变动的代码提交到本地仓库中，本地仓库位于你的电脑上
- 远程仓库：远端用来托管代码的仓库，通过 `git push` 指令，会将本地仓库的代码推送到远程仓库中

![](.\Image\底层逻辑.png)





# Seventh Submission

##### 合并分支基本步骤

```sh
# 1.本地分支间的合并
比如我在develop分支开发完一个新功能之后，想要把develop分支的代码合并到main分支，然后再推送到远程仓库
git checkout main # 切换分支
git pull origin main # 避免冲突
git merge origin develop # 合并分支

# 如果出现冲突，需要修改完冲突后提交并推送一次
git add <冲突文件名>  # 标记为已解决
git commit -m "解决合并冲突：整合音频API配置"
git push
```

