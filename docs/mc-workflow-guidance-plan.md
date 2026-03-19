# Minecraft 代理监控系统（Workflow）项目指导计划

## 0. 目标与边界
本计划基于需求文档 [MC-requirement-doc.md](MC-requirement-doc.md)，按 MVP 方式实现：
- 仅支持 Minecraft 1.21.11（协议版本 774）
- C++17 + Workflow
- 内存态黑名单与在线玩家
- HTTP API + 极简前端
- 不做持久化与认证

协议版本依据见：[mc-protocol-reference.md](mc-protocol-reference.md)

## 1. 技术路线（与 Workflow 对齐）

### 1.1 代理侧
- 用 Workflow server 接收玩家连接。
- 在 `process` 回调中做“前置读取 + 协议解析 + 放行/拒绝”。
- 放行后建立到后端服务器连接，进行双向透传。

可参考：
- HTTP server 基础示例 [tutorial/tutorial-04-http_echo_server.cc](tutorial/tutorial-04-http_echo_server.cc)
- 工厂与任务入口 [src/factory/WFTaskFactory.h](src/factory/WFTaskFactory.h#L110)
- 任务状态语义 [src/factory/WFTask.h](src/factory/WFTask.h#L41)

### 1.2 管理侧
- 用 Workflow HTTP server 提供 `/api/players`、`/api/bans`、`/api/ban`。
- 前端使用静态 HTML + AJAX 轮询 3 秒。

可参考：
- HTTP server 教程 [docs/tutorial-04-http_echo_server.md](docs/tutorial-04-http_echo_server.md)

### 1.3 状态管理
- 全局内存容器：
  - 在线玩家：`unordered_map<session_id, PlayerSession>`
  - 黑名单：`unordered_set<string> banned_names` / `unordered_set<string> banned_ips`
- 并发保护：先用 `std::mutex`，MVP 不做无锁优化。

## 2. 分阶段里程碑（建议 10~14 天）

## M1：项目骨架与配置（Day 1）
目标：能启动两个服务端口（MC 代理端口 + HTTP 管理端口），但暂不转发业务。

你需要完成：
1. 建立目录结构（`src/proxy`、`src/http`、`src/core`、`web`）。
2. 加载配置文件（`proxy_port`、`backend_host`、`backend_port`、`http_port`）。
3. 启动两个 server，打印监听日志。

产物：
- 程序启动后可看到两个端口监听成功日志。

验收：
- `curl` 管理端口返回占位 JSON。

---

## M2：协议最小解析（Day 2-3）
目标：只解析握手和登录起始包，拿到 `protocol_version`、`next_state`、`name`。

你需要完成：
1. 实现 VarInt 解码器。
2. 解析 Handshake 包（Packet ID 0x00）。
3. 当 `next_state == 2` 时解析 Login Start（Packet ID 0x00），提取玩家名。
4. 若协议版本不是 774，直接断开。

产物：
- 日志可打印：`ip`、`protocol_version`、`name`。

验收：
- 用 1.21.11 客户端可触发正确日志。
- 非 774 版本客户端被拒绝。

---

## M3：黑名单与准入控制（Day 4）
目标：支持用户名/IP 封禁，并在连接入口生效。

你需要完成：
1. 内存黑名单容器（name/ip 两套）。
2. 准入检查：命中即断开。
3. 在线玩家表维护（连接成功加入，断开删除）。

产物：
- 日志可显示 ban hit 事件。

验收：
- 加入黑名单后新连接被拒绝。

---

## M4：透传核心（Day 5-6）
目标：打通“客户端 <-> 代理 <-> 后端”双向转发。

你需要完成：
1. 建立到后端 TCP 连接。
2. 把前置读取到的握手/登录数据补发给后端。
3. 两个方向做持续透传，任一端关闭时完整清理会话。

产物：
- 玩家可通过代理正常进入后端服务器。

验收：
- Ping 与登录都能通过代理。
- 断线不会导致会话泄漏。

---

## M5：HTTP API（Day 7）
目标：实现文档中的 4 个 API。

你需要完成：
1. `GET /api/players`
2. `GET /api/bans`
3. `POST /api/ban`
4. `DELETE /api/ban`

实现要求：
- `POST /api/ban` 成功后，立即踢出匹配在线玩家。
- 返回统一 JSON：`{"ok":true/false,"msg":"..."}`。

产物：
- 使用 `curl` 可完整封禁/解封并观察效果。

---

## M6：前端页面与联调（Day 8-9）
目标：交付可演示页面。

你需要完成：
1. 在线玩家表格 + 每行封禁按钮。
2. 黑名单 name/ip 列表 + 解封按钮。
3. 两个新增封禁表单。
4. 每 3 秒轮询刷新。

产物：
- 浏览器中可完成完整操作闭环。

---

## M7：稳定性与收尾（Day 10）
目标：达到可演示 MVP 稳定性。

你需要完成：
1. 基础压测（500 并发连接的轻量验证）。
2. 检查异常路径：后端宕机、半开连接、重复封禁。
3. 输出 README（启动方式 + API + 已知限制）。

产物：
- MVP 演示版可交付。

## 3. 建议的代码结构

```text
src/
  core/
    config.h/.cpp
    global_state.h/.cpp
    types.h
  protocol/
    varint.h/.cpp
    mc_handshake_parser.h/.cpp
  proxy/
    mc_proxy_server.h/.cpp
    proxy_session.h/.cpp
    relay_pipe.h/.cpp
  http/
    admin_http_server.h/.cpp
    handlers_players.h/.cpp
    handlers_bans.h/.cpp
web/
  index.html
  app.js
  style.css
```

## 4. 关键风险与规避

1. 风险：握手包是分片到达，首次 `recv` 读不全。
- 规避：先做“缓冲区累积 + 最小长度检查”，不足就继续读。

2. 风险：会话生命周期混乱，导致悬挂指针。
- 规避：统一 `session_id` 管理，容器里只存轻量句柄；关闭路径集中在一个函数。

3. 风险：封禁时踢人并发冲突。
- 规避：先快照匹配会话 ID，再逐个执行关闭动作，避免长时间持锁。

4. 风险：透传异常时资源未释放。
- 规避：所有失败路径都走同一 cleanup 逻辑，确保双端 fd/任务都退出。

## 5. 每阶段你怎么和我配合（推荐）

每完成一个里程碑，你给我：
1. 关键类头文件与核心 cpp（不是整仓库）。
2. 你的测试命令和日志片段。
3. 你不确定的 1~2 个点。

我会按这个顺序回复：
1. 先做 correctness review（是否会崩、会泄漏、会误判）。
2. 再做工程 review（结构、边界、可维护性）。
3. 最后给对应 Workflow 代码对照与优化建议。

## 6. 第一周最小交付线（务必达成）

1. Day 1-3：能识别 774 + 取到玩家名
2. Day 4-6：能通过代理进服 + 黑名单生效
3. Day 7：API 可用并可踢出在线玩家

只要这三点达成，你的 MVP 就成立，后续优化都可迭代。
