#!/bin/bash

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 计数器
total_tests=0
passed_tests=0
failed_tests=0

# 项目目录
PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"
TESTCASES_DIR="$PROJECT_DIR/testcases"
EXECUTABLE="$BUILD_DIR/final"

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}  Slay the Balatro 测试系统${NC}"
echo -e "${BLUE}================================${NC}"
echo ""

# 创建 build 目录
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 使用 CMake 编译
if cmake .. && make; then
    echo -e "${GREEN}编译成功！${NC}"
    echo ""
else
    echo -e "${RED}编译失败！${NC}"
    exit 1
fi

cd "$PROJECT_DIR"

# 获取所有测试用例
test_files=$(ls "$TESTCASES_DIR"/*.in 2>/dev/null | sort -V)

if [ -z "$test_files" ]; then
    echo -e "${RED}错误: 在 $TESTCASES_DIR 中没有找到测试用例${NC}"
    exit 1
fi

echo -e "${BLUE}开始运行测试...${NC}"
echo ""

# 运行每个测试用例
for input_file in $test_files; do
    # 获取测试编号
    test_name=$(basename "$input_file" .in)
    output_file="$TESTCASES_DIR/${test_name}.out"
    
    total_tests=$((total_tests + 1))
    
    # 检查对应的输出文件是否存在
    if [ ! -f "$output_file" ]; then
        echo -e "${YELLOW}[SKIP] 测试 $test_name: 缺少 .out 文件${NC}"
        continue
    fi
    
    # 运行程序并捕获输出
    actual_output=$("$EXECUTABLE" < "$input_file" 2>&1)
    expected_output=$(cat "$output_file")
    
    # 比较输出
    if [ "$actual_output" = "$expected_output" ]; then
        echo -e "${GREEN}[PASS]${NC} 测试 $test_name"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "${RED}[FAIL]${NC} 测试 $test_name"
        failed_tests=$((failed_tests + 1))
        
        # 显示详细的差异
        echo -e "${YELLOW}期望输出:${NC}"
        echo "$expected_output" | head -20
        if [ $(echo "$expected_output" | wc -l) -gt 20 ]; then
            echo "... (输出被截断)"
        fi
        
        echo -e "${YELLOW}实际输出:${NC}"
        echo "$actual_output" | head -20
        if [ $(echo "$actual_output" | wc -l) -gt 20 ]; then
            echo "... (输出被截断)"
        fi
        
        echo -e "${YELLOW}差异:${NC}"
        diff -u <(echo "$expected_output") <(echo "$actual_output") | head -30
        echo ""
    fi
done

# 输出统计结果
echo ""
echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}         测试结果统计${NC}"
echo -e "${BLUE}================================${NC}"
echo -e "总测试数:   ${BLUE}$total_tests${NC}"
echo -e "通过:       ${GREEN}$passed_tests${NC}"
echo -e "失败:       ${RED}$failed_tests${NC}"

if [ $failed_tests -eq 0 ]; then
    echo -e "${GREEN}所有测试通过！ ✓${NC}"
    exit 0
else
    echo -e "${RED}有 $failed_tests 个测试失败 ✗${NC}"
    exit 1
fi
