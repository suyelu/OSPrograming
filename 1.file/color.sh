#!/bin/bash

# 文本颜色
echo "文本颜色："
for i in {30..37}; do
    echo -e "\033[0;${i}m文本颜色 $i\033[0m"
done

echo

# 背景颜色
echo "背景颜色："
for i in {40..47}; do
    echo -e "\033[${i}m背景颜色 $i\033[0m"
done

echo

# 组合效果
echo "组合效果："
for fg in {30..37}; do
    for bg in {40..47}; do
        echo -e "\033[${fg};${bg}m文本颜色 $fg, 背景颜色 $bg\033[0m"
    done
    echo
done
