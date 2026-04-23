# 测试数据文档

## 测试说明

本测试套件包含 8 个单元测试，2个综合测试，2个压力测试。ACMOJ上的所有测试均已在本地发放给各位。禁止面向测试点进行有针对性的调试和修改代码，违者代码将被Voided。

---

## 单元测试 (Unit Tests)

### 测试 1: 基础抽牌和打牌
**测试目标**: 验证基础的抽牌、打牌、查询和 finish 功能
- 洗牌模式: 1 (outShuffle)
- 卡牌: 5 张得分牌 (1, 2, 3, 4, 5)
- 验证点: 各牌堆数量、分数累计

### 测试 2: Power 能力牌测试
**测试目标**: 验证能力牌的永久加成效果
- 洗牌模式: 1 (outShuffle)
- 卡牌: 2 张 POWER (3, 5)，3 张 SCORE (10, 20, 30)
- 验证点: 能力加成累积，能力牌不进入弃牌堆

### 测试 3: Multiplier 倍率测试
**测试目标**: 验证 Multiplier 技能牌的倍率增加和叠加效果
- 洗牌模式: 1 (outShuffle)
- 卡牌: 3 张 SKILL Multiplier，2 张 SCORE (10, 20)
- 验证点: 倍率叠加，倍率重置

### 测试 4: HeadButt 技能测试
**测试目标**: 验证 HeadButt 将弃牌堆 tail 移到抽牌堆 head
- 洗牌模式: 1 (outShuffle)
- 卡牌: 2 张 SKILL HeadButt，3 张 SCORE (1, 2, 3)
- 验证点: HeadButt 正确移动牌，抽牌顺序正确

### 测试 5: 洗牌模式 2 (inShuffle) 测试
**测试目标**: 验证逆序洗牌功能
- 洗牌模式: 2 (inShuffle)
- 卡牌: 5 张 SCORE (1, 2, 3, 4, 5)
- 验证点: inShuffle 正确反转弃牌堆

### 测试 6: 洗牌模式 3 (oddEvenShuffle) 测试
**测试目标**: 验证奇偶洗牌功能
- 洗牌模式: 3 (oddEvenShuffle)
- 卡牌: 7 张 SCORE (1, 2, 3, 4, 5, 6, 7)
- 验证点: oddEvenShuffle 正确分组

### 测试 7: 抽牌堆不足时的洗牌触发
**测试目标**: 验证抽牌过程中抽牌堆为空时自动触发洗牌
- 洗牌模式: 1 (outShuffle)
- 卡牌: 8 张 SCORE (1-8)
- 验证点: 中途触发洗牌，继续抽牌

### 测试 8: 空弃牌堆 HeadButt 测试
**测试目标**: 验证对空弃牌堆使用 HeadButt 无效果
- 洗牌模式: 1 (outShuffle)
- 卡牌: 2 张 SKILL HeadButt，1 张 SCORE (10)
- 验证点: 空弃牌堆时 HeadButt 不报错但无效

---

## 综合测试 (Integration Tests)

### 测试 9 与 10: 复杂组合技与完整游戏流程

## 压力测试 (Stress Tests)

### 测试 11 与 12: 大规模数据
---

## 测试数据格式

每个测试包含两个文件：
- `<编号>.in`: 输入文件
- `<编号>.out`: 期望输出文件

输入格式：
```
<shuffle_mode>
<n>
<card_1>
<card_2>
...
<command_1>
<command_2>
...
exit
```

输出格式：
```
<query_result_1>
<query_result_2>
...
```

## 本地测试
在本地运行测试可以使用我们提供的测试脚本。具体来说，请在根目录下运行
```bash
./run_tests.sh
```
如果出现`Permission denied`错误，请先运行
```bash
chmod +x run_tests.sh
```

## OJ 测试

提交`game.h`文件即可。为了保证不出现编译错误，即使你没有实现某接口，也请确保该接口有一个空实现。
**OJ上测试点13-24内容与1-12相同，但会对你的程序进行内存检查**。
