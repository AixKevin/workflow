# Minecraft 1.21.11 协议版本参考（MVP）

## 1. 结论

- 当前项目统一使用：Minecraft Java Edition `1.21.11` 对应协议版本 `774`。
- 需求文档与指导计划中原先的 `767` 为错误占位值，已替换。

## 2. 证据链（按可信度分层）

### 2.1 官方来源（Mojang）

1. 官方版本清单（manifest）显示最新正式版为 `1.21.11`，并给出该版本元数据 URL：
   - `https://piston-meta.mojang.com/mc/game/version_manifest_v2.json`
2. 官方 `1.21.11` 元数据 JSON（launcher metadata）：
   - `https://piston-meta.mojang.com/v1/packages/2a0ce34eae0f2f4d49ff8747583c1b3dc34914b0/1.21.11.json`

说明：
- 上述官方 launcher 元数据用于启动参数、下载地址、依赖信息，不直接包含 `protocol_version` 字段。
- `protocol_version` 位于游戏包内的 `version.json`（`client.jar`/`server.jar` 内嵌文件）。

### 2.2 社区技术资料（辅助交叉）

1. Minecraft Wiki 的协议版本对照页给出 `1.21.11 -> 774`。
2. Minecraft Wiki 的 version.json 词条说明 `protocol_version` 字段定义与所在位置（`client.jar`/`server.jar`）。

说明：
- 社区资料用于快速交叉验证。
- 项目实施时应以“官方包内 `version.json` 实测值”作为最终准入常量。

## 3. 可复现的官方核验流程

在可访问 Mojang 下载地址的环境中，执行以下命令直接从官方 `server.jar` 提取 `protocol_version`：

```bash
curl -fsSLO https://piston-data.mojang.com/v1/objects/64bb6d763bed0a9f1d632ec347938594144943ed/server.jar
unzip -p server.jar version.json | grep -o '"protocol_version"[[:space:]]*:[[:space:]]*[0-9]\+'
```

预期输出：

```text
"protocol_version": 774
```

如果本机无 `unzip`，可改用 JDK 的 `jar`：

```bash
jar xf server.jar version.json
grep -o '"protocol_version"[[:space:]]*:[[:space:]]*[0-9]\+' version.json
```

## 4. 对项目实现的落地建议

1. 在代码中集中定义常量，例如 `kSupportedProtocolVersion = 774`，避免散落硬编码。
2. 在握手拒绝日志中打印实际值与期望值，便于排查：
   - `got=<client_protocol>, expected=774`
3. 后续升级版本时，优先按“官方 jar 内嵌 `version.json`”更新常量，再更新文档。
