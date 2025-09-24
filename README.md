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
```

