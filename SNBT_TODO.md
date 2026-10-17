# SNBT 接口设计（待实现）

## 1. 模块定位

- 新增 `include/nbt_cpp/NBT_Snbt.hpp`，加入 `NBT_All.hpp`。
- 与 `NBT_Reader` / `NBT_Writer` 同层，不放入 `NBT_Helper`。
- 读写在同一个类 `NBT_Snbt` 中，共用 tokenizer 与生成逻辑。

## 2. 语义分层

| 层 | 表示 | List 异构 | 根 |
|---|---|---|---|
| 语义层（内存树） | `NBT_Node` / `Compound` | 原生支持 | 任意值 |
| SNBT 层 | 文本 | 原生支持 | 必须是 Compound |
| NBT 二进制层 | 字节流 | wire format 限制，需 `{"": value}` 包装/解包 | kv 对序列 |

- `ReadNBT` / `WriteNBT` 的 `Compound` 是文件语义：只表示文件顶层 kv 对，容器本身不写入。
- `ReadSnbt` / `WriteSnbt` 的 `Compound` 采用相同文件语义；SNBT 文本根必须是 Compound，裸值（`42`、`[1,2]`）报 `InvalidRootType`。
- `WriteSnbt` 必须输出根 `{}`；`WriteNBT` 不写容器本身。这是唯一的不对称。
- `{"": value}` 包装/解包只存在于二进制编解码层，不得泄漏到内存树或 SNBT 层。
- `ReadNBT<bUnwrapMixedList = false>` 保留原始包装层，仅用于只读分析，不用于读-改-写。

## 3. 公开接口

```cpp
class NBT_Snbt
{
    NBT_Snbt(void) = delete;
    ~NBT_Snbt(void) = delete;

public:
    // 读取：UTF-8
    template<typename InfoFunc = NBT_Print>
    static bool ReadSnbt(std::string_view svUtf8, NBT_Type::Compound &tFileContent,
                         size_t szStackDepth = 512, InfoFunc funcInfo = InfoFunc{}) noexcept;

    // 读取：MUTF-8（零转换）
    template<typename InfoFunc = NBT_Print>
    static bool ReadSnbt(NBT_Type::String::View svMu8, NBT_Type::Compound &tFileContent,
                         size_t szStackDepth = 512, InfoFunc funcInfo = InfoFunc{}) noexcept;

    template<typename InfoFunc = NBT_Print>
    static bool ReadSnbt(const NBT_Type::String &sMu8, NBT_Type::Compound &tFileContent,
                         size_t szStackDepth = 512, InfoFunc funcInfo = InfoFunc{}) noexcept;

    // 写出：UTF-8
    template<typename SortPolicy = DefaultCompoundSort<true>, typename InfoFunc = NBT_Print>
    static bool WriteSnbt(std::string &sOut, const NBT_Type::Compound &tFileContent,
                          size_t szStackDepth = 512, InfoFunc funcInfo = InfoFunc{}) noexcept;

    // 写出：MUTF-8（可直接赋给 NBT_Type::String）
    template<typename SortPolicy = DefaultCompoundSort<true>, typename InfoFunc = NBT_Print>
    static bool WriteSnbt(NBT_Type::String &sOut, const NBT_Type::Compound &tFileContent,
                          size_t szStackDepth = 512, InfoFunc funcInfo = InfoFunc{}) noexcept;

    // 便捷版
    template<typename SortPolicy = DefaultCompoundSort<true>>
    static std::string ToSnbt(const NBT_Type::Compound &tFileContent);

    template<typename SortPolicy = DefaultCompoundSort<true>>
    static NBT_Type::String ToSnbtMU8(const NBT_Type::Compound &tFileContent);
};
```

约定：

- 所有接口 `noexcept`，内部异常由实现收敛，不向外抛出。
- `ReadSnbt` 不清空输出容器，失败时保留已解析部分并返回 `false`（与 `ReadNBT` 一致）。
- 重复键覆盖旧值并发出 `DuplicateKeyWarn`。
- `WriteSnbt` 覆盖 `sOut` 原有内容；失败时 `sOut` 内容未定义。
- `ToSnbt` / `ToSnbtMU8` 失败返回空字符串。
- 默认深度限制 512。
- 诊断前缀 `Snbt Read Err[...]` / `Snbt Write Err[...]`，带 `line:column`。

可选文件接口（不做压缩）：

```cpp
template<typename InfoFunc = NBT_Print>
static bool SimpleReadSnbtFile(const std::filesystem::path &pathFileName,
                               NBT_Type::Compound &tCompound, InfoFunc funcInfo = InfoFunc{});

template<typename SortPolicy = DefaultCompoundSort<true>, typename InfoFunc = NBT_Print>
static bool SimpleWriteSnbtFile(const std::filesystem::path &pathFileName,
                                const NBT_Type::Compound &tCompound, InfoFunc funcInfo = InfoFunc{});
```

## 4. 编码契约

- UTF-8 接口使用 `std::string` / `std::string_view`（`char`）。
- MUTF-8 接口使用 `NBT_Type::String` / `NBT_Type::String::View`，字符串内容零转换。
- 结构字符、数字、布尔、后缀均为 ASCII，两种编码字节一致；编码差异只存在于字符串值内容（U+0000、辅助平面字符）。
- 引号外的字节必须 `< 0x80`，否则 `UnexpectedToken`；含非 ASCII 的字符串必须加引号。
- MUTF-8 输出不是合法 UTF-8 文本，不得直接写入文本文件、日志或网络文本协议；仅用于赋值给 `NBT_Type::String`。
- 同一文本的 UTF-8 与 MUTF-8 表示按字节不等，禁止跨编码比较或哈希。
- 数字解析使用 `std::from_chars`，不依赖 locale。

## 5. 语法

### 5.1 根

- 必须是 Compound：`{...}`。
- 空文本 / 纯空白、根后尾随内容：见第 12 节。

### 5.2 空白

- 空格、`\t`、`\r`、`\n` 任意出现。
- 不支持注释。

### 5.3 字符串

- 单引号或双引号。
- 转义：`\\`、`\"`、`\'`；其他转义见第 12 节。
- 引号内按原始字节收集，不要求解码。

### 5.4 未加引号 token

- 字符集：`[0-9A-Za-z_+\-.]`。
- 解析顺序：先尝试数字 / 布尔，失败再当字符串。
- 键名同样支持 quoted / unquoted。

### 5.5 数字

| 文本 | 类型 |
|---|---|
| 无后缀整数 | `Int`；超出 `int32` 报 `NumberOutOfRange`，不提升 |
| `1b` / `1B` | `Byte` |
| `1s` / `1S` | `Short` |
| `1l` / `1L` | `Long` |
| 带小数点或指数、无后缀 | `Double` |
| `1.0f` / `1.0F` | `Float` |
| `1.0d` / `1.0D` | `Double` |
| `true` / `false` | `Byte(1/0)` |
| `NaN` / `Infinity` / `-Infinity` | 按后缀，默认 `Double` |
| `0x...` | 不支持，`InvalidNumber` |

### 5.6 Compound

- `{key:value, ...}`；`{}` 合法。
- 尾随逗号默认拒绝。

### 5.7 List

- `[a,b,...]`；原生异构，直接构造 `NBT_Type::List`。
- `[]` 的 tag 为 `End`。
- 不做任何包装 / 解包。

### 5.8 数组

- `[B;...]`、`[I;...]`、`[L;...]`。
- 元素数超过 `ArrayLength_Max` 报错。
- 元素无法转换为目标类型报 `InvalidArrayElement`。

## 6. 写出规则

- 根输出 `{...}`；空容器输出 `{}`。
- 键名默认总加双引号；可选省略。
- 字符串值总加引号，转义 `\` 与所用引号。
- 数字总带类型后缀，保证类型往返：`1b`、`1s`、`1l`、`1.0f`、`1.0d`。
- 浮点使用最短往返表示，确保含小数点或指数。
- List 按实际元素 tag 原样输出，不做类型统计、不包装。
- `End` 元素忽略并发出 `EndElementIgnoreWarn`。
- 排序策略与 `WriteNBT` 共用同一个 `SortPolicy`。
- 默认紧凑输出；美化缩进作为可选参数。

## 7. 错误码 / 警告码

```cpp
enum ErrCode : uint8_t
{
    AllOk = 0,
    UnknownError, StdException, OutOfMemoryError,
    StackDepthExceeded,
    EmptyInput,
    UnexpectedEndOfInput,
    UnexpectedToken,
    InvalidRootType,
    UnterminatedString,
    InvalidEscapeSequence,
    InvalidNumber,
    NumberOutOfRange,
    InvalidArrayPrefix,
    InvalidArrayElement,
    TrailingData,
};

enum WarnCode : uint8_t
{
    NoWarn = 0,
    DuplicateKeyWarn,
    EndElementIgnoreWarn,
};
```

## 8. 实现策略

- 一套 tokenizer / 生成逻辑，两个输出适配器、两个输入适配器。
- `Utf8Output`：写入 `std::string`，字符串内容调用 `ToCharTypeUTF8()` 转码。
- `Mu8Output`：写入 `NBT_Type::String`，字符串内容直接追加。
- 读取时：MU8 输入直接拷贝到 `NBT_Type::String`；UTF-8 输入调用 `U8ToMU8`。
- 结构字符 ASCII 直写；仅字符串值 / 键名内容按编码分支。
- 不得先生成 UTF-8 再整体转 MUTF-8。

## 9. Helper 迁移

- `NBT_Helper::Serialize<bSnbtType = true>` 保留签名与返回类型（`NBT_Type::String`），实现转发到 `NBT_Snbt::WriteSnbt(NBT_Type::String &, ...)`。
- 非 SNBT 分支保持现状。
- 行为变化：字符串正确转义、受深度限制、可诊断。
- `NBT_Snbt.hpp` 不得 include `NBT_Helper.hpp`。

## 10. 前置重构

- 将 `NoSortCompound` / `DefaultCompoundSort` 从 `NBT_Helper`、`NBT_Writer` 提取到公共头（如 `NBT_Sort.hpp`），三处共用。
- 更新 `doxygen_resource/ROOT.md`，删除“不涉及 snbt 转换与处理，不提供 snbt 相关功能”。
- 更新 `NBT_Type.hpp` 与 `NBT_List.hpp` 的 List 注释：内存模型可异构；二进制写出时包装 / 解包；SNBT 无此限制。

## 11. 往返保证

- `ReadSnbt(WriteSnbt(c)) == c`，UTF-8 与 MUTF-8 两种编码均成立。
- `WriteSnbt(ReadSnbt(text))` 不保证与原文本逐字节一致（空白、引号、键序自由）。
- 文件容器对应关系：

| 操作 | 结果容器 |
|---|---|
| `ReadSnbt("{a:1b}")` | `{a → 1b}` |
| `ReadNBT`（裸 entries 文件） | `{a → 1b}` |
| `ReadNBT`（标准 Mojang 文件） | `{"" → Compound}` |
| `WriteNBT({a → 1b})` | 裸 entries 文件 |
| `WriteSnbt({a → 1b})` | `{a:1b}` |
| `WriteSnbt({"" → Compound})` | `{"": {…}}` |

## 12. 待定

| 项 | 可选方案 | 倾向 |
|---|---|---|
| 空输入 / 纯空白 | 成功返回空容器 / 报 `EmptyInput` | 对齐二进制空文件，成功返回空容器 |
| 根后尾随内容 | 报 `TrailingData` / 忽略 | 报错 |
| 未知转义 `\x` | 报 `InvalidEscapeSequence` / 原样保留 | 报错 |
| 非法输入编码 | 严格校验 / 按 `MUTF8_Tool` 容错 | 结构层严格；字符串内容按 `MUTF8_Tool`，文档说明 |
